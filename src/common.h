// common.h - shared types used by BOTH modules. Agree on this first; do not change without telling your partner.
#ifndef COMMON_H
#define COMMON_H
#include <string>

const int DEFAULT_K  = 5;    // default words per chunk
const int MAX_FILES  = 50;   // max files the menu accepts

// ---- Our own dynamic array of words (NOT std::vector) ----  (owner: Taha, common.cpp)
struct WordList {
    std::string* words;   // heap array
    int count;            // words used
    int capacity;         // words allocated
};
void initWordList(WordList& wl);                         // empty list
void addWord(WordList& wl, const std::string& w);        // append, doubles capacity when full
void freeWordList(WordList& wl);                         // delete[] and reset

// ---- One hash per chunk of k words ----  (filled by Shahamat's computeChunkHashes)
struct ChunkHashes {
    unsigned long long* hashes;  // hashes[i] = hash of words i .. i+k-1
    int count;                   // = wordCount - k + 1 (or 0 if too short)
};

// ---- Everything we know about one input file ----
struct FileData {
    std::string name;     // file path typed by the user
    WordList    words;    // cleaned words
    ChunkHashes chunks;   // rolling hashes of the chunks
};

// ---- Result for one pair of files ----
struct PairResult {
    int    fileA, fileB;      // indexes in the files array (fileA < fileB)
    int    commonChunks;      // chunks of the smaller file found in the other file
    double similarity;        // 0..100
};
#endif
