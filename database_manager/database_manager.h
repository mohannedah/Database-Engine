#pragma once
#include<json/json.h>
#include "../index_manager/index_manager.h"
#include "../storage_manager/storage_manager.h"

using namespace IndexManagerNameSpace;

namespace DatabaseManagerNameSpace {
	class DatabaseManager {
	private:
		IndexManager* indexManager;
	public:

		DatabaseManager(IndexManager* indexManager);

		const Json::Value get(int key) const;

		/*const Json::Value get(int key);*/

		bool put(int key, const Json::Value& record);
	};
};

