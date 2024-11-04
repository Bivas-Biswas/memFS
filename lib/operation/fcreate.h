#ifndef CREATE_OPERATIONS_H

#define CREATE_OPERATIONS_H

#include <iostream>

#include "../constants.h"
#include "../types.h"
#include "../global.h"
#include "../utils/verifyAndUpdateOperationStatus.h"

using namespace std;

int fcreate(string fileName, int operationIdx = -1)
{
    if (fileDescriptorTable.find(fileName) != fileDescriptorTable.end())
    {
        return verifyAndUpdateOperationStatus(operationIdx, OPERATION_STATUS_DUPLICATE);
    }

    FileDescriptor *newFileDescriptor = new FileDescriptor;

    if (!newFileDescriptor)
    {
        return verifyAndUpdateOperationStatus(operationIdx, OPERATION_STATUS_MEMORY_FULL);
    }

    scTime timeStamp = std::chrono::system_clock::now();

    newFileDescriptor->name = fileName;
    newFileDescriptor->created_at = timeStamp;
    newFileDescriptor->last_modified = timeStamp;
    newFileDescriptor->size = 0;
    newFileDescriptor->file_content = "";
    newFileDescriptor->isLocked = false;

    fileDescriptorTable[fileName] = newFileDescriptor;

    return verifyAndUpdateOperationStatus(operationIdx, OPERATION_STATUS_SUCCESS);
}

#endif