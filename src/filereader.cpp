// filereader.cpp - OWNER: Taha. Read a file -> clean it -> split it into words.
#include <fstream>
#include "filereader.h"

// Read the whole file into one string. Returns false if the file cannot be opened.
bool readFileText(const std::string& path, std::string& outText) {
    std::ifstream in(path.c_str(), std::ios::binary);
    if (!in) return false;
    outText = "";
    char c;
    while (in.get(c)) outText += c;
    return true;
}

// Keep only letters and digits (lowercase). Apostrophes are dropped so "don't" -> "dont".
// Everything else (space, comma, dot, newline, symbols, non-English bytes) becomes a single space.
std::string cleanText(const std::string& raw) {
    std::string result = "";
    for (size_t i = 0; i < raw.size(); i++) {
        unsigned char c = (unsigned char)raw[i];
        if (c >= 'A' && c <= 'Z')      result += (char)(c - 'A' + 'a');   // make lowercase
        else if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) result += (char)c;
        else if (c == '\'')            continue;                          // skip apostrophe
        else                           result += ' ';                     // separator
    }
    return result;
}

// Walk through the text; every run of non-space characters is one word.
void splitIntoWords(const std::string& clean, WordList& out) {
    std::string word = "";
    for (size_t i = 0; i < clean.size(); i++) {
        if (clean[i] == ' ') {
            if (!word.empty()) { addWord(out, word); word = ""; }
        } else {
            word += clean[i];
        }
    }
    if (!word.empty()) addWord(out, word);      // last word (no space after it)
}

// Full pipeline. Returns 0 = OK, 1 = file missing, 2 = file empty (no words).
int loadFile(const std::string& path, FileData& fd) {
    fd.name = path;
    initWordList(fd.words);
    fd.chunks.hashes = NULL;
    fd.chunks.count = 0;

    std::string raw;
    if (!readFileText(path, raw)) return 1;

    splitIntoWords(cleanText(raw), fd.words);
    if (fd.words.count == 0) {          // nothing useful inside
        freeWordList(fd.words);
        return 2;
    }
    return 0;
}
