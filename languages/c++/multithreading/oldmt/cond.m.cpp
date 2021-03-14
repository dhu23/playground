#include <thread>
#include <mutex>
#include <deque>
#include <chrono>
#include <iostream>

std::deque<int> q;
std::mutex mu;
std::condition_variable cond;

void function1()
{
    int count = 10;
    while (count > 0)
    {
        std::unique_lock<std::mutex> locker(mu);
        q.push_back(count);
        locker.unlock();

        cond.notify_one(); // notify one waiting thread if there is one

        std::this_thread::sleep_for(std::chrono::seconds(1));
        --count;
    }
}

void function2()
{
    int data = 0;
    while (data != 1) 
    {
        std::unique_lock<std::mutex> locker(mu);
        /*
        if (!q.empty())
        {
            data = q.back();
            q.pop_back();
            locker.unlock();
            std::cout << "t2 got a value from t1:" << data << std::endl;
        }
        else
        {
            locker.unlock();
            // std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        */

        // cond helps to make the two threads to run in a predefined order
        cond.wait(locker);
        data = q.back();
        q.pop_back();
        locker.unlock();
        std::cout << "t2 got a value from t1:" << data << std::endl;
    }
}

int main(int argc, char* argv[])
{
    std::thread t1(function1);
    std::thread t2(function2);
    t1.join();
    t2.join();
}
