#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h> // for sysconf

// Function to get CPU usage of the current process
double getCpuUsage() {
    std::ifstream stat_file("/proc/self/stat");
    if (!stat_file.is_open()) {
        std::cerr << "Error opening /proc/self/stat" << std::endl;
        return -1;
    }

    std::string ignore;
    long utime, stime;
    for (int i = 0; i < 13; ++i) stat_file >> ignore;  // Skip fields
    stat_file >> utime >> stime;

    long total_time = utime + stime;
    double uptime = sysconf(_SC_CLK_TCK); // Clock ticks per second

    return (total_time / uptime) * 100.0; // CPU usage as a percentage
}
