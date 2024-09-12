
#include <bits/bits-stdc++.h>
#include <framework.h>
#include<json/json.h>
#include "database_manager.h"
#include "index_manager.h"
#include <rpc/server.h>

using namespace DatabaseManagerNameSpace;
using namespace IndexManagerNameSpace;
using namespace StorageManagerNameSpace;

int main()
{

	DatabaseManager manager(new LSMIndexManager());
	
	for(int i = 0; i < 1e4; i++)
	{
		Json::Value record;
		record["key"] = i;
		record["name"] = "whatever";
		manager.put(i, record);
	}

	cout << manager.get(100).toStyledString() << endl;

	return 0;
}	
