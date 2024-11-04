#ifndef WRITE_OPERATION_H
#define WRITE_OPERATION_H

#include <iostream>
#include <chrono>

#include "../constants.h"
#include "../types.h"
#include "../global.h"
#include "../utils/verifyAndUpdateOperationStatus.h"

using namespace std;

int fwrite(std::string fileName, std::string fileContent, int operationIdx = -1)
{
    if (fileDescriptorTable.find(fileName) == fileDescriptorTable.end())
    {
        return verifyAndUpdateOperationStatus(operationIdx, OPERATION_STATUS_NOT_EXIST);
    }

    if (fileContent.size() > MAX_FILE_SIZE)
    {
        return verifyAndUpdateOperationStatus(operationIdx, OPERATION_STATUS_FILE_SIZE_EXECCED);
    }

    FileDescriptor *fileDescripttor = fileDescriptorTable[fileName];
    fileDescripttor->file_content = fileContent;
    fileDescripttor->last_modified = std::chrono::system_clock::now();
    fileDescripttor->size = fileContent.size();

    return verifyAndUpdateOperationStatus(operationIdx, OPERATION_STATUS_SUCCESS);
}

#endif