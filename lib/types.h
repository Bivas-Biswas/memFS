#ifndef TYPES_H
#define TYPES_H

#include <iostream>
#include <vector>
#include <functional>
#include <chrono>
using namespace std;

typedef std::vector<std::string> SplitVect;
typedef std::chrono::time_point<std::chrono::system_clock> scTime;
typedef std::function<void ()> OperationTask;

typedef struct
{
    std::string name;
    std::string file_content;
    int size;
    scTime last_modified;
    scTime created_at;
    bool isLocked;
} FileDescriptor;

typedef struct
{
    std::string type;
    void (*run)(SplitVect);
} Operation;

#endif