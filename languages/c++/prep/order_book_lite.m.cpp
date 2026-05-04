// this is to track a aggregated Level 2 limit order book (lite)
// assume 1 tick = $0.01, 1 cent
// 101.5    10150      1000    A         
// 100.5    10050      1200    A
// 100.1    10010      1500    A
// --------------------------------- spread
// 99.7      9970      1400    B
// 99.0      9900      2000    B

// supported APIs
// 1. add quantity to a level
// 2. remove quantity to a level (cannot except existing size)
// 3. remove a whole price level
// 4. fetch level size
// 5. get bid/ask/spread etc
// 6. 

#include <cstdint>
#include <map>
#include <optional>
#include <iostream>
class OrderBookLite {
public:
    enum class RemoveResult {
        LevelSizeReduced,
        LevelRemoved,
        OverdraftRejected,
        UnknownLevel
    };

    enum class AddResult {
        CrossedBookRejection,
        LevelSizeAdded,
        LevelCreated
    };

private:
    template <typename Compare> class OrderBookSide {
        std::map<int64_t, uint64_t, Compare> levels_;

    public:
        OrderBookSide() = default;

        AddResult add(int64_t price, uint64_t quantity) {
            auto found = levels_.find(price);
            if (found == levels_.end()) {
                levels_.emplace(price, quantity);
                return AddResult::LevelCreated;
            } else {
                found->second += quantity;
                return AddResult::LevelSizeAdded;
            }
        }

        RemoveResult remove(int64_t price, uint64_t quantity) {
            auto found = levels_.find(price);
            if (found == levels_.end()) {
                return RemoveResult::UnknownLevel;
            } else {
                if (found->second > quantity) {
                    found->second -= quantity;
                    return RemoveResult::LevelSizeReduced;
                } else if (found->second == quantity) {
                    levels_.erase(found);
                    return RemoveResult::LevelRemoved;
                } else {
                    return RemoveResult::OverdraftRejected;
                }
            }
        }

        std::optional<int64_t> bestPrice() const {
            if (levels_.empty()) {
                return std::nullopt;
            } else {
                return levels_.begin()->first;
            }
        }

        RemoveResult remove(int64_t price) {
            auto found = levels_.find(price);
            if (found == levels_.end()) {
                return RemoveResult::UnknownLevel;
            } else {
                levels_.erase(found);
                return RemoveResult::LevelRemoved;
            }
        }

        std::optional<uint64_t> getQuantity(int64_t price) const {
            auto found = levels_.find(price);
            if (found == levels_.end()) {
                return std::nullopt;
            } else {
                return found->second;
            }
        }

        std::ostream& print(std::ostream& os, bool bestPriceFirst=true) const {
            if (bestPriceFirst) {
                // traverse the book from best price to worst price
                for (const auto& kv : levels_) {
                    os << "px: " << kv.first << " ==> " << kv.second << std::endl;
                }
            } else {
                for (auto it = levels_.rbegin(); it != levels_.rend(); ++it) {
                    os << "px: " << it->first << " ==> " << it->second << std::endl;
                }
            }
            return os;
        }
    };

    // bids are sorted descendingly by prices
    OrderBookSide<std::greater<int64_t>> bids_;
    // asks are sorted ascendingly by prices
    OrderBookSide<std::less<int64_t>> asks_;
public:
    enum class Side {
        BID,
        ASK
    };

    OrderBookLite() = default;

    AddResult add(int64_t price, uint64_t quantity, Side side) {
        switch (side) {
            case Side::BID: {
                std::optional<int64_t> bestAsk = asks_.bestPrice();
                if (bestAsk && price >= *bestAsk) {
                    return AddResult::CrossedBookRejection;
                }
                return bids_.add(price, quantity);
            }

            case Side::ASK: {
                std::optional<int64_t> bestBid = bids_.bestPrice();
                if (bestBid && price <= *bestBid) {
                    return AddResult::CrossedBookRejection;
                }
                return asks_.add(price, quantity);
            }
        }
        __builtin_unreachable();
    }

    std::optional<int64_t> bestBid() const {
        return bids_.bestPrice();
    }

    std::optional<int64_t> bestAsk() const {
        return asks_.bestPrice();
    }

    std::optional<uint64_t> getQuantity(int64_t price) const {
        std::optional<int64_t> bestAsk = asks_.bestPrice();
        if (bestAsk && price >= *bestAsk) {
            return asks_.getQuantity(price);
        }

        std::optional<int64_t> bestBid = bids_.bestPrice();
        if (bestBid && price <= *bestBid) {
            return bids_.getQuantity(price);
        }

        return std::nullopt;
    }

    RemoveResult remove(int64_t price, uint64_t quantity) {
        std::optional<int64_t> bestAsk = asks_.bestPrice();
        if (bestAsk && price >= *bestAsk) {
            return asks_.remove(price, quantity);
        }

        std::optional<int64_t> bestBid = bids_.bestPrice();
        if (bestBid && price <= *bestBid) {
            return bids_.remove(price, quantity);
        }

        return RemoveResult::UnknownLevel;
    }

    RemoveResult remove(int64_t price) {
        std::optional<int64_t> bestAsk = asks_.bestPrice();
        if (bestAsk && price >= *bestAsk) {
            return asks_.remove(price);
        }

        std::optional<int64_t> bestBid = bids_.bestPrice();
        if (bestBid && price <= *bestBid) {
            return bids_.remove(price);
        }
        
        // the price is in the spread
        return RemoveResult::UnknownLevel;
    }

    std::ostream& print(std::ostream& os) const {
        os << "========= Book ==========" << std::endl;
        os << "Ask:" << std::endl;
        asks_.print(os, false) << std::endl;
        os << "Bid:" << std::endl;
        bids_.print(os) << std::endl;
        os << "=========================" << std::endl;
        return os;
    }
};


std::ostream& 
operator<<(std::ostream& os, OrderBookLite::AddResult addResult) {
    switch (addResult) {
        case OrderBookLite::AddResult::CrossedBookRejection: {
            os << "CrossedBookRejection";
            return os;
        }
        case OrderBookLite::AddResult::LevelSizeAdded: {
            os << "LevelSizeAdded";
            return os;
        }
        case OrderBookLite::AddResult::LevelCreated: {
            os << "LevelCreated";
            return os;
        }
    }
    __builtin_unreachable();
}

std::ostream& 
operator<<(std::ostream& os, OrderBookLite::RemoveResult removeResult) {
    switch (removeResult) {
        case OrderBookLite::RemoveResult::LevelSizeReduced: {
            os << "LevelSizeReduced";
            return os;
        }
        case OrderBookLite::RemoveResult::LevelRemoved: {
            os << "LevelRemoved";
            return os;
        }
        case OrderBookLite::RemoveResult::OverdraftRejected: {
            os << "OverdraftRejected";
            return os;
        }
        case OrderBookLite::RemoveResult::UnknownLevel: {
            os << "UnknownLevel";
            return os;
        }
    }
    __builtin_unreachable();
}

int main(int argc, char* argv[]) {
    OrderBookLite book{};

    book.add(9950, 1000, OrderBookLite::Side::BID);
    book.add(9980, 1500, OrderBookLite::Side::BID);
    book.add(10100, 200, OrderBookLite::Side::ASK);
    book.add(10050, 2000, OrderBookLite::Side::ASK);

    book.print(std::cout) << std::endl;

    book.add(10050, 500, OrderBookLite::Side::ASK);
    auto removeResult = book.remove(9950);
    std::cout << removeResult << std::endl;

    book.print(std::cout) << std::endl;
    
    auto addResult = book.add(12000, 100, OrderBookLite::Side::BID);
    std::cout << addResult << std::endl;

    auto qty = book.getQuantity(10050);
    if (qty) {
        std::cout << *qty << std::endl;
    }

    removeResult = book.remove(10050, 2000);
    std::cout << removeResult << std::endl;
    book.print(std::cout) << std::endl;

    return 0;
}