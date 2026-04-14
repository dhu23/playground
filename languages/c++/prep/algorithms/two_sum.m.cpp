// classical interview questions to find pairs of numbers from an array
// that sum up to a given target


#include <format>
#include <iostream>
#include <vector>


// the data contains sorted data ascendingly 
void runTwoSum(const std::vector<int>& data, int target) {
    int low = 0;
    int high = data.size() - 1;

    // keep searching when there are at least two data points
    while (low < high) {
        int sum = data[low] + data[high];
        if (sum == target) {
            std::cout << 
                std::format("{}(pos={}) + {}(pos={}) = {}",
                    data[low], low, data[high], high, target)
                << std::endl;
            ++low;
        } else if (sum > target) {
            --high;
        } else {
            ++low;
        }
    }
}


int main(int argc, char* argv[]) {
    std::vector<int> sortedInput{1, 3, 4, 5, 7, 8, 9, 10, 12, 13};
    runTwoSum(sortedInput, 15);
    return 0;
}