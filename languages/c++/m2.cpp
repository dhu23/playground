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
#include <fstream>
#include <bitset>
#include <iterator>

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


inline void //std::vector<ComplexObj>& 
populateFunction(size_t total, std::vector<ComplexObj>& x)
{
    x.reserve(total);
    for (size_t i = 0; i < total; ++i)
    {
        x.emplace_back(i);
    }
    // return x;
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
        // populateFunction(totalCount, testVec1);
        
        testVec1.reserve(totalCount);
        for (size_t i = 0; i < totalCount; ++i)
        {
            testVec1.emplace_back(i);
        }

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
    std::cout << "using emplace with const T&" << std::endl;
    m.emplace(sa, a);
}

void testBinarySearch()
{
    std::vector<int> vec{1, 1, 22, 22, 33, 33, 33, 44, 555};
    print(vec);
    for (auto x : std::vector<int>{0, 1, 2, 3, 21, 22, 23, 554, 555, 556})
    {
        std::cout << "testing " << x;
        auto ltr = std::lower_bound(vec.begin(), vec.end(), x);
        std::cout 
            << ",lb=" 
            << ((ltr == vec.end()) ? "end" : std::to_string(*ltr));
        auto utr = std::upper_bound(vec.begin(), vec.end(), x);
        std::cout 
            << ",ub="
            << ((utr == vec.end()) ? "end" : std::to_string(*utr));
        std::cout << std::endl;
    }
}

void testLambda()
{
    struct Data
    {
        int x;
        std::string y;
    };

    std::vector<Data> vec{
        {11, "one"},
        {22, "two"},
        {33, "three"},
        {44, "four"}, 
        {55, "five"}, 
        {66, "six"}
    };
    
    for (int t : {5, 11, 15, 22, 33, 40, 56, 65, 66, 70})
    {
        std::cout << "checking t=" << t << " ";
        if (vec.front().x > t || vec.back().x < t) 
        {
            std::cout << "off the range" << std::endl;
        }
        else
        {
            auto it = std::upper_bound(
                vec.begin(), vec.end(), t,
                [](double t, const Data& d){return t < d.x;});
            --it;
            std::cout << "using " << it->y << std::endl;
        }
    }
}

void testIOStream()
{
    std::unordered_map<std::string, std::ofstream> m;
    for (const std::string& s : {"a", "b", "c"})
    {
        std::string fileName = s;
        m.insert({s, std::ofstream(fileName.c_str())});
    }
}

void testLayout(int x)
{
    std::cout << sizeof(x) << std::endl;
    char* p = (char*)&x;
    std::bitset<4> b(x);
    std::cout << b << std::endl;
    std::cout << (int)(*p) << std::endl;
    std::cout << (int)(*(p+1)) << std::endl;
    std::cout << (int)(*(p+2)) << std::endl;
    std::cout << (int)(*(p+3)) << std::endl;
    std::cout << (int)(*(p+4)) << std::endl;
    std::cout << (*p == 0) << std::endl;
}

void testLayout()
{
    testLayout(2147483648-1);
    testLayout(1);
}

std::vector<int>& getVec()
{
    static std::vector<int> vec{1, 2, 3};
    return vec;
}

void testAuto()
{
    auto v = getVec();
    v.push_back(4);
    print(v);
    std::cout << std::endl;
    
    std::vector<int>& v2 = getVec();
    v2.push_back(5);
    print(v2);
    std::cout << std::endl;

    print(getVec());
    std::cout << std::endl;
}

void testBackinserter()
{
}

void testAugment()
{
    double c = 2.5;
    double x = 2.0;
    x *= 1 + c;
    std::cout << x << std::endl;
}

/// recurisve print for map<K0, map<K1, V1, K1Comp>, K0Comp>
struct K0
{
    int x;
};
std::ostream& printType(std::ostream& os, const K0& k0)
{
    os << "K0[x=" << k0.x << "]";
    return os;
}
struct K0Comp
{
    bool operator()(const K0& lhs, const K0& rhs) const
    {
        return lhs.x < rhs.x;
    }
};
struct K1
{
    int x;
};
std::ostream& printType(std::ostream& os, const K1& k1)
{
    os << "K1[x=" << k1.x << "]";
    return os;
}
struct K1Comp
{
    bool operator()(const K1& lhs, const K1& rhs) const
    {
        return lhs.x < rhs.x;
    }
};
struct V1
{
    int x;
};
std::ostream& printType(std::ostream& os, const V1& v1)
{
    os << "V1[x=" << v1.x << "]";
    return os;
}

template<typename Key, typename Val, typename Comp>
std::ostream& printType(std::ostream& os, const std::map<Key, Val, Comp>& m)
{
    os << "map[";
    size_t i = 0;
    for (const auto& kv : m)
    {
        if (i > 0) {os << ",";}
        printKV(os, kv.first, kv.second);
        ++i;
    }
    os << "]";
    return os;
}

std::ostream& printKV(std::ostream& os, const K1& k1, const V1& v)
{
    printType(os, k1);
    os << "-->";
    printType(os, v);
    return os;
}
std::ostream& printKV(std::ostream& os, const K0& k0,
                      const std::map<K1, V1, K1Comp>& v)
{
    printType(os, k0);
    os << "-->";
    printType(os, v);
    return os;
}
void testRecurisvePrint()
{
    std::map<K0, std::map<K1, V1, K1Comp>, K0Comp> m;
    /*
    {
        {{1}, {{11, 111}, {22, 222}}},
        {{2}, {{33, 333}, {44, 444}}}
    };
    */
    m.emplace(K0{1}, 
              std::map<K1, V1, K1Comp>{{K1{11}, V1{111}}, {K1{22}, V1{222}}});
    m.emplace(K0{2}, 
              std::map<K1, V1, K1Comp>{{K1{33}, V1{333}}, {K1{44}, V1{444}}});
    printType(std::cout, m);
}
/// test upper case
std::string convertStrToUpper(const std::string& s)
{
    std::string ret(s);
    std::transform(ret.begin(), ret.end(), ret.begin(), ::toupper);
    return std::move(ret);
}

std::vector<std::string> convertVecToUpper(const std::vector<std::string>& vec)
{
    std::vector<std::string> ret(vec);
    std::transform(ret.begin(), ret.end(), ret.begin(), convertStrToUpper);
    return std::move(ret);
}

void testUpperCase()
{
    std::vector<std::string> ss{"Dealer fade on ", "Target price not met"};
    print(ss);

    print(convertVecToUpper(ss));
}

int main(int argc, char* argv[])
{
    //testStringAppend();
    //testParser();
    //testMemoryAllocation();
    //testSwap();
    // testMovePerformance();
    //testStreamParse();
    // testNan();
    // testDivideByZero();
    //testFlip();

    // testAggBook();
    //testMove();
    //testBinarySearch();
    //testLambda();

    //testIOStream();
    //testLayout();
    // testAuto();
    // testAugment();
    testRecurisvePrint();
    // testUpperCase();
    return 0;
}
