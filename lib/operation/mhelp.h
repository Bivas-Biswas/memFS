#ifndef HELP_OPERATION_H
#define HELP_OPERATION_H

#include<iostream>

#include "../utils/printLine.h"
#include "../types.h"

using namespace std;

void mhelp(SplitVect inputs){
    printLine("Command Usage: ");
    printLine("to create a file    - create [-n (number of files)] [filenames (in order)]");
    printLine("to write to a file  - write [-n (number of files)] [<filename> \"<file_content>\"]");
    printLine("to remove a file    - delete [-n (number of files)] [filenames (in any order)]");
    printLine("to read a file      - read [filename]");
    printLine("to list all files   - ls, ls -l");
    printLine("to clear terminal   - clear");
    printLine("to exit from        - exit");
}

#endif