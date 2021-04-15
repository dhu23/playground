#include <vector>
#include <stdexcept>

template<typename T>
struct Grid
{
    Grid(size_t h, size_t l):height(h), length(l), data(h*l, T()) {}

    size_t getIndex(size_t x, size_t y) const
    {
        if (!withinGrid(x, y))
        {
            throw std::runtime_error("bad position");
        }
        return x*length+y;
    }

    bool withinBoard(size_t x, size_t y) const
    {
        return x < height && y < length;
    }

    T& get(size_t x, size_t y)
    {
        return data[this->getIndex(x, y)];
    }

    const T& get(size_t x, size_t y) const
    {
        return data[this->getIndex(x, y)];
    }
    
    size_t height;
    size_t length;
    std::vector<T> data;
};
