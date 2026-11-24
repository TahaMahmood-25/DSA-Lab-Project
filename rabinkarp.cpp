// rabinkarp.cpp - OWNER: Shahamat. Word-level Rabin-Karp rolling hash.
//
// Idea: first turn every word into a number. A chunk of k words is then treated like a
// k-digit number written in base BASE:   H = w0*BASE^(k-1) + w1*BASE^(k-2) + ... + w(k-1)
// Everything is done "mod MOD" so the number never gets too big.
#include <cstddef>
#include "rabinkarp.h"

static const unsigned long long MOD  = 1000000007ULL;  // big prime
static const unsigned long long BASE = 131ULL;         // base of the polynomial

// Word -> number: h = (h * 31 + letter) mod MOD for every letter.
unsigned long long hashWord(const std::string& w) {
    unsigned long long h = 0;
    for (size_t i = 0; i < w.size(); i++)
        h = (h * 31ULL + (unsigned char)w[i]) % MOD;
    return h;
}

// Hash of every window of k words, using the ROLLING trick:
// moving one word forward = remove the first word, shift everything, add the new word.
void computeChunkHashes(const WordList& wl, int k, ChunkHashes& out) {
    out.hashes = NULL;
    out.count = 0;
    int n = wl.count;
    if (k <= 0 || n < k) return;               // file too short for even one chunk

    out.count = n - k + 1;
    out.hashes = new unsigned long long[out.count];

    // 1) number for each word
    unsigned long long* wh = new unsigned long long[n];
    for (int i = 0; i < n; i++) wh[i] = hashWord(wl.words[i]);

    // 2) P = BASE^(k-1) mod MOD  (the "weight" of the first word in a window)
    unsigned long long P = 1;
    for (int i = 0; i < k - 1; i++) P = (P * BASE) % MOD;

    // 3) hash of the first window, calculated normally
    unsigned long long H = 0;
    for (int i = 0; i < k; i++) H = (H * BASE + wh[i]) % MOD;
    out.hashes[0] = H;

    // 4) slide the window one word at a time
    for (int i = 1; i < out.count; i++) {
        unsigned long long removed = (wh[i - 1] * P) % MOD;   // contribution of the word that leaves
        H = (H + MOD - removed) % MOD;                        // take it out
        H = (H * BASE + wh[i + k - 1]) % MOD;                 // shift and add the word that enters
        out.hashes[i] = H;
    }
    delete[] wh;
}

// The real Rabin-Karp check: equal hashes do not PROVE equal chunks, so compare the words.
bool chunksEqual(const WordList& a, int posA, const WordList& b, int posB, int k) {
    for (int i = 0; i < k; i++)
        if (a.words[posA + i] != b.words[posB + i]) return false;
    return true;
}

void freeChunkHashes(ChunkHashes& ch) {
    delete[] ch.hashes;
    ch.hashes = NULL;
    ch.count = 0;
}
