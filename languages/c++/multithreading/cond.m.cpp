#include <thread>
#include <mutex>
#include <deque>
#include <chrono>
#include <iostream>
#include <condition_variable>

std::deque<int> q;
std::mutex mu;
std::condition_variable cond;

void function1_0()
{
    int count = 10;
    while (count > 0)
    {
        std::unique_lock<std::mutex> locker(mu);
        q.push_back(count);
        locker.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        --count;
    }
}

void function2_0()
{
    int data = 0;
    while (data != 1)
    {
        std::unique_lock<std::mutex> locker(mu);
        if (!q.empty()) 
        {
            data = q.back();
            q.pop_back();
            locker.unlock();
            std::cout << "t2 got a value from t1:" << data << std::endl;
        }
        else
        {
            // std::cout << "got nothing out" << std::endl;
            locker.unlock();
        }
    }
}

void test1()
{
    std::thread t1(function1_0);
    std::thread t2(function2_0);
    t1.join();
    t2.join();
}

void function2_1()
{
    int data = 0;
    while (data != 1)
    {
        std::unique_lock<std::mutex> locker(mu);
        if (!q.empty()) 
        {
            data = q.back();
            q.pop_back();
            locker.unlock();
            std::cout << "t2 got a value from t1:" << data << std::endl;
        }
        else
        {
            // std::cout << "got nothing out" << std::endl;
            locker.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            // you won't see 2 printed out
        }
    }
}

void test2()
{
    // you won't see 2 printed out
    std::thread t1(function1_0);
    std::thread t2(function2_1);
    t1.join();
    t2.join();
}

void function1_1()
{
    int count = 10;
    while (count > 0)
    {
        std::unique_lock<std::mutex> locker(mu);
        q.push_back(count);
        locker.unlock();
        cond.notify_one(); // notify one waiting thread, if there is one
        std::this_thread::sleep_for(std::chrono::seconds(1));
        --count;
    }
}

void function2_2()
{
    int data = 0;
    while (data != 1)
    {
        std::unique_lock<std::mutex> locker(mu);
        cond.wait(locker); // puts thread 2 in sleep, until notified by  t1

        // before it goes to sleep, it will unlock the locker, so other threads
        // can acquire the mutex.
        // once it is waken up by the notifier, it will lock again. That's the 
        // reason we have to use unique_lock, not lock_guard
        //
        // cond.wait(locker) is equivalent to the following pseudo code
        //
        // locker.unlock();
        // std::this_thread::sleep_for(...);
        //
        // # onwakeup
        // locker.lock(); // wait() exits;
        //

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
            std::cout << "spurious wakeup that did nothing" << std::endl;
        }
    }
}

void test3()
{
    std::thread t1(function1_1);
    std::thread t2(function2_2);
    t1.join();
    t2.join();
}

void function2_3()
{
    int data = 0;
    while (data != 1)
    {
        std::unique_lock<std::mutex> locker(mu);
        cond.wait(locker, [](){ return !q.empty(); }); // puts thread 2 in sleep, until notified by  t1

        // unlock locker, goes to sleep, 
        // on wakeup: 
        // lock, 
        // check pred
        // if not satisfied, unlock, sleep
        // else, proceed
        //
        // cond.wait(locker, pred);
        //
        // wakeup:
        //   locker.lock()
        //   if (pred) { locker.unlock(); goto wakeup; sleep_for(...); }
        //   else goto next;
        // next:
        //   ...
        //
        // or in another way:
        // locker.lock()
        // while (!pred)
        // {
        //     wait(locker); // unlock on sleep, lock again on wakeup
        // }
        data = q.back();
        q.pop_back();
        locker.unlock();
        std::cout << "t2 got a value from t1:" << data << std::endl;
    }
}

void test4()
{
    std::thread t1(function1_1);
    std::thread t2(function2_3);
    t1.join();
    t2.join();
}

int main(int argc, char* argv[])
{
    // test1(); due to t2 in busy-waiting state, one CPU core usage at 100%
    test2();
    // test3();
    // test4();
    return 0;
}
