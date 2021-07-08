#include "sign.h"

#include <stdexcept>

namespace dhm{

Sign Sign::sign(const int number)
{
    if(0 < number)
    {
        return positive();
    }
    else if(0 > number)
    {
        return negative();
    }
    else
    {
        return zero();
    }
}

Sign Sign::operator*(const Sign& s) const
{
    return Sign(this->m_signNum*s.m_signNum);
}

Sign Sign::operator/(const Sign& s) const
{
    if(s.isZero())
    {
        throw std::runtime_error("CANNOT divide by ZERO");
    }
    return Sign(this->m_signNum/s.m_signNum);
}

bool Sign::operator<(const Sign& s) const 
{
    return this->m_signNum < s.m_signNum;
}

bool Sign::operator==(const Sign& s) const 
{
    return this->m_signNum == s.m_signNum;
}

bool Sign::operator>(const Sign& s) const
{
    return this->m_signNum > s.m_signNum;
}

Sign::Sign(const Sign& s):m_signNum(s.m_signNum)
{
}

} // end of namespace dhm
