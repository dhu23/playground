#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>


class UnionFind
{
public:
    UnionFind(size_t size);

    void doUnion(size_t n1, size_t n2);
    bool find(size_t n1, size_t n2);

    void verify(size_t n) const;
    size_t getRoot(size_t n); // does a update on root

    std::vector<size_t> _data;
    std::vector<size_t> _groupSize;
    // std::vector<size_t> _groupMax;
};

UnionFind::UnionFind(size_t size):
    _data(),
    _groupSize(size, 1)
    // _groupMax()
{
    _data.reserve(size);
    // _groupMax.reserve(size);
    for (size_t i = 0; i < size; ++i)
    {
        _data.push_back(i);
        //_groupMax.push_back(i);
    }
}

void UnionFind::doUnion(size_t n1, size_t n2)
{
    this->verify(n1);
    this->verify(n2);

    if (n1 == n2)
    {
        return;
    }

    size_t n1Root = this->getRoot(n1);
    size_t n2Root = this->getRoot(n2);

    if (n1Root == n2Root)
    {
        return;
    }

    // size_t newMax = std::max(_groupMax[n1Root], _groupMax[n2Root]);

    if (_groupSize[n1Root] >= _groupSize[n2Root])
    {
        // attach n2Root group to n1Root
        _data[n2Root] = n1Root;
        _groupSize[n1Root] += _groupSize[n2Root];
        // _groupMax[n1Root] = newMax;
    }
    else
    {
        // attach n1Root group to n2Root
        _data[n1Root] = n2Root;
        _groupSize[n2Root] += _groupSize[n1Root];
        // _groupMax[n2Root] = newMax;
    }
}

bool UnionFind::find(size_t n1, size_t n2)
{
    this->verify(n1);
    this->verify(n2);

    if (n1 == n2)
    {
        return true;
    }

    return this->getRoot(n1) == this->getRoot(n2);
}

void UnionFind::verify(size_t n) const
{
    if (n >= _data.size())
    {
        throw std::runtime_error("number not in range");
    }
}

size_t UnionFind::getRoot(size_t n)
{
    this->verify(n);

    while (_data[n] != n)
    {
        _data[n] = _data[_data[n]];
        n = _data[n];
    }
    return n;
}

template<typename T>
void printVec(const std::vector<T>& vec, const std::string& name)
{
    std::cout << "name=" << name << ",";
    for (const T& e : vec)
    {
        std::cout << e << ",";
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[])
{
    UnionFind uf(15);

    uf.doUnion(3, 4);
    uf.doUnion(4, 5);
    uf.doUnion(10, 11);
    uf.doUnion(12, 13);
    uf.doUnion(11, 12);

    printVec(uf._data, "data");
    printVec(uf._groupSize, "size");
    // printVec(uf._groupMax, "max");

    return 0;
}
