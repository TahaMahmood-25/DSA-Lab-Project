// compare.cpp - OWNER: Shahamat. Compare two files using the shared hash table.
#include <iostream>
#include <cstddef>
#include "compare.h"
#include "rabinkarp.h"

// Decide which file is the "smaller" one (fewer chunks). Ties: a is the smaller.
static void pickSmaller(const FileData files[], int a, int b, int& small, int& large) {
    if (files[a].chunks.count <= files[b].chunks.count) { small = a; large = b; }
    else                                                { small = b; large = a; }
}

// Does chunk number c of file 'small' also exist in file 'large'?
// Returns the position of the match in 'large', or -1 if there is none.
// Steps: go to the bucket of the hash, look at entries of file 'large' with the same hash,
// and confirm with the real words (rules out hash collisions).
static int findChunkInFile(const FileData files[], int small, int c, int large, int k, const HashTable& t) {
    unsigned long long h = files[small].chunks.hashes[c];
    for (HashEntry* e = getBucket(t, h); e != NULL; e = e->next) {
        if (e->hash == h && e->fileId == large &&
            chunksEqual(files[small].words, c, files[large].words, e->position, k))
            return e->position;
    }
    return -1;
}

int countCommonChunks(const FileData files[], int a, int b, int k, const HashTable& t) {
    int small, large;
    pickSmaller(files, a, b, small, large);
    if (files[small].chunks.count == 0) return 0;
    int common = 0;
    for (int c = 0; c < files[small].chunks.count; c++)
        if (findChunkInFile(files, small, c, large, k, t) >= 0) common++;
    return common;
}

double similarityPercent(int common, int chunksA, int chunksB) {
    int smaller = (chunksA < chunksB) ? chunksA : chunksB;
    if (smaller <= 0) return 0.0;
    return 100.0 * (double)common / (double)smaller;
}

// Print the words from..to (word numbers start at 1) of a file; long passages are shortened.
static void printWords(const WordList& wl, int from, int to) {
    const int LIMIT = 25;
    int last = to;
    bool cut = false;
    if (to - from + 1 > LIMIT) { last = from + LIMIT - 1; cut = true; }
    for (int i = from; i <= last; i++) std::cout << wl.words[i] << (i < last ? " " : "");
    if (cut) std::cout << " ...";
}

// Mark which chunks of the smaller file are found in the other file. Neighbouring matching chunks
// overlap, so a run of matching chunks is one copied passage: chunks i..j cover words i..j+k-1.
void printMatchingParts(const FileData files[], int a, int b, int k, const HashTable& t) {
    int small, large;
    pickSmaller(files, a, b, small, large);
    int n = files[small].chunks.count;
    if (n == 0) return;

    int* matchPos = new int[n];                       // position in 'large', or -1
    for (int c = 0; c < n; c++) matchPos[c] = findChunkInFile(files, small, c, large, k, t);

    std::cout << "     Matching passages (words counted from 1):\n";
    int number = 0, i = 0;
    while (i < n) {
        if (matchPos[i] < 0) { i++; continue; }
        int j = i;
        while (j + 1 < n && matchPos[j + 1] >= 0) j++;      // extend the run
        number++;
        int firstWord = i, lastWord = j + k - 1;
        std::cout << "       " << number << ") " << files[small].name << " words " << firstWord + 1
                  << "-" << lastWord + 1 << "  (in " << files[large].name << " from word "
                  << matchPos[i] + 1 << "): \"";
        printWords(files[small].words, firstWord, lastWord);
        std::cout << "\"\n";
        i = j + 1;
    }
    delete[] matchPos;
}
