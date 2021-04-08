#include <iostream>
#include <functional>
using namespace std;
// test types
struct FeatureSig {};
struct PriceSig {};

// class hierarchy
/*struct Base
{
    virtual void onSignal(FeatureSig& fs) = 0;
    virtual void onSignal(PriceSig& ps) = 0;
};*/

struct Derived // : public Base
{
    void onSignal(FeatureSig & fs)
    {
        cout << "in Derived feature sig" << endl;
    }
    void onSignal(PriceSig& ps)
    {
        cout << "in Derived Price sig" << endl;
    }
};



template<typename T>
struct OnSig
{
    using F = typename std::function<void(T&)>;
    static void onSignalT(T t, F f)
    {
        f(t);
        cout << "here" << endl;
    }
};

int main(int argc, char* argv[])
{
    Derived* b1 = new Derived();
    void (*pf)(FeatureSig&) = &(Derived::onSignal);
    /*
    OnSig<FeatureSig>::onSignalT(
        FeatureSig(), 
        static_cast<OnSig<FeatureSig>::F>(&(b1->onSignal)));
    */
    return 0;
}
