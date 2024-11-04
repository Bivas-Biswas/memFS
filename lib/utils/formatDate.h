#ifndef FORMATDATE_H
#define FORMATDATE_H

#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>

#include "../types.h"

std::string formatDate(const scTime &timePoint)
{
    // Convert to std::time_t, which represents time in seconds since epoch
    std::time_t timeT = std::chrono::system_clock::to_time_t(timePoint);

    // Convert to local time
    std::tm *tmPtr = std::localtime(&timeT);

    // Use an ostringstream to format the date into a string
    std::ostringstream oss;
    oss << std::put_time(tmPtr, "%d/%m/%Y");

    return oss.str(); // Return the formatted string
}

#endif