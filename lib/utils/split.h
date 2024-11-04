#ifndef SPLIT_UTILS_H
#define SPLIT_UTILS_H

#include <iostream>
#include <vector>

#include "../types.h"

SplitVect split(std::string str, char separator)
{
    SplitVect strings;
    int startIndex = 0, endIndex = 0;
    bool haveQuote = false;
    for (int i = 0; i <= str.size(); i++)
    {

        if (str[i] == '"')
        {
            // end quote
            if(haveQuote){
                endIndex--;
            }
            // start quote
            else{
                startIndex++;
            }

            haveQuote = !haveQuote;
            str[i] = ' ';
            continue;
        }

        if (haveQuote == false &&(str[i] == separator || i == str.size()))
        {
            endIndex = i;
            std::string temp;
            temp.append(str, startIndex, endIndex - startIndex);
            strings.push_back(temp);
            startIndex = endIndex + 1;
        }
    }

    return strings;
}

#endif