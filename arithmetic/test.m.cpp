#include "natural_number.h"
#include "integer_number.h"
#include "rational_number.h"

#include <iostream>

using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
    using namespace dhm;

    NaturalNumber numOfDaysPerYear(356);
    cout << numOfDaysPerYear.print() << endl;
 
    NaturalNumber zero;
    cout << (zero < numOfDaysPerYear) << endl;
    cout << (zero <= numOfDaysPerYear) << endl;
    // cout << (numOfDaysPerYear == numOfDaysPerYear) << endl;

    NaturalNumber num1(731001);
    NaturalNumber num2(1004881);

    NaturalNumber quotient(num1/0);
    NaturalNumber remainder(num1%100);

    cout 
        << "quotient = " << quotient.print() 
        << " remainder = " << remainder.print() 
        << endl;

    NaturalNumber sum(num1+num2);
    cout << sum.print() << endl;

    NaturalNumber diff(num2-num1);
    cout << diff.print() << endl;

    NaturalNumber product1(num2*num1);
    cout << product1.print() << endl;

    NaturalNumber num3 = product1 + 731000;

    NaturalNumber quotient2(num3/num1);
    NaturalNumber remainder2(num3%num1);

    cout 
        << "quotient = " << quotient2.print() 
        << " remainder = " << remainder2.print() 
        << endl;

    IntegerNumber int1(num1, Sign::negative());
    cout << int1.print() << endl;

    IntegerNumber int2(num2, Sign::positive());
    cout << int2.print() << endl;
    
    IntegerNumber product2(int1*int2);
    cout << product2.print() << endl;
 
    RationalNumber r1(num1, num2, Sign::negative());
    cout << r1.print() << endl;

    return 0;
}
