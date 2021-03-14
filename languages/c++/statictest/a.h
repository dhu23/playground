#ifndef _INCLUDED_A_H_
#define _INCLUDED_A_H_

#include <iostream>
enum class E : int
{
    E1 = 10,
    E2 = 11
};
inline
std::ostream& operator<<(std::ostream& os, E e)
{
    switch(e)
    {
    case E::E1:
        os << "E1(10)";
        break;
    case E::E2:
        os << "E2(11)";
        break;
    default:
        os << "Uknown";
        break;
    }
    return os;
}
struct A
{
    static const int i = 1;
    static const E e = E::E1;
    double x;
    double  y;

    int hypot() const;
};

#endif
