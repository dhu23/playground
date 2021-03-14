#include <string>
#include <iostream>
#include <sstream>
#include <array>
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <memory>
#include <chrono>
#include <utility>
#include <cmath>
#include <iomanip>
#include <bitset>
#include <stdexcept>
#include <boost/optional/optional.hpp>

template<typename T, typename UnaryOperation, size_t k>
void parseSimpleCsv(const std::string& line, char delim, UnaryOperation f,
                    const std::array<T, k>& defaultVals,
                    std::array<T, k>& res)
{
    if (line.empty())
    {
        std::copy(defaultVals.begin(), defaultVals.end(), res.begin());
    }
    size_t start = 0;
    size_t count = 0;
    while (count < k)
    {
        size_t end = line.find_first_of(delim, start);
        if (end != std::string::npos)
        {
            res[count++] = f(line.substr(start, end-start));
            start = end+1;
        }
        else
        {
            res[count++] = f(line.substr(start));
            break;
        }
    }
    if (count < k)
    {
        std::copy(defaultVals.begin()+count, defaultVals.end(), 
                  res.begin()+count);
    }
}

template<typename Iterable>
void print(const Iterable& iter)
{
    std::cout << "size=" << iter.size() << "==>";
    for (auto x : iter)
    {
        std::cout << x << '|';
    }
}

int toInt(const std::string& s)
{
    return std::stoi(s);
}

void printDuration(
    const std::chrono::time_point<std::chrono::system_clock>& t1,
    const std::chrono::time_point<std::chrono::system_clock>& t2)
{
    std::cout 
        << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count()
        << " microseconds"
        << std::endl;
}

void testMemoryAllocation()
{
    struct Dummy
    {
        int x;
        double y;
        std::string z;
        std::string s;
    };


    std::chrono::time_point<std::chrono::system_clock> s1 = 
        std::chrono::system_clock::now();
    std::vector<Dummy> v1;
    for (size_t i = 0; i < 30000; ++i)
    {
        v1.push_back(Dummy());
    }

    std::chrono::time_point<std::chrono::system_clock> e1 = 
        std::chrono::system_clock::now();

    std::cout << "\nno dynamic allocation:" << std::endl;
    printDuration(s1, e1);


    std::chrono::time_point<std::chrono::system_clock> s2 = 
        std::chrono::system_clock::now();
    std::vector<std::shared_ptr<Dummy>> v2;
    for (size_t i = 0; i < 30000; ++i)
    {
        std::shared_ptr<Dummy> p(new Dummy);
        v2.push_back(p);
    }

    std::chrono::time_point<std::chrono::system_clock> e2 = 
        std::chrono::system_clock::now();

    std::cout << "\nwith dynamic allocation:" << std::endl;
    printDuration(s2, e2);
}

struct InnerObj
{
    int x {0};
    int y {0};
};
struct SwapObj
{
    SwapObj()
    {
        std::cout << "In SwapObj default ctor" << std::endl;
    }
    SwapObj(int a, InnerObj* p):a(a), p(p) 
    {
        std::cout << "In SwapObj ctor" << std::endl;
    }

    SwapObj(SwapObj&& other): a(other.a), p(other.p) 
    {
        std::cout << "In SwapObj move ctor" << std::endl;
        other.p = nullptr;
    }

    SwapObj(const SwapObj& other): a(other.a)
    {
        std::cout << "In SwapObj copy ctor" << std::endl;
        if (other.p)
        {
            std::cout << "allocating..." << std::endl;
            p = new InnerObj;
            p->x = other.p->x;
            p->y = other.p->y;
        }
    }
    SwapObj& operator=(const SwapObj& other)
    {
        std::cout << "In SwapObj copy assignment" << std::endl;
        if (this == &other) return *this;

        a = other.a;
        if (p) delete p;
        p = nullptr;
        if (other.p)
        {
            std::cout << "allocating..." << std::endl;
            p = new InnerObj;
            p->x = other.p->x;
            p->y = other.p->y;
        }
        return *this;
    }

    SwapObj& operator=(SwapObj&& other)
    {
        std::cout << "In SwapObj move assignement" << std::endl;
        a = other.a;
        if (p) delete p;
        p = other.p;
        other.p = nullptr;

        return *this;
    }

    ~SwapObj()
    {
        std::cout << "In SwapObj dtor" << std::endl;
        if (p) delete p;
    }
    std::ostream& print(std::ostream& os) const
    {
        os << "[a=" << a << ",p=";
        if (p)
        {
            os << "(x=" << p->x << ",y=" << p->y << ')';
        }
        else
        {
            os << "null";
        }
        os << ']';
        return os;
    }

    int a {0};
    InnerObj* p {nullptr};
};

void testSwap()
{
    SwapObj so1;
    so1.a = 45;
    so1.p = new InnerObj;
    so1.p->x = 4;
    so1.p->y = 5;
    SwapObj so2;

    std::cout << "Printing..." << std::endl;
    so1.print(std::cout) << std::endl;
    so2.print(std::cout) << std::endl;
    
    // std::swap test with move assignable
    std::cout << "Swapping so1 and so2..." << std::endl;

    // swap implementation
    // T temp = std::move(t1);
    // t1 = std::move(t2);
    // t2 = std::move(temp);
    std::swap(so1, so2);
    so1.print(std::cout) << std::endl;
    so2.print(std::cout) << std::endl;
}



void testStringAppend()
{
    std::string s = "string";
    std::cout << s.append(1, '1') << std::endl;
    std::cout << s.append(2, '2') << std::endl;
    std::cout << s << std::endl;
}

void testParser()
{
    std::string input1 = "1,2,3,4,5,6";
    std::array<int, 7> res1;
    std::array<int, 7> dfs1{11, 22, 33, 44, 55, 66, 77};
    parseSimpleCsv(input1, ',', toInt, dfs1, res1);
    print(res1);

    std::string input2 = "1,2,3,4,5,6,7,8";
    std::array<int, 7> res2;
    std::array<int, 7> dfs2{11, 22, 33, 44, 55, 66, 77};
    parseSimpleCsv(input2, ',', toInt, dfs2, res2);
    print(res2);
}

// two functions for move performance
// one that return data structure and one that fills data structure
struct ComplexObj
{
    ComplexObj(size_t i):
        x(static_cast<double>(i)),
        y(std::to_string(i)),
        z(i, static_cast<short>(i))
    {
    }

    double x;
    std::string y;
    std::vector<short> z;
};


std::vector<ComplexObj>& 
populateFunction(size_t total, std::vector<ComplexObj>& x)
{
    x.reserve(total);
    for (size_t i = 0; i < total; ++i)
    {
        x.emplace_back(i);
    }
    return x;
}

std::vector<ComplexObj> returnFunction(size_t total)
{
    std::vector<ComplexObj> x;
    x.reserve(total);
    for (size_t i = 0; i < total; ++i)
    {
        x.emplace_back(i);
    }
    return x;
}

void testMovePerformanceLoop(size_t repeat)
{
    std::chrono::time_point<std::chrono::system_clock> s1 = 
        std::chrono::system_clock::now();
    
    ///
    std::vector<ComplexObj> v1;
    for (size_t i = 0; i < repeat; ++i)
    {
        populateFunction(100, v1);
        v1.clear(); // capacity may not be reduced
    }
    std::chrono::time_point<std::chrono::system_clock> e1 = 
        std::chrono::system_clock::now();
    printDuration(s1, e1);


    std::chrono::time_point<std::chrono::system_clock> s2 = 
        std::chrono::system_clock::now();

    ///
    std::vector<ComplexObj> v2;
    for (size_t i = 0; i < repeat; ++i)
    {
        v2 = returnFunction(100);
    }
    std::chrono::time_point<std::chrono::system_clock> e2 = 
        std::chrono::system_clock::now();
    printDuration(s2, e2);
}

void testMovePerformance(size_t totalCount)
{
    std::cout 
        << "testing population vs returning with size=" << totalCount 
        << std::endl;

    {
        std::chrono::time_point<std::chrono::system_clock> s1 = 
            std::chrono::system_clock::now();
        
        /// 
        std::vector<ComplexObj> testVec1;
        populateFunction(totalCount, testVec1);

        std::chrono::time_point<std::chrono::system_clock> e1 = 
            std::chrono::system_clock::now();
        std::cout << "population method..." << std::endl;
        std::cout << testVec1.size() << std::endl;
        printDuration(s1, e1);
    }
    {
        std::chrono::time_point<std::chrono::system_clock> s2 = 
            std::chrono::system_clock::now();

        /// 
        std::vector<ComplexObj> testVec2 = returnFunction(totalCount);

        std::chrono::time_point<std::chrono::system_clock> e2 = 
            std::chrono::system_clock::now();
        std::cout << "return method..." << std::endl;
        std::cout << testVec2.size() << std::endl;
        printDuration(s2, e2);
    }
}

void testMovePerformance()
{
    testMovePerformance(10);
    testMovePerformance(100);
    testMovePerformance(1000);
    testMovePerformance(10000);
    // testMovePerformance(100000);
    testMovePerformanceLoop(100);
}


void testStreamParse()
{
    std::stringstream lineStream("a,b,c,");
    std::string cell;
    std::vector<std::string> v;
    while (std::getline(lineStream, cell, ','))
    {
        v.push_back(cell);
    }
    print(v);
    std::cout << std::endl;
}

template<typename T>
void divideByZero(T x)
{
    std::cout << "dividing " << x << " by zero" << std::endl;
    T a = x/0;
    std::cout << (a == a ? "real" : "nan") << std::endl;
    std::cout << std::setprecision(15) << a << std::endl;
}
void testNan()
{
    float a = std::sqrt(-2);
    std::cout << (a == a ? "real" : "nan") << std::endl;
    std::cout << std::setprecision(15) << a << std::endl;
    if (a > 0.0) std::cout << "greater than 0" << std::endl;
    if (a < 0.0) std::cout << "less than 0" << std::endl;
}

void testDivideByZero()
{
    /*
    std::cout << "float case" << std::endl;
    divideByZero<float>(1.0);
    std::cout << "double case" << std::endl;
    divideByZero<double>(1.0);
    std::cout << "int case" << std::endl;
    divideByZero<int>(1);
    std::cout << "long long case" << std::endl;
    divideByZero<long long>(1);
    */
}

size_t flip(size_t i)
{
    return 1 ^ i;
}

void testFlip()
{
    std::cout << "testing flips" << std::endl;
    std::cout << flip(1) << std::endl;
    std::cout << flip(0) << std::endl;
}

constexpr size_t MAX_LEVELS = 5;
template<size_t k>
struct AggBook 
{
    static_assert(k <= 10, "illegal k");
    struct Level
    {
        Level(): size(0), price(-1.0) {}
        size_t size;
        double price;
    };

    std::array<std::array<Level, k>, 2> book;
    size_t passIndex;

    AggBook(bool isBuy=true):book(), passIndex(isBuy ? 0 : 1) {}
    
    const std::array<Level, k>& getPassSide() const 
    {return book[passIndex];}

    const std::array<Level, k>& getAggSide() const
    {return book[1 ^ passIndex];}

    const Level& getPassAt(size_t i) const {return getPassSide()[i];}
    const Level& getAggAt(size_t i) const {return getAggSide()[i];}
};

void testAggBook()
{
    AggBook<1> ab1;
    std::cout << "depth(book1)=" << ab1.book[0].size() << std::endl;
    AggBook<10> ab10;
    std::cout << "depth(book10)=" << ab10.book[0].size() << std::endl;
    // the following would cause compilation error 
    // AggBook<100> ab100;
    // std::cout << "depth(book100)=" << ab100.book[0].size() << std::endl;
}

void testMove()
{
    std::map<std::string, SwapObj> m;

    SwapObj x;
    std::cout << "using emplace" << std::endl;
    m.emplace("5", std::move(x));
    std::cout << "end of emplace" << std::endl;

    SwapObj y;
    std::cout << "using operator[]" << std::endl;
    m["7"] = y;
    std::cout << "end of operator[]" << std::endl;

    SwapObj z;
    std::cout << "using move/operator[]" << std::endl;
    m["8"] = std::move(z);
    std::cout << "end of move/operator[]" << std::endl;

    SwapObj a;
    std::string sa("abc");
    m.emplace(sa, a);

    // causing compilation error
    // SwapObj z;
    // m.emplace(6, z);
}

template<typename T>
void printv(T&& t)
{
    std::cout << t << std::endl;
}

template<typename T, typename... Args>
void printv(T&& t, Args&&... args)
{
    printv(t);
    printv(args...);
}

struct Printer
{
    template<typename T>
    void print(T&& t)
    {
        std::cout << t << std::endl;
    }

    template<typename T, typename... Args>
    void print(T&& t, Args&&... args)
    {
        // printv(t, args...); this works
        this->print(t);
        this->print(args...);
    }
};

void testRecursiveVariadicTemplate()
{
    Printer p;
    int x = 1;
    float y = 2.0;
    std::string s = "a string";
    p.print(x, y, s);
    // p.print(); this fails compilation
}

void variadicFunc(int x, double y)
{
    std::cout << "x=" << x << ",y=" << y << std::endl;
}
void variadicFunc(int x, double y, bool z)
{
    std::cout << "x=" << x << ",y=" << y << ",z=" << z << std::endl;
}

struct Passthrough
{
    template<typename... Args>
    void print(int x, double y, Args&&... args)
    {
        variadicFunc(x, y, args...);
    }
};

void testVariadicTemplatePassthrough()
{
    Passthrough pt;
    int x = 1;
    double y = 2.0;
    pt.print(x, y);
    pt.print(x, y, true);
}

/*
struct Base
{
    void runInt(int x) {std::cout << "runInt" << std::endl;}
    void runDouble(double x) {std::cout << "runDouble" << std::endl;}
};

template<typename T>
struct RunFunc
{
    typedef void (Base::*value_type)(T);
};

template<typename T>
void runFuncPointer(Base b, RunFunc<T>::value_type f, T t)
{
    (b.*f)(t);
}

void testFuncPointer()
{
    Base b;
    runFuncPointer(b, &Base::runInt, 1);
    runFuncPointer(b, &Base::runDouble, 1.0);
}
*/

// test template virtual function which shouldn't compile
// error message: template may not be virtual
/*
struct Base
{
    template<typename T>
    virtual void f(T t) = 0;
};

struct D1 : public Base
{
    template<typename T>
    virtual void f(T t)
    {
        std::cout << "in D1: t=" << t << std::endl;
    }
};

void testTemplateVirtual()
{
    Base* p = new D1;
    int i = 1;
    double d = 5.0;
    p->f(i);
    p->f(d);
}
*/

void testUnsignint()
{
    std::cout << "in test unsigned int" << std::endl;
    std::vector<int> x;
    std::cout << x.size() - 1 << std::endl;
    int y = x.size() - 1;
    
    for (int i = x.size()-1; i >= 0; --i)
    {
        std::cout << "x[i]=" <<  x[i] << std::endl;
    }

    std::cout << y << std::endl;
}

void testArrayToString()
{
    std::array<char, 4> s1;
    s1[0] = 'A';
    s1[1] = 'B';
    s1[2] = '\0';
    s1[3] = 'C';
    
    std::cout << std::string(s1.begin(), s1.end()) << std::endl;
}

void testReadBool()
{
    std::stringstream ss;
    ss << "1 ";
    bool b;
    ss >> b;
    std::cout << "b=" << b << std::endl;
}

struct TestInitList
{
    TestInitList(double input):
        x(input),
        y(x*2.0),
        z(y*2.0) 
    {}
    double y;
    double x;
    double z;
    std::ostream& print(std::ostream& os) const
    {
        os << "x=" << x << ",y=" << y << ",z=" << z;
        return os;
    }
};

void printMMap(const std::unordered_multimap<int, std::string>& m)
{
    for (const auto& kv : m) 
    {
        std::cout << kv.first << "--->" << kv.second << std::endl;
    }
}

void testInitializationList()
{
    TestInitList t(3.0);
    t.print(std::cout) << std::endl;
}



void testUnorderedMultimap()
{
    std::unordered_multimap<int, std::string> m;
    m.emplace(1, "odd");
    m.emplace(1, "one");
    m.emplace(2, "even");
    m.emplace(2, "two");

    printMMap(m);

    auto range = m.equal_range(2);
    for (auto it = range.first; it != range.second; ++it)
    {
        it->second += " updated";
    }
    printMMap(m);

    m.erase(1);
    printMMap(m);
}

bool test1()
{
    std::cout << "in test1" << std::endl;
    return false;
}

bool test2()
{
    std::cout << "in test2" << std::endl;
    return true;
}

bool test3()
{
    std::cout << "in test3" << std::endl;
    return true;
}
void testAndEquals()
{
    bool ok = test1() 
        && test2()
        && test3();
    std::cout << "starting over" << std::endl;
    ok = test1();
    ok &= test2();
    ok &= test3();
}

/*
class TestB
{
public:
    TestB(int x, double y) { std::cout << "creating TestB" << std::endl; }
    virtual ~TestB() {}

    virtual void foo() = 0;
};

template<typename T>
class TestB2
{
public:
    TestB2(int x, int y):TestB(x, y) {}
    virtual ~TestB2() {}

    static T* create(int x, double y) {return new T(x, y);}
};

class TestD1 : public TestB2<TestD1>
{
public:
    TestD1(int x, double y):
        TestB2<TestD1>(x, y)
    { std::cout << "creating TestD1" << std::endl; }
    virtual ~TestD1() {}

    virtual void foo() { std::cout << "in TestD1" << std::endl; }
    //static TestB* create(int x, double y) {return new TestD1(x, y);}
};

class TestD2 : public TestB2<TestD2>
{
public:
    TestD2(int x, double y):
        TestB2<TestD2>(x, y)
    { std::cout << "creating TestD2" << std::endl; }
    virtual ~TestD2() {}

    virtual void foo() { std::cout << "in TestD2" << std::endl; }
    //static TestB* create(int x, double y) {return new TestD2(x, y);}
};

template<typename T, typename...Args>
std::unique_ptr<T> make_unique_ptr(Args&&... args)
{
    return std::unique_ptr<T>(std::forward<Args>(args)...);
}

void testCreateFunc()
{
    typedef TestB* (*CreateF)(int x, double y);
    std::unordered_map<std::string, CreateF> m;
    m.emplace("D1", &TestD1::create);
    m.emplace("D2", &TestD2::create);

    auto it = m.find("D1");
    if (it != m.end())
    {
        TestB* p = it->second(5, 6);
        p->foo();
        delete p;
    }
}
*/

void testBidSet()
{
    enum Types
    {
        T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, 
        T11
    };
    std::bitset<T11> b;
    for (size_t i = 1; i < b.size(); i += 2)
    {
        b.set(i);
        std::cout << b << std::endl;
    }
}

// test nested class in template
template<typename T>
struct Outer
{
    struct Inner
    {
        T t;
    };
    std::vector<Inner> vec;
};

void testNestedStructInTemplate()
{
    Outer<int>::Inner i{ 5 };
    Outer<int> o;
    o.vec.push_back(i);
    o.vec.emplace_back(Outer<int>::Inner{6});

    for ( const Outer<int>::Inner& x : o.vec)
    {
        std::cout << x.t << std::endl;
    }
}


template<typename T>
bool less(T&& t1, T&& t2)
{
    if (t1 < t2) { return true; }
    return false;
}

template<typename T, typename... Args>
bool less(T&& t1, T&& t2, Args&&... args)
{

    if (less(t1, t2)) { return true; }
    if (less(t2, t1)) { return false; }
    return less(args...);
}

struct InnerStruct
{
    int ix;
    double iy;
};

bool operator<(const InnerStruct& i1, const InnerStruct& i2)
{
    return less(i1.ix, i2.ix,
                i1.iy, i2.iy);
}

struct CompStruct
{
    int x;
    double y;
    std::string z;
    InnerStruct s;
};

bool operator<(const CompStruct& lhs, const CompStruct& rhs)
{
    return less(lhs.x, rhs.x,
                lhs.y, rhs.y,
                lhs.z, rhs.z, 
                lhs.s, rhs.s);
}

void testVariadicComp()
{
    InnerStruct i { 5, 2.0 };
    InnerStruct j = { 1, 0.0 };

    std::cout << (i < j) << std::endl;

    CompStruct lhs { 5, 2.0, "abc", { 50, 20.0 } };
    CompStruct rhs = lhs;
    std::cout << (lhs < rhs) << std::endl;
}

template<typename T>
bool wildMatch(T&& t1, T&& t2)
{
    return t1 == t2;
}

template<typename T>
bool wildMatch(T&& t1, boost::optional<T>&& t2)
{
    if (!t2) { return true; }
    return wildMatch(t1, *t2);
}

template<typename T>
bool wildMatch(boost::optional<T>&& t1, T&& t2)
{
    return wildMatch(t2, t1);
}

template<typename T>
bool wildMatch(boost::optional<T>&& t1, boost::optional<T>&& t2)
{
    if (t1) { return wildMatch(*t1, t2); }
    else { return true; }
}

void testWildMatch()
{
    CompStruct s { 5, 2.0, "abc", { 50, 20.0 } };
}

void testPartialLambdaApplication()
{
    struct Context // for the sake of testing
    {
        void foo() { std::cout << "printing foo" << std::endl; }
        void bar() { std::cout << "printing bar" << std::endl; }

        void run()
        {
            std::vector<std::string> v1(2, "test");
            std::vector<std::string> v2(2, "prod");
            auto f = [&](const std::string& s, bool isProd) 
            {
                if (isProd && s == v2.front())
                {
                    std::cout << "is prod and passes prod check" << std::endl;
                    foo();
                }
                else if (!isProd && s == v1.front())
                {
                    std::cout << "is not prod and passes test check" << std::endl;
                    bar();
                }
                else
                {
                    std::cout << "unknown case" << std::endl;
                }
            };
            const std::string ss = "prod";
            auto checkProd = std::bind(f, std::placeholders::_1, true);
            auto checkTest = std::bind(f, std::placeholders::_1, false);

            std::vector<std::string> inputs = { "prod", "test", "prod1", "test1" };
            std::for_each(inputs.begin(), inputs.end(), checkProd);
            std::for_each(inputs.begin(), inputs.end(), checkTest);
        }
    };

    Context ctxt;
    ctxt.run();
}

int main(int argc, char* argv[])
{
    // testStringAppend();
    // testParser();
    // testMemoryAllocation();
    // testSwap();
    // testMovePerformance();
    // testStreamParse();
    // testNan();
    // testDivideByZero();
    // testFlip();

    // testAggBook();
    // testMove();

    // testRecursiveVariadicTemplate();
    // testUnsignint();
    // testArrayToString();
    // testReadBool();
    // testInitializationList();
    // testVariadicTemplatePassthrough();
    // testTemplateVirtual();
    // testUnorderedMultimap();
    // testAndEquals();
    // testCreateFunc();
    // testBidSet();
    // testNestedStructInTemplate();
    // testVariadicComp();
    testPartialLambdaApplication();

    return 0;
}
