#ifndef WRITE_OPERATION_H
#define WRITE_OPERATION_H

#include <iostream>
#include <chrono>

#include "../constants.h"
#include "../types.h"
#include "../global.h"
#include "../utils/verifyAndUpdateOperationStatus.h"

using namespace std;

int fwrite(std::string fileName, std::string fileContent,  scTime timeStamp, int operationIdx = -1)
{
    FileDescriptor *fileDescriptor = fileDescriptorTable->find(fileName);

    if (fileDescriptor == nullptr)
    {
        return verifyAndUpdateOperationStatus(operationIdx, OPERATION_STATUS_NOT_EXIST);
    }

    if (fileContent.size() > MAX_FILE_SIZE)
    {
        return verifyAndUpdateOperationStatus(operationIdx, OPERATION_STATUS_FILE_SIZE_EXECCED);
    }

    if (timeStamp < fileDescriptor->last_modified)
    {
        return verifyAndUpdateOperationStatus(operationIdx, OPERATION_STATUS_WRITE_MISS_TIME);
    }

    fileDescriptor->file_content = fileContent;
    fileDescriptor->last_modified = timeStamp;
    fileDescriptor->size = fileContent.size();

    return verifyAndUpdateOperationStatus(operationIdx, OPERATION_STATUS_SUCCESS);
}

#endif