#include <cstdint>
#include <iostream>

struct Position
{
    int32_t x;
    int32_t y;
};

template<typename T>
inline std::size_t sizeT() { return sizeof(T); }

template<>
inline std::size_t sizeT<Position>() { return sizeT<int32_t>() + sizeT<int32_t>(); }

template<typename T, typename... Ts>
inline std::size_t sizeT() 
{
    return sizeT<T>() + sizeT<Ts...>();
}

int main(int argc, char* argv[])
{
    std::cout << "int8_t:" << sizeT<int8_t>() << std::endl;
    std::cout << "Position:" << sizeT<Position>() << std::endl;
    return 0;
}
