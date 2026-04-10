#include <iostream>
#include <functional>

struct Option {};
struct MarketData {};

double runBlackScholes(const Option& option, const MarketData& md) {
    std::cout << "running BS model" << std::endl;
    return 1.0;
}

double runMonteCarlo(const Option& option, const MarketData& md) {
    std::cout << "running MC model" << std::endl;
    return 2.0;
}


class PricerByFunction {
    std::function<double(const Option&, const MarketData&)> function_;
public:
    PricerByFunction(std::function<double(const Option&, const MarketData&)> function)
    : function_(function) {
    }

    double run(const Option& option, const MarketData& md) {
        return function_(option, md);
    }
};


struct TestPricerTypeErassure {
    static const Option OPTION;
    static const MarketData MD; 

    static void testPricerByFunction() {
        PricerByFunction pricerByBS(::runBlackScholes);
        std::cout << pricerByBS.run(OPTION, MD);

        PricerByFunction pricerByMC(::runMonteCarlo);
        std::cout << pricerByMC.run(OPTION, MD);
    }
};

const Option TestPricerTypeErassure::OPTION{};
const MarketData TestPricerTypeErassure::MD{};

// clang++ -std=c++20 type_erassure.m.cpp -o type_erassure.tsk
int main(int argc, char* argv[]) {
    TestPricerTypeErassure::testPricerByFunction();
    return 0;
}