
# Database Engine utilizing LSMTrees(Log-structured merge-tree) implementation.

The goal of this Database Engine was to store JSON records on disk and enable fast retrieval using an LSM-Tree index structured around the keys of these records.

# System Components.
The system consists of three main components, the Index Manager, the Storage Manager, the WAL and the Database Manager.

## Index Manager

The **Index Manager** employs an LSM structure with an in-memory AVL tree. When the AVL tree reaches a certain size threshold, an inorder traversal is performed, and the sorted keys are flushed to disk. During a retrieval operation, the system first checks the AVL tree for the requested key. If the key is not found in memory, it then searches the sorted segments on disk, starting with the most recent flush and working backward, using binary search on each segment. Once the key is located, the retrieval operation returns the offset of the record's position on disk, enabling the system to seek to that offset and retrieve the record. The average time complexity of the insertion operation is O(log N) due to the self-balancing AVL tree being stored in memory. For retrieval, the time complexity is O(log N) if the key is found instantly in memory. If the key isn't found in the AVL tree, the time complexity becomes O(M log N), where M represents the number of stored segments on disk, as each segment is searched using binary search.

## Write-Ahead-Log (WAL)

The system also utilizes a **Write-Ahead Log (WAL)** to ensure data durability and crash recovery. Before any changes are made to the in-memory AVL tree, the operation is first written to the WAL, which is stored on disk. This guarantees that in the event of a system crash, the log can be replayed to restore the in-memory structure to its most recent consistent state.


## Storage Manager

The system includes a **Storage Manager** responsible for handling all file system operations. This includes tasks such as opening files, appending entries to specific files, and retrieving records from a file based on a given starting and ending offset. Additionally, the Storage Manager is responsible for deserializing these records into JSON-like objects.

## Database Manager

The system includes a **Database Manager** that acts as the intermediary between the APIs of the Index Manager and the Storage Manager. The Database Manager coordinates the interactions between these components to handle operations such as querying, updating, and maintaining data consistency. It ensures that requests are processed efficiently, leveraging the Index Manager for quick lookups and the Storage Manager for file operations and record management.


## Basic Example

```cpp
#include <framework.h>
#include <json/json.h>
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
```
The ouput of the previous code snippet :-

```output
  {
      "key" : 100,
      "name" : "whatever"
  }
```



## Warning
I am no expert regarding database design etc. please DO NOT USE THIS IN PRODUCTION. The project was done for the purpose of learning and POC.

