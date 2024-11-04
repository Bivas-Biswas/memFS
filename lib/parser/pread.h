#ifndef READ_PARSER_H
#define READ_PARSER_H

#include <iostream>

#include "../types.h"
#include "../constants.h"
#include "../utils/printLine.h"
#include "../operation/fread.h"

using namespace std;

void pread(SplitVect inputs){

    if(inputs.size() == 0){
        printLine("error: provide a <file_name> with read.");
    }

    std::string fileName = inputs[0];

    try
    {
        std::string fileContent = fread(fileName);
        printLine(fileContent);
        return;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return;
}

#endif