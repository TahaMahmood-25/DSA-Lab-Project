// hashtable.h - OWNER: Taha. Our own hash table: chaining + rehashing.
#ifndef HASHTABLE_H
#define HASHTABLE_H

struct HashEntry {                 // one node of a bucket's linked list
    unsigned long long hash;       // chunk hash
    int fileId;                    // which file the chunk came from
    int position;                  // index of the chunk's first word in that file
    HashEntry* next;
};

struct HashTable {
    HashEntry** buckets;           // array of linked-list heads
    int size;                      // number of buckets
    int count;                     // number of entries stored
};

void   initHashTable(HashTable& t, int initialSize);   // e.g. 101 buckets
void   insertChunk(HashTable& t, unsigned long long hash, int fileId, int position); // rehashes if loadFactor > 0.75
HashEntry* getBucket(const HashTable& t, unsigned long long hash); // head of the chain; caller walks ->next and checks entry->hash == hash
double loadFactor(const HashTable& t);                 // count / size
void   rehash(HashTable& t);                           // roughly doubles size, re-inserts every entry
void   freeHashTable(HashTable& t);                    // delete every node and the array
#endif
