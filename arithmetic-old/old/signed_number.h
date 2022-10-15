#ifndef INCLUDED_DHM_SIGNED_NUMBER_H
#define INCLUDED_DHM_SIGNED_NUMBER_H

#include "sign.h"

namespace dhm{

class SignedNumber
{
    public:
        SignedNumber():m_sign(Sign::positive())
        {
        }

        SignedNumber(const Sign& sign):m_sign(sign)
        {
        }

        virtual ~SignedNumber() = 0;

        /*
        virtual SignedNumber& operator+=(const SignedNumber& sn) = 0;
        virtual SignedNumber& operator-=(const SignedNumber& sn) = 0;
        virtual SignedNumber& operator*=(const SignedNumber& sn) = 0;
        virtual SignedNumber& operator/=(const SignedNumber& sn) = 0;
        */

        bool isZero() const {return m_sign.isZero();}
        bool isPositive() const {return m_sign.isPositive();}
        bool isNegative() const {return m_sign.isNegative();}

        Sign& sign() {return m_sign;}
        const Sign& sign() const {return m_sign;}

    private:
        Sign m_sign;

};

} // end of namespace dhm

#endif
