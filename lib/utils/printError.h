#ifndef PRINTERROR_UTILS_H
#define PRINTERROR_UTILS_H

#include <iostream>

#include "../constants.h"
#include "./printLine.h"

using namespace std;

void printError(std::string fileName, int errorCode)
{

    std::string errorMsg = "error: ";

    switch (errorCode)
    {
    case OPERATION_STATUS_DUPLICATE:
        errorMsg.append(fileName).append(" another file with same name exists!!");
        break;
    case OPERATION_STATUS_MEMORY_FULL:
        errorMsg.append(fileName).append(" memory is full");

    default:
        errorMsg.append("unkown error!");
        break;
    }

    printLine(errorMsg);
}

#endif