#include <iostream>
#include <array>
#include <utility>
#include <chrono>

// idx  0 1 2 3 4 5 6  7  8  9 10  
// fib  0 1 1 2 3 5 8 13 21 34 55

using namespace std;

// calculate fibonacci recursively during compile time
template<unsigned int>
struct CompileTimeFibonacci;

template<>
struct CompileTimeFibonacci<0>
{
    static constexpr unsigned int value = 0;
};

template<>
struct CompileTimeFibonacci<1>
{
    static constexpr unsigned int value = 1;
};

template<unsigned int k>
struct CompileTimeFibonacci
{
    static constexpr unsigned int value = 
        CompileTimeFibonacci<k-1>::value + CompileTimeFibonacci<k-2>::value;
};


// calculate fibonacci recursively during run time
unsigned int recursivefib(unsigned int i)
{
    if (i < 2) return i;
    else return recursivefib(i-1)+recursivefib(i-2);
}

constexpr unsigned int recursivefibonacci(unsigned int i)
{
    return i < 2 ? i : (recursivefibonacci(i-1)+recursivefibonacci(i-2));
}


// recursive implementation of the linear roll algorithm
// as a linear roll method, it should be fast at run time.
// even faster for compile time calculation
constexpr 
pair<unsigned int, unsigned int> fib1(pair<unsigned int, unsigned int> input)
{
    return make_pair(input.second, input.second+input.first);
}

// idx     value     pair
//   0         0     1, 0 // 1 is imaginary
//   1         1     0, 1
//   2         1     1, 1
//   3         2     1, 2
//   4         3     2, 3
//   5         5
//   6         8
//   7        11

constexpr 
pair<unsigned int, unsigned int> fib2(unsigned int idx)
{
    return idx == 0 ? make_pair<unsigned int>(1u, 0u) : fib1(fib2(idx-1));
}

constexpr unsigned int recursivelinearrollfib(unsigned int i)
{
    return fib2(i).second;
}

// loop based linear roll
unsigned int looplinearrollfib(unsigned int n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    unsigned int a = 0;
    unsigned int b = 1;

    while (n > 1)
    {
        unsigned int tmp = a;
        a = b;
        b += tmp;
        --n;
    }
    return b;
}

struct Matrix
{
    Matrix(unsigned int m11, 
           unsigned int m12, 
           unsigned int m21, 
           unsigned int m22):
        a(m11), b(m12), c(m21), d(m22)
    {
    }
    unsigned int a;
    unsigned int b;
    unsigned int c;
    unsigned int d;
};

Matrix multiple(const Matrix& m1, const Matrix& m2)
{
    return Matrix(m1.a*m2.a + m1.b*m2.c,
                  m1.a*m2.b + m1.b*m2.d,
                  m1.c*m2.a + m1.d*m2.c,
                  m1.c*m2.b + m1.d*m2.d);
}

Matrix powerOfMatrix(const Matrix& m, unsigned int n)
{
    if (n == 0) return Matrix(1, 0, 0, 1);
    if (n == 1) return m;

    unsigned int h = n/2;

    Matrix p = powerOfMatrix(m, h);
    return n % 2 == 0 
        ? multiple(p, p)
        : multiple(multiple(p, p), m);
}

// +-       -+
// |  1   1  |  F(n)               F(n)+F(n-1)        F(n+1)
// |         |               =                    =
// |  0   1  |  F(n-1)             F(n)               F(n)
// +-       -+
//
// there for with the matrix M, 
// from the transpose of [ F(1), F(0) ], as V0, aka [1, 0]
// to the transpose of [ F(2), F(1) ], as V1 = M V0
// ...
// to the transpose of [ F(n+1), F(n) ], as V(n) = M^n V0 = M^n.get(2, 1) => c
unsigned int recursivematrixrollfib(unsigned int n)
{
    Matrix init(1, 1, 1, 0);
    Matrix ret = powerOfMatrix(init, n);
    return ret.c;
}

void compare(unsigned int n)
{
    using namespace std::chrono;

    high_resolution_clock::time_point p1 = high_resolution_clock::now();
    looplinearrollfib(n);
    high_resolution_clock::time_point p2 = high_resolution_clock::now();
    recursivematrixrollfib(n);
    high_resolution_clock::time_point p3 = high_resolution_clock::now();

    cout 
        << "n=" << n
        << ",linear=" << duration_cast<nanoseconds>(p2-p1).count()
        << ",matrix=" << duration_cast<nanoseconds>(p3-p2).count()
        << endl;
}

void test()
{
    cout << CompileTimeFibonacci<8>::value << endl;   
    cout << CompileTimeFibonacci<100>::value << endl; // instant
    cout << recursivefib(8) << endl;
    constexpr unsigned int fib10 = recursivefibonacci(10);
    cout << fib10 << endl;
    cout << recursivefibonacci(8) << endl; 
    array<int, recursivefibonacci(10)> arr; // calculated at compile time
    for (int i : arr) cout << i << ' ';
    cout << endl;

    constexpr unsigned int fib99 = recursivelinearrollfib(99);
    cout << fib99 << endl; // compile time
    cout << CompileTimeFibonacci<99>::value << endl; // compile time
    cout << recursivelinearrollfib(100) << endl; // instant. runtime
    unsigned int i = 100;
    cout << recursivelinearrollfib(i-5) << endl; // instant. runtime

    cout << looplinearrollfib(200) << endl;
    cout << recursivematrixrollfib(200) << endl;

    for (unsigned int i = 2; i < 300; ++i) compare(i);
}

void timeTest1()
{
    unsigned int diffCount = 0;
    for (unsigned int c = 0; c < 100; ++c) 
    {
        for (unsigned int i = 1; i < 1000; ++i)
        {
            auto fibx = looplinearrollfib(i);
            // auto fiby = recursivematrixrollfib(i);
            // cout << "fibx:" << fibx << ",fiby:" << fiby << endl;
            //if (fibx != fiby) ++diffCount;
        }
    }
}

unsigned int timeTest2(unsigned int v)
{
    unsigned int ret = 0;
    for (unsigned int i = 1; i < v; ++i)
    {
        ret += recursivefibonacci(i);
    }
    return ret;
}

int main(int argc, char* argv[])
{
    cout << timeTest2(44) << endl; // up to fib 41
    return 0;
}
