#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include "./types.h"

using namespace std;

std::unordered_map<std::string, FileDescriptor *> fileDescriptorTable;
int *multiFileOperationStatus = nullptr;

#endif