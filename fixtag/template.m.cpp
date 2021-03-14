#include <vector>
#include <iostream>
#include <map>

struct Data
{
    Data(size_t x, size_t y):x(x), y(y) {}
    size_t x;
    size_t y;
};

bool put(size_t i)
{
    std::cout << "running size_t" << std::endl;
    return true;
}

bool put(const Data& t)
{
    std::cout << "running Data" << std::endl;
    return true;
}

bool put(const Data* d)
{
    std::cout << "running Data*" << std::endl;
    return true;
}

template<typename T>
bool put(const std::vector<T>& v)
{
    std::cout << "running vector<T>" << std::endl;
    bool r = true;
    r &= put(v.size()); 
    for (size_t i = 0; r && i < v.size(); ++i)
    {
        r &= put(v[i]);
    }
    return r;
}

template<typename T>
bool put(const std::vector<T*>& v)
{
    std::cout << "running vector<T*>" << std::endl;
    bool r = true;
    r &= put(v.size());
    for (size_t i = 0; r && i < v.size(); ++i)
    {
        r & put(*v[i]);
    }
    return r;
}

int main(int argc, char* argv[])
{
    std::vector<Data> dVec;
    dVec.emplace_back(1, 1);
    dVec.emplace_back(2, 2);

    std::vector<Data*> dpVec;
    dpVec.push_back(new Data(11, 11));
    dpVec.push_back(new Data(22, 22));

    put(dVec);
    put(dpVec);

    return 0;
}
