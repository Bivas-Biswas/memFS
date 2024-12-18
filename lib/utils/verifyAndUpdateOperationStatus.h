#ifndef VERIFY_AND_UPDATE_OPERATION_STATUS_UTILS_H
#define VERIFY_AND_UPDATE_OPERATION_STATUS_UTILS_H

#include <iostream>

#include "../global.h"

using namespace std;

int verifyAndUpdateOperationStatus(int operationIdx = -1, int operationCode = -1)
{
    if (operationIdx != -1)
    {
        if (multiFileOperationStatus)
        {
            multiFileOperationStatus[operationIdx] = operationCode;
        }
        ++atomicCounter;
        // std::cout << atomicCounter.load() << std::endl;
    }

    return operationCode;
}

#endif