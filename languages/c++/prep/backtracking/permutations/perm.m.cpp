// generate permutation of integer numbers
// so for a given list of unique integers, it generates all permutations
// e.g., given 1, 2, 3
// the program would give back
// 1, 2, 3
// 1, 3, 2
// 2, 1, 3
// 2, 3, 1
// 3, 1, 2
// 3, 2, 1

#include <iostream>
#include <vector>
#include <algorithm>
// #include <unordered_set>


std::ostream& printVec(std::ostream& os, const std::vector<int> &data) {
    if (data.empty()) {
        os << "[]";
    } else {
        os << '[' << data[0];
        for (int i = 1; i < data.size(); ++i) {
            os << ',' << data[i];
        }
        os << ']';
    }
    return os;
}

class Visitor {
public:
    Visitor() {}
    virtual ~Visitor() {}

    virtual void accept(const std::vector<int>& data) = 0;
};

class Printer : public Visitor {
public:
    Printer() {}
    ~Printer() {}

    void accept(const std::vector<int>& data) {
        printVec(std::cout, data) << std::endl;
    }
};

void 
runPermutation(
    const std::vector<int>& choices,
    std::vector<int>& chosen, // capture backtracking state
    Visitor& visitor
) {
    // if chosen is full, verify if this should be printed
    if (chosen.size() == choices.size()) {
        visitor.accept(chosen);
    } else {
        // chosen.size() positions are chosen, work on the next position
        // each integer in choices array is a potential candidate
        for (int candidate : choices) {
            // not a verify efficient way to search
            auto alreadyChosen = std::find(chosen.begin(), chosen.end(), candidate);
            // proceed if it is not seen before
            if (chosen.end() == alreadyChosen) {
                // put a candidate in
                chosen.push_back(candidate);
                runPermutation(choices, chosen, visitor);
                // remove the candidate to try the next one
                chosen.pop_back();
            }
        }
    }
}


// backtracking state and path logic (very helpful way to think)
// this is a pre-order full traversal of the choice tree. The state should contain
// 1. a vector that are chosen. (this can be a set, that tracks path)
// 2. a method to determine the next one to choose. (this can be just a simple loop)
// beyond these, we need a way to test if
// 1. The state is a leaf state (with all nodes chosen)
// 2. whether the leaf state is a valid state. (or short cut to avoid bad ones)
void permutation(const std::vector<int>& input, Visitor& visitor) {
    std::vector<int> chosen{};
    runPermutation(input, chosen, visitor);
}

// an alternative way to search by using array for O(1) search 
class ChoiceState {
    // track the permutation state, size is variable
    std::vector<int> state_;
    // track if a number is choise, size is always the full element size
    // whether number i is found in state_ is valued as bucket_[i] = 1/0
    // this can probably be implemented by a bitmap
    std::vector<int> bucket_; 

private:
    bool doesSupport(int num) const {
        return num > 0 && num < this->bucket_.size();
    }

public:
    ChoiceState(int size)
    : state_{}
    , bucket_(size + 1, 0) {}

    bool add(int num) {
        if (this->doesSupport(num)) {
            this->state_.push_back(num);
            this->bucket_[num] = 1;
            return true;
        }
        return false;
    }

    bool backout() {
        if (this->state_.empty()) {
            return false;
        }
        int num = this->state_.back();
        this->state_.pop_back();
        this->bucket_[num] = 0;
        return true;
    }

    bool has(int num) const {
        if (!this->doesSupport(num)) {
            return false;
        }
        return this->bucket_[num] == 1;
    }

    int size() const {
        return state_.size();
    }

    const std::vector<int>& getState() const {
        return this->state_;
    }

    std::ostream& print(std::ostream& os) const {
        os << "state:";
        printVec(os, state_);
        os << ", bucket:";
        printVec(os, bucket_);
        os << ", size=" << this->size();
        return os;
    }
};


void runPermuation_(const std::vector<int>& input, ChoiceState& state, int maxDepth, Visitor& visitor) {
    if (state.size() == input.size() || state.size() == maxDepth) {
        visitor.accept(state.getState());
    } else {
        for (int candidate : input) {
            if (!state.has(candidate)) {
                state.add(candidate);
                runPermuation_(input, state, maxDepth, visitor);
                state.backout();
            }
        }
    }
}

void permutation_(const std::vector<int>& input, int depth, Visitor& visitor) {
    ChoiceState state(input.size());
    runPermuation_(input, state, depth, visitor);
}


void testState() {
    ChoiceState cs{3};
    cs.print(std::cout) << std::endl;

    cs.add(3);
    cs.print(std::cout) << std::endl;

    cs.add(2);
    cs.print(std::cout) << std::endl;

    cs.backout();
    cs.print(std::cout) << std::endl;

    cs.backout();
    cs.print(std::cout) << std::endl;
}


int main(int argc, char* argv[]) {
    std::vector<int> original{1, 2, 3};
    Printer p{};

    permutation(original, p);
    testState();
    permutation_(original, 2, p);

    return 0;
}