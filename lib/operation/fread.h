#ifndef READ_OPERATIONS_H

#define READ_OPERATIONS_H

#include <iostream>

#include "../types.h"
#include "../global.h"

using namespace std;

string fread(string fileName)
{
    FileDescriptor *fileDescriptor = fileDescriptorTable->find(fileName);

    if (fileDescriptor == nullptr)
    {
        std::string errorMsg = "error: ";
        errorMsg.append(fileName).append(" does not exist!!");

        throw std::runtime_error(errorMsg);
    }

    return fileDescriptor->file_content;
}

#endif