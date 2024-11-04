#ifndef DELETE_OPERATIONS_H

#define DELETE_OPERATIONS_H

#include <iostream>

#include "../constants.h"
#include "../types.h"
#include "../global.h"
#include "../utils/verifyAndUpdateOperationStatus.h"

using namespace std;

int fdelete(std::string fileName, int operationIdx = -1)
{

    if (fileDescriptorTable.find(fileName) == fileDescriptorTable.end())
    {
        return verifyAndUpdateOperationStatus(operationIdx, OPERATION_STATUS_NOT_EXIST);
    }

    FileDescriptor *fileDescriptor = fileDescriptorTable[fileName];

    fileDescriptorTable.erase(fileName);

    delete fileDescriptor;

    return verifyAndUpdateOperationStatus(operationIdx, OPERATION_STATUS_SUCCESS);
}

#endif