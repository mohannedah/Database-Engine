
#include "pch.h"
#include "framework.h"
#include "index_manager.h"
#include "../storage_manager/storage_manager.h"

using namespace StorageManagerNameSpace;

namespace IndexManagerNameSpace {
	
	LSMIndexManager::LSMIndexManager() : thresholdSize(1e5) {
		this->countWals = this->countWAL();
		this->tree = checkWAL();
		this->segments = SortedSegments();
	}


	int LSMIndexManager::countWAL() {
		int leftPointer = 1, rightPointer = 1e9;

		int ans = 0;

		while (rightPointer >= leftPointer) {
			int middlePointer = (leftPointer + rightPointer) / 2;
			string filePath = "wals/wal_" + to_string(middlePointer) + ".json";
			if(StorageManager::exists(filePath)) {
				ans = middlePointer;
				leftPointer = middlePointer + 1;
			}
			else {
				rightPointer = middlePointer - 1;
			}
		}

		return ans;
	}


	AvlTree& LSMIndexManager::checkWAL() {
		

		if (!this->countWals) return *new AvlTree();
		string filePath = "wals/wal_" + to_string(this->countWals) + ".json";
		ifstream* file = StorageManager::openRead(filePath);
		int endingOffset = StorageManager::getEndOffset(filePath);
		int size = Key::getJsonSize();

		vector<Key> keys;
		for (int i = 0; i < (endingOffset / size); i++) {
			int startingOffset = i * size;
			Json::Value record = StorageManager::get(file, startingOffset, startingOffset + size);
			keys.push_back(Key::jsonToRecord(record));
		}
		
		StorageManager::closeReadFile(file);
		AvlTree* tree = new AvlTree();

		for (Key& key : keys) {
			
			tree->insert(key.key, key.info);
		}
		return *tree;
	}

	const RecordInfo LSMIndexManager::getMemory(int key) const {
		const AvlTreeNode* node = tree.get(key);
		
		if (node == nullptr) return RecordInfo(-1, -1);

		return node->info;
	}

	const RecordInfo LSMIndexManager::getDisk(int key) const {
		return segments.get(key);
	}

	
		
	const RecordInfo LSMIndexManager::get(int key) const {
		const RecordInfo infoMem = getMemory(key);

		if (infoMem.recordSize != -1) return infoMem;
		
		const RecordInfo infoDisk = getDisk(key);

		if (infoDisk.recordSize != -1) return infoDisk;

		return RecordInfo(-1, -1);
	}

	bool LSMIndexManager::put(const Key &key) {
		const int size = tree.getSize();
		if (size + sizeof(AvlTreeNode) > thresholdSize) {
			vector<Key*> keys = this->tree.getAllKeys();
			this->segments.flushToDisk(keys);
			this->countWals += 1;
			this->tree = AvlTree();
		}

		string filePath = "wals/wal_" + to_string(this->countWals) + ".json";

		ofstream* file = StorageManager::openWrite(filePath);

		StorageManager::put(file, key.toJson());

		StorageManager::closeWriteFile(file);

		tree.insert(key.key, key.info);
		
		return true;
	}
	
};