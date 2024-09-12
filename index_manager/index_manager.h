#pragma once
#include "avl_tree.h"
#include "sorted_segments.h"

using namespace AvlTreeNameSpace;
using namespace SortedSegmentsNameSpace;

namespace IndexManagerNameSpace {
	class IndexManager {

	public:
		virtual const RecordInfo get(int key) const = 0;

		virtual bool put(const Key &key) = 0;
	};


	class LSMIndexManager : public IndexManager {
	private:
		AvlTree tree;
		SortedSegments segments;
		const long long thresholdSize = 1e3;

		const RecordInfo getMemory(int key) const;

		const RecordInfo getDisk(int key) const;
		int countWAL();
		int countWals;
		AvlTree& checkWAL();

	public:
		LSMIndexManager();
		virtual const RecordInfo get(int key) const;
		virtual bool put(const Key &key);
	};
}