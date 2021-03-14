// why does a destructor in base class need to be declared virtual? 

// we declare destructors to be virtual so that the destructor of the most
// derived class is called. It is crucial when base class pointer is used to
// handle derived class. However this is not required if the class hierarchy
// was not designed to be used polymorphically

#include <iostream>

using namespace std;

class PolymorphicBase
{
    public: 
        PolymorphicBase() {cout << __PRETTY_FUNCTION__ << endl;}
        virtual ~PolymorphicBase() {cout << __PRETTY_FUNCTION__ << endl;}

        virtual void run() const {cout << __PRETTY_FUNCTION__ << endl;}
};

class NonPolymorphicBase
{
    public:
        NonPolymorphicBase() {cout << __PRETTY_FUNCTION__ << endl;}
        ~NonPolymorphicBase() {cout << __PRETTY_FUNCTION__ << endl;}

        void run() const {cout << __PRETTY_FUNCTION__ << endl;}
};

class PolymorphicDerived : public PolymorphicBase
{
    public:
        PolymorphicDerived() {cout << __PRETTY_FUNCTION__ << endl;}
        ~PolymorphicDerived() {cout << __PRETTY_FUNCTION__ << endl;}
        void run() const {cout << __PRETTY_FUNCTION__ << endl;} 
};

class NonPolymorphicDerived : public NonPolymorphicBase
{
    public:
        NonPolymorphicDerived() {cout << __PRETTY_FUNCTION__ << endl;}
        ~NonPolymorphicDerived() {cout << __PRETTY_FUNCTION__ << endl;}
        void run() const {cout << __PRETTY_FUNCTION__ << endl;} 
};

int main(int argc, char* argv[])
{
    PolymorphicBase* p1 = new PolymorphicDerived;
    delete p1;

    NonPolymorphicBase* p2 = new NonPolymorphicDerived;
    delete p2;

    return 0;
}
