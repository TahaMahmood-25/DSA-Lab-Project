// menu.h - OWNER: Taha. Console menu and the glue that runs the whole analysis.
#ifndef MENU_H
#define MENU_H
#include "common.h"

void runMenu();   // main loop: 1 add file, 2 set k, 3 run analysis, 4 list files, 5 clear, 0 exit

// Builds hashes + hash table for all files, compares every pair, ranks with the heap and prints the report.
void runAnalysis(FileData files[], int fileCount, int k);
#endif
