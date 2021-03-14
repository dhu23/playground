// given a positive integer, print the next smallest and the next largest 
// number that have the same number of 1 bits in their binary representation


//                    001101001001100101011100    trailing 1s followed by 0s
// next largest       001101001001100101100011    getNext (full case)
// previous smaller   001101001001100101011010    getPrev (simple)

//                    001101001001100101000011    trailing 0s followed by 1s
// next largest       001101001001100101000101    getNext (simple)
// previous smaller   001101001001100100111000    getPrev (full case)

// next largest method: 
// find the least significant 01. change it to 10 and move all the 1s to its
// right to the right end

#include <stdexcept>
#include <iostream>
#include <cassert>
#include <bitset>

bool isOn(int x, int i)
{
    return (x & (1 << i)) != 0; // true means the bit is 1
}


int setBit(int x, int i, bool on)
{
    if (on)
    {
        x |= (1 << i);
    }
    else
    {
        x &= ~(1 << i);
    }
    return x;
}

int getPrev(int x)
{
    int i = 1;
    size_t oneCount = ((x & 1) != 0) ? 1 : 0;
    while (i < sizeof(x)*8-1)
    {
        if (isOn(x, i))
        {
            ++oneCount;
        }
        // search for least significant 10
        if (isOn(x, i) && !isOn(x, i-1))
        {
            x = setBit(x, i, false);
            x = setBit(x, i-1, true);

            --oneCount; // 1s to the right of this 10(to 01) bits
            // set the right most 000001111 to 
            //                    111100000
            x &= ~((1 << (i-1))-1);
            int mask = ((1 << (i-1))-1) - ((1 << (i-1-oneCount))-1);
            x |= mask;
            return x;
        }
        ++i;
    }
    throw std::runtime_error("next smallest not available");
}

int getNext(int x)
{
    int i = 1;
    size_t oneCount = ((x & 1) != 0) ? 1 : 0;
    while (i < sizeof(x)*8-1)
    {
        if (isOn(x, i))
        {
            ++oneCount;
        }
        // search for least significant 01
        if (!isOn(x, i) && isOn(x, i-1))
        {
            x = setBit(x, i, true);
            x = setBit(x, i-1, false);
            --oneCount;
            // set #oneCount 1s to the right end

            // clear to the right
            x &= ~((1 << (i-1))-1); 
            // std::cout << "---" << oneCount << std::endl;
            // std::cout << std::bitset<32>(x) << std::endl;
            x |= ((1 << oneCount) - 1);
            // std::cout << std::bitset<32>(x) << std::endl;
            
            return x;
        }
        ++i;
    }
    throw std::runtime_error("next largest not available");
}   

void test(int x)
{
    std::cout << std::bitset<32>(x) << std::endl; 
    std::cout << std::bitset<32>(getNext(x)) << std::endl; 
    std::cout << std::bitset<32>(getPrev(x)) << std::endl; 
}

int main(int argc, char* argv[])
{
    test(0x34995);
    test(0x34995c);
    test(0x349943);
    return 0;
}
