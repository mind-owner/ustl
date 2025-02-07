// This file is part of the uSTL library, an STL implementation.
//
// Copyright (c) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.

#include "stdtest.h"
#include "../uheap.h"

static void HeapSize (size_t nElements, size_t& layerWidth, size_t& nLayers)
{
    layerWidth = 0;
    nLayers = 0;
    for (size_t fts = 0; nElements > fts; fts += layerWidth) {
	layerWidth *= 2;
	if (!layerWidth)
	    ++ layerWidth;
	++ nLayers;
    }
}

static void PrintSpace (size_t n)
{
    for (uoff_t s = 0; s < n; ++ s)
	cout << ' ';
}

static void PrintHeap (const vector<int>& v)
{
    size_t maxWidth, nLayers;
    HeapSize (v.size(), maxWidth, nLayers);
    vector<int>::const_iterator src (v.begin());
    cout << ios::width(3);
    maxWidth *= 3;
    for (uoff_t i = 0; i < nLayers; ++ i) {
	const size_t w = 1 << i;
	const size_t spacing = max (0, int(maxWidth / w) - 3);
	PrintSpace (spacing / 2);
	for (uoff_t j = 0; j < w && src != v.end(); ++ j) {
	    cout << *src++;
	    if (j < w - 1 && src != v.end() - 1)
		PrintSpace (spacing);
	}
	cout << endl;
    }
}

static void TestHeapOperations (void)
{
    static const int c_Values [31] = {	// 31 values make a full 4-layer tree
	93, 12, 90, 86, 83, 86, 17, 40, 72, 36, 68, 42, 62, 67, 63, 65,
	26, 26, 49, 21, 11, 62, 67, 27, 11, 30, 35, 23, 59, 35, 99
    };
    vector<int> v;
    v.reserve (VectorSize(c_Values));
    for (uoff_t i = 0; i < VectorSize(c_Values); ++ i) {
	v.push_back (c_Values[i]);
	push_heap (v.begin(), v.end());
	cout << "------------------------------------------------\n";
	if (!is_heap (v.begin(), v.end()))
	    cout << "Is NOT a heap\n";
	PrintHeap (v);
    }
    cout << "------------------------------------------------\n";
    cout << "make_heap on the full range:\n";
    v.resize (VectorSize (c_Values));
    copy (VectorRange(c_Values), v.begin());
    make_heap (v.begin(), v.end());
    PrintHeap (v);
    if (!is_heap (v.begin(), v.end()))
	cout << "Is NOT a heap\n";
    cout << "------------------------------------------------\n";
    cout << "pop_heap:\n";
    pop_heap (v.begin(), v.end());
    v.pop_back();
    PrintHeap (v);
    if (!is_heap (v.begin(), v.end()))
	cout << "Is NOT a heap\n";

    cout << "------------------------------------------------\n";
    cout << "sort_heap:\n";
    v.resize (VectorSize (c_Values));
    copy (VectorRange(c_Values), v.begin());
    make_heap (v.begin(), v.end());
    sort_heap (v.begin(), v.end());
    foreach (vector<int>::const_iterator, i, v)
	cout << *i;
    cout << endl;

    cout << "------------------------------------------------\n";
    cout << "priority_queue push and pop:\n";
    priority_queue<int> q;
    for (uoff_t i = 0; i < VectorSize(c_Values); ++ i)
	q.push (c_Values[i]);
    q.emplace (73);
    while (!q.empty()) {
	cout << q.top();
	q.pop();
    }
    cout << endl;
}

StdTestMain (TestHeapOperations)
