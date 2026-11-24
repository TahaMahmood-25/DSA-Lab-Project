// test_shahamat.cpp - checks Shahamat's module on its own.
#include <iostream>
#include <cstdlib>
#include "../src/common.h"
#include "../src/rabinkarp.h"
#include "../src/maxheap.h"

static int fails = 0;
static void check(bool cond, const std::string& name) {
    std::cout << (cond ? "PASS: " : "FAIL: ") << name << "\n";
    if (!cond) fails++;
}
static void fill(WordList& wl, const std::string& text) {   // helper: split on single spaces
    initWordList(wl);
    std::string w = "";
    for (size_t i = 0; i <= text.size(); i++) {
        if (i == text.size() || text[i] == ' ') { if (!w.empty()) addWord(wl, w); w = ""; }
        else w += text[i];
    }
}

int main() {
    // --- Rabin-Karp ---
    WordList a, one; ChunkHashes ca, c1;
    fill(a, "the cat sat on the mat and the cat sat on the mat");
    computeChunkHashes(a, 3, ca);
    check(ca.count == 11, "13 words, k=3 -> 11 chunks");
    // Rolling hash must equal the hash computed from scratch for that window.
    bool rollingOk = true;
    for (int i = 0; i < ca.count; i++) {
        WordList w; initWordList(w);
        for (int j = 0; j < 3; j++) addWord(w, a.words[i + j]);
        computeChunkHashes(w, 3, c1);                 // exactly one window -> computed directly
        if (c1.count != 1 || c1.hashes[0] != ca.hashes[i]) rollingOk = false;
        freeChunkHashes(c1); freeWordList(w);
    }
    check(rollingOk, "rolling hash == hash computed from scratch (all windows)");
    check(ca.hashes[0] == ca.hashes[7], "same words at positions 0 and 7 -> same hash");
    check(ca.hashes[0] != ca.hashes[1], "different chunks -> different hash");
    check(chunksEqual(a, 0, a, 7, 3) && !chunksEqual(a, 0, a, 1, 3), "chunksEqual checks the real words");
    freeChunkHashes(ca);
    fill(one, "only two");
    computeChunkHashes(one, 5, c1);
    check(c1.count == 0 && c1.hashes == NULL, "file shorter than k -> 0 chunks");
    freeChunkHashes(c1);
    computeChunkHashes(a, 13, c1);
    check(c1.count == 1, "k equal to word count (13) -> 1 chunk");
    freeChunkHashes(c1);
    freeWordList(a); freeWordList(one);

    // --- Max-heap ---
    MaxHeap h; initMaxHeap(h, 2);                      // tiny start to force growing
    check(heapIsEmpty(h), "new heap is empty");
    PairResult out;
    check(!heapExtractMax(h, out), "extract from empty heap returns false");
    srand(7);
    const int N = 1000;
    for (int i = 0; i < N; i++) {
        PairResult p; p.fileA = i; p.fileB = i + 1; p.commonChunks = i;
        p.similarity = (rand() % 10001) / 100.0;
        heapInsert(h, p);
    }
    check(h.size == N, "heap holds 1000 items");
    double prev = 1000; bool sorted = true; int got = 0;
    while (heapExtractMax(h, out)) { if (out.similarity > prev) sorted = false; prev = out.similarity; got++; }
    check(sorted && got == N, "items come out from highest to lowest similarity");
    freeMaxHeap(h);

    std::cout << (fails == 0 ? "\nALL TESTS PASSED\n" : "\nSOME TESTS FAILED\n");
    return fails;
}
