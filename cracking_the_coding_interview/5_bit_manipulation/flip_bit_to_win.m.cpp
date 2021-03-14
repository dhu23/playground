// you have an integer and you can flip exactly one bit from a 0 to a 1.
// write code to find the length of the longest sequence of 1s you could create

// 01010100110110011101010011111011
// for a sequence like 000000111111111000
// remove all zeros at the end, obtain 000000000111111111
// +1, 
#include <bitset>
#include <stdexcept>
#include <iostream>

bool endsWithOneZero(int x) // true if x ends with "10"
{
    return (3 & x) == 2;
}

bool endsWithOne(int x)
{
    return (1 & x) == 1;
}

bool allOnes(int x)
{
    return ((-1) ^ x) == 0;
}

bool hasOnlyOneZero(int x)
{
    x ^= (-1); // if x has only 1 zero, then x right now is a 1 followed by 0s
    return (x & (x-1)) == 0;
}

size_t countLongestOnes(int x)
{
    int mask = (1 << (sizeof(x)*8-1)) - 1;
    size_t longestSequence = 0;
    size_t currentLongestRun = 0;
    while (x != 0)
    {
        //std::cout << std::bitset<32>(x) << std::endl;
        if (endsWithOne(x))
        {
            ++currentLongestRun;
        }
        else
        {
            currentLongestRun = 0;
        }
        longestSequence = std::max(longestSequence, currentLongestRun);
        x >>= 1;
        x &= mask; // perform logical right shift
    }
    return longestSequence;
}

size_t flipAndWinNaive(int x)
{
    size_t longestSeq = 0;
    size_t sizeOfInt = sizeof(x)*8;
    int mask = 1;
    for (size_t i = 0; i < sizeOfInt; ++i)
    {
        if ((x & mask) == 0) // the ith bit from right side is 0
        {
            longestSeq = std::max(longestSeq, countLongestOnes(x | mask));
        }
        mask <<= 1;
    }
    return longestSeq;
}

size_t flipAndWin(int x)
{
    if (allOnes(x))
    {
        throw std::runtime_error("all 1s. nothing to flip");
    }

    // use something that is similar to Kadena's algorithm
    size_t previousSeq = 0; 
    size_t currentSeq = 0;
    size_t maxSeq = 1; // there will always be at least 1
        
    size_t c = 0;
    while (c++ < sizeof(x)*8)
    {
        if ((x & 1) == 0)
        {
            previousSeq = currentSeq;
            currentSeq = 0;
        }
        else
        {
            ++currentSeq;
        }
        maxSeq = std::max(maxSeq, previousSeq+1+currentSeq);
        x >>= 1;
    }

    return maxSeq;
}


void compare(int x)
{
    std::cout << "comparing x=" << std::bitset<sizeof(x)*8>(x) << std::endl;
    std::cout << flipAndWinNaive(x) << std::endl;
    std::cout << flipAndWin(x) << std::endl;
}

int main(int argc, char* argv[])
{
    int a = 5;
    std::cout << sizeof(int) << std::endl;
    std::cout << sizeof(a) << std::endl;
    std::cout << countLongestOnes(0xfffa053b) << std::endl;

    std::cout << flipAndWinNaive(0xfffa053b) << std::endl;
    std::cout << flipAndWin(0xfffa053b) << std::endl;

    std::cout << hasOnlyOneZero(-2) << std::endl; // prints out 1

    compare(0xf456fabc);
    compare(0x129cfade);
    return 0;
}
