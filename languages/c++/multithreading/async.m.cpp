#include <future>
#include <chrono>
#include <iostream>

void factorial0(int n)
{
    int res = 1;
    for (int i = n; i > 1; --i) { res *= i; }
    std::cout << "fac(" << n << ") is " << res << std::endl;
}

void test0()
{
    std::thread t1(factorial0, 4);
    t1.join();
}

void factorial1(int n, int& x)
{
    int res = 1;
    while (n > 1) 
    {
        res *= n;
        --n;
    }
    std::cout << "factorial is " << res << std::endl;
    x = res;
}

void test1()
{
    int x;
    std::thread t1(factorial1, 4, std::ref(x));
    t1.join();
    std::cout << "result is " << x << std::endl;

    // now since x is shared between the parent and child thread, 
    // we should set up mutex to protect access
    // and since child thread has to be finished before parent thread uses x,
    // we need to have a condition variable
    //
    // it gets messy
}

int factorial2(int n)
{
    int res = 1;
    while (n > 1) 
    {
        res *= n;
        --n;
    }
    std::cout << "factorial is " << res << std::endl;
    return res;
}

void test2()
{
    std::future<int> fut = std::async(factorial2, 4);
    int x = fut.get();
    std::cout << "fac is " << x << std::endl;

    // x = fut.get(); // throws future_error no associated state
}

int factorial3(std::future<int>& f)
{
    int res = 1;
    int n = f.get();
    for (int i = n; i > 1; --i) { res *= i; }
    std::cout << "factorial3 is " << res << std::endl;
    return res;
}

void test3()
{
    std::promise<int> p;
    std::future<int> f = p.get_future();

    std::future<int> fut = std::async(std::launch::async, factorial3, std::ref(f));

    std::this_thread::sleep_for(std::chrono::seconds(1));

    p.set_value(4);
    int x = fut.get();
    std::cout << "get from child task:" << x << std::endl;
}

// promise and future are move-only objects
int factorial4_0(std::shared_future<int>& f)
{
    int res = 1;
    int n = f.get();
    for (int i = n; i > 1; --i) { res *= i; }
    std::cout << "factorial3 is " << res << std::endl;
    return res;
}
int factorial4_1(std::shared_future<int> f)
{
    int res = 1;
    int n = f.get();
    for (int i = n; i > 1; --i) { res *= i; }
    std::cout << "factorial3 is " << res << std::endl;
    return res;
}

void test4()
{
    std::promise<int> p;
    std::future<int> f = p.get_future();

    // cannot pass the same future to all the threads because f can only get() once
    std::shared_future<int> sf = f.share();
    std::future<int> fut1 = std::async(factorial4_1, sf); // sf supports copy
    std::future<int> fut2 = std::async(factorial4_1, sf);
    std::future<int> fut3 = std::async(factorial4_0, std::ref(sf));

    p.set_value(4);
}

// ----------------------------- packaged_task -----------------------------

void test5()
{
    std::packaged_task<int(int)> p(factorial2);
    std::future<int> fut = p.get_future();
    p(4);

    int x = fut.get();
    std::cout << "get result : " << x << std::endl;
}

int main(int argc, char* argv[])
{
    // test0();
    // test1();
    // test2();
    // test3();
    test4();
    test5();
    return 0;
}
