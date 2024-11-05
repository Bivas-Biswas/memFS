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
    FileDescriptor *fileDescriptor = fileDescriptorTable->erase(fileName);

    if (fileDescriptor == nullptr)
    {
        return verifyAndUpdateOperationStatus(operationIdx, OPERATION_STATUS_NOT_EXIST);
    }

    delete fileDescriptor;

    return verifyAndUpdateOperationStatus(operationIdx, OPERATION_STATUS_SUCCESS);
}

#endif