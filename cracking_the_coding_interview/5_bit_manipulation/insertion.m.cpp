// insertion 
// you are given two 32-bit numbers N and M and two bit positions, i and j.
// write a method to insert M into N such that M starts at bit j and ends at 
// bit i. You can assume that the bits j through i have enough space to 
// fit all of M
// that is if M=10011 you can assume that there are at least 5 bits between
// j and i. You would not for example have j = 3 and i = 2, because M could not
// fit fully between bit 3 and bit 2

// ex
// input N = 10000000000, M = 10011, i  = 2, j = 6
// output N = 10001001100

#include <cassert>
#include <iostream>
#include <bitset>

//        j     i      0
// 000000011111110000000
int insert(int n, int m, int i, int j)
{
    assert(j > i);
    std::cout << std::bitset<32>(n) << std::endl;
    std::cout << std::bitset<32>(m) << std::endl;

    int mask = ~((1 << j) ^ (1 << i)); // for clearning bits for m
    return (n & mask) | (m << i);
}


int main(int argc, char* argv[])
{
    std::cout 
        << std::bitset<32>(insert(0b10000000000, 0b10011, 2, 6)) 
        << std::endl;
    return 0;
}
