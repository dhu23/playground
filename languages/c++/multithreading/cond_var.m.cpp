#include <iostream>
#include <condition_variable>
#include <mutex>
#include <thread>

std::mutex mutex_;
std::condition_variable condVar;

void doTheWork() {
    std::cout << "Processing shared data." << std::endl;
}

void waitingForWork() {
    std::cout << "Worker::Waiting for work." << std::endl;
    std::unique_lock<std::mutex> lck(mutex_);
    condVar.wait(lck);
    doTheWork();
    std::cout << "Work done." << std::endl;
}

void setDataReady() {
    std::cout << "Sender: Data is ready." << std::endl;
    condVar.notify_one();
}

int main() {
    std::cout << std::endl;
    std::thread t1(waitingForWork);
    std::thread t2(setDataReady);

    t1.join();
    t2.join();

    std::cout << std::endl;
}

// this code is susceptible to spurious wakeup and lost wakeup
// 
// Sometimes it prints out the following and gets stuck:
// Sender: Data is ready.
// Worker: Waiting for work.
//
// The problem is, if the sender sends its notificaiton before the receiver
// gets to a wait state, the notification gets lost. The C++ standard 
// describes condition variables as synchronization mechanism at the same 
// time: "The condition_variable class is a syncrhonization primitive that
// can be used to block a thread, or multiple threads at the same time".
// So the notification gets lost and the receiver is waiting and waiting...
//
// The predicate which gets rid of spurious wakeups will also help with
// lost wakeups. In case the predicate is true, the receiver is able to
// continue its work independently of the notification of the sender. 
