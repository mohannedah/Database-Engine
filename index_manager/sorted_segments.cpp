#include "pch.h"
#include "framework.h"
#include "sorted_segments.h"

#include "../storage_manager/storage_manager.h"

using namespace StorageManagerNameSpace;

namespace SortedSegmentsNameSpace {

	SortedSegments::SortedSegments(){
		this->count = countSegments();
	}

	const int SortedSegments::countSegments() const {
		int leftPointer = 1, rightPointer = 1e9;
		int count = 0;
		while (rightPointer >= leftPointer) {
			int middlePointer = (leftPointer + rightPointer) / 2;
			string currPath = "segments/segment_" + to_string(middlePointer) + ".json";

			if (StorageManager::exists(currPath)) {
				count = middlePointer;
				leftPointer = middlePointer + 1;
			}
			else {
				rightPointer = middlePointer - 1;
			}
		}
		return count;
	}

	

	
	const Key SortedSegments::get(string filePath, int key) const {
		const int recordSize = Key::getJsonSize();
		int leftPointer = 0, rightPointer = (StorageManager::getEndOffset(filePath) / recordSize) - 1;
		
		ifstream *file = StorageManager::openRead(filePath);
			
		while (rightPointer >= leftPointer) {
			int middlePointer = (leftPointer + rightPointer) / 2;
			int startingOffset = recordSize * middlePointer;
			const Key currRecord = Key::jsonToRecord(StorageManager::get(file,startingOffset, startingOffset + recordSize));

			int currKey = currRecord.key;
			
			if (currKey == key) return currRecord;

			if (currKey > key) {
				rightPointer = middlePointer - 1;
			}
			else {
				leftPointer = middlePointer + 1;
			}
		}
		StorageManager::closeReadFile(file);
		
		return Key(-1, RecordInfo(-1, -1));
	}

	const RecordInfo SortedSegments::get(int key) const {
		

		for (int i = this->count; i > 0; i--) {
			string filePath = "segments/segment_" + to_string(i) + ".json";
			const Key record = get(filePath, key);
			if (record.key == key) return record.info;
		}
		return RecordInfo(-1, -1);
	};


	void SortedSegments::flushToDisk(vector<Key*>& keys) {
		string filePath = "segments/segment_" + to_string(this->count + 1) + ".json";
		ofstream* file = StorageManager::openWrite(filePath);
		for (Key* key : keys) {
			StorageManager::put(file, key->toJson());
		}
		StorageManager::closeWriteFile(file);
		this->count += 1;
	}
}