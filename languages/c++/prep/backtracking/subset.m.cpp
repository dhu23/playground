
// subset can be seen a tree traversal of multiple layers of decision
// each node decides if element i is chosen or not. 
// the Backtracking tree is of N layers and every leaf is accepted
// so there are 2^N leaves

// 1. state of should be vector<int> that captures the chosen subset
// 2. the decision at each level should be either choose or not choose the element

#include "utils.h"
#include <vector>


void 
runSubset_(
    const std::vector<int>& input,
    std::vector<int>& subset, 
    int candidateIdx, 
    Visitor& visitor,
    bool emptyLast
) {
    // recurision is finished, now traversaled to the leaf node
    if (candidateIdx >= input.size()) {
        visitor.accept(subset);
    } else {
        // for candidiate index, we can choose or skip
        // the following is equivalent to a loop to iterate every subtree
        // try skip
        if (!emptyLast) {
            runSubset_(input, subset, candidateIdx + 1, visitor, emptyLast);
        }
        // try choose
        subset.push_back(input[candidateIdx]);
        runSubset_(input, subset, candidateIdx + 1, visitor, emptyLast);
        subset.pop_back();
        if (emptyLast) {
            runSubset_(input, subset, candidateIdx + 1, visitor, emptyLast);
        }
    }
}


void runSubset(const std::vector<int>& input, Visitor& visitor, bool emptyLast) {
    std::vector<int> subset{};
    runSubset_(input, subset, 0, visitor, emptyLast);
}


int main(int argc, char* argv[]) {
    std::vector<int> original{1, 2, 3};
    Printer printer{};
    runSubset(original, printer, false);

    runSubset(std::vector<int>{3, 4, 5, 6}, printer, true);
}