#include <future>
#include <iostream>
#include <thread>
#include <utility>

// or
// void product(std::promise<int>&& intPromise, int a, int b)
void product(std::promise<int>& intPromise, int a, int b) {
    intPromise.set_value(a*b);
}

struct Div {
    // or 
    // void operator()(std::promise<int>&& intPromise, int a, int b) const
    void operator()(std::promise<int>& intPromise, int a, int b) const {
        intPromise.set_value(a/b);
    }
};

int main(int argc, char* argv[]) {
    int a = 20;
    int b = 10;

    std::cout << std::endl;

    // define the promises
    std::promise<int> prodPromise;
    std::promise<int> divPromise;

    // get the futures
    std::future<int> prodResult = prodPromise.get_future();
    std::future<int> divResult = divPromise.get_future();

    // calculate the result in a separate thread
    std::thread prodThread(product, std::ref(prodPromise), a, b);
    std::thread divThread(Div(), std::ref(divPromise), a, b);

    // or
    // std::thread prodResult(product, std::move(prodPromise), a, b);
    // std::thread divThread(Div(), std::move(divPromise), a, b);

    // get the result
    std::cout << "20*10 = " << prodResult.get() << std::endl;
    std::cout << "20/10 = " << divResult.get() << std::endl;

    prodThread.join();
    divThread.join();

    std::cout << std::endl;

    return 0;
}
