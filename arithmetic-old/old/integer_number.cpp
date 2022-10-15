#include "integer_number.h"

namespace dhm{

using std::string;

namespace{


} // end of anonymous namespace

IntegerNumber::IntegerNumber():
    SignedNumber(Sign::zero()),
    m_naturalNumber()
{
}

IntegerNumber::IntegerNumber(int num):
    SignedNumber(Sign::sign(num)),
    m_naturalNumber(num >= 0 ? NaturalNumber(num) : NaturalNumber(-num))
{
}

IntegerNumber::IntegerNumber(const NaturalNumber& n, const Sign& sign):
    SignedNumber(sign),
    m_naturalNumber(n)
{
    if(this->isZero())
    {
        m_naturalNumber = NaturalNumber(0);
    }
}

IntegerNumber::IntegerNumber(const IntegerNumber& i):
    SignedNumber(i.sign()),
    m_naturalNumber(i.m_naturalNumber)
{
}

IntegerNumber& IntegerNumber::operator=(const IntegerNumber& i)
{
    if(&i != this)
    {
        m_naturalNumber = i.m_naturalNumber;
        this->sign() = i.sign();
    }
    return *this;
}

IntegerNumber::~IntegerNumber()
{
}

string IntegerNumber::print() const
{
    if(!this->isNegative())
    {
        return m_naturalNumber.print();
    }
    else
    {
        return "-"+m_naturalNumber.print();
    }
}

bool operator<(const IntegerNumber& lhs, const IntegerNumber& rhs)
{   
    if(lhs.sign() == rhs.sign())
    {
        if(lhs.isZero())
        {
            return false;
        }
        else if(lhs.isPositive())
        {
            return lhs.naturalNumber() < rhs.naturalNumber();
        }
        else
        {
            return lhs.naturalNumber() > rhs.naturalNumber();
        }
    }
    else
    {
        return lhs.sign() < rhs.sign();
    }
}

bool operator>(const IntegerNumber& lhs, const IntegerNumber& rhs)
{
    return rhs < lhs;
}

bool operator==(const IntegerNumber& lhs, const IntegerNumber& rhs)
{
    return lhs.naturalNumber() == rhs.naturalNumber() && 
        lhs.sign() == rhs.sign();
}

bool operator<=(const IntegerNumber& lhs, const IntegerNumber& rhs)
{
        return lhs < rhs || lhs == rhs;
}

bool operator>=(const IntegerNumber& lhs, const IntegerNumber& rhs)
{
        return lhs > rhs || lhs == rhs;
}

IntegerNumber operator+(const IntegerNumber& lhs, const IntegerNumber& rhs)
{
}

IntegerNumber operator-(const IntegerNumber& lhs, const IntegerNumber& rhs)
{
}

IntegerNumber operator*(const IntegerNumber& lhs, const IntegerNumber& rhs)
{
    return IntegerNumber(
            lhs.naturalNumber()*rhs.naturalNumber(), 
            lhs.sign()*rhs.sign());
}

IntegerNumber operator/(const IntegerNumber& lhs, const IntegerNumber& rhs)
{
}

} // end of namespace dhm
