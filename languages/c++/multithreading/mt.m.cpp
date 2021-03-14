#include <iostream>
#include <thread>
#include <functional>
#include <algorithm>
#include <vector>
#include <string>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <future>

using std::cout;
using std::endl;

void threadFunction()
{
    for (int i = 0; i < 20; ++i) {
        cout << "thread function pointer executing..." << i << endl;
    }
}

void testThreadFuncPtr()
{
    std::thread threadObj(threadFunction);
    for (int i = 0; i < 30; ++i) {
        cout << "display from main thread..." << i << endl;
    }
    threadObj.join(); // program crash without it
    cout << "exiting main..." << endl;
}

void threadArgFunction(int x, const std::string& s)
{
    for (int i = 0; i < x; ++i) {
        cout << i << " " << s << endl;
    }
}

void testThreadArgFuncPtr()
{
    std::thread threadObj(threadArgFunction, 5, "teststr");
    threadObj.join();
}

void testThreadFuncObj()
{
    class Display
    {
    public:
        void operator()() const
        {
            for (int i = 0; i < 15; ++i) {
                cout << "Displaying func object executing..." << i << endl;
            }
        }
    };

    std::thread threadObj((Display())); // needs the extra ()
    for (int i = 0; i < 10; ++i) {
        cout << "Displaying from main thread..." << i << endl;
    }
    cout << "waiting for thread to complete" << endl;
    threadObj.join();
    cout << "exiting main..." << endl;
}

void testThreadLambda()
{
    std::thread threadObj(
        [] {
            for (int i = 0; i < 5; ++i) {
                cout << "display in lambda..." << i << endl;
            }
        }
    );
    for (int i = 0; i < 8; ++i) {
        cout << "display in main..." << i << endl;
    }
    threadObj.join();
    cout << "exiting..." << endl;
}

void testThreadId()
{
    auto threadFunc = []()
    {
        cout << "Inside thread ID=" << std::this_thread::get_id() << endl;
    };

    std::thread tobj1(threadFunc);
    std::thread tobj2(threadFunc);

    cout << "in main: thread 1 id=" << tobj1.get_id() << endl;
    cout << "in main: thread 2 id=" << tobj2.get_id() << endl;

    tobj1.join();
    tobj2.join();
}

void testListJoin()
{
    auto threadFunc = []()
    {
        cout << "Inside thread ID=" << std::this_thread::get_id() << endl;
    };
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.push_back(std::thread(threadFunc));
    }
    cout << "wait for all the work threads to finish" << endl;
    std::for_each(
        std::begin(threads), std::end(threads), 
        std::mem_fn(&std::thread::join));

    auto tryJoin = [](std::thread& t)
    {
        if (t.joinable()) {
            cout << "Joining thread id=" << t.get_id() << endl;
            t.join();
        } else {
            cout << "not joinable id=" << t.get_id() << endl;
        }
    };
    std::for_each(std::begin(threads), std::end(threads), tryJoin);
    cout << "Exiting..." << endl;
}

// RAII wrapper that supports automatic join
class ThreadJoinRAII
{
    std::thread thread_;
public:
    ThreadJoinRAII(std::thread&& t):thread_(std::move(t)) 
    // ThreadJoinRAII()
    {
        cout << "ThreadJoinRAII ctor" << endl;
    }

    //ThreadJoinRAII(const ThreadJoinRAII& other) = delete;
    //ThreadJoinRAII& operator=(const ThreadJoinRAII& other) = delete;

    //ThreadJoinRAII(ThreadJoinRAII&& other):thread_(std::move(other.thread_))
    //{
    //    cout << "ThreadJoinRAII move ctor" << endl;
    //}
    //ThreadJoinRAII& operator=(ThreadJoinRAII&& other)
    //{
    //    cout << "ThreadJoinRAII move op" << endl;
    //    thread_ = std::move(other.thread_);
    //    return *this;
    //}
    
    ~ThreadJoinRAII()
    {
        cout << "in ThreadJoinRAII dtor" << endl;
        if (thread_.joinable()) { 
            thread_.join();
            cout << "join thread id = " << thread_.get_id() << endl;
        } else {
            cout << "thread not joinable. id=" << thread_.get_id() << endl;
        }
    }
};

// RAII wrapper that supports automatic detach
class ThreadDetachRAII
{
    std::thread thread_;
public:
    ThreadDetachRAII(std::thread&& t):thread_(std::move(t)) {
        cout << "ThreadDetachRAII ctor" << endl;
    }
    
    ~ThreadDetachRAII()
    {
        cout << "in ThreadDetachRAII dtor" << endl;
        if (thread_.joinable()) {
            thread_.detach();
            cout << "detach thread id = " << thread_.get_id() << endl;
        } else {
            cout << "thread not joinable. id=" << thread_.get_id() << endl;
        }
    }
};

class Something
{
    std::vector<int> vec_;
public:
    Something(std::vector<int>&& v):vec_(std::move(v)) {
        cout << "Something ctor" << endl;
    }

};

template<typename T>
struct TD;

void testRAII()
{
    ThreadJoinRAII t1{std::thread(threadFunction)};
    ThreadDetachRAII t2{std::thread(threadFunction)};
    // Something s1(std::vector<int>(3, 4));
}

std::ostream& printNow(std::ostream& os)
{
    using Clock = std::chrono::high_resolution_clock;
    os << "time[" << Clock::now().time_since_epoch().count() << "]";
    return os;
}


void testSleepfor()
{
    std::thread threadObj(threadArgFunction, 35, "teststr");
    cout << "before main sleep. ";
    printNow(cout);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    cout << "after main sleep. ";
    printNow(cout);
    threadArgFunction(20, "mainteststr");
    threadObj.join();
}

void threadFuncWithRValue(int&& x)
{
    cout << "Inside RValue Func thread before x=" << x << endl;
    ++x;
    cout << "Inside RValue Func thread after x=" << x << endl;
}

void threadFuncWithConstRef(const int& x)
{
    int& y = const_cast<int&>(x);
    cout << "Inside ConstRef Func thread before y=" << y << endl;
    ++y;
    cout << "Inside ConstRef Func thread after y=" << y << endl;
}

void threadFuncWithPointer(int* p)
{
    cout << "Inside Ptr Func thread before *p=" << *p << endl;
    ++(*p);
    cout << "Inside Ptr Func thread after *p=" << *p << endl;
}

void testPassing()
{
    int x1 = 9;
    int x2 = 9;
    int x3 = 9;

    struct Dummy
    {
        void run(int x) const
        {
            cout << "Running Dummy::run with input x=" << x << endl;
        }
    };
    Dummy d;

    cout << "main thread: start x1=" << x1 << endl;
    cout << "main thread: start x2=" << x2 << endl;
    cout << "main thread: start x3=" << x3 << endl;
    std::thread t1(threadFuncWithRValue, x1);
    // if x2, then it won't change in main thread
    std::thread t2(threadFuncWithConstRef, std::ref(x2)); 
    std::thread t3(threadFuncWithPointer, &x3);
    std::thread t4(&Dummy::run, &d, 5);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    cout << "main thread: after x1=" << x1 << endl;
    cout << "main thread: after x2=" << x2 << endl;
    cout << "main thread: after x3=" << x3 << endl;
}

class Wallet
{
    int money_ = 0;
public:
    int money() const { return money_; }
    void deposit(int cash) { 
        for (int i = 0; i < cash; ++i) { 
            ++money_; 
            // load money_ variable in register
            // increment the value
            // update variable money_ with the register result
        }
    }
};

class Wallet0
{
    int money_ = 0;
    mutable std::mutex mutex_;
public:
    int money() const { return money_; }
    void deposit(int cash) { 
        mutex_.lock();
        for (int i = 0; i < cash; ++i) { 
            ++money_; 
            // load money_ variable in register
            // increment the value
            // update variable money_ with the register result
        }
        mutex_.unlock();
    }
    void deposit0(int cash) {
        std::lock_guard<std::mutex> lg(mutex_);
        for (int i = 0; i < cash; ++i) { ++money_; }
    }
};

template<typename W>
void testRaceCondition()
{
    W w;
    std::vector<std::thread> ts;
    for (int i = 0; i < 5; ++i) {
        ts.push_back(std::thread(&W::deposit, &w, 5000));
    }
    for (int i = 0; i < 5; ++i) {
        ts[i].join();
    }
    cout << "money in the wallet=" << w.money() << endl;
}


// thread 1
// perform some handshaking with server..
// wait for data to be loaded from XML by thread 2
// do processing on data loaded fro XML
//
// thread 2
// load data from XML
// notify another thread i.e. waiting for the message

// demo of necessity of event handling
class Application
{
    std::mutex mutex_;
    bool dataLoaded_ = false;
public:
    void loadData()
    {
        // simulate loading data by sleeping for 1 sec
        cout << "Loading Data from XML..." << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::lock_guard<std::mutex> lg(mutex_);
        dataLoaded_ = true;
    }

    bool isDataLoaded() const { return dataLoaded_; }

    void mainTask()
    {
        cout << "Do Something Handshaking..." << endl;
        mutex_.lock(); // I don't think you need to lock here

        while (!dataLoaded_) {
            mutex_.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            mutex_.lock();
        }
        mutex_.unlock();
        cout << "Do processing on loaded data" << endl;
    }
};

// thread 1 calls the wait on condition variable, which internally acquires
// the mutex and check if required condition is met or not
//
// if not then it releases the lock and waits for condition variable to get
// signaled (thread gets blocked). Condition variable's wait() function 
// provides both these operations in atomic manner.
//
// Another thread like thread 2 signals the condition variable when condition
// is met
//
// once condition variable get signaled the thread 1 which was waiting for it
// resumes. It then acquires the mutex lock again and checks if the 
// condition associated with Condition variable is actually met or if it is
// superior call. If more than one thread was waiting then notify_one will
// unblock only one thread
//
// if it was a superiors call then it again calls the wait() function

class Application0
{
    std::mutex mutex_;
    std::condition_variable condVar_;
    bool dataLoaded_ = false;
public:
    void loadData() {
        // make this thread sleep for 1 second
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        cout << "Loading Data from XML" << endl;

        // lock the data structure
        std::lock_guard<std::mutex> lg(mutex_);
        dataLoaded_ = true;
        condVar_.notify_one();
    }

    bool isDataLoaded() const { return dataLoaded_; }

    void mainTask0() {
        cout << "Do something Handshaking" << endl;
        std::unique_lock<std::mutex> ml(mutex_);
        condVar_.wait(ml, std::bind(&Application0::isDataLoaded, this));
        cout << "Do processing on loaded data" << endl;
    }

    void mainTask() {
        cout << "Do something Handshaking..." << endl;
        std::unique_lock<std::mutex> ml(mutex_);
        if (this->isDataLoaded()) {
            cout << "Do processing on loaded data: already loaded..." << endl;
        } else {
            cout << "putting the main task thread to block" << endl;
            condVar_.wait(ml, std::bind(&Application0::isDataLoaded, this));
            cout << "Do processing on loaded data" << endl;
        }
    }
};

template<typename A>
void testEventHandlingApp()
{
    A app;
    std::thread t2(&A::loadData, &app);
    std::thread t1(&A::mainTask, &app);
    t1.join();
    t2.join();
}


void testDeadlock()
{
    std::mutex m1;
    std::mutex m2;
    auto f1 = [&m1, &m2] 
    {
        std::lock_guard<std::mutex> lg1(m1);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::lock_guard<std::mutex> lg2(m2);
        // std::lock(m1, m2);
        std::cout << "running in f1" << std::endl;
        // m1.unlock();
        // m2.unlock();
    };
    auto f2 = [&m1, &m2] 
    {
        std::lock_guard<std::mutex> lg2(m2);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::lock_guard<std::mutex> lg1(m1);
        // std::lock(m1, m2);
        std::cout << "running in f2" << std::endl;
        // m1.unlock();
        // m2.unlock();
    };

    std::thread t1(f1);
    std::thread t2(f2);

    t1.join();
    t2.join();
}


void testFutureAndPromise()
{
    using namespace std::chrono_literals;
    auto f = [](std::promise<int>* promObj1 ,std::promise<std::string>* promObj2)
    {
        cout << "inside thread func f" << endl;
        promObj2->set_value("testval");
        cout << "sleeping for 5s" << endl;
        std::this_thread::sleep_for(5s);
        cout << "done sleeping, setting value -> 35" << endl;
        promObj1->set_value(35);
    };

    std::promise<int> promiseObj1;
    std::promise<std::string> promiseObj2;

    std::future<int> futureObj1 = promiseObj1.get_future();
    std::future<std::string> futureObj2 = promiseObj2.get_future();

    std::thread th(f, &promiseObj1, &promiseObj2);

    cout << "getting future obj value 2 ..." << endl;
    cout << futureObj2.get() << endl;
    cout << "getting future obj value 1 ..." << endl;
    cout << futureObj1.get() << endl;

    th.join();
}

void testAsync()
{
    using namespace std::chrono_literals;

    // std::async is a function template that accepts a callback 
    // (i.e. function or function object) as an argument and 
    // potentially executes them asynchronously
    // 
    // std::launch::async 
    //     it guarantees the asynchronous behavior (on separate thread)
    // std::launch::deferred
    //     non asynchronous. (called on get() demand)
    // std::launch::async | std::launch::deferred  (default behavior)
    //     depends. We have no control
    auto fetchDataFromDB = [](std::string recvdData)
    {
        std::this_thread::sleep_for(5s);
        return "DB_" + recvdData;
    };

    auto fetchDataFromFile = [](std::string recvdData)
    {
        std::this_thread::sleep_for(5s);
        return "File_" + recvdData;
    };

    using namespace std::chrono;
    
    auto test1 = [&fetchDataFromDB, &fetchDataFromFile]
    {
        system_clock::time_point start = system_clock::now();

        auto dbData = fetchDataFromDB("Data");
        auto fileData = fetchDataFromFile("Data");

        auto end = system_clock::now();

        auto diff = duration_cast<std::chrono::seconds>(end-start).count();
        cout << "Total time taken = " << diff << " seconds" << endl;

        // combine the data
        auto data = dbData + " :: " + fileData;
        cout << "Data = " << data << endl;
    };

    auto test11 = [&fetchDataFromDB, &fetchDataFromFile]
    {
    };

    auto test2 = [&fetchDataFromDB, &fetchDataFromFile]
    {
        system_clock::time_point start = system_clock::now();

        std::future<std::string> resultFromDB = std::async(
            std::launch::async, fetchDataFromDB, "Data");
        std::string fileData = fetchDataFromFile("Data");

        std::string dbData = resultFromDB.get();

        auto end = system_clock::now();

        auto diff = duration_cast<std::chrono::seconds>(end-start).count();
        cout << "Total Time taken = " << diff << " seconds" << endl;

        // combine the data
        auto data = dbData + " :: " + fileData;
        cout << "Data = " << data << endl;
    };

    test1();
    test2();
}

int main(int argc, char* argv[])
{
    // testThreadFuncPtr();
    // testThreadArgFuncPtr();
    // testThreadFuncObj();
    // testThreadLambda();
    // testThreadId();
    // testListJoin();
    // testRAII();
    // testSleepfor();
    // testPassing();
    // testRaceCondition<Wallet0>();
    // testRaceCondition<Wallet>();
    // testEventHandlingApp<Application>();
    // testEventHandlingApp<Application0>();
    // testDeadlock();
    testFutureAndPromise();
    testAsync();
    return 0;
}
