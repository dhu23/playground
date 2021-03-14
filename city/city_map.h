#ifndef INCLUDED_CITY_MAP_H
#define INCLUDED_CITY_MAP_H

#include <iostream>
#include <vector>

namespace dhc
{

class Location
{
    public:
        Location(const int x, const int y): m_x(x), m_y(y) {}
        Location(const Location& l): m_x(l.x()), m_y(l.y()) {}
        
        const int x() const {return m_x;}
        const int y() const {return m_y;}

        std::ostream& print(std::ostream& os) const;
    private:
        int m_x;
        int m_y;
};

bool operator==(const Location& l1, const Location& l2);
bool operator!=(const Location& l1, const Location& l2);
std::ostream& operator<<(std::ostream& os, const Location& l);

class StraightRoad
{
    public:
        enum StraightRoadDirection {POINT = 1, VERTICAL, HORIZONTAL};
        StraightRoad(const Location& l1, const Location& l2);

        bool onRoad(const Location& loc) const;
    public:
        Location m_loc1;
        Location m_loc2;
        StraightRoadDirection m_roadDirection;
};

class RoadSystem
{
    public:
        RoadSystem();
        void addRoad(const StraightRoad& l);
        void removeRoad(const StraightRoad& l);
    private:
        std::vector<
};

}

#endif 
