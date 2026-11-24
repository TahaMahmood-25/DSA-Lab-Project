// compare.h - OWNER: Shahamat. Compares two files using the shared hash table.
#ifndef COMPARE_H
#define COMPARE_H
#include "common.h"
#include "hashtable.h"

// How many chunks of the SMALLER file also appear in the other file (hash match + real word check).
int countCommonChunks(const FileData files[], int a, int b, int k, const HashTable& t);

// common / chunks of smaller file * 100. Returns 0 if the smaller file has no chunks.
double similarityPercent(int common, int chunksA, int chunksB);

// Prints the copied passages (runs of consecutive matching chunks merged into one passage).
void printMatchingParts(const FileData files[], int a, int b, int k, const HashTable& t);
#endif
