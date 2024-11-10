#ifndef multiFileOperationStatus_h
#define multiFileOperationStatus_h

#include<iostream>

#include "printLine.h"
#include "../global.h"

int *initilizeDarray(int size, int defaultValue=0){
    atomicCounter.store(0);
    int *darry = new int[size];

    for(int i = 0; i < size; i++){
        darry[i] = defaultValue;
    }

    if(!darry){
        printLine("Error: Darry is not initialized.");
        return nullptr;
    }

    return darry;
}
#endif