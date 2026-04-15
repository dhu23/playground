// merge sorted lists. It can be two lists or more 

#include <iostream>
#include <vector>

std::vector<int> 
mergeTwoLists(const std::vector<int>& list1, const std::vector<int>& list2) {
    std::vector<int> ret;

    size_t idx1 = 0;
    size_t idx2 = 0;

    // merge until one list is exhausted
    while (idx1 < list1.size() && idx2 < list2.size()) {
        int data1 = list1[idx1];
        int data2 = list2[idx2];
        if (data1 <= data2) {
            ret.push_back(data1);
            ++idx1;
        } else {
            ret.push_back(data2);
            ++idx2;
        }
    }

    // idx1 or idx2 is at the end of the list
    int idx = idx1 == list1.size() ? idx2 : idx1;
    size_t size = idx1 == list1.size() ? list2.size() : list1.size();
    const std::vector<int>& vec = idx1 == list1.size() ? list2 : list1;
    for (int i = idx; i < size; ++i) {
        ret.push_back(vec[i]);
    } 

    return ret;
}


std::ostream& printVec(std::ostream& os, const std::vector<int>& vec) {
    os << "vec=";
    for (int x : vec) {
        os << x << ' ';
    }
    return os;
} 

void runTest(const std::vector<int>& list1, const std::vector<int>& list2) {
    printVec(std::cout, mergeTwoLists(list1, list2)) << std::endl;
}


int main(int argc, char* argv[]) {
    runTest(std::vector<int>{}, std::vector<int>{});
    runTest(std::vector<int>{}, std::vector<int>{1, 2, 3});
    runTest(std::vector<int>{1, 2, 3}, std::vector<int>{});
    runTest(std::vector<int>{1, 2, 3, 99, 100}, std::vector<int>{4, 5, 6, 7});

    return 0;
}