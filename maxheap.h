// maxheap.h - OWNER: Shahamat. Our own max-heap (array based) ordered by PairResult.similarity.
#ifndef MAXHEAP_H
#define MAXHEAP_H
#include "common.h"

struct MaxHeap {
    PairResult* data;   // dynamic array
    int size;
    int capacity;
};

void initMaxHeap(MaxHeap& h, int capacity);
void heapInsert(MaxHeap& h, const PairResult& p);        // add at end, sift up (grows array if full)
bool heapExtractMax(MaxHeap& h, PairResult& out);        // false if empty; else largest similarity
bool heapIsEmpty(const MaxHeap& h);
void freeMaxHeap(MaxHeap& h);
#endif
