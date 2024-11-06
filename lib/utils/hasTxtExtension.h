#ifndef hasTxtExtension_H
#define hasTxtExtension_H

#include <iostream>
#include <string>

bool hasTxtExtension(const std::string& filename) {
    size_t dotPos = filename.rfind('.');
    
    if (dotPos != std::string::npos && filename.substr(dotPos + 1) == "txt") {
        return true;
    }
    return false;
}

#endif