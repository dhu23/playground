#include <thread>
#include <string>
#include <iostream>
#include <fstream>
#include <mutex>

std::mutex mu;

// you can't protect the std::cout completely as it is a globle variable 
// that people can still access without using shared_print
void shared_print0(std::string msg, int id)
{
    mu.lock();
    // what if this line throws an exception, then it is locked forever
    std::cout << msg << id << std::endl;
    mu.unlock();
}

void shared_print(std::string msg, int id)
{
    std::lock_guard<std::mutex> guard(mu); // RAII
    std::cout << msg << id << std::endl;
}

// a more realistic example of using mutex would be as follows:

class LogFile
{
    std::mutex m_mutex;
    std::ofstream f;
public:
    LogFile() 
    {
        f.open("log.txt");
    }
    void shared_print(std::string id, int value)
    {
        std::lock_guard<std::mutex> locker(m_mutex);

        // or with unique_lock
        // std::unique_lock<mutex> locker(m_mutex);
        // and once the work is done, unlock it: locker.unlock();
        //
        // or even:
        // std::unique_lock<mutex> locker(m_mutex, std::defer_lock);
        //
        // then locker.lock();
        // locker.unlocker()
        //
        // And this mutex wrapper classes are not copyable, but moveable

        // f << "from " << id << ":" << value << std::endl;
        // for easier testing:
        std::cout << "from " << id << ":" << value << std::endl;
    }
    // never return f to the outside world
    // std::ofstream& getStream() {return f;}
    // never pass f as an argument to user provided function
    // void processf(void fun(ofstream&)) {fun(f);}
};


// lazy initialization for the file stream
class LogFile2
{
    std::mutex _mu;
    std::mutex _mu_open;

    // a better solution is to use 
    // std::once_flag _flag;
    std::ofstream _f;
public:
    LogFile2() {}

    void shared_print(std::string id, int value) 
    {
        { 
            // this would be thread safe however it hinders performance
            // every time when shared_print is called it locks to check
            std::unique_lock<std::mutex> locker2(_mu_open);
            if (!_f.is_open())
            {
                // the following would cause file open twice when both
                // threads try to call shared_print at the same time
                // if std::unique_lock<std::mutex> locker2(_mu_open);
                _f.open("log.txt");
            }
        }

        // this will ensure file is opened once
        // std::call_once(_flag, [&](){_f.open("log.txt");});

        std::unique_lock<std::mutex> locker(_mu, std::defer_lock);
        locker.lock();
        _f << "From " << id << ": " << value << std::endl;
    }
};

/*
   class Stack
   {
   int* _data;
   std::mutex _mu;
   public:
// this is not thrread safe as pop and top are two separate calls
// thread safe version would be int& pop()
void pop(); // use int& pop(); however not exception safe
int& top();
};

void process(int v);

void function2(Stack& st)
{
// int v = st.top();
// st.pop();
int v = st.pop();
process(v);
}
*/

void function1(LogFile& log) 
{
    for (int i = 0; i > -100; --i)
    {
        // std::cout << "From t1:" << i << std::endl;
        log.shared_print(std::string("From t1:"), i);
    }
}

int main(int argc, char* argv[])
{
    LogFile log;
    std::thread t1(function1, std::ref(log));

    for (int i = 0; i < 100; ++i)
    {
        // std::cout << "From main:" << i << std::endl;
        log.shared_print(std::string("From main:"), i);
    }

    t1.join();
    return 0;
}


// avoid data race:
// 1, use mutex to synchronize data access
// 2, never leak a handle of data to outside
// 3, design interface appropriately
//
//
//
// to avoid deadlock: 
// 1) make sure every part of the program locks in the same order
// 2) std::lock(_mu1, _mu2); it uses locking algorithm
//    std::lock_guard<mutex> locker1(_mu1, std::adopt_lock); // _mu1 locked
//    std::lock_guard<mutex> locker2(_mu2, std::adopt_lock);
// 3) do you really need to use two mutex?! perfer locking a single mutex
// 4) don't lock and then call some user provided function
// 5) use std::lock() to lock more than one mutex
// 6) lock mutex in the same order. (mutex hierarchy)
//
// locking granularity:
// - fine-grained lock: protects small amount of data (deadlock risk)
// - coarse-grained lock: protects big amount of data (performance loss)
