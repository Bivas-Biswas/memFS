#ifndef WAIT_FOR_OPERATION_FINISHED_H
#define WAIT_FOR_OPERATION_FINISHED_H

#include <iostream>

#include "../global.h"

void waitForOperationFinished(int count, int *multiFileOperationStatus, int defaultStatus)
{
    int noOperationDone = count;

    int i = 0;
    while (atomicCounter < count)
    {
    }
}

#endif