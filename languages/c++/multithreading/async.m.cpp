#include <future>
#include <chrono>
#include <iostream>
#include <thread>

// added for dot product example
#include <random>
#include <vector>
#include <numeric>
#include <algorithm>

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

// async task that returns void
void doSomething(int x)
{
    std::cout << "received:" << x << std::endl;
}

void test6()
{
    std::future<void> f = std::async(doSomething, 5);
    f.get(); // do nothing. 
    // auto y = f.get() or void y = f.get() cannot compile
}

// Eager or lazy evaluation
void test7()
{
    auto begin = std::chrono::system_clock::now();
    auto asyncLazy = std::async(
        std::launch::deferred, 
        []{ return std::chrono::system_clock::now(); });
    auto asyncEager = std::async(
        std::launch::async, 
        []{ return std::chrono::system_clock::now(); });

    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto lazyStart = asyncLazy.get() - begin;
    auto eagerStart = asyncEager.get() - begin;

    auto lazyDuration = std::chrono::duration<double>(lazyStart).count();
    auto eagerDuration = std::chrono::duration<double>(eagerStart).count();

    std::cout 
        << "asyncLazy evaluated after: " << lazyDuration << " seconds" 
        << std::endl;

    std::cout 
        << "asyncEager evaluated after: " << eagerDuration << " seconds"
        << std::endl;

    std::cout << std::endl;
}

void test8()
{
    std::vector<std::future<int>> futs;
    for (auto i = 0; i < 4; ++i)
    {
        futs.emplace_back(std::async([idx=i] { return idx; }));
    }

    int ret = 0;
    std::for_each(
        futs.begin(), futs.end(), 
        [&ret](std::future<int>& x) 
        {
            ret += x.get();
            // std::cout << x.get() << std::endl; 
        });
    std::cout << "final:" << ret << std::endl; // prints out 6
}

// dot product async manner
// chop the two vectors into 4 pieces and calculate them independently
long long getDotProduct(std::vector<int>& v, std::vector<int>& w) 
{
    auto future1 = std::async(
        [&]
        {
            return std::inner_product(
                &v[0], &v[v.size()/4],
                &w[0], 0LL);
        });

    auto future2 = std::async(
        [&]
        {
            return std::inner_product(
                &v[v.size()/4], &v[v.size()/2],
                &w[v.size()/4], 0LL);
        });

    auto future3 = std::async(
        [&]
        {
            return std::inner_product(
                &v[v.size()/2], &v[v.size()*3/4],
                &w[v.size()/2], 0LL);
        });

    auto future4 = std::async(
        [&]
        {
            return std::inner_product(
                &v[v.size()*3/4], &v[v.size()],
                &w[v.size()*3/4], 0LL);
        });

    return future1.get() + future2.get() + future3.get() + future4.get();
}

long long getDotProdByN(
    const std::vector<int>& v, const std::vector<int>& w,
    size_t n)
{
    std::vector<std::future<long long>> futs;

    for (decltype(n) i = 0; i < n; ++i)
    {
        auto fut = std::async(
            [&,i=i] // to capture i by copy, otherwise there will be funny bugs
            {
                return std::inner_product(
                    &v[v.size()*i/n], &v[v.size()*(i+1)/n],
                    &w[v.size()*i/n], 0LL);
            });
        futs.emplace_back(std::move(fut));
    }

    long long ret = 0;
    for (decltype(n) idx = 0; idx < n; ++idx)
    {
        ret += futs[idx].get();
    }
    return ret;
}

template<typename T>
void printVec(std::vector<T>& vec)
{
    auto len = vec.size();
    decltype(len) count = 0;
    while (count < 10 && count < len)
    {
        std::cout << vec[count++] << ",";
    }
    if (len > 10) 
    {
        std::cout << "...";
    }
    std::cout << std::endl;
}

void test9()
{
    std::random_device seed;

    std::mt19937 engine(seed());

    std::uniform_int_distribution<int> dist(0, 100);

    const int NUM = 100000000;
    // const int NUM = 4;
    std::vector<int> v(NUM);
    std::vector<int> w(NUM);

    for (int i = 0; i < NUM; ++i)
    {
        v.push_back(dist(engine));
        w.push_back(dist(engine));
    }

    //printVec(v);
    //printVec(w);

    // when compiled with -O3, 
    // the huge difference between dur0 and dur disappears.

    std::chrono::system_clock::time_point start0 =
        std::chrono::system_clock::now();
    std::cout 
        << "inner_product(v, w):" 
        << std::inner_product(v.begin(), v.end(), w.begin(), 0LL) 
        << std::endl;
    std::chrono::duration<double> dur0 = 
        std::chrono::system_clock::now() - start0;
    std::cout << "original exeuction:" << dur0.count() << std::endl;


    std::chrono::system_clock::time_point start = 
        std::chrono::system_clock::now();
    std::cout << "getDotProduct(v, w):" << getDotProduct(v, w) << std::endl;
    std::chrono::duration<double> dur = 
        std::chrono::system_clock::now() - start;
    std::cout << "Parallel execution:" << dur.count() << std::endl;


    std::chrono::system_clock::time_point start1 = 
        std::chrono::system_clock::now();
    std::cout << "getDotProdBy4(v, w):" << getDotProdByN(v, w, 4) << std::endl;
    std::chrono::duration<double> dur1 = 
        std::chrono::system_clock::now() - start1;
    std::cout << "Parallel execution:" << dur1.count() << std::endl;


    std::chrono::system_clock::time_point start2 = 
        std::chrono::system_clock::now();
    std::cout << "getDotProdBy16(v, w):" << getDotProdByN(v, w, 16) << std::endl;
    std::chrono::duration<double> dur2 = 
        std::chrono::system_clock::now() - start2;
    std::cout << "Parallel execution:" << dur2.count() << std::endl;

    std::chrono::system_clock::time_point start3 = 
        std::chrono::system_clock::now();
    std::cout << "getDotProdBy128(v, w):" << getDotProdByN(v, w, 128) << std::endl;
    std::chrono::duration<double> dur3 = 
        std::chrono::system_clock::now() - start3;
    std::cout << "Parallel execution:" << dur3.count() << std::endl;
}

int main(int argc, char* argv[])
{
    // test0();
    // test1();
    // test2();
    // test3();
    // test4();
    // test5();
    // test6();
    // test7();
    test8();
    test9();
    return 0;
}
