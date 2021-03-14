#ifndef INCLUDED_CITY_PERSON_H
#define INCLUDED_CITY_PERSON_H

#include <city_location.h>
#include <memory>

namespace dhc
{

class Building;
typedef std::tr1::shared_ptr<Building> BuildingPtr;

class Person 
{
    public:
        Person();
        virtual ~Person();

    private:
        Location m_loc;
        BuildingPtr m_pDestination;
        BuildingPtr m_pHome
};

}

#endif
