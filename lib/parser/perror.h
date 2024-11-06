#ifndef ERROR_PARSER_H

#define ERROR_PARSER_H

#include <iostream>

#include "../types.h"
#include "../constants.h"
#include "../utils/printLine.h"

using namespace std;

void perror(int errorCode)
{
    switch (errorCode)
    {
    case UNKNOWN_OPERATION:
        printLine("Unknown Command!");
        printLine("use \"help\" command to see all supported commands");
        break;

    default:
        break;
    }
}

#endif