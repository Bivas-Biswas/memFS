#ifndef DELETE_PARSER_H
#define DELETE_PARSER_H

#include <iostream>

#include "../types.h"
#include "../operation/fdelete.h"
#include "../utils/printLine.h"
#include "../constants.h"
#include "../global.h"
#include "../utils/waitForOperationFinished.h"
#include "../utils/initilizeDarray.h"

using namespace std;

void printMultiErrorDelete(SplitVect inputs, int offset, int count);

void pdelete(SplitVect inputs)
{
    if (inputs.size() == 0)
    {
        printLine("error: provide a <file_name> with delete!!");
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

    multiFileOperationStatus = initilizeDarray(noOfFiles, OPERATION_STATUS_DEFAULT);

    for (int i = 0; i < noOfFiles; i++)
    {
        ready_queue->enqueue(
            [=]()
            {
                fdelete(inputs[i + offset], i);
            });
    }

    printMultiErrorDelete(inputs, offset, noOfFiles);

    delete[] multiFileOperationStatus;
    multiFileOperationStatus = nullptr;
}

void printMultiErrorDelete(SplitVect inputs, int offset, int count)
{
    std::string notExistFiles;
    std::string msg;
    int noOFNotExistFiles = 0;

    waitForOperationFinished(count, multiFileOperationStatus, OPERATION_STATUS_DEFAULT);

    // // to debug
    // for(int i=0; i < count; i++){
    //     int code = multiFileOperationStatus[i];
    //     std::cout<< "pdelete " << code << std::endl;
    // }

    for (int i = 0; i < count; i++)
    {
        int code = multiFileOperationStatus[i];
        int idx = i + offset;

        switch (code)
        {
        case OPERATION_STATUS_NOT_EXIST:
            if (!notExistFiles.empty())
            {
                notExistFiles.append(", ");
            }
            notExistFiles.append(inputs[idx]);
            noOFNotExistFiles++;
            break;

        default:
            break;
        }
    }

    if (notExistFiles.empty())
    {
        if (count == 1)
        {
            msg.append(inputs[0 + offset]);
            msg = "deleted successfully.";
        }
        else
        {
            msg = "successfully deleted the given files.";
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

        int totalError = noOFNotExistFiles;

        if (totalError < count)
        {
            msg.append(" remaining files deleted successfully.");
        }
    }

    printLine(msg);
}
#endif