// menu.cpp - OWNER: Taha. Console menu + the function that runs the whole analysis.
#include <iostream>
#include <iomanip>
#include <string>
#include "menu.h"
#include "filereader.h"
#include "hashtable.h"
#include "rabinkarp.h"
#include "compare.h"
#include "maxheap.h"

// ---------- small input helpers ----------

// Ask for a whole number between lo and hi. Keeps asking until the input is valid.
// If the input stream is closed (Ctrl+D / end of redirected file) it returns lo.
static int readInt(const std::string& prompt, int lo, int hi) {
    while (true) {
        std::cout << prompt;
        std::string line;
        if (!std::getline(std::cin, line)) return lo;
        bool ok = !line.empty() && line.size() <= 6;
        for (size_t i = 0; i < line.size() && ok; i++)
            if (line[i] < '0' || line[i] > '9') ok = false;
        if (ok) {
            int v = 0;
            for (size_t i = 0; i < line.size(); i++) v = v * 10 + (line[i] - '0');
            if (v >= lo && v <= hi) return v;
        }
        std::cout << "  Please type a number from " << lo << " to " << hi << ".\n";
    }
}

static bool askYesNo(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    if (!std::getline(std::cin, line)) return false;
    return !line.empty() && (line[0] == 'y' || line[0] == 'Y');
}

static void printMenu(int fileCount, int k) {
    std::cout << "\n===== PLAGIARISM CHECKER =====\n"
              << " Files loaded: " << fileCount << "   Chunk size k: " << k << "\n"
              << " 1. Add a file\n"
              << " 2. Change chunk size k\n"
              << " 3. Run similarity check\n"
              << " 4. List loaded files\n"
              << " 5. Remove all files\n"
              << " 0. Exit\n";
}

// ---------- menu actions ----------

static void addFileOption(FileData files[], int& fileCount) {
    if (fileCount >= MAX_FILES) {
        std::cout << "  Maximum of " << MAX_FILES << " files reached.\n";
        return;
    }
    std::cout << "  Enter file path: ";
    std::string path;
    if (!std::getline(std::cin, path) || path.empty()) {
        std::cout << "  No path entered.\n";
        return;
    }
    int status = loadFile(path, files[fileCount]);
    if (status == 1)      std::cout << "  Error: cannot open \"" << path << "\" (file not found).\n";
    else if (status == 2) std::cout << "  Error: \"" << path << "\" is empty (no words).\n";
    else {
        std::cout << "  Loaded \"" << path << "\" (" << files[fileCount].words.count << " words).\n";
        fileCount++;
    }
}

static void listFiles(const FileData files[], int fileCount, int k) {
    if (fileCount == 0) { std::cout << "  No files loaded.\n"; return; }
    for (int i = 0; i < fileCount; i++) {
        int n = files[i].words.count;
        std::cout << "  [" << i + 1 << "] " << files[i].name << " - " << n << " words";
        if (n < k) std::cout << "  (shorter than k=" << k << ", it has no chunks)";
        std::cout << "\n";
    }
}

static void removeAllFiles(FileData files[], int& fileCount) {
    for (int i = 0; i < fileCount; i++) freeWordList(files[i].words);
    fileCount = 0;
    std::cout << "  All files removed.\n";
}

// ---------- the whole analysis ----------

void runAnalysis(FileData files[], int fileCount, int k) {
    if (fileCount < 2) {
        std::cout << "  Please load at least 2 files first.\n";
        return;
    }

    // Step 1: hash every file's chunks and put them in the shared hash table.
    HashTable table;
    initHashTable(table, 101);
    for (int f = 0; f < fileCount; f++) {
        computeChunkHashes(files[f].words, k, files[f].chunks);          // Shahamat
        for (int c = 0; c < files[f].chunks.count; c++)
            insertChunk(table, files[f].chunks.hashes[c], f, c);         // Taha
    }

    // Step 2: compare every pair of files and put the result in the max-heap.
    MaxHeap heap;
    initMaxHeap(heap, 16);
    for (int a = 0; a < fileCount; a++) {
        for (int b = a + 1; b < fileCount; b++) {
            PairResult p;
            p.fileA = a;
            p.fileB = b;
            p.commonChunks = countCommonChunks(files, a, b, k, table);
            p.similarity = similarityPercent(p.commonChunks, files[a].chunks.count, files[b].chunks.count);
            heapInsert(heap, p);
        }
    }

    bool showParts = askYesNo("  Also show the matching parts? (y/n): ");

    // Step 3: take pairs out of the heap, most similar first, and print them.
    std::cout << "\n========== RESULTS (most similar first) ==========\n";
    std::cout << std::fixed << std::setprecision(2);
    PairResult p;
    int rank = 1;
    while (heapExtractMax(heap, p)) {
        std::cout << "#" << rank++ << "  " << files[p.fileA].name << "  vs  " << files[p.fileB].name
                  << "  ->  " << p.similarity << "%  (" << p.commonChunks << " common chunks)\n";
        if (showParts && p.commonChunks > 0)
            printMatchingParts(files, p.fileA, p.fileB, k, table);       // Shahamat
    }

    // Step 4: free everything we created in this function.
    freeMaxHeap(heap);
    freeHashTable(table);
    for (int f = 0; f < fileCount; f++) freeChunkHashes(files[f].chunks);
}

// ---------- main loop ----------

void runMenu() {
    FileData files[MAX_FILES];
    int fileCount = 0;
    int k = DEFAULT_K;

    while (true) {
        printMenu(fileCount, k);
        int choice = readInt(" Choose: ", 0, 5);
        if (choice == 0) break;
        else if (choice == 1) addFileOption(files, fileCount);
        else if (choice == 2) k = readInt("  New k (1 to 50): ", 1, 50);
        else if (choice == 3) runAnalysis(files, fileCount, k);
        else if (choice == 4) listFiles(files, fileCount, k);
        else if (choice == 5) removeAllFiles(files, fileCount);
    }
    for (int i = 0; i < fileCount; i++) freeWordList(files[i].words);
    std::cout << "Goodbye.\n";
}
