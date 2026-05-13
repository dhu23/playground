// you have an array with all the numbers from 1 to N, where N is at most 32000
// The array may have duplicate entries and you do not know what N is. With
// only 4 kilobytes of memory available, how would you print all duplicate
// element in the array.

// 4 kilobytes of memory is 4096 bytes, that's 4096 * 8 > 32000 bits
// 1 to 32000 can be fully covered by a 4KB bitmap 
// we read in the numbers one by one can calculate the 
// the idea is to use a bit map

#include <array>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <iostream>

bool isBitSet(int16_t integer, int16_t bitPos) {
    int16_t mask = 1 << bitPos;
    return (integer & mask) == 1;
}

void setBit(int16_t& integer, int16_t bitPos) {
    int16_t mask = 1 << bitPos;
    integer |= mask;
}

template<size_t N>
class BitMap {
    constexpr static int16_t elementSize = sizeof(int16_t);
    std::array<int16_t, N> data_;

public:
    BitMap(): data_{0, N} {}

    void observe(int16_t num) {
        int16_t bitPos = num % elementSize;
        size_t intPos = num / elementSize;
        ::setBit(data_[intPos], bitPos);
    }

    bool has(int16_t num) const {
        int16_t bitPos = num % elementSize;
        size_t intPos = num / elementSize;
        return ::isBitSet(data_[intPos], bitPos);
    }
};

void printDuplicate(std::initializer_list<int16_t> inputs) {
    // BitMap uses 16-bit / 2-bye integer underlying types
    BitMap<2048> bm{};
    for (int16_t num : inputs) {
        if (bm.has(num)) {
            std::cout << num << ' ';
        } else {
            bm.observe(num);
        }
    }  
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    printDuplicate({1, 2, 3, 2, 1024, 1024, 2048, 8192, 8192, 2});
    return 0;
}