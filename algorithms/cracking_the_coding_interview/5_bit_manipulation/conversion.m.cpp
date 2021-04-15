// write a function to determine the number of bits you would have to flip
// to convert integer A to integer B. 

// example 
// input   29 ( or 11101 ), 15 ( or 01111 )
// output  2

#include <iostream>
#include <bitset>

int logicRightShift(int x)
{
    x >>= 1;
    x &= ~(1 << (sizeof(x)*8-1));
    return x;
}

size_t getNumFlipsNaive(int x, int y)
{
    int ret = x ^ y; // # of 1s indicates # of flips
    size_t count = 0;
    int mask = 1;
    for (size_t i = 0; i < sizeof(ret)*8-1; ++i)
    {
        if ((ret & mask) != 0)
        {
            ++count;
        }
        mask <<= 1;
    }
    return count;
}

// take advantage of n & (n-1), which clear the least significant bit

size_t getNumFlips(int x, int y)
{
    size_t count = 0;
    // a faster way to count 1s. O(#1s)
    for (int ret = x ^ y; ret != 0; ret = ret & (ret-1))
    {
        ++count;
    }
    return count;
}

int main(int argc, char* argv[])
{
    std::cout << getNumFlipsNaive(29, 15) << std::endl;
    std::cout << getNumFlips(29, 15) << std::endl;

    std::cout << std::bitset<32>(-10) << std::endl;
    std::cout << std::bitset<32>((-10) >> 2) << std::endl;
    std::cout << std::bitset<32>(1 >> 1) << std::endl;
    return 0;
}
