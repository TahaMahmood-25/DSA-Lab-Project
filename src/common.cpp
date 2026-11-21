// common.cpp - OWNER: Taha. Our own growable array of words (we do not use std::vector).
#include "common.h"

// Start with an empty list: no memory yet.
void initWordList(WordList& wl) {
    wl.words = NULL;
    wl.count = 0;
    wl.capacity = 0;
}

// Add one word at the end. If the array is full, make a bigger one (double size) and copy.
void addWord(WordList& wl, const std::string& w) {
    if (wl.count == wl.capacity) {
        int newCap = (wl.capacity == 0) ? 16 : wl.capacity * 2;
        std::string* bigger = new std::string[newCap];
        for (int i = 0; i < wl.count; i++) bigger[i] = wl.words[i];
        delete[] wl.words;            // free the old array
        wl.words = bigger;
        wl.capacity = newCap;
    }
    wl.words[wl.count] = w;
    wl.count++;
}

// Give the memory back and reset to the empty state.
void freeWordList(WordList& wl) {
    delete[] wl.words;
    initWordList(wl);
}
