#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

// Function to get memory usage of the current process
long getMemoryUsage() {
    std::ifstream status_file("/proc/self/status");
    if (!status_file.is_open()) {
        std::cerr << "Error opening /proc/self/status" << std::endl;
        return -1;
    }

    std::string line;
    while (getline(status_file, line)) {
        if (line.rfind("VmRSS:", 0) == 0) { // Find Resident Set Size (RSS)
            long memory;
            std::istringstream iss(line);
            std::string label;
            iss >> label >> memory; // memory is in kilobytes
            return memory;
        }
    }
    return -1;
}
