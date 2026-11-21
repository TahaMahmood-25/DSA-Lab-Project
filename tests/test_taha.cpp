// test_taha.cpp - checks Taha's module on its own (no Rabin-Karp / heap needed).
#include <iostream>
#include "../src/common.h"
#include "../src/filereader.h"
#include "../src/hashtable.h"

static int fails = 0;
static void check(bool cond, const std::string& name) {
    std::cout << (cond ? "PASS: " : "FAIL: ") << name << "\n";
    if (!cond) fails++;
}

int main() {
    // cleanText
    check(cleanText("Hello, World!") == "hello  world ", "cleanText lowercases and removes punctuation");
    check(cleanText("Don't") == "dont", "cleanText drops apostrophe");

    // WordList + splitIntoWords (grows past the first capacity of 16)
    WordList wl; initWordList(wl);
    splitIntoWords("  a  bb   ccc ", wl);
    check(wl.count == 3 && wl.words[0] == "a" && wl.words[2] == "ccc", "splitIntoWords ignores extra spaces");
    for (int i = 0; i < 100; i++) addWord(wl, "x");
    check(wl.count == 103 && wl.capacity >= 103, "WordList grows automatically");
    freeWordList(wl);
    check(wl.words == NULL && wl.count == 0, "freeWordList resets");

    // loadFile edge cases
    FileData fd;
    check(loadFile("data/does_not_exist.txt", fd) == 1, "missing file returns 1");
    check(loadFile("data/empty.txt", fd) == 2, "empty file returns 2");
    check(loadFile("data/one_word.txt", fd) == 0 && fd.words.count == 1 && fd.words.words[0] == "hello", "one-word file OK");
    freeWordList(fd.words);
    check(loadFile("data/normal.txt", fd) == 0 && fd.words.count == 23, "normal file has 23 words");
    freeWordList(fd.words);

    // hash table: insert many (forces several rehashes), then find every one
    HashTable t; initHashTable(t, 7);
    const int N = 5000;
    for (int i = 0; i < N; i++) insertChunk(t, (unsigned long long)i * 1000003ULL, i % 3, i);
    check(t.count == N, "table holds all entries");
    check(loadFactor(t) <= 0.75, "load factor kept <= 0.75 (rehash worked)");
    bool allFound = true;
    for (int i = 0; i < N; i++) {
        unsigned long long h = (unsigned long long)i * 1000003ULL;
        bool found = false;
        for (HashEntry* e = getBucket(t, h); e != NULL; e = e->next)
            if (e->hash == h && e->position == i && e->fileId == i % 3) found = true;
        if (!found) allFound = false;
    }
    check(allFound, "every inserted entry can be found again after rehashing");
    // two different hashes forced into the same bucket (collision)
    HashTable c; initHashTable(c, 10);
    insertChunk(c, 5, 0, 0); insertChunk(c, 15, 1, 1);   // 5 and 15 both -> bucket 5
    int seen = 0; for (HashEntry* e = getBucket(c, 5); e != NULL; e = e->next) seen++;
    check(seen == 2, "collision: both entries live in the same chain");
    freeHashTable(c);
    freeHashTable(t);
    std::cout << (fails == 0 ? "\nALL TESTS PASSED\n" : "\nSOME TESTS FAILED\n");
    return fails;
}
