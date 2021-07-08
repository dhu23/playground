#ifndef INCLUDED_DHM_SIGN_H
#define INCLUDED_DHM_SIGN_H

namespace dhm{

class Sign
{
    public:
        static Sign positive() {return Sign(1);}
        static Sign zero() {return Sign(0);}
        static Sign negative() {return Sign(-1);}

        static Sign sign(const int number);

        Sign operator*(const Sign& s) const;
        Sign operator/(const Sign& s) const;

        bool operator<(const Sign& s) const;
        bool operator==(const Sign& s) const;
        bool operator>(const Sign& s) const;

        bool isPositive() const {return 1 == m_signNum;}
        bool isZero() const {return 0 == m_signNum;}
        bool isNegative() const {return -1 == m_signNum;}

        Sign(const Sign& s);

    private:
        Sign(const signed char signNum): m_signNum(signNum)
        {
        }

        signed char m_signNum;
};


} // end of namespace dhm

#endif
