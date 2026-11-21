# Plagiarism Checker (C++)

Data Structures lab project - FAST NUCES Karachi, BAI-3A.
Members: Taha Mahmood (25K-0124), Syed Shahamat Shakeel (25K-0143)

Compares 2 or more text files, shows a similarity % for every pair, ranks the pairs from most to
least similar and prints the copied passages. Offline, no STL containers for the main structures.

## Build
Needs g++ (C++11).
    make                 (Linux / Windows with MinGW make)
or, without make:
    g++ -std=c++11 -Wall -Wextra -o plagcheck src/*.cpp

## Run
    ./plagcheck          (Windows: plagcheck.exe)

Menu: 1 add a file (type its path) | 2 change chunk size k (default 5) | 3 run the check
      4 list files | 5 remove all files | 0 exit
Option 3 asks whether to also print the matching passages.

## How it works
1. Read each file, lowercase, remove punctuation, split into words (own dynamic array).
2. Chunks of k words; Rabin-Karp rolling hash at word level (one new hash per word moved).
3. Every chunk hash goes into our own hash table (chaining, rehash when load factor > 0.75).
4. For a pair, each chunk of the smaller file is looked up in the table; on equal hashes the real
   words are compared (collision check). Similarity % = common chunks / chunks of smaller file x 100.
5. All pairs go into our own max-heap; extracting gives the most similar pair first.

## Files
src/common.*      WordList (dynamic array), shared structs            (Taha)
src/filereader.*  read, clean, split                                  (Taha)
src/hashtable.*   hash table with chaining + rehashing                (Taha)
src/menu.*        menu and runAnalysis                                (Taha)
src/rabinkarp.*   word hash, rolling chunk hash, collision check      (Shahamat)
src/compare.*     common chunks, similarity %, matching passages      (Shahamat)
src/maxheap.*     array-based max-heap                                (Shahamat)
tests/            unit tests, sample files (tests/data), run_tests.sh

## Tests
    bash tests/run_tests.sh
Runs the unit tests and the end-to-end cases (identical, reordered, unrelated, empty, one word,
missing file, large input).

## AI tools
Code was written with help from Claude (Anthropic); both members have read and can explain all of it.
