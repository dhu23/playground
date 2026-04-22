// this is to track a limit order book (lite)
// assume 1 tick = $0.01, 1 cent
// 101.5    10150      1000    A         
// 100.5    10050      1200    A
// 100.1    10010      1500    A
// 99.7      9970      1400    B
// 99.0      9900      2000    B

#include <cstdint>
#include <map>
#include <optional>
#include <iostream>
class OrderBookLite {
public:
    template <typename Compare> class OrderBookSide {
       std::map<int64_t, int32_t, Compare> levels_;

    public:
        bool add(int64_t price, int32_t quantity) {
            auto found = levels_.find(price);
            if (found == levels_.end()) {
                if (quantity <= 0) {
                    return false;
                } else {
                    levels_.emplace(price, quantity);
                    return true;
                }
            } else {
                int32_t qty = found->second + quantity;
                if (qty <= 0) {
                    return false;
                } else {
                    found->second = qty;
                    return true;
                }
            }
        }

        std::optional<int64_t> bestPrice() const {
            if (levels_.empty()) {
                return std::optional<int64_t>();
            } else {
                return levels_.begin()->first;
            }
        }

        bool remove(int64_t price) {
            auto found = levels_.find(price);
            if (found == levels_.end()) {
                return false;
            } else {
                levels_.erase(found);
                return true;
            }
        }

        std::ostream& print(std::ostream& os, bool inOrder=true) const {
            if (inOrder) {
                // in order traverse the book from bestPrice to
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

private:
    // bids are sorted descendingly by prices
    OrderBookSide<std::greater<int64_t>> bids_;
    // asks are sorted ascendingly by prices
    OrderBookSide<std::less<int64_t>> asks_;
public:
    enum class Side {
        BID,
        ASK
    };

    OrderBookLite() {}

    bool add(int64_t price, int32_t quantity, Side side) {
        switch (side) {
            case Side::BID:
                return addBid(price, quantity);
                
            case Side::ASK:
                return addAsk(price, quantity);
        }
    }

    bool addBid(int64_t price, int32_t quantity) {
        return bids_.add(price, quantity);
    }

    bool addAsk(int64_t price, int32_t quantity) {
        return asks_.add(price, quantity);
    }

    std::optional<int64_t> bestBid() const {
        return bids_.bestPrice();
    }

    std::optional<int64_t> bestAsk() const {
        return asks_.bestPrice();
    }

    bool remove(int64_t price) {
        if (price >= asks_.bestPrice()) {
            return asks_.remove(price);
        } else if (price <= bids_.bestPrice()) {
            return bids_.remove(price);
        } else {
            // the price is in the spread
            return false;
        }
    }

    std::ostream& print(std::ostream& os) const {
        os << "Ask:" << std::endl;
        asks_.print(os, false) << std::endl;
        os << "Bid:" << std::endl;
        bids_.print(os) << std::endl;
        return os;
    }
};


int main(int argc, char* argv[]) {
    OrderBookLite book{};

    book.addBid(9950, 1000);
    book.addBid(9980, 1500);
    book.addAsk(10100, 200);
    book.addAsk(10050, 2000);

    book.print(std::cout) << std::endl;

    book.addAsk(10050, 500);
    book.remove(9950);

    book.print(std::cout) << std::endl;

    return 0;
}