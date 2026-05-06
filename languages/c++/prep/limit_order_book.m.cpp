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

#include <cassert>
#include <chrono>
#include <cstdint>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

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

struct Trade {
    std::string aggressorId;
    std::string passiveId;
    int64_t price;
    uint32_t qty;

    std::ostream& print(std::ostream& os) const {
        os 
            << "Trade[`aggressorId=" << aggressorId
            << ",`passiveId=" << passiveId << ",`px=" << price
            << ",`qty=" << qty << ']';
        return os;
    }
};

std::ostream& operator<<(std::ostream& os, const Trade& trade) {
   return trade.print(os);
}

template<typename Itr>
std::ostream& printIterator(std::ostream& os, Itr begin, Itr end, char separator) {
    os << '[';
    Itr it = begin;
    while (it != end) {
        os << *it << separator;
        ++it;
    }
    os << ']';
    return os; 
}

struct AddResult {
    enum class Type {
        DuplicateOrderIdRejection,
        NonPositiveQtyRejection,
        PassiveOrderAdded,
        AggressiveOrderAdded,
        AggressiveOrderFilled,
        AggressiveOrderRejection
    };

    static AddResult duplicatRejection() {
        return AddResult{Type::DuplicateOrderIdRejection, {}};
    }

    static AddResult nonPositiveQtyRejection() {
        return AddResult{Type::NonPositiveQtyRejection, {}};
    }

    static AddResult passiveOrderAdded() {
        return AddResult{Type::PassiveOrderAdded, {}};
    }

    // Order is inserted into the book after sweeping the far side
    static AddResult aggressiveOrderAdded(std::vector<Trade>&& trades) {
        return AddResult{Type::AggressiveOrderAdded, std::move(trades)};
    }

    // the far side is swept and the order is fully filled
    static AddResult aggressiveOrderFilled(std::vector<Trade>&& trades) {
        return AddResult{Type::AggressiveOrderFilled, std::move(trades)};
    }

    static AddResult aggressiveOrderRejection() {
        return AddResult{Type::AggressiveOrderRejection, {}};
    }

    Type type;
    std::vector<Trade> trades;
};

std::ostream& operator<<(std::ostream& os, AddResult::Type type) {
    switch (type) {
        case AddResult::Type::DuplicateOrderIdRejection:
            os << "DuplicateOrderIdRejection";
            return os;
        case AddResult::Type::NonPositiveQtyRejection:
            os << "NonPositiveQtyRejection";
            return os;
        case AddResult::Type::PassiveOrderAdded:
            os << "PassiveOrderAdded";
            return os;
        case AddResult::Type::AggressiveOrderAdded:
            os << "AggressiveOrderAdded";
            return os;
        case AddResult::Type::AggressiveOrderFilled:
            os << "AggresiveOrderFilled";
            return os;
        case AddResult::Type::AggressiveOrderRejection:
            os << "AggressiveOrderRejection";
            return os;
        }
    __builtin_unreachable();
}

std::ostream& operator<<(std::ostream& os, const AddResult& res) {
    os << "AddResult[`type=" << res.type << ",`trades=";
    printIterator(os, res.trades.begin(), res.trades.end(), ',') << ']';
    return os;
}

struct CancelResult {
    enum class Type {
        UnknownOrderId,
        Cancelled,
        CancelRej
    };

    Type type;

    static CancelResult unknownOrderId() {
        return CancelResult{Type::UnknownOrderId};
    }

    static CancelResult cancelled() {
        return CancelResult{Type::Cancelled};
    }

    static CancelResult cancelRejection() {
        return CancelResult{Type::CancelRej};
    }
};

std::ostream& operator<<(std::ostream& os, CancelResult::Type type) {
    switch (type) {
        case CancelResult::Type::UnknownOrderId:
            os << "UnknownOrderId";
            return os;
        case CancelResult::Type::Cancelled:
            os << "Cancelled";
            return os;
        case CancelResult::Type::CancelRej:
            os << "CancelRej";
            return os;
    }
    __builtin_unreachable();
}

std::ostream& operator<<(std::ostream& os, const CancelResult& res) {
    os << "CancelResult[`type=" << res.type << ']';
    return os;
}

struct ModifyResult {
    enum class Type {
        UnknownOrderId,
        ModificationRejected,
        NothingToChange,
        QuantityChanged,
        PriceChanged,
        OrderFilled
    };

    Type type;
    std::vector<Trade> trades;

    static ModifyResult unknownOrderId() {
        return ModifyResult{Type::UnknownOrderId, {}};
    }

    static ModifyResult modificationRejected() {
        return ModifyResult{Type::ModificationRejected, {}};
    }

    static ModifyResult nothingToChange() {
        return ModifyResult{Type::NothingToChange, {}};
    }

    static ModifyResult quantityChanged() {
        return ModifyResult{Type::QuantityChanged, {}};
    }

    static ModifyResult priceChanged(std::vector<Trade>&& trades) {
        return ModifyResult{Type::PriceChanged, std::move(trades)};
    }

    static ModifyResult orderFilled(std::vector<Trade>&& trades) {
        return ModifyResult{Type::OrderFilled, std::move(trades)};
    }
};

std::ostream& operator<<(std::ostream& os, ModifyResult::Type type) {
    switch (type) {
        case ModifyResult::Type::UnknownOrderId:
            os << "UnknownOrderId";
            return os;
        case ModifyResult::Type::ModificationRejected:
            os << "ModificationRejected";
            return os;
        case ModifyResult::Type::NothingToChange:
            os << "NothingToChange";
            return os;
        case ModifyResult::Type::QuantityChanged:
            os << "QuantityChanged";
            return os;
        case ModifyResult::Type::PriceChanged:
            os << "PricedChanged";
            return os;
        case ModifyResult::Type::OrderFilled:
            os << "OrderFilled";
            return os;
    }
    __builtin_unreachable();
}

std::ostream& operator<<(std::ostream& os, const ModifyResult& res) {
    os << "ModifyResult[`type=" << res.type << ']';
    return os;
}

class LimitOrderBook {

    struct Level;
    struct Order {
        std::string orderId_;
        int64_t price_;
        uint32_t qty_;
        Side side_;
        Timestamp timestamp_;

        Order* prev_ = nullptr; // accessed by LimitOrderBookSide
        Order* next_ = nullptr; // accessed by LimitOrderBookSide

        // data structure is responsible for keeping order <-> level consistent
        Level* level_ = nullptr; 

        Order(const std::string &orderId, int64_t price, uint32_t qty,
              Side side, Timestamp timestamp)
            : orderId_(orderId)
            , price_(price), qty_(qty), side_(side), timestamp_(timestamp) {}

        const std::string &orderId() const { return orderId_; }
        int64_t price() const { return price_; }
        uint32_t qty() const { return qty_; }
        Side side() const { return side_; }
        Timestamp timestamp() const { return timestamp_; }

        std::ostream& print(std::ostream& os) const {
            os 
                << "Order[`orderId=" << orderId_ 
                << ",`px=" << price_ << ",`qty=" << qty_
                << ",`side=";
            switch(side_) {
                case Side::Buy: 
                    os << 'B';
                    break;
                case Side::Sell:
                    os << 'S';
                    break;
                default:
                    break;    
            }
            os << ']';
            return os;
        }
    };

    struct Level {
        int64_t price;
        Order* head; // point to the first Order, not the sentinel node
        Order* tail; // point to the last Order, not the sentinel node
        size_t orderCount = 0;
        uint32_t totalQty = 0;

        std::ostream& print(std::ostream& os) const {
            os << "Level[`px=" << price;
            os << ",`ord#=" << orderCount << ",`qty=" << totalQty;
            os << ",`orders=[";
            Order* order = head;
            while (order != nullptr) {
                os << order->orderId_ << ',';
                order = order->next_;
            }
            os << "]]";
            return os;
        }

        friend std::ostream& operator<<(std::ostream& os, const Level& level) {
            return level.print(os);
        }
    };

    template<typename Compare>
    class BookSide {
        // track levels by map to order prices from best to worst
        std::map<int64_t, Level, Compare> levels_;
        Compare priceComparator_;

    public:
        BookSide(): levels_{}, priceComparator_(Compare()) {}

        // for a cross price, check if this is marketable:
        // if this is a bid book, cross price(aggressive sell) <= bestBid
        // if this is an ask book, cross price(aggressive buy) >= bestAsk  
        bool isCrossPriceMarketable(int64_t crossPrice) const {
            if (empty()) {
                return false;
            }
            
            bool crossable = !priceComparator_(crossPrice, levels_.begin()->first);
            std::cout << "crossPrice:" << crossPrice
                      << ",this side:" << levels_.begin()->first 
                      << ",crossable:" << crossable
                      << std::endl;
            return crossable;
        }

        bool empty() const {
            return levels_.empty();
        }

        std::optional<int64_t> bestPrice() const {
            if (empty()) {
                return std::nullopt;
            }
            return levels_.begin()->first;
        }

        void addOrder(Order& order) {
            auto foundLevel = levels_.find(order.price_);
            if (foundLevel == levels_.end()) {
                // the first order in this price level
                // the order are not linked to another other orders
                auto [it, inserted] = levels_.emplace(
                    order.price_, 
                    Level{order.price_, &order, &order, 1, order.qty_});

                // TODO check inserted flag
                
                // update order's level pointer to point the new level for fast access
                order.level_ = &(it->second);

            } else {
                // append to the end of the price level
                Level& existingLevel = foundLevel->second;
                Order* pLastOrder = existingLevel.tail;
                if (pLastOrder == nullptr) {
                    // Should not happen!
                } else {
                    std::cout 
                        << "Appending " << order.orderId()
                        << ",lastOrd=" << pLastOrder->orderId() << std::endl;
                    pLastOrder->next_ = &order; // point the new one to last
                    order.prev_ = pLastOrder; // point the last to the new one
                    
                    existingLevel.tail = &order; // update level tail to the new one
                    existingLevel.orderCount += 1;
                    existingLevel.totalQty += order.qty_;

                    // update the order level pointer to the level location
                    order.level_ = &existingLevel;
                }
            }
        }

        Order* nextOrder() {
            if (empty()) {
                return nullptr;
            }
            Level& level = levels_.begin()->second;
            if (!level.head) {
                // TODO inconsistent state if we clear empty level
                return nullptr;
            }
            return level.head;
        }

        bool removeOrder(Order& order) {
            // in production, order.level_ would be used directly.
            // added other code for debugging internal inconsistency
            
            // comment out O(logN) debug code
            // auto levelIt = levels_.find(order.price_);
            // bool foundPriceLevel = levelIt != levels_.end();
            bool hasLevelPtr = order.level_ != nullptr;
            // if (hasLevelPtr != foundPriceLevel) {
            //     // INCONSISTENT
            //     std::cerr 
            //         << "inconsistent state, order:" << order.orderId()
            //         << ", hasLevelPtr: " << hasLevelPtr 
            //         << ", foundPriceLevel" << foundPriceLevel
            //         << std::endl;
            // } else 
            if (!hasLevelPtr) {
                // Don't do anything, this order isn't in the book levels
                std::cerr 
                    << "inconsistent state, order: " << order.orderId()
                    << " is not in the book" << std::endl;
            } else {
                Level* pLevel = order.level_;
                Order* prev = order.prev_;
                Order* next = order.next_;
                bool firstInLevel = prev == nullptr; 
                bool lastInLevel = next == nullptr;
                std::cout 
                    << "removing " << order.orderId()
                    << ", first? : " << firstInLevel
                    << ", last? : " << lastInLevel
                    << std::endl;

                // TODO this can use some refactoring to reduce duplication
                // for now keep it in this explicit way
                if (firstInLevel && lastInLevel) {
                    // this is the only element
                    // set the level that contains it to empty
                    pLevel->head = nullptr;
                    pLevel->tail = nullptr;

                    // no need to reset order's prev or next, they are nullptr
                } else if (firstInLevel) {
                    // reset level head to the next one
                    pLevel->head = order.next_;

                    // break the chain between order and its next
                    order.next_->prev_ = nullptr;
                    order.next_ = nullptr;

                } else if (lastInLevel) {
                    // reset level tail to the prev one
                    pLevel->tail = order.prev_;

                    // break the chain betwen order and its prev
                    order.prev_->next_ = nullptr;
                    order.prev_ = nullptr;

                } else {
                    // break chains with both prev and next
                    order.next_->prev_ = order.prev_;
                    order.prev_->next_ = order.next_;

                    order.next_ = nullptr;
                    order.prev_ = nullptr;
                }

                order.level_ = nullptr; // remove order link to level

                // reduce level size
                pLevel->orderCount -= 1;
                pLevel->totalQty -= order.qty_;
                
                // if the level is now empty, clean up the level.
                // this would cause the order cancel to be O(logN)
                // maybe an alternative is to leave the empty level as is
                if (pLevel->orderCount == 0) {
                    // this is O(logN)
                    // levels_.erase(levelIt);
                }
                return true;
            }
            return false;
        }

        std::ostream& print(std::ostream& os, bool bestPriceFirst) const {
            if (levels_.empty()) {
                os << "No Levels";
                return os;
            }

            os << "Levels# : " << levels_.size() << std::endl;
            if (bestPriceFirst) {
                for (auto it = levels_.begin(); it != levels_.end(); ++it) {
                    os << it->first << " ====> ";
                    it->second.print(os) << std::endl;
                }
            } else {
                for (auto it = levels_.rbegin(); it != levels_.rend(); ++it) {
                    os << it->first << " ====> ";
                    it->second.print(os) << std::endl;
                }
            }
            return os;
        }
    };

    Timestamp now() {
        auto now = std::chrono::system_clock::now();
        auto epochDuration = now.time_since_epoch();
        long millis = std::chrono::duration_cast<std::chrono::milliseconds>(epochDuration).count(); 
        return Timestamp{static_cast<uint64_t>(millis)};
    }

    template<typename FarSideBook>
    void 
    match(
        FarSideBook& farSide, const std::string& orderId, int64_t price,
        uint32_t& remainingQuantity, std::vector<Trade>& generatedTrades
    ) {
        while (farSide.isCrossPriceMarketable(price) && remainingQuantity > 0) {
            Order* pNextOrder = farSide.nextOrder();
            if (!pNextOrder) {
                // no more orders to be matched on the far-side
                break;
            }

            uint32_t fillQty = std::min(pNextOrder->qty(), remainingQuantity); 
            generatedTrades.emplace_back(orderId, pNextOrder->orderId(), pNextOrder->price(), fillQty);
            remainingQuantity -= fillQty;
            if (fillQty == pNextOrder->qty()) {
                // the passive order is fully filled
                cancelOrder(pNextOrder->orderId());
            } else { // fillQty < next qty
                uint32_t postFillQty = pNextOrder->qty() - fillQty;
                modifyOrderQuantity(pNextOrder->orderId(), postFillQty);
                // no need to keep looping as there is not enough quantity to match
                break;
            }
        }
    }

    // the call site of this function has figured the near side and far side
    // so there is no need to check against side in this function
    // TODO: NearSideBook/FarSideBook can have function to provide Buy/Sell
    template <typename NearSideBook, typename FarSideBook>
    AddResult 
    addOrder(
        NearSideBook& nearSide, FarSideBook& farSide,
        const std::string& orderId, int64_t price, uint32_t quantity, Side side,
        bool rejectCrossOrder
    ) {
        if (quantity == 0) {
            return AddResult::nonPositiveQtyRejection();
        }
        auto found = orderMap_.find(orderId);
        if (orderMap_.end() != found) {
            // cannot add a new order with duplicate order ID
            return AddResult::duplicatRejection();
        }

        // if it is a passive order on the near side or improving spread
        // passively insert that to the book
        // otherwise, an aggressive order may trigger matching engine logic
        // that generates trades, if not rejected
        std::optional<int64_t> bestFarSide = farSide.bestPrice();
        uint32_t remainingQuantity = quantity;
        std::vector<Trade> generatedTrades{};

        if (farSide.isCrossPriceMarketable(price)) {
            // this is where the new order is an aggressive order
            if (rejectCrossOrder) {
                return AddResult::aggressiveOrderRejection();
            } else {
                // invoke matching engine logic to generate fills and update 
                // remainingQuantity and generatedTrades.
                match(farSide, orderId, price, remainingQuantity, generatedTrades);
            }
        }

        // either the aggressive order has some remaining quantity 
        // or it was originall passive
        if (remainingQuantity > 0) {
            std::unique_ptr<Order> pOrder = std::make_unique<Order>(
                orderId, price, remainingQuantity, side, now());
            nearSide.addOrder(*pOrder);
            orderMap_.emplace(orderId, std::move(pOrder));
            if (generatedTrades.empty()) {
                return AddResult::passiveOrderAdded();
            } else {
                return AddResult::aggressiveOrderAdded(std::move(generatedTrades));
            }
        } else {
            // no quantity to add to the book
            if (generatedTrades.empty()) {
                // this only happens when input quantity is zero which is handled at the top
                std::cerr 
                    << "Inconsistent state with no remaining quantity or trades, order: "
                    << orderId << std::endl;
                return AddResult::nonPositiveQtyRejection();
            } else {
                return AddResult::aggressiveOrderFilled(std::move(generatedTrades));
            }
        }
    }

    template<typename NearSideBook, typename FarSideBook>
    ModifyResult
    modifyOrderPrice(
        NearSideBook& nearSide, FarSideBook& farSide, 
        Order& order, int64_t price
    ) {
        nearSide.removeOrder(order);

        uint32_t remainingQuantity = order.qty();
        std::vector<Trade> generatedTrades{};

        if (farSide.isCrossPriceMarketable(price)) {    
            // invoke matching engine logic to generate fills and update
            // remainingQuantity and generatedTrades.
            match(farSide, order.orderId(), price, remainingQuantity, generatedTrades);
        }

        if (remainingQuantity > 0) {
            // the order still exists after 
            order.price_ = price;
            order.qty_ = remainingQuantity;
            nearSide.addOrder(order);
        
            return ModifyResult::priceChanged(std::move(generatedTrades));
        } else {
            // the order is full filled
            orderMap_.erase(order.orderId_);
            return ModifyResult::orderFilled(std::move(generatedTrades));
        }
    }

    // Limit Order Book data internals
    // orders are owned by the orderId -> order map
    std::unordered_map<std::string, std::unique_ptr<Order>> orderMap_;
    BookSide<std::greater<int64_t>> bidSide_;
    BookSide<std::less<int64_t>> askSide_;

public:
    LimitOrderBook(): orderMap_{}, bidSide_{}, askSide_{} {}

    // cannot have any copy/move as that invalidates internal pointers
    LimitOrderBook(const LimitOrderBook& other) = delete;
    LimitOrderBook& operator=(const LimitOrderBook& other) = delete;
    LimitOrderBook(LimitOrderBook&& other) = delete;
    LimitOrderBook& operator=(LimitOrderBook&& other) = delete;

    std::optional<int64_t> bestBid() const {
        return bidSide_.bestPrice();
    }

    std::optional<int64_t> bestAsk() const {
        return askSide_.bestPrice();
    }

    // add a new order to the internal FIFO priority
    AddResult addOrder(
        const std::string& orderId, int64_t price, uint32_t quantity, Side side, 
        bool rejectCrossOrder
    ) {
        switch (side) {
            case Side::Buy:
                return addOrder(
                    bidSide_, askSide_,
                    orderId, price, quantity, side, rejectCrossOrder);
            case Side::Sell:
                return addOrder(
                    askSide_, bidSide_, 
                    orderId, price, quantity, side, rejectCrossOrder);
        }
        __builtin_unreachable();
    }

    CancelResult cancelOrder(const std::string& orderId) {
        auto found = orderMap_.find(orderId);
        if (found == orderMap_.end()) {
            return CancelResult::unknownOrderId();
        }
        switch (found->second->side()) {
            case Side::Buy: {
                if (bidSide_.removeOrder(*found->second)) {
                    orderMap_.erase(found);
                    return CancelResult::cancelled();
                } else {
                    return CancelResult::cancelRejection();
                }
            }
            case Side::Sell: {
                if (askSide_.removeOrder(*found->second)) {
                    orderMap_.erase(found);
                    return CancelResult::cancelled();
                } else {
                    return CancelResult::cancelRejection();
                }
            }
        }
        __builtin_unreachable();
    }

    // modifying price changes book/level structure 
    ModifyResult modifyOrderPrice(const std::string& orderId, int64_t price) {
        auto found = orderMap_.find(orderId);
        if (found == orderMap_.end()) {
            return ModifyResult::unknownOrderId();
        }

        Order& order = *found->second;
        if (order.price() == price) {
            return ModifyResult::nothingToChange();
        }

        switch (order.side()) {
            case Side::Buy: 
                return modifyOrderPrice(bidSide_, askSide_, order, price);
            case Side::Sell:
                return modifyOrderPrice(askSide_, bidSide_, order, price);
        }
        __builtin_unreachable();
    
    }

    // modifying order quantity doesn't change book/level structure
    ModifyResult modifyOrderQuantity(const std::string& orderId, uint32_t quantity) {
        if (quantity <= 0) {
            return ModifyResult::modificationRejected();
        }
        auto found = orderMap_.find(orderId);
        if (found == orderMap_.end()) {
            return ModifyResult::unknownOrderId();
        } else {
            Order* pOrder = found->second.get();
            // TODO: maybe just use int32_t for quantity
            int32_t delta = static_cast<int32_t>(quantity) - static_cast<int32_t>(pOrder->qty_);
            pOrder->qty_ = quantity;
            pOrder->level_->totalQty += delta;
            return ModifyResult::quantityChanged();
        }
    }

    std::ostream& print(std::ostream& os) const {
        os << "Book:" << std::endl;
        
        os << "orders:[";
        for (const auto& kv : orderMap_) {
            os << kv.first << ',';
        } 
        os << ']' << std::endl;

        os << "----- Asks -----" << std::endl;
        askSide_.print(os, false);
        os << "----- Bids -----" << std::endl;
        bidSide_.print(os, true);
        return os;
    }
};


int main(int argc, char *argv[]) {
    LimitOrderBook book{};
    book.print(std::cout) << std::endl;

    // cancel non-existing order 
    CancelResult cancelRes = book.cancelOrder("order-1");
    assert(cancelRes.type == CancelResult::Type::UnknownOrderId);
    book.print(std::cout) << std::endl;

    // add passive order
    AddResult addRes = book.addOrder("s1", 101, 10, Side::Sell, true);
    assert(addRes.type == AddResult::Type::PassiveOrderAdded);
    book.print(std::cout) << std::endl;

    // add more passive orders at new levels
    addRes = book.addOrder("s2", 102, 20, Side::Sell, true);
    assert(addRes.type == AddResult::Type::PassiveOrderAdded);
    addRes = book.addOrder("s3", 103, 30, Side::Sell, true);
    assert(addRes.type == AddResult::Type::PassiveOrderAdded);
    book.print(std::cout) << std::endl;

    // add more passive orders at existing levels
    addRes = book.addOrder("s4", 103, 31, Side::Sell, true);
    assert(addRes.type ==  AddResult::Type::PassiveOrderAdded);
    book.print(std::cout) << std::endl;

    addRes = book.addOrder("s5", 102, 21, Side::Sell, true);
    assert(addRes.type == AddResult::Type::PassiveOrderAdded);
    addRes = book.addOrder("s6", 102, 22, Side::Sell, true);
    assert(addRes.type == AddResult::Type::PassiveOrderAdded);
    book.print(std::cout) << std::endl;

    // populate the other side
    addRes = book.addOrder("b1", 99, 10, Side::Buy, true);
    assert(addRes.type == AddResult::Type::PassiveOrderAdded);
    addRes = book.addOrder("b2", 99, 11, Side::Buy, true);
    assert(addRes.type == AddResult::Type::PassiveOrderAdded);
    book.print(std::cout) << std::endl;

    addRes = book.addOrder("b3", 99, 12, Side::Buy, true);
    assert(addRes.type == AddResult::Type::PassiveOrderAdded);
    book.print(std::cout) << std::endl;

    addRes = book.addOrder("b4", 98, 20, Side::Buy, true);
    assert(addRes.type == AddResult::Type::PassiveOrderAdded);
    addRes = book.addOrder("b5", 98, 21, Side::Buy, true);
    assert(addRes.type == AddResult::Type::PassiveOrderAdded);
    
    addRes = book.addOrder("b6", 97, 30, Side::Buy, true);
    assert(addRes.type == AddResult::Type::PassiveOrderAdded);
    book.print(std::cout) << std::endl;

    // cancel non-existing order
    cancelRes = book.cancelOrder("order-1");
    assert(cancelRes.type == CancelResult::Type::UnknownOrderId);

    // cancel the only order in a level
    cancelRes = book.cancelOrder("b6");
    assert(cancelRes.type == CancelResult::Type::Cancelled);
    book.print(std::cout) << std::endl;

    // cancel the first order in a level
    cancelRes = book.cancelOrder("s3");
    assert(cancelRes.type == CancelResult::Type::Cancelled);
    book.print(std::cout) << std::endl;

    // cancel the last order in a level
    cancelRes = book.cancelOrder("b5");
    assert(cancelRes.type == CancelResult::Type::Cancelled);
    book.print(std::cout) << std::endl;

    // cancel something in the middle
    cancelRes = book.cancelOrder("s5");
    assert(cancelRes.type == CancelResult::Type::Cancelled);
    book.print(std::cout) << std::endl;

    // cancel order by order to remove a whole level
    cancelRes = book.cancelOrder("b1");
    assert(cancelRes.type == CancelResult::Type::Cancelled);
    cancelRes = book.cancelOrder("b3");
    assert(cancelRes.type == CancelResult::Type::Cancelled);
    cancelRes = book.cancelOrder("b2");
    assert(cancelRes.type == CancelResult::Type::Cancelled);
    book.print(std::cout) << std::endl;

    // modify order quantity down
    ModifyResult modRes = book.modifyOrderQuantity("b4", 10);
    assert(modRes.type == ModifyResult::Type::QuantityChanged);
    book.print(std::cout) << std::endl;

    // modify order quantity up
    modRes = book.modifyOrderQuantity("b4", 100);
    assert(modRes.type == ModifyResult::Type::QuantityChanged);
    book.print(std::cout) << std::endl;

    // add aggressive order (no matching allowed)
    addRes = book.addOrder("b100", 102, 15, Side::Buy, true);
    assert(addRes.type == AddResult::Type::AggressiveOrderRejection);

    // add aggressive order (small)
    addRes = book.addOrder("b100", 102, 32, Side::Buy, false);
    std::cout << addRes << std::endl;
    assert(addRes.type == AddResult::Type::AggressiveOrderFilled);
    assert(addRes.trades.size() == 3);
    book.print(std::cout) << std::endl;

    // add aggressive order (large)
    addRes = book.addOrder("s100", 95, 101, Side::Sell, false);
    std::cout << addRes << std::endl;
    assert(addRes.type == AddResult::Type::AggressiveOrderAdded);
    assert(addRes.trades.size() == 1);
    book.print(std::cout) << std::endl;

    // modify order price level
    modRes = book.modifyOrderPrice("s4", 105);
    assert(modRes.type == ModifyResult::Type::PriceChanged);
    book.print(std::cout) << std::endl;

    // modify order price to cross with the other side, but still survives
    book.addOrder("b101", 90, 20, Side::Buy, false);
    modRes = book.modifyOrderPrice("b101", 99);
    assert(modRes.type == ModifyResult::Type::PriceChanged);
    book.print(std::cout) << std::endl;

    // modify order to be full filled
    modRes = book.modifyOrderPrice("b101", 103);
    assert(modRes.type == ModifyResult::Type::OrderFilled);
    book.print(std::cout) << std::endl;

    return 0; 
}