#ifndef WAIT_FOR_OPERATION_FINISHED_H
#define WAIT_FOR_OPERATION_FINISHED_H

#include <iostream>

void waitForOperationFinished(int count, int *multiFileOperationStatus, int defaultStatus)
{
    int noOperationDone = count;

    int i = 0;
    while (i < count)
    {
        int code = multiFileOperationStatus[i];
        if (code != defaultStatus)
        {
            // std::cout << "wati " << code << std::endl;
            noOperationDone--;
        }
        i++;
        if (i == count)
        {
            if (noOperationDone != 0)
            {
                i = 0;
            }
            else
            {
                return;
            }
        }
    }

    // while (noOperationDone < count)
    // {
    //     for (int i = 0; i < count; i++)
    //     {
    //         int code = multiFileOperationStatus[i];
    //         if (code != defaultStatus)
    //         {
    //             noOperationDone++;
    //         }
    //     }
    // }
}

#endif