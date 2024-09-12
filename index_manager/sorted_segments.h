#pragma once

#include "pch.h"
#include "avl_tree.h"

using namespace AvlTreeNameSpace;

namespace SortedSegmentsNameSpace {
	class SortedSegments {
	private:
		int count = 0;
		const string getFilePath();
		const int countSegments() const;
		const Key get(string filePath, int key) const;
	public:
		SortedSegments();
		const RecordInfo get(int key) const;
		
		void flushToDisk(vector<Key*> &keys);
	};
}