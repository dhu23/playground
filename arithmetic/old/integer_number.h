#ifndef INCLUDED_DHM_INTEGER_NUMBER_H
#define INCLUDED_DHM_INTEGER_NUMBER_H

#include "natural_number.h"
#include "signed_number.h"

namespace dhm{

class IntegerNumber : public SignedNumber
{
    public:
        IntegerNumber();
        IntegerNumber(const int number);
        IntegerNumber(const NaturalNumber& n, const Sign& sign);

        IntegerNumber(const IntegerNumber& i);
        IntegerNumber& operator=(const IntegerNumber& i);
        ~IntegerNumber();

        std::string print() const;

        const NaturalNumber& naturalNumber() const {return m_naturalNumber;}

        IntegerNumber& operator+=(const IntegerNumber& i);
        IntegerNumber& operator-=(const IntegerNumber& i);
       
        IntegerNumber& operator*=(const IntegerNumber& i);
        IntegerNumber& operator/=(const IntegerNumber& i);
        
        // IntegerNumber& operator%=(const IntegerNumber& i);

    private:
        NaturalNumber m_naturalNumber;
};

bool operator<(const IntegerNumber& lhs, const IntegerNumber& rhs);
bool operator>(const IntegerNumber& lhs, const IntegerNumber& rhs);
bool operator==(const IntegerNumber& lhs, const IntegerNumber& rhs);
bool operator>=(const IntegerNumber& lhs, const IntegerNumber& rhs);
bool operator<=(const IntegerNumber& lhs, const IntegerNumber& rhs);

IntegerNumber operator+(const IntegerNumber& lhs, const IntegerNumber& rhs);
IntegerNumber operator-(const IntegerNumber& lhs, const IntegerNumber& rhs);

IntegerNumber operator*(const IntegerNumber& lhs, const IntegerNumber& rhs);
IntegerNumber operator/(const IntegerNumber& lhs, const IntegerNumber& rhs);

// IntegerNumber operator(const IntegerNumber& lhs, const IntegerNumber& rhs);

} // end of namespace dhm

#endif
