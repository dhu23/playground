// this is a concept demo of back-testing engine
// the engine takes in multiple data source streams, for example signals
// and market data. And generate testing result

// it also allows scheduling and cancelling of ad-hoc timer events

#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>
#include <optional>
#include <queue>
#include <variant>
#include <vector>

using SimTime = int64_t;

// simulation clock that allows set and read of current time
class SimulationClock {
public:
};

struct MarketData {
    int32_t security;
    double price;
    uint32_t marketDataId; // for sequence number

    std::ostream& print(std::ostream& os) const {
        os 
            << "MarketData[`sec=" << security
            << ",px=" << price << ",mdId=" << marketDataId
            << ']';
        return os;
    }
};

struct Signal {
    int32_t security;
    double price;
    int32_t qty;
    uint32_t signalId; // for sequence number

    std::ostream& print(std::ostream& os) const {
        os 
            << "Signal[`sec=" << security
            << ",px=" << price << ",qty=" << qty
            << ",signalId=" << signalId << ']';
        return os;
    }
};

// tracks ad-hoc scheduled event
struct Scheduled {
    uint32_t scheduleId; // for sequence number
    std::function<void()> callback;

    std::ostream& print(std::ostream& os) const {
        os << "Scheduled[`schId=" << scheduleId << ']'; 
        return os;
    }
};

template<typename... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

template<typename... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

struct Event {
    using EventData = std::variant<MarketData, Signal, Scheduled>;

    int64_t timestamp;
    size_t sourceId; // used to indicate if it is from a stream
    EventData data;

    std::ostream& print(std::ostream& os) const {
        os << "Event[`ts=" << timestamp << ",srcId=" << sourceId << ",data=";
        std::visit(overloaded{
            [&os](const MarketData& marketData) -> std::ostream& {return marketData.print(os);},
            [&os](const Signal& signal) -> std::ostream& {return signal.print(os);},
            [&os](const Scheduled& scheduled) -> std::ostream& {return scheduled.print(os);}
        }, data);

        return os;
    }

    int getTypeId() const {
        return std::visit(overloaded{
            [](const MarketData& marketData) {return 1;},
            [](const Signal& signal) {return 2;},
            [](const Scheduled& scheduled) {return 1000;}
        }, data);
    }

    uint32_t sequenceNum() const {
        return std::visit(overloaded{
            [](const MarketData& marketData) {return marketData.marketDataId;},
            [](const Signal& signal) {return signal.signalId;},
            [](const Scheduled& scheduled) {return scheduled.scheduleId;}
        }, data);
    }

    bool operator<(const Event& other) const {
        if (timestamp != other.timestamp) {
            return timestamp < other.timestamp;
        }
        if (getTypeId() != other.getTypeId()) {
            return getTypeId() < other.getTypeId();
        }
        // now same type, same timestamp, use sequence number to break tie
        return sequenceNum() < other.sequenceNum();
    }
    
    bool operator>(const Event& other) const {
        return other.operator<(*this);
    }
};

class EventSource {
    std::string name_; 
public:
    EventSource(const std::string& name): name_(name) {}
    virtual ~EventSource() = default;
    virtual bool hasNext() const = 0;
    virtual Event getNext() = 0;

    std::string name() { return name_; }
};


template<typename T>
concept EventProcessorT = requires(T t, const Event& event) {
    { t.process(event) };
};

template<typename EventProc>
requires EventProcessorT<EventProc>
class Engine {
    EventProc processor_;
    std::vector<EventSource*> sources_;
    std::priority_queue<Event, std::vector<Event>, std::greater<Event>> mergeQueue_;

    // advance time and return the next event
    std::optional<Event> getNext() {
        if (mergeQueue_.empty()) {
            return std::nullopt;
        }
        std::cout << "popping from merge queue, size=" << mergeQueue_.size() << std::endl;
        auto next = mergeQueue_.top();

        // std::cout << "popped event: ";
        // next.print(std::cout) << std::endl;
        mergeQueue_.pop();

        // if event comes from a source Id then replenish
        std::cout << "trying to replenish source-id: " << next.sourceId;
        std::cout << ", sources size: " << sources_.size() << std::endl;
        EventSource* pSource = sources_[next.sourceId];
        if (pSource->hasNext()) {
            mergeQueue_.push(pSource->getNext());
        }

        return next;
    }

public:
    Engine()
    : processor_{}
    , sources_{}
    , mergeQueue_{} {}

    void addEventSource(EventSource& eventSource) {
        if (!eventSource.hasNext()) {
            return;
        }
        Event event = eventSource.getNext();
        mergeQueue_.push(event);
        // TODO make it robust
        // assuming event source Id matches vector location
        sources_.push_back(&eventSource);
    }

    // schedule a callback in `milliseconds` and gets a handle
    uint32_t schedule(std::function<void()> callback, int64_t milliseconds) {}
    void cancel(uint32_t scheduleId) {}

    void run() {
        while (true) {
            std::optional<Event> event = getNext();
            if (!event) {
                break;
            }
            processor_.process(*event);
        }
    }
};

struct EngineTest {
    struct TestEventProcessor {
        TestEventProcessor() = default;

        void process(const Event& event) {
            std::cout << "processing ";
            event.print(std::cout) << std::endl;
        }
    };

    class VectorDataSource : public EventSource {
        size_t index_;
        std::vector<Event> data_;
    public:
        VectorDataSource(const std::string& name)
        : EventSource(name)
        , index_(0)
        , data_{} {}

        virtual bool hasNext() const {
            return index_ < data_.size();
        }

        virtual Event getNext() {
            return data_[index_++];
        }

        void add(const Event& event) {
            data_.push_back(event);
        }
    };

    constexpr static int32_t AAPL = 2654;
    constexpr static int32_t TSLA = 960;

    static VectorDataSource makeMarketDataSource(size_t sourceId) {
        VectorDataSource mds{"test-md-source"};
        uint32_t seqNum = 0;
        int64_t ts = 1;
    
        for (double px : {150.0, 151.0, 152.0}) {
            mds.add(Event{ts, sourceId, MarketData{AAPL, px, seqNum++}});
            ts += 10;
            mds.add(Event{ts, sourceId, MarketData{TSLA, px + 1000.0, seqNum++}});
            ts += 10;
        }

        return mds;
    }

    static VectorDataSource makeSignalSource(size_t sourceId) {
        VectorDataSource signals{"test-signal-source"};
        uint32_t seqNum = 1;
        int64_t ts = 5;

        for (double px : {150.0, 151.0, 152.0}) {
            signals.add(Event{ts, sourceId, Signal{AAPL, px, 100, seqNum++}});
            ts += 10;
            signals.add(Event{ts, sourceId, Signal{TSLA, px + 1000.0, 200, seqNum++}});
            ts += 10;
        }

        return signals;
    }

    static void testReplayMarketData() {
        Engine<TestEventProcessor> engine{};
        VectorDataSource mds = makeMarketDataSource(0);
        VectorDataSource signals = makeSignalSource(1);
        engine.addEventSource(mds);
        engine.addEventSource(signals);
        engine.run();
    }
};

int main(int argc, char* argv[]) {
    EngineTest::testReplayMarketData();
    return 0;
}