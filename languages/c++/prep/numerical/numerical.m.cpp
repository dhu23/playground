// implementation of fundamental numerical methods
// - root-finding
// - integral 

#include <cassert>
#include <functional>
#include <iostream>

// a < b, f(a) > 0, f(b) < 0, and the root is in (a, b)
double bisection(
    std::function<double(double)> func,
    double a, double b, double error
) {
    assert(a < b);
    assert(func(a) > func(b));
    assert(error > 0);

    double low = a;
    double high = b;

    while (true) {
        double x = (low + high) / 2;
        double f = func(x);
        if (f > error) {
            low = x;
        } else if (f < -error) {
            high = x;
        } else {
            return x;
        }
    }
}

// pure Newton's method which can be unstable
double newton(
    std::function<double(double)> func, double x0, double h, double error
) {
    assert(error > 0);

    double x = x0;
    while (true) {
        double f = func(x); 
        if (f < error && f > -error) {
            return x; // found a root that satisfies the function
        }

        x -= f * h / (func(x+h) - f); // Newton's iteration
        // std::cout << "x is " << x << std::endl;
    }
}


struct TestRootFinding {
    constexpr static double error = 1e-5;
    constexpr static double h = 1e-2;

    // with two roots: -2 and 2
    static double simpleQuadratic(double x) {
        return x*x - 4;
    }

    static double simpleCubic(double x) {
        return x*x*x - 64; 
    }

    static void testBisection() {
        std::cout << "running simple bisection" << std::endl;
        std::function<double(double)> func = TestRootFinding::simpleQuadratic;
        // use -100 and 0 to find the first root
        double x1 = bisection(func, -100, 0, error);
        std::cout << "x1: " << x1 << std::endl;

        // use 0 and -100 to find the second root to function -func
        double x2 = bisection(
            [func](double x) { return -func(x); },
            0, 100, error
        );
        std::cout << "x2: " << x2 << std::endl;
    }

    static void testPureNewtonWithSimpleQuadratic() {
        std::cout << "running naive Newton's method for x^2 - 4" << std::endl;
        std::function<double(double)> func = TestRootFinding::simpleQuadratic;

        // start with -1
        double x1 = newton(func, -1, h, error);
        std::cout << "x1: " << x1 << std::endl;

        // start with 100
        double x2 = newton(func, 100, h, error);
        std::cout << "x2: " << x2 << std::endl;
    }

    static void testPureNewtonWithSimpleCubic() {
        std::cout << "running naive Newton's method for x^3 - 64" << std::endl;
        std::function<double(double)> func = TestRootFinding::simpleCubic;

        // start with -1
        double x = newton(func, -1, h, error);
        std::cout << "x: " << x << std::endl;

        // start with 0.01, where f'(0) is around 0
        double y = newton(func, 0.01, h, error);
        std::cout << "y: " << y << std::endl;

        // start 100
        double z = newton(func, 100, h, error);
        std::cout << "z: " << z << std::endl;
    }
};


int main(int argc, char* argv[]) {
    TestRootFinding::testBisection();
    TestRootFinding::testPureNewtonWithSimpleQuadratic();
    TestRootFinding::testPureNewtonWithSimpleCubic();
    return 0;
}