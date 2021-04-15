// write a program to swap odd and even bits of an integer with as few
// instructions as possible. 
// ie. bit 0 and bit 1 are swapped. bit 2 and bit 3 are swapped and so on

// 1010100010010101
// 1 1 1 0 1 0 0 0   odd bits
//  0 0 0 0 0 1 1 1  even bits

// mask with 1s at odd positions would look like 0xaaaaaaaa
// mask with 1s at even positions woudl look like 0x55555555

#include <bitset>
#include <iostream>

int logicRightShift(int x)
{
    x >>= 1;
    x &= ~(1 << (sizeof(x)*8-1));
    return x;
}

// generate 10101010 (odd)
//          01010101 (even)
template<typename T>
T getAlternatingBits(bool odd)
{
    size_t bits = sizeof(T)*8;
    // use hornet's algorithm
    // 10101010 = 10 * 4^3 + 10 * 4^2 + 10 * 4^1
    T ret = 0;
    for (size_t i = 0; i < bits/2; ++i)
    {
        ret *= 4;
        ret += odd ? 2 : 1;
    }
    std::cout << std::bitset<sizeof(T)*8>(ret) << std::endl;
    return ret;
}


int pairwiseSwap(int x)
{
    int oddMask = 0xaaaaaaaa;
    int evenMask = 0x55555555;

    int oddBits = x & oddMask;
    int evenBits = x & evenMask;
    // return (evenBits << 1) + (oddBits >> 1);
    return (evenBits << 1) | logicRightShift(oddBits);
}


int main(int argc, char* argv[])
{
    getAlternatingBits<int>(true);
    getAlternatingBits<int>(false);
    std::cout << std::bitset<32>(0xa895) << std::endl;
    std::cout << std::bitset<32>(pairwiseSwap(0xa895)) << std::endl;
    return 0;
}
