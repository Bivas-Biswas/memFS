#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
#include <unordered_map>
#include <functional>

#include "types.h"
#include "lockfreequeue.h"
#include "threadpool.h"
#include "concurrentMap.h"

using namespace std;

ConcurrentMap<std::string, FileDescriptor *> *fileDescriptorTable = nullptr;
int *multiFileOperationStatus = nullptr;

LockFreeQueue<OperationTask> *ready_queue = nullptr;
ThreadPool *threadPool = nullptr;

std::atomic<int> atomicCounter(0);

#endif