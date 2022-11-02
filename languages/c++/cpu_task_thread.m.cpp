#include <iostream>
#include <chrono>
#include <ctime>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <map>
#include <vector>
#include <numeric>
#include <optional>
#include <functional>

const std::vector<unsigned> FIB_INPUT{30, 31, 32, 33, 34, 35};
// const std::optional<>

int fib(int n) {
    if (n < 2) {
        return n;
    } else {
        return fib(n-2) + fib(n-1);
    }
}

int backoutFib(int fibVal) {
    int n = 0;
    while (fib(n) < fibVal) {
        ++n;
    }
    return n;
}

template<typename T>
class BlockingQueue {
private:
    std::mutex m_mutex;
    std::queue<T> m_queue;
    std::condition_variable m_cond;
public:
    void push(const T& t) {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_queue.push(t);
    }

    T pop() {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cond.wait(lock, [this]{return !this->m_queue.empty();});
        T ret(std::move(m_queue.front()));
        m_queue.pop();
        return ret;
    }
};

template<typename T>
T getMean(const std::vector<T>& ns) {
    T sum = 0;
    for (T t : ns) {
        sum += t;
    }
    return sum * 1.0 / ns.size();
}

struct Result {
    int arg;
    int ans;
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::chrono::time_point<std::chrono::high_resolution_clock> end;
};

Result runTask(std::function<int(int)> f, int arg) {
    auto start = std::chrono::high_resolution_clock::now();
    int ans = f(arg);
    auto end = std::chrono::high_resolution_clock::now();
    return Result{arg, ans, start, end};
}

double getMills(
    std::chrono::time_point<std::chrono::high_resolution_clock> start,
    std::chrono::time_point<std::chrono::high_resolution_clock> end) {
    return std::chrono::duration<double, std::milli>(end-start).count();
}


struct ResultSummary {
    std::optional<std::chrono::time_point<std::chrono::high_resolution_clock>> firstStart;
    std::optional<std::chrono::time_point<std::chrono::high_resolution_clock>> lastEnd;
    // std::map<int, int> ansMap;
    // std::map<int, std::vector<std::clock_t>> startMap;
    // std::map<int, std::vector<std::clock_t>> endMap;
    std::map<int, std::vector<double>> costMap;

    void add(Result ret) {
        if (!firstStart || *firstStart > ret.start) {
            firstStart = ret.start;
        }
        if (!lastEnd || *lastEnd < ret.end) {
            lastEnd = ret.end;
        }
        auto it = costMap.find(ret.arg);
        double diff = getMills(ret.start, ret.end);
        if (it == costMap.end()) {
            costMap.emplace(ret.arg, std::vector<double>(1, diff));
        } else {
            it->second.push_back(diff);
        }
    }

    void summarize() {
        for (auto& kv: costMap) {
            std::cout << kv.first << "\t\t| " << getMean(kv.second) << " ms" << std::endl; 
        }
    }
};

struct RunResult {
    ResultSummary prets;
    ResultSummary crets;
    double cost;
};

RunResult singleThreadRun(int repeatLimit) {
    auto start = std::chrono::high_resolution_clock::now();
    int repeated = 0;

    RunResult rres = RunResult();

    while (true) {
        for (unsigned n: FIB_INPUT) {
            Result producerResult = runTask(fib, n);
            Result consumerResult = runTask(backoutFib, producerResult.ans);
            rres.prets.add(producerResult);
            rres.crets.add(consumerResult);
        }
        ++repeated;

        if (repeated < repeatLimit) {
            // no sleep for now
        } else {
            break;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    rres.cost = getMills(start, end);

    std::cout << "run singlethreaded against " << FIB_INPUT.size() 
        << " elements, repeated " << repeatLimit << " times, cost:" 
        << rres.cost
        << std::endl;
    return std::move(rres);
}

int main(int argc, char* argv[]) {
    int repeatLimit = 40;
    RunResult sret = singleThreadRun(repeatLimit);
    std::cout << "-------------- single producer -----------------" << std::endl;
    sret.prets.summarize();
    std::cout << "-------------- single consumer -----------------" << std::endl;
    sret.crets.summarize();
    double scost = getMills(*sret.prets.firstStart, *sret.crets.lastEnd);
    std::cout << "computing cost: " << scost << std::endl;
    return 0;
}
