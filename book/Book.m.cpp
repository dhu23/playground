#include <array>
#include <stdexcept>
#include <iostream>


enum class Side : char {
    BUY = 'b', 
    SELL = 's'
};

Side getOtherSide(Side side) {
    if (side == Side::BUY) {
        return Side::SELL;
    } else {
        return Side::BUY;
    }
}

class AggBook {
public:

    struct Level {
        double price;
        size_t size;
    };

    Level& getLevel(size_t i, Side side) {
        if (i >= 10) {
            throw std::runtime_error("has only 10 levels");
        }
        return getSide(side)[i];
    }

    const Level& getLevel(size_t i, Side side) const {
        if (i >= 10) {
            throw std::runtime_error("has only 10 levels");
        }
        return getSide(side)[i];
    }

    // get level at aggressive side or passive side
    const Level& getAggressiveLevel(size_t i, Side side) const {
        return getSide(getOtherSide(side))[i];
    }

    const Level& getPassiveLevel(size_t i, Side side) const {
        return getSide(side)[i];
    }

    // get inside 
    // get sweep quantity/price

private:
    const std::array<Level, 10>& getSide(Side side) const {
        if (side == Side::BUY) {
            return _bids;
        } else {
            return _asks;
        }
    }
    std::array<Level, 10>& getSide(Side side) {
        if (side == Side::BUY) {
            return _bids;
        } else {
            return _asks;
        }
    }

    
    std::array<Level, 10> _bids;
    std::array<Level, 10> _asks;

};

size_t convertSide(size_t s) {
    return s ^ 1;
}

int main(int argc, char* argv[]) {
    AggBook bk;

    bk.getLevel(0, Side::BUY);


    std::cout << convertSide(1) << std::endl;
    std::cout << convertSide(0) << std::endl;

    return 0;
}
