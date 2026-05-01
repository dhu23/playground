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

#include <array>
#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <stop_token>
#include <string_view>
#include <thread>

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
    { T::impl_type } -> std::same_as<const std::string_view&>;
};

struct BoundedQueueTest {
    template<typename QueueType>
    requires BoundedQ<QueueType, int>
    static void runIntTest(QueueType& queue, int count, bool retryOnFull) {
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

        int result = 0;
        auto reader = [&queue, &result, &isProducerDone](std::stop_token stopToken) {
            int temp = 0;
            while (!stopToken.stop_requested()) {
                if (queue.pop(temp)) {    
                    ++result;
                } else if (isProducerDone.load()) {
                    // if the queue is empty and producer is done, exit
                    break;
                }
            }
        };

        auto start = std::chrono::system_clock::now();
        // std::cout << "starting" << std::endl;
        std::jthread consumer(reader); // start consumer first
        std::jthread producer(writer);

        producer.join();
        // std::cout << "producer joined" << std::endl;
        consumer.request_stop();
        consumer.join();

        auto end = std::chrono::system_clock::now();

        auto elapsedInMicros = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        std::cout 
            << "Count Result ===> expected: " << count << ", collected: " << result 
            << ", elapsed=" << elapsedInMicros.count() << "us"
            << std::endl;
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
};


int main(int argc, char* argv[]) {
    BoundedQueueTest::runComparisonExperiments<128>(100);
    BoundedQueueTest::runComparisonExperiments<128>(1000);
    BoundedQueueTest::runComparisonExperiments<1024>(10000);
    BoundedQueueTest::runComparisonExperiments<8192>(500000);

    return 0;
}
