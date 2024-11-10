#ifndef EXIT_PARSER_H
#define EXIT_PARSER_H

#include <iostream>

#include "../types.h"
#include "../utils/printLine.h"
#include "../operation/mexit.h"

using namespace std;

void pexit(SplitVect inputs){
    printLine("exiting memFS");
    mexit();
    exit(0);
}

#endif