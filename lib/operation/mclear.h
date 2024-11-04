#ifndef CLEAR_OPERATION_H
#define CLEAR_OPERATION_H

#include<iostream>
#include <cstdlib>

#include "../types.h"

using namespace std;

void clearTerminal() {
#ifdef _WIN32
    system("cls"); // Windows
#else
    system("clear"); // Linux/macOS
#endif
}

void mclear(SplitVect inputs){
    std::cout<< "I am in mclear" << std::endl;
    clearTerminal();
}

#endif