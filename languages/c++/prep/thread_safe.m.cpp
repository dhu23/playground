// thread safe increment() and get() functions
// there are two ways of doing it: a mutex version and an automic version


// mutex based version

#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>


template<typename T>
concept Counter = requires (T t) {
    t.increment();
    { t.get() } -> std::same_as<int>;
};


class ThreadNotSafe {
    volatile int count_; // expose the data racing without longer runs
public:
    void increment() {
        count_ += 1;
    }

    int get() const {
        return count_;
    }
};


class MutexBased {
    int count_;
    mutable std::mutex mutex_;
public:
    void increment() {
        std::lock_guard<std::mutex> lock(mutex_);
        ++count_;
    }

    int get() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return count_; 
    }
};


class AtomicBased {
    std::atomic<int> count_;
public:
    void increment() {
        count_.fetch_add(1);
    }

    int get() const {
        return count_.load();
    }
};


struct ConcurrencyTest {
    static void runThreads(Counter auto& counter, int count) {
        using namespace std::chrono_literals;

        auto increment = [&counter](std::stop_token stopToken, int count) {
            while (count > 0) {
                counter.increment();
                // std::cout << "t1:count=" << counter.get() << std::endl;
                --count;
            }
        };
        
        std::jthread t1(increment, count);
        std::jthread t2(increment, count);

        std::this_thread::sleep_for(1s);

        t1.request_stop();
        t2.request_stop();

        std::cout << "value=" << counter.get() << std::endl;
    }

    static void testThreadNotSafe() {
        ThreadNotSafe tnf{};
        // a large number can expose it
        runThreads(tnf, 100000);
    }

    static void testMutexBased() {
        MutexBased mb{};
        runThreads(mb, 100000);
    }

    static void testAtomicBased() {
        AtomicBased ab{};
        runThreads(ab, 100000);
    }
};



int main(int argc, char* argv[]) {
    // ConcurrencyTest::testThreadNotSafe();
    // ConcurrencyTest::testMutexBased();
    ConcurrencyTest::testAtomicBased();
    return 0;
}