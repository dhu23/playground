// Effective modern C++ item 25
// apply std::move to rvalue reference and std::forward to universal reference
// the last time each is used
//
// do the same thing for rvalue reference and universal reference being
// returned from functions that return by value
//
// never apply std::move or std::forward to local objects if they would 
// otherwise be eligible for the return value optimization

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>

using std::cout;
using std::endl;

template<typename T>
std::ostream& printVec(std::ostream& os, const std::vector<T>& vec)
{
    os << "[";
    std::copy(
        vec.begin(), vec.end(), std::ostream_iterator<std::string>(os, ","));
    os << "]";
    return os;
}

class Dummy
{
private:
    int x_;
    double y_;
    std::vector<std::string> z_;
public:
    Dummy(): x_(0), y_(0.0), z_() 
    {
        cout << "Calling default ctor..." << endl;
    }
    Dummy(int i):
        x_(i),
        y_(static_cast<double>(i)),
        z_(1, std::to_string(i)) 
    {
        cout << "Calling int ctor..." << endl;
    }
        
    ~Dummy()
    {
        cout << "Calling dtor..." << endl;
    }

    Dummy(const Dummy& other):
        x_(other.x_),
        y_(other.y_),
        z_(other.z_)
    {
        cout << "Calling copy ctor..." << endl;
    }

    Dummy& operator=(const Dummy& other)
    {
        cout << "Calling copy assignment..." << endl;
        if (&other == this) { return *this; }
        x_ = other.x_;
        y_ = other.y_;
        z_ = other.z_;
        return *this;
    }

    Dummy(Dummy&& other):
        x_(std::move(other.x_)),
        y_(std::move(other.y_)),
        z_(std::move(other.z_))
    {
        cout << "Calling move ctor..." << endl;
    }

    Dummy& operator=(Dummy&& other)
    {
        cout << "Calling move assignment..." << endl;
        x_ = std::move(other.x_);
        y_ = std::move(other.y_);
        z_ = std::move(other.z_);
        return *this;
    }

    std::ostream& print(std::ostream& os) const
    {
        os 
            << "x=" << x_
            << ",y=" << y_
            << ",z=";
        printVec(os, z_);
        os << endl;
        return os;
    }
};

std::ostream& operator<<(std::ostream& os, const Dummy& d)
{
    return d.print(os);
}

// rule #1. 
// apply std::move to rvalue reference and std::forward to universal reference
// the last time each is used

template<typename T>
void testRule1(T&& t)
{
    t.print(cout);
}

template<typename T>
void test2Rule1(T&& t)
{
    auto&& t1 = std::forward<T>(t);
    t1.print(cout);
}

// rule #2.
// do the same thing for rvalue reference and universal reference being
// returned from functions that return by value

Dummy runIt2(Dummy&& d)
{
    d.print(cout);
    return d;
}


// rule #3.
// never apply std::move or std::forward to local objects if they would 
// otherwise be eligible for the return value optimization

Dummy makeDummy1()
{
    return Dummy(1);
}

Dummy makeDummy2()
{
    Dummy tmp(2);
    return tmp;
}

Dummy makeDummy3()
{
    Dummy tmp(3);
    return std::move(tmp);
}

Dummy makeDummy4()
{
    return std::move(Dummy(4));
}

int main(int argc, char* argv[])
{
    Dummy d1(5);
    cout << d1 << endl;

    // move doesn't change d1 in any way
    cout << std::move(d1) << endl;
    cout << d1 << endl;

    cout << "================================" << endl;

    testRule1(d1);
    cout << d1 << endl;

    cout << "================================" << endl;

    testRule1(std::move(d1));
    cout << d1 << endl;
    cout << "================================" << endl;

    auto d2 = runIt2(std::move(d1));
    d2.print(cout);
    d1.print(cout);

    cout << "-------------------------" << endl;
    auto d31 = makeDummy1(); 
    // only ctor is called
    cout << "-------------------------" << endl;
    auto d32 = makeDummy2();
    // only ctor is called
    cout << "-------------------------" << endl;
    auto d33 = makeDummy3();
    // ctor
    // move ctor
    // dtor
    cout << "-------------------------" << endl;
    auto d34 = makeDummy4();
    // ctor
    // move ctor
    // dtor
    cout << "-------------------------" << endl;

    return 0;
}
