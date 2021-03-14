#include <mutex>
#include <atomic>
#include <condition_variable>
#include <iostream>
#include <chrono>
#include <thread>
#include <string>

#include "util.h"

std::mutex mutex1, mutex2;
std::condition_variable cond_var;

bool cond = false;
std::atomic<bool> done{false};

void take_locks(int i) {
    using namespace std::chrono_literals;

    // if not done, keep trying to acquire locks every 1 second.
    while (!done) {
        std::cout << concat("t", i, ": is going to sleep for 1s\n");
        std::this_thread::sleep_for(1s);
        
        std::cout << concat("t", i, ": done sleeping. trying to grab two locks\n");
        std::unique_lock<std::mutex> lock1(mutex1, std::defer_lock);
        std::cout << concat("t", i, ":lock1 constructed\n");
        std::unique_lock<std::mutex> lock2(mutex2, std::defer_lock);
        std::cout << concat("t", i, ":lock2 constructed\n");
        // std::lock(lock1, lock2);
        lock1.lock(); // this gives better CPU usage than std::lock
        std::cout << concat("t", i, ":lock1.locked\n");
        lock2.lock();
        std::cout << concat("t", i, ":lock2.locked\n");

        std::cout << concat("t", i, ": grabbed the locks\n");
        
        lock1.unlock();
        lock2.unlock();
    }
}

void conditional_code() {
    std::unique_lock<std::mutex> lock1(mutex1, std::defer_lock);
    std::unique_lock<std::mutex> lock2(mutex2, std::defer_lock);
    std::lock(lock1, lock2);
    std::cout << "t4: lock1, lock2 both locked\n";

    std::cout << "t4: waiting \n";
    while (!cond) {
        std::cout << "t4: wait on cond_var. lock1 unlocked\n";
        cond_var.wait(lock1);
        std::cout << "t4: condition met \n";
    }
    std::cout << "t4: done\n";
}

int main()
{
    using namespace std::chrono_literals;

    std::thread t1(take_locks, 1), t2(take_locks, 2), t3(take_locks, 3);
    std::thread t4(conditional_code);

    std::cout << "threads started \n";
    std::this_thread::sleep_for(10s);
    std::cout << "main thread done sleeping for 10s\n";

    // high CPU usage before this line
    std::cout << "main thread done sleeping\n";
    std::unique_lock<std::mutex> lock1(mutex1);
    std::cout << "mutex1 locked \n";
    std::this_thread::sleep_for(5s);
    std::cout << "main thread done sleeping for 5s #1\n";

    std::cout << "setting condition/notify \n";
    cond = true;
    cond_var.notify_one();
    std::cout << "main thread start sleeping for 5s #2\n";
    std::this_thread::sleep_for(5s);
    std::cout << "main thread done sleeping for 5s #2\n";

    lock1.unlock();
    std::cout << "mutex1 unlocked \n";
    std::this_thread::sleep_for(6s);
    std::cout << "main thread done sleeping for 6s\n";

    done = true;
    t4.join();
    t3.join();
    t2.join();
    t1.join();

    return 0;
}
