#include <iostream>

#include "lib/parser/pmain.h"
#include "lib/global.h"
#include "lib/threadpool.h"
#include "lib/lockfreequeue.h"
#include "lib/concurrentMap.h"
#include "lib/types.h"

using namespace std;

int main()
{
    fileDescriptorTable = new ConcurrentMap<std::string, FileDescriptor *>(1);

    if (fileDescriptorTable == nullptr)
    {
        std::cerr << "Error: fileDescriptorTable is not initialized.\n";
        exit(-1);
    }

    ready_queue = new LockFreeQueue<OperationTask>();

    if (ready_queue == nullptr)
    {
        std::cerr << "Error: ready_queue is not initialized.\n";
        exit(-1);
    }

    threadPool = new ThreadPool(1, ready_queue);

    if (threadPool == nullptr)
    {
        std::cerr << "Error: threadpoll is not initialized.\n";
        exit(-1);
    }

    pmain();

    return 0;
}