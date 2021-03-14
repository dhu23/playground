#ifndef _INCLUDED_TYPES_H
#define _INCLUDED_TYPES_H

#include <iostream>

struct PrimaryStats
{
    int strength;
    int dexterity;
    int intelligence;

    int vitality;
};

inline std::ostream& operator<<(std::ostream& os, const PrimaryStats& rhs)
{
    os 
        << "PrimaryStats[`strength=" << rhs.strength 
        << ",`dexterity=" << rhs.dexterity 
        << ",`intelligence=" << rhs.intelligence
        << ",`vitality=" << rhs.vitality
        << "]";
    return os;
}

struct Damage
{
    int dmg;
};

inline std::ostream& operator<<(std::ostream& os, const Damage& rhs)
{
    os << "Damage[dmg=" << rhs.dmg << "]";
    return os;
}

template<typename T>
class TD;

#endif
