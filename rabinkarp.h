// rabinkarp.h - OWNER: Shahamat. Word-level Rabin-Karp rolling hash.
#ifndef RABINKARP_H
#define RABINKARP_H
#include "common.h"

// Turns one word into a number (polynomial hash over its letters).
unsigned long long hashWord(const std::string& w);

// Rolling hash over every window of k words. Allocates out.hashes. count = n-k+1, or 0 if n < k.
void computeChunkHashes(const WordList& wl, int k, ChunkHashes& out);

// Real word-by-word check of two chunks (rules out hash collisions).
bool chunksEqual(const WordList& a, int posA, const WordList& b, int posB, int k);

void freeChunkHashes(ChunkHashes& ch);
#endif
