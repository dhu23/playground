#ifndef INCLUDED_CITY_BUILDING_H
#define INCLUDED_CITY_BUILDING_H

#include "city_map.h"
#include <map>

namespace dhc {

class Building
{
    public:
        Building(
                const int length, const int width, const Location& bottomLeft, 
                const RoadSystem& roadSystem);
        virtual ~Building() = 0;

    protected:
        const int m_length;
        const int m_width;
        const Location m_bottomLeft;
       
        std::map<Location, StraightRoad> m_connectingLocations;
};


}

#endif
