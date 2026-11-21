// filereader.h - OWNER: Taha. Reading a file and turning it into clean words.
#ifndef FILEREADER_H
#define FILEREADER_H
#include "common.h"

// Reads the whole file into outText. Returns false if the file cannot be opened.
bool readFileText(const std::string& path, std::string& outText);

// Lowercases everything and replaces punctuation/symbols with spaces. Returns the cleaned text.
std::string cleanText(const std::string& raw);

// Splits cleaned text on spaces and appends each word to 'out'.
void splitIntoWords(const std::string& clean, WordList& out);

// Full pipeline for one file: read -> clean -> split. Fills fd.name and fd.words.
// Returns 0 = OK, 1 = file missing, 2 = file empty (no words).
int loadFile(const std::string& path, FileData& fd);
#endif
