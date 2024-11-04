#ifndef READ_OPERATIONS_H

#define READ_OPERATIONS_H

#include <iostream>

#include "../types.h"
#include "../global.h"

using namespace std;

string fread(string fileName)
{

    if (fileDescriptorTable.find(fileName) == fileDescriptorTable.end())
    {
        std::string errorMsg = "error: ";
        errorMsg.append(fileName).append(" does not exist!!");

        throw std::runtime_error(errorMsg);
    }

    return fileDescriptorTable[fileName]->file_content;
}

#endif