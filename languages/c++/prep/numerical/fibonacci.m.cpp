// this explores fibonacci sequence as an exercise
// which is also in root/languages/compare

// here we explore multiple ways of computing F(N) in runtime
// we establish that F(0) = 0, F(1) = 1, andd F(N) = F(N-1) + F(N-2)
// 1. recursive by definition
// 2. linear roll
// 3. using the matrix multiplication 
//    exponential can be done log(N) instead of linear N


#include <cassert>
#include <chrono>
#include <cstdint>
#include <functional>
#include <iostream>
#include <vector>

struct Fibonacci {
    // by its recursive definition
    static uint64_t recursive(uint16_t n) {
        if (n <= 1) {
            return n;
        } 
        return recursive(n-1) + recursive(n-2);
    }

    // basically this calculates F(2), F(3), ... all the way to F(N)
    static uint64_t linearRoll(uint16_t n) {
        assert(n >= 0);
        if (n <= 1) {
            return n;
        }
        // we iterate through a pair of F(0), F(1)
        // and arrive at F(1), F(2)
        // and repeat to get to F(N-1), F(N)
        // this takes N-1 iterations
        uint64_t a = 0;
        uint64_t b = 1;

        uint16_t count = n-1;
        while (count > 0) {
            // go from (a, b) -> (b, a+b)
            uint64_t temp = a + b;
            a = b;
            b = temp;
            --count;
        }
        return b;
    }

    // utilize how Fibonacci closed form is derived in math
    // F(N) = F(N-1) + F(N-2)
    // add a new redundant equation:
    // F(N-1) = F(N-1)
    // so the system can be expressed as 
    // --        --           --      --  --        --
    // |  F(N)    |    ---->  |  1  1  |  |  F(N-1)  |
    // |  F(N-1)  |           |  1  0  |  |  F(N-2)  |
    // --        --           --      --  --        -- 
    struct Matrix {
        uint64_t a;
        uint64_t b;
        uint64_t c;
        uint64_t d;
    };

    // m1       m2
    // a   b    a   b
    // c   d    c   d
    static Matrix multiple(const Matrix& m1, const Matrix& m2) {
        return Matrix{
            m1.a * m2.a + m1.b * m2.c,
            m1.a * m2.b + m1.b * m2.d,
            m1.c * m2.a + m1.d * m2.b,
            m1.c * m2.b + m1.d * m2.d
        };
    }

    struct ExponentialByRecursion {
        static Matrix exponential(const Matrix& m, uint16_t n) {
            if (n == 1) {
                return m;
            }
            Matrix ret = exponential(m, n / 2);
            ret = multiple(ret, ret);
            if (n & 1) {
                return multiple(ret, m);
            }
            return ret;
        }
    };

    struct ExponentialBySquaringBase {
        static Matrix exponential(const Matrix& m, uint16_t n) {
            if (n == 1) {
                return m;
            }
            Matrix base{m};
            Matrix ret{1, 0, 0, 1}; // identity matrix
            while (n > 0) {
                if (n & 1) {
                    // n is odd
                    ret = multiple(ret, base); 
                }
                n >>= 1; 
                // now n is divided by 2, so the base needed to be squared
                base = multiple(base, base);
            }
            return ret;
        }
    };

    struct ExponentialByStackIteration {
        static Matrix exponential(const Matrix& m, uint16_t n) {
            if (n == 1) {
                return m;
            }
            // iterative way of calculating exponential
            // build up a vector that holds the power of T^1, T^2, T^4, T^8...
            // this is basically the binary representation of exponent
            std::vector<Matrix> powers{};
            std::vector<uint16_t> exponents{};

            Matrix power = m;
            uint16_t exponent = 1;

            while (exponent <= n) {
                powers.push_back(power);
                exponents.push_back(exponent);

                power = multiple(power, power);
                exponent *= 2;
            }

            Matrix result{1, 0, 0, 1};

            while (!exponents.empty()) {
                uint16_t lastExponent = exponents.back();
                if (lastExponent <= n) {
                    // multiple that to the results
                    result = multiple(result, powers.back());
                    n -= lastExponent;
                }
                powers.pop_back();
                exponents.pop_back();
            }

            assert(n == 0);
            return result;
        }
    };

    constexpr static Matrix transition{1, 1, 1, 0};

    // let vector V0=(F(1), F(0))
    // so transition matrix T*V0 = V1, that is (F(2), F(1))
    // ...
    // T*T*...T*V0 (with n-1 Ts) gives V(n-1), that is (F(N), F(N-1))
    // so we calculate T^(n-1)
    template<typename E>
    static uint64_t matrix(uint16_t n) {
        if (n <= 1) {
            return n;
        }

        // T^(n-1) * V0 gives the right vector, which is just field a of matrix
        return E::exponential(transition, n-1).a;
    }
};

struct Test {
    static void testConsistency() {
        for (uint16_t i = 0; i < 10; ++i) {
            std::cout << "---- i: " << i << " ----" << std::endl;
            std::cout << "recursive: " << Fibonacci::recursive(i) << std::endl;
            std::cout << "linear-roll: " << Fibonacci::linearRoll(i) << std::endl;
            std::cout 
                << "matrix(explicit iteration): " 
                << Fibonacci::matrix<Fibonacci::ExponentialByStackIteration>(i) 
                << std::endl;
            std::cout 
                << "matrix(recursive exponential): "
                << Fibonacci::matrix<Fibonacci::ExponentialByRecursion>(i)
                << std::endl;
            std::cout 
                << "matrix(squaring base): "
                << Fibonacci::matrix<Fibonacci::ExponentialBySquaringBase>(i)
                << std::endl;
        }
    }

    static void profile(std::function<uint64_t(uint16_t)> fib, uint16_t max) {
        for (uint16_t i = 0; i < max; ++i) {
            auto start = std::chrono::steady_clock::now();
            uint64_t res = fib(i);
            auto end = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
            std::cout 
                << "i: " << i << ", res: " << res 
                << ", elapsed(ns): " << elapsed.count() << std::endl;
        }
    }

    static void testPerformance() {
        profile(Fibonacci::recursive, 35);
        profile(Fibonacci::linearRoll, 100);
        profile(Fibonacci::matrix<Fibonacci::ExponentialByStackIteration>, 100);
        profile(Fibonacci::matrix<Fibonacci::ExponentialByRecursion>, 100);
        profile(Fibonacci::matrix<Fibonacci::ExponentialBySquaringBase>, 100);
    }
};

int main(int argc, char* argv[]) {
    Test::testConsistency();
    
    Test::testPerformance();

    return 0;
}