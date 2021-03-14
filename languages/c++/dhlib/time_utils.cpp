#include "time_utils.h"
#include <ctime>

std::string getLocalTimeYYYYMMDD()
{
    time_t rawtime;
    std::time(&rawtime);

    struct tm* timeInfo;
    timeInfo = std::localtime(&rawtime);

    char buffer[80];
    std::strftime(buffer, 80, "%Y%m%d", timeInfo);
    return std::string(buffer);
}

std::string getLocalTimeYYYYMMDD_r()
{
    time_t rawtime;
    std::time(&rawtime);

    struct tm timeInfo;
    ::localtime_r(&rawtime, &timeInfo);
    
    char buffer[80];
    std::strftime(buffer, 80, "%Y%m%d", &timeInfo);
    return std::string(buffer);
}


// g++ time_util.cpp -std=c++17 -o time_utils.o
