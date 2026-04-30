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
#include <iostream>
#include <mutex>
#include <numeric>
#include <optional>
#include <stop_token>
#include <string>
#include <thread>
#include <unordered_set>

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

    bool pop() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (tail_ == head_) { // empty
            return false;
        }
        head_ = (head_ + 1) % N;
        return true;
    }

    std::optional<T> peek() const {
        if (tail_ == head_) {
            return std::optional<T>();
        }
        return data_[head_];
    }
};

// lock free version

template<typename T, typename U>
concept BoundedQ = requires(T t, const U& u) {
    { t.pop() } -> std::same_as<bool>;
    { t.push(u) } -> std::same_as<bool>;
    { t.peek() } -> std::same_as<std::optional<U>>;
};

struct BoundedQueueTest {
    template<typename QueueType>
    requires BoundedQ<QueueType, std::string>
    static void runTest(QueueType& queue, int count) {
        using namespace std::chrono_literals;

        auto writer = [&queue, count]() {
            for (int i = 0; i < count; ++i) {
                queue.push(std::to_string(i));
            }
        };

        std::unordered_set<int> result{};
        auto reader = [&queue, &result](std::stop_token stopToken) {
            while (!stopToken.stop_requested()) {
                std::optional<std::string> item = queue.peek();
                if (item) {
                    queue.pop();
                    try {
                        result.insert(std::stoi(*item));
                    } catch (...) {
                        std::cout
                            << "cannot convert " << *item << " to int"
                            << std::endl;
                    }
                }
            }
        };

        std::cout << "starting" << std::endl;
        std::jthread consumer(reader); // start consumer first
        std::jthread producer(writer);

        producer.join();
        std::cout << "producer joined" << std::endl;
        std::this_thread::sleep_for(1s);
        consumer.request_stop();
        consumer.join();

        int expected = count * (count - 1) / 2;
        int collected = std::accumulate(result.begin(), result.end(), 0);
        std::cout << "expected: " << expected << ", collected: " << collected << std::endl;
    }

    static void testMutexBased() {
        MutexBased<std::string, 128> mb{};
        runTest(mb, 1000);
    }
};


int main(int argc, char* argv[]) {
    BoundedQueueTest::testMutexBased();

    return 0;
}
