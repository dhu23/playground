#include "city_map.h"

#include <stdexcept>
#include <sstream>

namespace dhc
{

std::ostream& Location::print(std::ostream& os) const
{
    os << "(" << m_x << "," << m_y << ")";
    return os;
}

bool operator==(const Location& l1, const Location& l2)
{
    return l1.x() == l2.x() && l1.y() == l2.y();
}

bool operator!=(const Location& l1, const Location& l2)
{
    return !(l1 == l2);
}

std::ostream& operator<<(std::ostream& os, const Location& l)
{
    return l.print(os);
}

StraightRoad::StraightRoad(const Location& l1, const Location& l2):
    m_loc1(l1), m_loc2(l2)
{
    if(l1 != l2)
    {
        if(l1.x() == l2.x())
        {
            m_roadDirection = VERTICAL;
        }
        else if(l1.y() == l2.y())
        {
            m_roadDirection = HORIZONTAL;
        }
        else
        {     
            std::stringstream ss;
            ss << l1 << " and " << l2 << " don't form a straight road";
            throw std::runtime_error(ss.str());
        }
    }
    else
    {
        m_roadDirection = POINT;
    }
}

bool StraightRoad::onRoad(const Location& loc) const
{
    switch(m_roadDirection)
    {
        case POINT:
            return loc == m_loc1;
        case VERTICAL:
            return loc.x() == m_loc1.x() && 
                (loc.y()-m_loc1.y())*(loc.y()-m_loc2.y()) <= 0;
        case HORIZONTAL:
            return loc.y() == m_loc1.y() && 
                (loc.x()-m_loc1.x())*(loc.x()-m_loc2.x()) <= 0;
        default:
            throw std::runtime_error("Road type NOT recognized");
    }
}

}
