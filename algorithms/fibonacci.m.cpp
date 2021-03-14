#include <iostream>
#include <array>
#include <utility>
#include <chrono>

// idx  0 1 2 3 4 5 6  7  8  9 10  
// fib  0 1 1 2 3 5 8 13 21 34 55

using namespace std;

template<unsigned int>
struct Fibonacci;

template<>
struct Fibonacci<0>
{
    static constexpr unsigned int value = 0;
};

template<>
struct Fibonacci<1>
{
    static constexpr unsigned int value = 1;
};

template<unsigned int k>
struct Fibonacci
{
    static constexpr unsigned int value = 
        Fibonacci<k-1>::value + Fibonacci<k-2>::value;
};

unsigned int fib(unsigned int i)
{
    if (i < 2) return i;
    else return fib(i-1)+fib(i-2);
}

constexpr unsigned int fibonacci(unsigned int i)
{
    return i < 2 ? i : (fibonacci(i-1)+fibonacci(i-2));
}

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

constexpr unsigned int fib3(unsigned int i)
{
    return fib2(i).second;
}


unsigned int fib_roll(unsigned int n)
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

unsigned int fib_binary(unsigned int n)
{
    Matrix init(1, 1, 1, 0);
    Matrix ret = powerOfMatrix(init, n);
    return ret.c;
}

void compare(unsigned int n)
{
    using namespace std::chrono;

    high_resolution_clock::time_point p1 = high_resolution_clock::now();
    fib_roll(n);
    high_resolution_clock::time_point p2 = high_resolution_clock::now();
    fib_binary(n);
    high_resolution_clock::time_point p3 = high_resolution_clock::now();

    cout 
        << "n=" << n
        << ",roll=" << duration_cast<nanoseconds>(p2-p1).count()
        << ",binary=" << duration_cast<nanoseconds>(p3-p2).count()
        << endl;
}

int main(int argc, char* argv[])
{
    cout << Fibonacci<8>::value << endl;   
    cout << Fibonacci<100>::value << endl; // instant
    cout << fib(8) << endl;
    // cout << fib(100) << endl; // this is very slow runtime calculation
    constexpr unsigned int fib100 = fibonacci(100);
    cout << fib100 << endl;
    cout << fibonacci(8) << endl; 
    // cout << fibonacci(100) << endl; // very slow runtime calculation
    array<int, fibonacci(10)> arr; // calculated at compile time
    for (int i : arr) cout << i << ' ';
    cout << endl;

    constexpr unsigned int fib99 = fib3(99);
    cout << fib99 << endl; // compile time
    cout << Fibonacci<99>::value << endl; // compile time
    cout << fib3(100) << endl; // instant. runtime
    unsigned int i = 100;
    cout << fib3(i-5) << endl; // instant. runtime

    cout << fib_roll(200) << endl;
    cout << fib_binary(200) << endl;

    for (unsigned int i = 2; i < 300; ++i) compare(i);

    return 0;
}
