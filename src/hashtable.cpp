// hashtable.cpp - OWNER: Taha. Hash table with chaining (linked lists) and rehashing.
#include <cstddef>      // for NULL
#include "hashtable.h"

// Which bucket does this hash belong to? Remainder after dividing by the number of buckets.
static int bucketIndex(const HashTable& t, unsigned long long hash) {
    return (int)(hash % (unsigned long long)t.size);
}

// Create 'initialSize' empty buckets (every head pointer is NULL = empty chain).
void initHashTable(HashTable& t, int initialSize) {
    t.size = initialSize;
    t.count = 0;
    t.buckets = new HashEntry*[t.size];
    for (int i = 0; i < t.size; i++) t.buckets[i] = NULL;
}

double loadFactor(const HashTable& t) {
    return (double)t.count / (double)t.size;
}

// Make the table bigger and move every node to its new bucket.
// We only re-link the existing nodes, we do not create or copy them.
void rehash(HashTable& t) {
    int oldSize = t.size;
    HashEntry** oldBuckets = t.buckets;

    t.size = oldSize * 2 + 1;                       // roughly double
    t.buckets = new HashEntry*[t.size];
    for (int i = 0; i < t.size; i++) t.buckets[i] = NULL;

    for (int i = 0; i < oldSize; i++) {
        HashEntry* cur = oldBuckets[i];
        while (cur != NULL) {
            HashEntry* nextNode = cur->next;        // remember next before we change it
            int idx = bucketIndex(t, cur->hash);    // new bucket
            cur->next = t.buckets[idx];             // put at the front of the new chain
            t.buckets[idx] = cur;
            cur = nextNode;
        }
    }
    delete[] oldBuckets;
}

// Add one chunk record at the FRONT of its chain. Rehash if the table is getting full.
void insertChunk(HashTable& t, unsigned long long hash, int fileId, int position) {
    HashEntry* node = new HashEntry;
    node->hash = hash;
    node->fileId = fileId;
    node->position = position;

    int idx = bucketIndex(t, hash);
    node->next = t.buckets[idx];
    t.buckets[idx] = node;
    t.count++;

    if (loadFactor(t) > 0.75) rehash(t);
}

// Return the first node of the chain for this hash (may be NULL).
// The chain can also contain other hashes that landed in the same bucket, so the caller
// must check entry->hash == hash while walking.
HashEntry* getBucket(const HashTable& t, unsigned long long hash) {
    return t.buckets[bucketIndex(t, hash)];
}

// Delete every node, then the bucket array.
void freeHashTable(HashTable& t) {
    for (int i = 0; i < t.size; i++) {
        HashEntry* cur = t.buckets[i];
        while (cur != NULL) {
            HashEntry* nextNode = cur->next;
            delete cur;
            cur = nextNode;
        }
    }
    delete[] t.buckets;
    t.buckets = NULL;
    t.size = 0;
    t.count = 0;
}
