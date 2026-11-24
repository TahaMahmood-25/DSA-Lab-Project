// maxheap.cpp - OWNER: Shahamat. Max-heap stored in an array.
// For the item at index i:  parent = (i-1)/2,  left child = 2i+1,  right child = 2i+2.
// Rule: a parent is always "better" than its children, so the best pair is at index 0.
#include <cstddef>
#include "maxheap.h"

// Is x ranked before y? Higher similarity first; ties are broken so the output is always the same.
static bool ranksHigher(const PairResult& x, const PairResult& y) {
    if (x.similarity != y.similarity)     return x.similarity > y.similarity;
    if (x.commonChunks != y.commonChunks) return x.commonChunks > y.commonChunks;
    if (x.fileA != y.fileA)               return x.fileA < y.fileA;
    return x.fileB < y.fileB;
}

static void swapPairs(PairResult& x, PairResult& y) {
    PairResult tmp = x; x = y; y = tmp;
}

// New item sits at the bottom: swap it with its parent while it is better than the parent.
static void siftUp(MaxHeap& h, int i) {
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (ranksHigher(h.data[i], h.data[parent])) { swapPairs(h.data[i], h.data[parent]); i = parent; }
        else break;
    }
}

// Item put at the top: swap it with its better child until it is better than both children.
static void siftDown(MaxHeap& h, int i) {
    while (true) {
        int left = 2 * i + 1, right = 2 * i + 2, best = i;
        if (left < h.size && ranksHigher(h.data[left], h.data[best]))   best = left;
        if (right < h.size && ranksHigher(h.data[right], h.data[best])) best = right;
        if (best == i) break;
        swapPairs(h.data[i], h.data[best]);
        i = best;
    }
}

void initMaxHeap(MaxHeap& h, int capacity) {
    if (capacity < 1) capacity = 1;
    h.data = new PairResult[capacity];
    h.size = 0;
    h.capacity = capacity;
}

void heapInsert(MaxHeap& h, const PairResult& p) {
    if (h.size == h.capacity) {                       // full -> double the array
        PairResult* bigger = new PairResult[h.capacity * 2];
        for (int i = 0; i < h.size; i++) bigger[i] = h.data[i];
        delete[] h.data;
        h.data = bigger;
        h.capacity *= 2;
    }
    h.data[h.size] = p;
    h.size++;
    siftUp(h, h.size - 1);
}

bool heapExtractMax(MaxHeap& h, PairResult& out) {
    if (h.size == 0) return false;
    out = h.data[0];                                  // the best one
    h.size--;
    h.data[0] = h.data[h.size];                       // move the last item to the top
    siftDown(h, 0);                                   // and let it sink to its place
    return true;
}

bool heapIsEmpty(const MaxHeap& h) { return h.size == 0; }

void freeMaxHeap(MaxHeap& h) {
    delete[] h.data;
    h.data = NULL;
    h.size = 0;
    h.capacity = 0;
}
