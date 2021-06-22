#include <thread>
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

void build10(std::chrono::time_point<std::chrono::high_resolution_clock> start)
{
    std::this_thread::sleep_for(10s);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end -start;
    std::cout << "build took " << elapsed.count() << " ms\n";
}

void build2(std::chrono::time_point<std::chrono::high_resolution_clock> start)
{
    std::this_thread::sleep_for(2s);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end -start;
    std::cout << "build took " << elapsed.count() << " ms\n";
}

int main(int argc, char* argv[])
{
    std::cout << "Starting...\n" << std::flush;
    auto start = std::chrono::high_resolution_clock::now();
    std::thread t1(build10, start);


    std::this_thread::sleep_for(8s);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> elapsed = end-start;
    std::cout << "waited for 3s:" << elapsed.count() << " ms\n";


    std::thread t2(build2, start);
    
    t1.join();
    t2.join();


    return 0;
}
