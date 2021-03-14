// how do virtual functions work in C++?

// virtual function is made possible through vtable. 
// For a class inheritance hierarchy, where a base class contains virtual
// functions with default implementation or a pure virtual function. 
// For class from this hierarchy, the compiler adds a virtual table(vtable) to
// store pointers to the associated function. If the function is overridden, 
// the pointer in the vtable points to the subclass function, if not, it points
// to its parent's function. Dynamimc binding is performed by going through the
// virtual table mechanism. 

// when we assign a pointer to SubClass to BaseClass, the vtable of that object
// still points to the subclass.

#include <vector>
#include <iostream>

using namespace std;

class Function1D // single variable function defined in real domain
{
    public:
        Function1D() {}
        virtual ~Function1D() {}

        virtual double getValue(double x) const = 0;
        virtual double getDerivative(double x) const = 0;
};

// vector stores all the coefficient associated with power of i
// polynominal = a_i * x ^ i where i runs from 0 to n
class Polynominal : public Function1D
{
    public:
        Polynominal(const vector<double>& coefficients):
            Function1D(), 
            _coefficients(coefficients) {}

        // sum_i (i a_i x^i)
        double getValue(double x) const
        {
            double v = 0;
            for (auto it = _coefficients.rbegin(); 
                 it != _coefficients.rend(); ++it)
            {
                v *= x;
                v += *it;
            }
            return v;
        }

        double getDerivative(double x) const
        {
            double d = 0;
            if (_coefficients.size() < 2)
            {
                return d; // just a constant function
            }
            for (size_t i = _coefficients.size()-1; i > 0; --i)
            {
                d *= x;
                d += (_coefficients[i]*i);
            }
            return d;
        }

    private:
        vector<double> _coefficients;
};


int main(int argc, char* argv[])
{
    // f = 3*x^2 + 2*x + 1
    // f'= 6*x + 2
    Polynominal p1(vector<double>{1.0, 2.0, 3.0});
    for (size_t i = 0; i < 10; ++i)
    {
        cout << i*0.1 << "--->" << p1.getValue(i*0.1) << endl;
    }
    for (size_t i = 0; i < 10; ++i)
    {
        cout << i*0.1 << "--->" << p1.getDerivative(i*0.1) << endl;
    }
    return 0;
}
