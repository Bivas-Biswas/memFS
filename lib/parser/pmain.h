#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>

#include "../utils/split.h"
#include "../constants.h"
#include "../types.h"
#include "./pls.h"
#include "./pcreate.h"
#include "./pdelete.h"
#include "./perror.h"
#include "./pread.h"
#include "./pwrite.h"
#include "./pexit.h"
#include "../operation/mclear.h"
#include "../operation/mhelp.h"
#include "../global.h"

using namespace std;

Operation operationMap[] = {
    {"create", pcreate},
    {"write", pwrite},
    {"delete", pdelete},
    {"read", pread},
    {"ls", pls},
    {"clear", mclear},
    {"exit", pexit},
    {"help", mhelp}};

bool verifyInput(std::string inputStr)
{
    if (inputStr.empty())
    {
        return true;
    }

    return false;
}

void checkOperattion(std::string inputStr)
{
    if (verifyInput(inputStr))
    {
        perror(UNKNOWN_OPERATION);
        return;
    }

    SplitVect inputs = split(inputStr, ' ');

    std::string opType = inputs.front();

    inputs.erase(inputs.begin());

    // searching the suitable operation
    int operationIdx = -1;
    for (auto &operation : operationMap)
    {
        if (operation.type == opType)
        {
            operation.run(inputs);
            return;
        }
    }

    perror(UNKNOWN_OPERATION);
}

void pmain()
{
    std::string str;

    std::cout << "memfs> ";
    std::getline(std::cin, str);

    checkOperattion(str);

    pmain();
}

#endif