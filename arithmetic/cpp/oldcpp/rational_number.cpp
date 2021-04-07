#include "rational_number.h"

using std::string;

namespace dhm{

RationalNumber::RationalNumber():
    SignedNumber(Sign::zero()),
    m_numerator(0),
    m_denominator(1)
{    
}

RationalNumber::RationalNumber(const NaturalNumber& n):
    SignedNumber(Sign::positive()),
    m_numerator(n),
    m_denominator(1)
{
    if(0 == n)
    {
        this->sign() = Sign::zero();
    }
}

RationalNumber::RationalNumber(const IntegerNumber& i):
    SignedNumber(i.sign()),
    m_numerator(i.naturalNumber()),
    m_denominator(1)
{
}

RationalNumber::RationalNumber(
        const NaturalNumber& numerator, 
        const NaturalNumber& denominator, 
        const Sign& sign):
    SignedNumber(sign),
    m_numerator(numerator),
    m_denominator(denominator)
{
}

RationalNumber::RationalNumber(const RationalNumber& r):
    SignedNumber(r.sign()),
    m_numerator(r.m_numerator),
    m_denominator(r.m_denominator)
{
}

RationalNumber& RationalNumber::operator=(const RationalNumber& r)
{
    if(&r != this)
    {
        this->sign() = r.sign();
        m_numerator = r.m_numerator;
        m_denominator = r.m_denominator;
    }
    return *this;
}

RationalNumber::~RationalNumber()
{
}

string RationalNumber::print() const
{
    if(this->isZero())
    {
        return m_numerator.print();
    }
    else if(1 == m_denominator)
    {
        IntegerNumber same(m_denominator, this->sign());
        return same.print();
    }
    else
    {
        string fraction = m_numerator.print() + "/" + m_denominator.print();
        if(this->isNegative())
        {   
            return "-"+fraction;
        }
        return fraction;
    }
}

} // end of namespace dhm
