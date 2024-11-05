#ifndef CREATE_PARSER_H
#define CREATE_PARSER_H

#include <iostream>

#include "../types.h"
#include "../operation/fcreate.h"
#include "../utils/printLine.h"
#include "../utils/waitForOperationFinished.h"
#include "../constants.h"
#include "../global.h"
#include "../utils/initilizeDarray.h"

using namespace std;

void printMultiErrorCreate(SplitVect inputs, int offset, int count);

void pcreate(SplitVect inputs)
{
    if (inputs.size() == 0)
    {
        printLine("error: provide a <file_name> with create!!");
        return;
    }

    int noOfFiles = 1;
    int offset = 0;

    if (inputs[0] == "-n")
    {
        size_t pos;
        noOfFiles = std::stoi(inputs[1], &pos);

        if (pos != inputs[1].size())
        {
            printLine("Invalid parameter <no_of_files>!!");
            return;
        }

        offset = 2;
    }

    int countGivenFiles = inputs.size() - offset;
    if (countGivenFiles != noOfFiles)
    {
        printLine("Missmatch between <no_of_files> and actual given file count!!");
        return;
    }

    multiFileOperationStatus = initilizeDarray(noOfFiles);

    for (int i = 0; i < noOfFiles; i++)
    {
        ready_queue->enqueue(
            [=]()
            {
                fcreate(inputs[i + offset], i);
            });
    }

    printMultiErrorCreate(inputs, offset, noOfFiles);

    delete[] multiFileOperationStatus;
    multiFileOperationStatus = nullptr;
}

void printMultiErrorCreate(SplitVect inputs, int offset, int count)
{
    std::string duplicateFiles;
    std::string memoryFullFiles;
    std::string msg;
    int noOfDuplicateFiles = 0;
    int noOfMemoryFullFiles = 0;

    waitForOperationFinished(count, multiFileOperationStatus, OPERATION_STATUS_DEFAULT);

    // extract the result
    for (int i = 0; i < count; i++)
    {
        int code = multiFileOperationStatus[i];
        int idx = i + offset;

        switch (code)
        {
        case OPERATION_STATUS_DUPLICATE:
            if (!duplicateFiles.empty())
            {
                duplicateFiles.append(", ");
            }
            duplicateFiles.append(inputs[idx]);
            noOfDuplicateFiles++;
            break;

        case OPERATION_STATUS_MEMORY_FULL:
            if (!memoryFullFiles.empty())
            {
                memoryFullFiles.append(", ");
            }
            memoryFullFiles.append(inputs[idx]);
            noOfMemoryFullFiles++;
            break;

        default:
            break;
        }
    }

    if (memoryFullFiles.empty() && duplicateFiles.empty())
    {
        msg = count == 1 ? "file created successfully." : "files created successfully.";
    }
    else
    {
        msg = "error: ";

        if (!memoryFullFiles.empty())
        {
            msg.append(count == 1 ? "file " : "files ");
            msg.append(memoryFullFiles);
            msg.append("can not be created memory is full.");
        }
        if (!duplicateFiles.empty())
        {
            msg.append(count == 1 ? "file " : "files ");
            msg.append(duplicateFiles);
            msg.append(" already exist.");
        }

        int totalError = noOfDuplicateFiles + noOfMemoryFullFiles;

        if (totalError < count)
        {
            msg.append(" remaining files created successfully.");
        }
    }

    printLine(msg);
}

#endif