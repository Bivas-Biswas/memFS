#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <chrono>
#include <functional>
#include <sys/types.h>
#include <sys/wait.h>

#include "../lib/global.h"
#include "../lib/threadpool.h"
#include "../lib/lockfreequeue.h"
#include "../lib/concurrentMap.h"
#include "../lib/types.h"
#include "../lib/operation/fcreate.h"
#include "../lib/operation/fdelete.h"
#include "../lib/operation/mexit.h"
#include "../lib/operation/fwrite.h"
#include "../lib/utils/initilizeDarray.h"
#include "../lib/utils/waitForOperationFinished.h"
#include "../lib/utils/formatDate.h"

#include "utils/getMemoryUsage.h"
#include "utils/getCPUUsages.h"

struct FileOperation
{
    int idx;
    std::string type;
    std::string filename;
    std::string content;
};

typedef std::vector<FileOperation> Operations;

Operations readFile(std::string inputFileName)
{
    std::ifstream inputFile("./10000/1.txt");
    if (!inputFile.is_open())
    {
        std::cerr << "Error opening file!" << std::endl;
        exit(-1);
    }

    std::vector<FileOperation> operations;
    std::string line;
    int i = 0;
    while (std::getline(inputFile, line))
    {
        FileOperation op;
        std::istringstream ss(line);

        ss >> op.type >> op.filename;
        if (op.type == "write")
        {
            std::getline(ss, op.content);
            op.content = op.content.substr(1);
        }
        else
        {
            op.content = "";
        }
        op.idx = i;
        operations.push_back(op);
        i++;
    }

    inputFile.close();

    return operations;
}

void run(Operations &operations, int noOfThread)
{
    auto start_time = std::chrono::high_resolution_clock::now();
    fileDescriptorTable = new ConcurrentMap<std::string, FileDescriptor *>(noOfThread);
    ready_queue = new LockFreeQueue<OperationTask>();
    threadPool = new ThreadPool(noOfThread, ready_queue);
    int noOfOp = operations.size();
    multiFileOperationStatus = initilizeDarray(noOfOp, OPERATION_STATUS_DEFAULT);

    int i = 0;
    for (const auto &op : operations)
    {
        OperationTask operationTask;

        if (op.type == "create")
        {
            operationTask = [=]()
            {
                return fcreate(op.filename, op.idx);
            };
        }

        else if (op.type == "write")
        {
            scTime timeStamp = std::chrono::system_clock::now();
            operationTask = [=]()
            {
                return fwrite(op.filename, op.content, timeStamp, op.idx);
            };
        }

        else if (op.type == "delete")
        {
            operationTask = [=]()
            {
                return fdelete(op.filename, op.idx);
            };
        }

        ready_queue->enqueue(operationTask);
        i++;
        // std::cout << i << std::endl;
    }

    waitForOperationFinished(noOfOp, multiFileOperationStatus, OPERATION_STATUS_DEFAULT);
    // std::cout << atomicCounter.load() << std::endl;

    // std::cout << "size\tcreated\t\tlast modified\tfilename" << std::endl;
    // fileDescriptorTable->traverse(
    //     [](auto key, auto value)
    //     {
    //         std::string lineText = std::to_string(value->size);
    //         lineText.append("\t");
    //         lineText.append(formatDate(value->created_at));
    //         lineText.append("\t");
    //         lineText.append(formatDate(value->last_modified));
    //         lineText.append("\t");
    //         lineText.append(value->name);
    //         printLine(lineText);
    //     });

    operations.clear();
    mexit();
    auto end_time = std::chrono::high_resolution_clock::now();
    auto latency = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    std::cout << "Latency: " << latency << " Milliseconds" << std::endl;
    std::cout << "CPU Usage: " << getCpuUsage() << " %\n";
    std::cout << "Memory Usage: " << getMemoryUsage() << " KB\n";

    exit(0);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file_name> <no_of_thread>" << std::endl;
        return 1;
    }

    std::string inputFile = argv[1];
    int noOfThread = std::stoi(argv[2]);

    Operations operations = readFile(inputFile);

    pid_t pid = fork();
    if (pid == -1)
    {
        std::cerr << "Failed to fork process." << std::endl;
        return 1;
    }
    else if (pid == 0)
    {
        run(operations, noOfThread);
        exit(0);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
    }

    return 0;
}