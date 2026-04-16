// calculate moving average of a stream

#include <cstddef>
#include <iostream>
#include <ostream>
#include <queue>
#include <stdexcept>

class MovingAverge {
    size_t windowSize_;
    std::queue<double> sequence_;
    double sum_;
public:
    MovingAverge(std::size_t windowSize)
    : windowSize_(windowSize), sequence_(), sum_(0.0) {
        if (windowSize_ <= 0) {
            throw std::runtime_error("need positive window size");
        }
    }

    double next(double value) {
        sequence_.push(value);
        sum_ += value;
        while (sequence_.size() > windowSize_) {
            double removed = sequence_.front();
            sequence_.pop();
            sum_ -= removed;
        }

        return this->average();
    }

    double average() const {
        if (sequence_.empty()) {
            throw std::runtime_error("no data in sequence");
        }
        return sum_ / sequence_.size();
    }
};


void testMV(MovingAverge& mv, double value) {
    std::cout << mv.next(value) << std::endl;
}


int main(int argc, char* argv[]) {
    MovingAverge mv(5);
    
    testMV(mv, 1); // 1.0
    testMV(mv, 1); // 1.0
    testMV(mv, 1); // 1.0
    testMV(mv, 1); // 1.0
    testMV(mv, 1); // 1.0
    testMV(mv, 1); // 1.0

    testMV(mv, 6); // 1 + 1 + 1 + 1 + 6 -> 2.0
    testMV(mv, 6); // 1 + 1 + 1 + 6 + 6 -> 3.0
    testMV(mv, 16); // 1 + 1 + 6 + 6 + 16 -> 6.0

    return 0;
}