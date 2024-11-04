#ifndef EXIT_OPERATION_H
#define EXIT_OPERATION_H

#include <iostream>

#include "../types.h"
#include "../global.h"

using namespace std;

void mexit()
{

    std::unordered_map<string, FileDescriptor *>::iterator itr;

    for (itr = fileDescriptorTable.begin(); itr != fileDescriptorTable.end(); itr++)
    {
        delete itr->second;
    }

    fileDescriptorTable.clear();
    
    delete[] multiFileOperationStatus;

    exit(0);
}

#endif