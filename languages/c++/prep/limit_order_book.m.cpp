// Limit order book tracking individual orders
// each order has Id, price, quantity, side, timestamp
// needs to support the following APIs
// 1. addOrder(orderId, price, quantity, side)
//    a. FIFO priority inside same price level (order queue)
//    b. multiple order per price
//    c. cannot cross the book
// 2. cancelOrder(orderId)
//    a. must be O(1)
// 3. modifyOrder(orderId, newQty)
//    a. changing qty keeps priorty
//    b. chaning price = cancel + add
// 4. Match Incoming Order
//    executeMarketOrder(side, quantity)
//    it acts like a market order sweep of the far side

// 5. Basic APIs (this is the aggregate Level 2 stuff in order_book_lite.m.cpp)
//    a. bestBid()
//    b. bestAsk()
//    c. spread()
//    d. totalVolume(price)

// variation => Execution Engine
// order can cross. Instead of rejecting crossed orders, automatically match
// and you must return vector<Trade>, where trade includes:
// - aggressor order id
// - passive order id
// - price
// - quantity

// variation => performance upgrade
// what if std::map to track the levels are too slow?
// what if prices are bounded within range and tick size are fixed

// variation => market data feed simulator
// applyUpdate(message) where message types are ADD, MODIFY, DELETE, TRADE, SNAPSOT
// you are building an exchange feed handler

// variation => Multi-Thread Twist
// matching thread + market data thread
// - lock?
// - lock-free?
// - snapshot consisitency?
// - reader/writer separation

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>

enum class Side {
    Buy,
    Sell
};

// there are ways to implement price, order-id etc wrapped by one template
// but I am not doing that here for simplicity. 
// I am just going to make simple wrapper struct
struct Timestamp {
    uint64_t value;
};


// forward declaration for Order friend class
template<typename Compare>
class LimitOrderBookSide; 

class Order {
    template<typename Compare>
    friend class LimitOrderBookSide;

    std::string orderId_;
    int64_t price_;
    uint32_t qty_;
    Side side_;
    Timestamp timestamp_;

    Order* prev_; // accessed by LimitOrderBookSide
    Order* next_; // accessed by LimitOrderBookSide
public:
    Order(
        const std::string& orderId, 
        int64_t price, uint32_t qty,
        Side side, Timestamp timestamp 
    ): orderId_(orderId), price_(price), qty_(qty), side_(side), timestamp_(timestamp) {}

    const std::string& orderId() const { return orderId_; }
    int64_t price() const { return price_; }
    uint32_t qty() const { return qty_; }
    Side side() const { return side_; }
    Timestamp timestamp() const { return timestamp_; }
};

enum class AddResult {

};

enum class CancelResult {

};

enum class ModifyResult {

};

struct Level {
    int64_t price;
    Order* head; // point to the first Order, not the sentinel node
    Order* tail; // point to the last Order, not the sentinel node
};

template<typename Compare>
class LimitOrderBookSide {
    std::unordered_map<std::string, std::unique_ptr<Order>> orderMap_;
    std::map<int64_t, Level> levels_;

public:
    LimitOrderBookSide(): orderMap_(), levels_() {}

    AddResult addOrder(const std::string& orderId, int64_t price, uint32_t quantity, Side side) {

    }

    CancelResult cancelOrder(const std::string& orderId) {

    }

    ModifyResult modifyOrder(const std::string& orderId, int64_t price) {

    }

    ModifyResult modifyOrder(const std::string& orderId, uint32_t quantity) {

    }
};


class LimitOrderBook {
    LimitOrderBookSide<std::greater<int64_t>> bidSide_;
    LimitOrderBookSide<std::less<int64_t>> askSide_;
public:
    LimitOrderBook(): bidSide_(), askSide_() {}

    // add a new order to the internal FIFO priority
    AddResult addOrder(const std::string& orderId, int64_t price, uint32_t quantity, Side side) {

    }

    CancelResult cancelOrder(const std::string& orderId) {

    }

    ModifyResult modifyOrder(const std::string& orderId, int64_t price) {

    }

    ModifyResult modifyOrder(const std::string& orderId, uint32_t quantity) {

    }
};


int main(int argc, char *argv[]) {
    return 0; 
}