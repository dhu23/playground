// bounded queue (ring buffer implementation)
// single producer single consumer queue
// for the ring buffer, head is where the data is read
// and tail is where data is written

// array layout
//  - x x x x x x x x x - - - -
//    |               |
//    head            tail
// in general, without considering edge cases (empty queue)
// next read(pop) happens at head
// next write(push) happens at tail

// the next of tail == head means full
// tail == head means empty
// so the queue's capacity is actually N-1

#include <array>
#include <atomic>
#include <chrono>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <mutex>
#include <ostream>
#include <ratio>
#include <string_view>
#include <thread>
#include <vector>

// mutex based version
template<typename T, size_t N>
class MutexBased {
    mutable std::mutex mutex_;
    std::array<T, N> data_;
    size_t head_;
    size_t tail_;
public:
    MutexBased(): data_{}, head_(0), tail_(0) {
    }

    // added as a feature to experiment new features 
    constexpr static std::string_view impl_type = "MutexBased";
    constexpr static size_t capacity = N;

    bool push(const T& t) {
        std::lock_guard<std::mutex> lock(mutex_);
        size_t next = (tail_ + 1) % N;
        if (next == head_) { // queue is full
            return false;
        }
        data_[tail_] = t;
        tail_ = next;
        return true;
    }

    bool pop(T& out) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (tail_ == head_) { // empty
            return false;
        }
        out = data_[head_]; 
        // if the above throws an exception, head_ doesn't move
        // it moves only when out is correctly populated
        // this assumes that T needs copy assignment
        head_ = (head_ + 1) % N;
        return true;
    }
};

// lock free version
template<typename T, size_t N>
class LockFreeBased {
    std::array<T, N> data_;
    std::atomic<size_t> head_;
    std::atomic<size_t> tail_;
public:
    LockFreeBased(): data_{}, head_{0}, tail_{0} {}

    constexpr static std::string_view impl_type = "LockFreeBased";
    constexpr static size_t capacity = N;

    // push(write) owns where tail_ is
    // pop(read) owns where head_ is

    // therefore in push, head_ should be loaded on acquire
    // and in pop, tail_ should be loaded on acquire

    bool push(const T& t) {
        size_t tail = tail_.load(std::memory_order_relaxed);
        size_t next = (tail + 1) % N;
        if (next == head_.load(std::memory_order_acquire)) {
            // queue is full
            return false;
        }
        data_[tail] = t;
        tail_.store(next, std::memory_order_release);
        return true;
    }

    bool pop(T& out) {
        size_t head = head_.load(std::memory_order_relaxed);
        if (head == tail_.load(std::memory_order::acquire)) {
            return false;
        }
        out = data_[head];
        head = (head + 1) % N;
        head_.store(head, std::memory_order::release);
        return true;
    }
};


template<typename T, typename U>
concept BoundedQ = requires(T t, const U& uin, U& uout) {
    { t.pop(uout) } -> std::same_as<bool>;
    { t.push(uin) } -> std::same_as<bool>;
    // need the reference part for the concept to match constexpr static variables
    { T::impl_type } -> std::same_as<const std::string_view&>;
    { T::capacity } -> std::same_as<const size_t&>;
};


struct TestResult {
    std::string type;
    size_t queueSize;
    int generated;
    int collected;
    long elapsedInMicros;
    bool retryOnFull;

    std::ostream& printCsv(std::ostream& os) const {
        os 
            << type << ',' << queueSize << ',' 
            << generated << ',' << collected << ','
            << elapsedInMicros << ',' << retryOnFull;
        return os;
    }

    static std::string header() {
        return "type,queueSize,generated,collected,elapsedInMicros,retryOnFull";
    }
};


struct BoundedQueueTest {
    template<typename QueueType>
    requires BoundedQ<QueueType, int>
    static TestResult runIntTest(QueueType& queue, int count, bool retryOnFull) {
        using namespace std::chrono_literals;

        std::cout
            << "impl: " << QueueType::impl_type << ", count: " << count 
            << ", retryOnFull:" << retryOnFull 
            << std::endl;;

        std::atomic<bool> isProducerDone{false};

        auto writer = [&queue, count, retryOnFull, &isProducerDone]() {
            for (int i = 0; i < count; ++i) {
                // we can have two flavors of the test: 
                // with retry: we won't be losing any messages and can be 
                // used to compare throughput more easily. Otherwise, many
                // other fact, such as reader speed, queue size come to play
                while (!queue.push(i) && retryOnFull);
            }
            isProducerDone.store(true);
        };

        int result = 0; // use it only after reader is done
        auto reader = [&queue, &result, &isProducerDone]() {
            int temp = 0;
            while (true) {
                if (queue.pop(temp)) {    
                    ++result;
                } else if (isProducerDone.load()) {
                    // when the queue is possibly empty and producer is done
                    // however it is possible producer wrote more after pop failure
                    // so drain them
                    while (queue.pop(temp)) {
                        ++result;
                    }
                    break;
                }
            }
        };

        auto start = std::chrono::steady_clock::now();
        // std::cout << "starting" << std::endl;
        std::jthread consumer(reader); // start consumer first
        std::jthread producer(writer);

        producer.join();
        // std::cout << "producer joined" << std::endl;
        consumer.join();

        auto end = std::chrono::steady_clock::now();

        auto elapsedInMicros = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        std::cout 
            << "Count Result ===> expected: " << count
            << ", collected: " << result
            << ", queue-capacity: " << QueueType::capacity  
            << ", elapsed=" << elapsedInMicros.count() << "us"
            << std::endl;
        return TestResult{
            std::string{QueueType::impl_type}, // need to call std::string(std::string_view) ctor
            QueueType::capacity,
            count, result, // produced message #, collected message #
            elapsedInMicros.count(),
            retryOnFull
        };
    }

    template<size_t N>
    static void testMutexBased(int count) {
        MutexBased<int, N> mb{};
        runIntTest(mb, count, true);
        runIntTest(mb, count, false);
    }

    template<size_t N>
    static void testLockFreeBased(int count) {
        LockFreeBased<int, N> lfb{};
        runIntTest(lfb, count, true);
        runIntTest(lfb, count, false);
    }

    template<size_t N>
    static void runComparisonExperiments(int count) {
        std::cout 
            << "------ running comparison for N: " << N 
            << ", count: " << count << " ------"
            << std::endl;
        testMutexBased<N>(count);
        testLockFreeBased<N>(count);
    }

    template<template<typename, size_t> class QueueType, size_t N>
    static void generateSurfacePointsForQueueType(int count, std::ostream& os) {
        QueueType<int, N> queue{};
        runIntTest(queue, count, true).printCsv(os) << std::endl;
        runIntTest(queue, count, false).printCsv(os) << std::endl;   
    }

    template<template<typename, size_t> class QueueType, size_t... Ns>
    static void generateSurfaceForQueueType(const std::vector<int>& counts, std::ostream &os) {
        for (int count : counts) {
            (generateSurfacePointsForQueueType<QueueType, Ns>(count, os), ...); 
        }
    }

    static void runSurface() {
        std::cout << "run Surface" << std::endl;

        std::ofstream output("./surface.csv");
        output << TestResult::header() << std::endl;

        std::initializer_list<int> counts = {
            100, 200, 300, 400, 500, 
            1'000, 1'500, 2'000, 2'500, 
            3'000, 3'500, 4'000, 4'500, 5'000, 
            10'000, 15'000, 20'000, 
            25'000, 30'000, 35'000, 40'000, 45'000, 50'000, 
            100'000, 150'000, 200'000, 250'000, 
            300'000, 350'000, 400'000, 450'000, 500'000, 
            1'000'000, 1'100'000, 1'200'000, 
            1'300'000, 1'400'000, 1'500'000
        };

        auto start = std::chrono::system_clock::now();
        
        generateSurfaceForQueueType<MutexBased, 256, 512, 1024, 2048, 4096, 8192>(counts, output);
        generateSurfaceForQueueType<LockFreeBased, 256, 512, 1024, 2048, 4096, 8192>(counts, output);
        
        auto end = std::chrono::system_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "runSurface took: " << elapsed.count() << "ms" << std::endl;
    }

    static void runDenseSurface() {
        std::cout << "run dense Surface" << std::endl;

        std::ofstream output("./surface-dense.csv");
        output << TestResult::header() << std::endl;

        // generates up to 1000, 2000, ..., 200000
        std::vector<int> counts;
        for (int i = 1; i <= 200; ++i) {
            counts.push_back(i * 1000);
        } 

        auto start = std::chrono::system_clock::now();

        generateSurfaceForQueueType<
            MutexBased, 
            100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 
            1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000>(counts, output);
        generateSurfaceForQueueType<
            LockFreeBased, 
            100, 200, 300, 400, 500, 600, 700, 800, 900, 1000,
            1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000>(counts, output);

        auto end = std::chrono::system_clock::now();
        auto elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "run dense Surface took: " << elapsed.count() << "ms" << std::endl;
    }
};


int main(int argc, char* argv[]) {
    BoundedQueueTest::runComparisonExperiments<128>(100);
    BoundedQueueTest::runComparisonExperiments<128>(1000);
    BoundedQueueTest::runComparisonExperiments<1024>(10000);
    BoundedQueueTest::runComparisonExperiments<8192>(500000);

    std::cout << "testing generateSurfacePointsForQueueType" << std::endl;
    BoundedQueueTest::generateSurfacePointsForQueueType<LockFreeBased, 128>(300, std::cout);

    std::cout << "testing generateSurfaceForQueueType" << std::endl;
    BoundedQueueTest::generateSurfaceForQueueType<LockFreeBased, 128, 256>({300, 600}, std::cout);

    BoundedQueueTest::runSurface();
    BoundedQueueTest::runDenseSurface();

    return 0;
}
