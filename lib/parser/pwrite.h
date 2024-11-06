#ifndef WRITE_PARSER_H
#define WRITE_PARSER_H

#include <iostream>
#include <chrono>

#include "../types.h"
#include "../utils/printLine.h"
#include "../operation/fwrite.h"
#include "../constants.h"
#include "../global.h"
#include "../utils/waitForOperationFinished.h"
#include "../utils/initilizeDarray.h"

using namespace std;

void printMultiErrorWrite(SplitVect inputs, int offset, int count);

void pwrite(SplitVect inputs)
{
    if (inputs.size() <= 1)
    {
        printLine("error: provide a <file_name> and <file_content> with write!!");
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
    if (countGivenFiles != noOfFiles * 2)
    {
        printLine("Mismatch between <no_of_files> and actual given file count!!");
        return;
    }

    multiFileOperationStatus = initilizeDarray(noOfFiles, OPERATION_STATUS_DEFAULT);

    for (int i = 0; i < noOfFiles; i++)
    {
        scTime timeStamp = std::chrono::system_clock::now();
        ready_queue->enqueue(
            [=]()
            {
                fwrite(inputs[i * 2 + offset], inputs[i * 2 + offset + 1], timeStamp, i);
            });
    }

    printMultiErrorWrite(inputs, offset, noOfFiles);

    delete[] multiFileOperationStatus;
    multiFileOperationStatus = nullptr;
}

void printMultiErrorWrite(SplitVect inputs, int offset, int count)
{
    std::string msg;
    std::string notExistFiles;
    std::string sizeExecedFiles;
    std::string duplicateFiles;
    int noOfNotExistFiles = 0;
    int noOfSizeExecedFiles = 0;
    int noOfDuplicateFiles = 0;

    waitForOperationFinished(count, multiFileOperationStatus, OPERATION_STATUS_DEFAULT);

    // // to debug
    // for(int i=0; i < count; i++){
    //     int code = multiFileOperationStatus[i];
    //     std::cout<< "pwrite " << code << std::endl;
    // }

    for (int i = 0; i < count; i++)
    {
        int code = multiFileOperationStatus[i];
        int idx = i * 2 + offset;

        switch (code)
        {
        case OPERATION_STATUS_NOT_EXIST:
            if (!notExistFiles.empty())
            {
                notExistFiles.append(", ");
            }
            notExistFiles.append(inputs[idx]);
            noOfNotExistFiles++;
            break;

        case OPERATION_STATUS_FILE_SIZE_EXECCED:
            if (!sizeExecedFiles.empty())
            {
                sizeExecedFiles.append(", ");
            }
            sizeExecedFiles.append(inputs[idx]);
            noOfSizeExecedFiles++;
            break;
        case OPERATION_STATUS_WRITE_MISS_TIME:
            if (!duplicateFiles.empty())
            {
                duplicateFiles.append(", ");
            }
            duplicateFiles.append(inputs[idx]);
            noOfDuplicateFiles++;
            break;

        default:
            break;
        }
    }

    if (notExistFiles.empty() && sizeExecedFiles.empty() && duplicateFiles.empty())
    {
        if (count == 1)
        {
            msg = "successfully written to ";
            msg.append(inputs[0 + offset]);
            msg.append(".");
        }
        else
        {
            msg = "successfully written to the given files.";
        }
    }
    else
    {
        msg = "error: ";

        if (!notExistFiles.empty())
        {
            msg.append(count == 1 ? "file " : "files ");
            msg.append(notExistFiles);
            msg.append(" not exist.");
        }
        if (!sizeExecedFiles.empty())
        {
            msg.append(count == 1 ? "file " : "files ");
            msg.append(sizeExecedFiles);
            msg.append(" execed the size limt 4KB.");
        }
        if (!duplicateFiles.empty())
        {
            msg.append(count == 1 ? "file " : "files ");
            msg.append(duplicateFiles);
            msg.append(" have multiple write. write the most right content.");
        }

        int totalError = noOfNotExistFiles + noOfSizeExecedFiles + noOfDuplicateFiles;

        if (totalError < count)
        {
            msg.append(" remaining files write successfully.");
        }
    }

    printLine(msg);
}

#endif