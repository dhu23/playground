#include <thread>
#include <vector>
#include <iostream>
#include <thread>
#include <ctime>
#include "time_utils.h"


void threadUnsafeLocalTime()
{
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, 80, "%Y%m%d", timeinfo);
    std::string tradedate(buffer);
    // std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::cout << ("trade date:" + tradedate) << std::endl;

    timeinfo->tm_mday += 1;
    time_t ntime = mktime(timeinfo);
    timeinfo = localtime(&ntime);

    strftime(buffer, 80, "%Y%m%d", timeinfo);
    std::string settledate(buffer);
    std::cout << ("settle date:" + settledate) << std::endl;
}

void threadSafeLocalTime()
{
    time_t rawtime;
    struct tm timeinfo;
    char buffer[80];

    time(&rawtime);
    localtime_r(&rawtime, &timeinfo);

    strftime(buffer, 80, "%Y%m%d", &timeinfo);
    std::string tradedate(buffer);
    // std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::cout << ("trade date:" + tradedate) << std::endl;

    timeinfo.tm_mday += 1;
    time_t ntime = mktime(&timeinfo);
    localtime_r(&ntime, &timeinfo);

    strftime(buffer, 80, "%Y%m%d", &timeinfo);
    std::string settledate(buffer);
    std::cout << ("settle date:" + settledate) << std::endl;
}

int main(int argc, char* argv[])
{
    std::vector<std::thread> ts;
    for (int i = 0; i < 100; ++i)
    {
        ts.push_back(std::thread(threadUnsafeLocalTime));
    }
    for (int i = 0; i < 100; ++i)
    {
        ts[i].join();
    }
    std::cout << "------------------------------------" << std::endl;
    for (int i = 0; i < 100; ++i)
    {
        ts.push_back(std::thread(threadSafeLocalTime));
    }
    for (int i = 0; i < 100; ++i)
    {
        ts[i].join();
    }
}
