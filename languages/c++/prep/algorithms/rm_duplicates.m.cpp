// remove duplicates in place from a sorted array such as 
// 1, 1, 1, 2, 3, 3, 4, 5, 6, 6, 6
// so that the unique ones reamins
// also return the new length


#include <iostream>
#include <vector>

// the idea is to maintain sorted data on the left side of the array
// keep an advancing index to look at data that needs to be evaluated
// keep another index to indicate the next postiion to fill for the clean range
void dedup(std::vector<int>& data) {
    size_t end = 0; // the last position of the clean range. the first is always good
    size_t idx = 1;

    size_t size = data.size();
    for (size_t idx = 1; idx < size; ++idx) {
        if (data[idx] == data[end]) {
            continue;
        } else {
            // when the current idx points to a new value
            data[++end] = data[idx];
        }
    }
    
    // print the clean set
    std::cout << "dedup to " << end + 1 << ':';
    for (size_t i = 0; i < size; ++i) {
        std::cout << data[i] << ' ';
        if (i == end) {
            std::cout << "| ";
        }
    }
    std::cout << std::endl;
}


int main(int argc, char* argv[]) {
    std::vector<int> input{1, 1, 1, 2, 3, 3, 4, 5, 6, 6, 6};
    dedup(input);
    input = std::vector<int>{1, 1, 100, 100, 100, 100, 200, 200};
    dedup(input);
    return 0;
}