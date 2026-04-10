#include <iostream>
#include <functional>
#include <memory>
#include <utility>

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

struct BlackScholes {};
struct MonteCarlo {};


////////////////////////////////
// templates based polymorphism 
////////////////////////////////
template<typename T>
double priceOptionHelper(const Option& option, const MarketData& md);

template<>
double priceOptionHelper<BlackScholes>(const Option& option, const MarketData& md) {
    return runBlackScholes(option, md);
}

template <>
double priceOptionHelper<MonteCarlo>(const Option &option, const MarketData &md) {
    return runMonteCarlo(option, md);
}

template<typename T>
double priceOption(const Option& option, const MarketData& md) {
    double ret = priceOptionHelper<T>(option, md);
    std::cout << "ret=" << ret << std::endl;
    return ret;
}

//////////////////////////////
// concept based polymorphism
//////////////////////////////

// not significant enough in this problem


////////////////////////////////////
// std::function based polymorphism
////////////////////////////////////
class PricerByFunction {
    std::function<double(const Option&, const MarketData&)> function_;
public:
    // PricerByFunction(std::function<double(const Option&, const MarketData&)> function)
    // : function_(function) {
    // }

    template<typename F>
    PricerByFunction(F&& f)
    : function_(std::forward<F>(f)) {
    }

    double run(const Option& option, const MarketData& md) {
        double ret = function_(option, md);
        std::cout << "ret=" << ret << std::endl;
        return ret;
    }
};


//////////////////////////////////////////
// Manual Type Erasure based Polymorphism
//////////////////////////////////////////

class PricerByTypeErasure {
    class Concept {
    public:
        virtual ~Concept() = default;
        virtual double run(const Option& op, const MarketData& md) = 0;
    };

    // If we use function or equivalent of function, then we can just reply on T
    template<typename T>
    class Model {};

    template<>
    class Model<BlackScholes> : public Concept {
    public: 
        double run(const Option& option, const MarketData& md) override {
            return runBlackScholes(option, md);
        }
    };

    template<>
    class Model<MonteCarlo> : public Concept {
    public:
        double run(const Option &option, const MarketData &md) override {
            return runMonteCarlo(option, md);
        }
    };

    PricerByTypeErasure(const std::shared_ptr<Concept> &model)
        : model_(model) {}

    std::shared_ptr<Concept> model_;
public:
    // however having these factory methods defeats the purpose of type erasure
    static PricerByTypeErasure makeBlackScholesPricer() {
        return PricerByTypeErasure(std::make_shared<Model<BlackScholes>>());
    }

    static PricerByTypeErasure makeMonteCarloPricer() {
        return PricerByTypeErasure(std::make_shared<Model<MonteCarlo>>());
    }

    double run(const Option& option, const MarketData& md) {
        double ret = model_->run(option, md);
        std::cout << "ret=" << ret << std::endl;
        return ret;
    }
};

/////////////////////////////////////////////////////
// full blown manual type erasure based polymorphism
/////////////////////////////////////////////////////

// this is another wrapping for more full blown type erasure
struct BlackScholesModel {
    double run(const Option &option, const MarketData &md) {
        return runBlackScholes(option, md);
    }
};

struct MonteCarloModel {
    double run(const Option &option, const MarketData &md) {
        return runMonteCarlo(option, md);
    }
};

template<typename T>
concept PricingModel = requires(T model, const Option& option, const MarketData& md) {
    { model.run(option, md) } -> std::same_as<double>;
};

class PricerByFullTypeErasure {
    class Concept {
    public:
        virtual ~Concept() = default;
        virtual double run(const Option& option, const MarketData& md) = 0;
    };

    template<PricingModel M>
    class Model : public Concept {
        M model_;
    public:
        Model(M&& model): model_(std::forward<M>(model)) {}
        double run(const Option& option, const MarketData& md) override {
            return model_.run(option, md);
        }
    };

    // data member
    std::unique_ptr<Concept> pModel_;

public:
    template<PricingModel M>
    PricerByFullTypeErasure(M&& model)
    : pModel_(std::move(std::make_unique<Model<M>>(std::forward<M>(model)))) {
    }

    double run(const Option& option, const MarketData& md) {
        return pModel_->run(option, md);
    }
};

struct TestPricerTypeErasure {
    static const Option OPTION;
    static const MarketData MD; 

    static void testPricerByTemplate() {
        std::cout << "Test Pricer By Template" << std::endl;
        priceOption<BlackScholes>(OPTION, MD);
        priceOption<MonteCarlo>(OPTION, MD);
    }

    static void testPricerByFunction() {
        std::cout << "Test Pricer by std::function" << std::endl;
        PricerByFunction pricerByBS(::runBlackScholes);
        pricerByBS.run(OPTION, MD);

        PricerByFunction pricerByMC(::runMonteCarlo);
        pricerByMC.run(OPTION, MD);

        PricerByFunction pricer = runBlackScholes;
        pricer.run(OPTION, MD);

        pricer = runMonteCarlo;
        pricer.run(OPTION, MD);
    }

    static void testPricerByTypeErasure() {
        std::cout << "Test Pricer by type erasure" << std::endl;
        PricerByTypeErasure pricer = PricerByTypeErasure::makeBlackScholesPricer();
        pricer.run(OPTION, MD);
        pricer = PricerByTypeErasure::makeMonteCarloPricer();
        pricer.run(OPTION, MD);
    }

    static void testPricerByFullTypeErasure() {
        std::cout << "Test Pricer by FULL type erasure" << std::endl;
        PricerByFullTypeErasure pricer = BlackScholesModel{};
        pricer.run(OPTION, MD);
        pricer = MonteCarloModel{};
        pricer.run(OPTION, MD);
    }
};

const Option TestPricerTypeErasure::OPTION{};
const MarketData TestPricerTypeErasure::MD{};


// clang++ -std=c++20 type_erasure.m.cpp -o type_erasure.tsk
int main(int argc, char* argv[]) {
    TestPricerTypeErasure::testPricerByTemplate();
    TestPricerTypeErasure::testPricerByFunction();
    TestPricerTypeErasure::testPricerByTypeErasure();
    TestPricerTypeErasure::testPricerByFullTypeErasure();
    return 0;
}