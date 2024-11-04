#ifndef WRITE_PARSER_H
#define WRITE_PARSER_H

#include <iostream>

#include "../types.h"
#include "../utils/printLine.h"
#include "../operation/fwrite.h"
#include "../constants.h"

using namespace std;

void printMultiErrorWrite(SplitVect inputs, int offset, int count)
{
    std::string msg;
    std::string notExistFiles;
    std::string sizeExecedFiles;
    int noOfNotExistFiles = 0;
    int noOfSizeExecedFiles = 0;

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

        default:
            break;
        }
    }

    if (notExistFiles.empty() && sizeExecedFiles.empty())
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

        int totalError = noOfNotExistFiles + noOfSizeExecedFiles;

        if (totalError < count)
        {
            msg.append(" remaining files write successfully.");
        }
    }

    printLine(msg);
}

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

    multiFileOperationStatus = new int[noOfFiles];

    for (int i = 0; i < noOfFiles; i++)
    {

        multiFileOperationStatus[i] = OPERATION_STATUS_DEFAULT;
        fwrite(inputs[i * 2 + offset], inputs[i * 2 + offset + 1], i);
    }

    printMultiErrorWrite(inputs, offset, noOfFiles);
}

#endif