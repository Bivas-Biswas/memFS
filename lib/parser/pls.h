#ifndef LS_PARSER_H
#define LS_PARSER_H

#include <iostream>
#include <unordered_map>
#include <string>
#include <iomanip>

#include "../types.h"
#include "../global.h"
#include "../utils/printLine.h"
#include "../utils/formatDate.h"

using namespace std;

void pls(SplitVect inputs)
{

    std::unordered_map<string, FileDescriptor *>::iterator itr;
    if (inputs.size() == 0)
    {

        fileDescriptorTable->traverse(
            [](auto key, auto value)
            {
                printLine(value->name);
            });

        // for (itr = fileDescriptorTable.begin(); itr != fileDescriptorTable.end(); itr++)
        // {
        //     printLine(itr->second->name);
        // }
        return;
    }

    if (inputs[0] == "-l")
    {
        std::cout << "size\tcreated\t\tlast modified\tfilename" << std::endl;
        fileDescriptorTable->traverse(
            [](auto key, auto value)
            {
                std::string lineText = std::to_string(value->size);
                lineText.append("\t");
                lineText.append(formatDate(value->created_at));
                lineText.append("\t");
                lineText.append(formatDate(value->last_modified));
                lineText.append("\t");
                lineText.append(value->name);
                printLine(lineText);
            });
    }
    else
    {

        printLine("error: given wrong options!!");
    }

    return;
}

#endif