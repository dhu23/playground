#ifndef INCLUDED_DHM_RATIONAL_NUMBER_H
#define INCLUDED_DHM_RATIONAL_NUMBER_H

#include "natural_number.h"
#include "integer_number.h"
#include "signed_number.h"

namespace dhm
{

class RationalNumber : public SignedNumber
{
    public:
        RationalNumber();
        RationalNumber(const NaturalNumber& n);
        RationalNumber(const IntegerNumber& i);

        RationalNumber(
                const NaturalNumber& numerator, 
                const NaturalNumber& denominator, 
                const Sign& sign);

        RationalNumber(const RationalNumber& r);
        RationalNumber& operator=(const RationalNumber& r);
        ~RationalNumber();

        std::string print() const;

        /*
        RationalNumber& operator+=(const RationalNumber& r); 
        RationalNumber& operator-=(const RationalNumber& r);

        RationalNumber& operator*=(const RationalNumber& r);
        RationalNumber& operator/=(const RationalNumber& r);
        */
    private:
        NaturalNumber m_numerator;
        NaturalNumber m_denominator;
};

} // end of namespace dhm

#endif
