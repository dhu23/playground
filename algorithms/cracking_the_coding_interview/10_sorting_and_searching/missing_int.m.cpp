// given an input file with 4 billion non-negative integers, provide an 
// algorithm to generate an integer that is not contained in the file. Assume
// you have 1GB of memory available for this task

#include <fstream>
#include <iostream>
#include <bitset>
#include <vector>
#include <array>
#include <cassert>

using std::min;
using std::vector;
using std::array;
using std::cout;
using std::endl;

// integer with 32 bit can have 2^31-1 non-negative integers out there
// thus this can be represented by using some sort of bit set
// total input size up to 4 billion
int findMissingNumber(const vector<int>& input)
{
    if (input.empty()) return -1;
    // using array to simulate a bitset
    
    // 67108864 = 2^31 / 2^5 = 2^26
    typedef array<int, 67108864> Bitset;
    Bitset* pBitset = new Bitset;
    Bitset& bitset = *pBitset;

    for (auto i : input)
    {
        assert(i > 0);
        // int 67108863 
        // int 67108862 
        // ...
        // int 1 
        // int 0         31, 30, ...., 2, 1, 0 
        size_t intIdx = i / 32;
        size_t bitIdx = i % 32;

        int mask = 1 << bitIdx;
        bitset[intIdx] |= mask;
    }
    // find the smallest number missing
    for (size_t i = 0; i < bitset.size(); ++i)
    {
        int mask = 1;
        for (short b = 0; b < 32; ++b)
        {
            if ((bitset[i] & mask) == 0 && i*32+b > 0) 
            {
                delete pBitset;
                return (i*32+b); 
            }
            mask = mask << 1;
        }
    }
    delete pBitset;
    return -1;
}

// same as above, there are 2^31-1 non-negative integers out there. 
// 2^20 byte is 1MB. for 10MB, we can use 8MB, which is 2^23 bytes. 
// that is 2^21 integers(4 byte)
// total input size up to 1 billion
int findMissingNumberDistinct(const vector<int>& input)
{
    // bin size 2^10 = 1024, covering all integers
    // bin 0: 0 ~ 1023
    // bin 1: 1024 _ 2047
    // ...
    // bin 2097151: .....
    typedef array<int, 2097152> Histogram; // each slot holds ~2 billion
    int binSize = 1024;
    Histogram* pHisto = new Histogram;
    for (auto i : input)
    {
        (*pHisto)[i/binSize] += 1;
    }
    int binSlotIdx = -1;
    for (size_t i = 0; i < pHisto->size(); ++i)
    {
        if ((i == 0 && (*pHisto)[i] < binSize-1) || 
            (i > 0 && (*pHisto)[i] < binSize))
        {
            binSlotIdx = i;
            break;
        }
    }
    delete pHisto;
    if (binSlotIdx < 0) 
    {
        return -1;
    }

    // find the missing number in histogram slot i, 
    // from i*1024 to (i+1)*1024-1
    int lo = binSlotIdx*1024;
    int hi = (binSlotIdx+1)*1024;

    // bin size is 1024 = 2^10, to minimize memory usage, 
    // use an array of size 2^8
    typedef array<int, 256> Bitset;
    Bitset* pBitset = new Bitset;
    Bitset& bitset = *pBitset;

    for (auto i : input)
    {
        if (i < lo || i >= hi) continue;
        size_t intIdx = (i-lo) / 32;
        size_t bitIdx = (i-lo) % 32;

        int mask = 1 << bitIdx;
        bitset[intIdx] |= mask;
    }
    // find the smallest number missing
    for (size_t i = 0; i < bitset.size(); ++i)
    {
        int mask = 1;
        for (short b = 0; b < 32; ++b)
        {
            if ((bitset[i] & mask) == 0 && i*32+b+lo > 0)
            {
                return (i*32+b+lo);
            }
            mask = mask << 1;
        }
    }
    return -1;
}

int main(int argc, char* argv[])
{
    cout << "finding" << endl;
    cout << findMissingNumber(vector<int>{1, 2, 5, 6, 7, 8, 9, 2012}) << endl;
    cout << findMissingNumberDistinct(
            vector<int>{1, 3, 4, 5, 6, 7, 8, 10, 23, 156}) << endl;

    return 0;
}
