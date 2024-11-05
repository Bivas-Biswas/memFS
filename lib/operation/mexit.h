#ifndef EXIT_OPERATION_H
#define EXIT_OPERATION_H

#include <iostream>

#include "../types.h"
#include "../global.h"

using namespace std;

void mexit()
{

    fileDescriptorTable->traverse([](auto first, auto second){
        delete second;
    });

    fileDescriptorTable->empty();
    
    delete fileDescriptorTable;
    delete[] multiFileOperationStatus;
    delete threadPool;
    delete ready_queue;

    multiFileOperationStatus = nullptr;
    fileDescriptorTable = nullptr;
    threadPool = nullptr;
    ready_queue = nullptr;

    exit(0);
}

#endif