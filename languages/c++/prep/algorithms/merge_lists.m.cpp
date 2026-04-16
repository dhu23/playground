// merge sorted lists. It can be two lists or more.  

#include <iostream>
#include <queue>
#include <vector>
#include <format>

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

struct MergePQNode {
    int value;
    size_t listId;
    size_t idx;

    bool operator>(const MergePQNode& other) const {
        return value > other.value;
    }

    bool operator==(const MergePQNode& other) const {
        return value == other.value;
    }

    std::ostream& print(std::ostream& os) const {
        os << std::format("MergePQNode[value={},listId={},idx={}]", value, listId, idx);
        return os; 
    }
};

std::vector<int>
mergeMultipleLists(const std::vector<std::vector<int>>& lists) {
    std::vector<int> ret;

    // the idea is to store the head of each list in a priority queue, so that
    // it is O(1) time to extract which list has the minimum head at the moment
    // keep drain the current min of all the lists until all lists are exhausted

    // to make the operation efficient, the node stored in the priority queue:
    // 1. value (for ranking)
    // 2. list id (the position in the source nested vector)

    using MergePQ = std::priority_queue<
        MergePQNode, 
        std::vector<MergePQNode>, 
        std::greater<MergePQNode> // std::less would result in a max heap
    >;

    MergePQ pq{};

    std::cout << "building..." << std::endl;
    // populate the priority queue
    for (size_t listId = 0; listId < lists.size(); ++listId) {
        const std::vector<int>& sortedList = lists[listId];
        if (!sortedList.empty()) {
            std::cout << "pushing value=" << sortedList.front() << ", listid=" << listId << ",Idx=0" << std::endl;
            pq.push(MergePQNode{sortedList.front(), listId, 0});
        }
    }

    std::cout << "draining..." << std::endl;
    // drain the priority queue
    // every time a node is removed from the queue, check if there are more
    // from the same list source. Replenish if needed
    while (!pq.empty()) {
        MergePQNode topNode = pq.top();
        std::cout << "popped ";
        topNode.print(std::cout) << std::endl;

        ret.push_back(topNode.value);
        const std::vector<int>& sourceList = lists[topNode.listId];
    
        size_t nextIdx = topNode.idx + 1;

        pq.pop();
        if (nextIdx < sourceList.size()) {
            std::cout << "pushing value=" << sourceList[nextIdx]
                      << ", listid=" << topNode.listId << ",Idx=" << nextIdx
                      << std::endl;
            pq.push(MergePQNode{sourceList[nextIdx], topNode.listId, nextIdx});            
        }
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

void runTwoListTest(const std::vector<int>& list1, const std::vector<int>& list2) {
    printVec(std::cout, mergeTwoLists(list1, list2)) << std::endl;
}


void runMultiListTest(const std::vector<std::vector<int>>& lists) {
    printVec(std::cout, mergeMultipleLists(lists)) << std::endl;
}


int main(int argc, char* argv[]) {
    runTwoListTest({}, {});
    runTwoListTest({}, {1, 2, 3});
    runTwoListTest({1, 2, 3}, {});
    runTwoListTest({1, 2, 3, 99, 100}, {4, 5, 6, 7});

    runMultiListTest({{}, {}});
    runMultiListTest({{}, {1, 2, 3}});
    runMultiListTest({{1, 2, 3}, {}});
    runMultiListTest({{1, 2, 3, 99, 100}, {4, 5, 6, 7}});
    runMultiListTest({
        {1, 4, 8, 102},
        {2, 3, 29, 132},
        {40, 60, 70},
        {7, 23, 55}
    });

    return 0;
}