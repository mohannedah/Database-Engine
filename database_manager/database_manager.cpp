// database_manager.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"
#include "database_manager.h"

using namespace StorageManagerNameSpace;
namespace DatabaseManagerNameSpace {
	

	DatabaseManager::DatabaseManager(IndexManager* indexManager) : indexManager(indexManager) {
		
		
	}

	const Json::Value DatabaseManager::get(int key) const {
		const RecordInfo recordInfo = indexManager->get(key);

		ifstream *file = StorageManager::openRead("records.json");
		const Json::Value record = StorageManagerNameSpace::StorageManager::get(file, recordInfo.offset, recordInfo.offset + recordInfo.recordSize);
		StorageManager::closeReadFile(file);
	
		return record;
	}

	bool DatabaseManager::put(int key, const Json::Value& record) {
		ofstream *file = StorageManager::openWrite("records.json");
		const int offset = StorageManagerNameSpace::StorageManager::put(file, record);
		StorageManager::closeWriteFile(file);
		
		const Key myKey = Key(key, RecordInfo(offset, record.toStyledString().size()));

		bool state = this->indexManager->put(myKey);
		return state;
	}
}