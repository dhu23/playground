// this is actually inspired by the solution that book gave for "parking lot" 
// this solution focuses on the management of slots(parking spots) and their
// occupants. In fact this is a fairly general issue so in this implementation
// cars and slots are all abstracted out. This can be used to solve building
// and land type of issue as well. 

#include <set>
#include <iostream>
#include <string>
#include <boost/optional/optional.hpp>

// Occupant class discribes the object that spans over slots.
struct Occupant; 

// Slot is the minimal cell/element that an area can be divided into
struct Coordinate;
struct Slot;

// a collection of slots that supplies space to occupants
struct Area;

struct Occupant
{
    Occupant(
            const std::string& name, size_t l, size_t w, 
            const std::vector<std::vector<Slot*>>& slots, Area& area);
    std::string name;
    size_t length;
    size_t width;

    std::vector<std::vector<Slot*>> occupiedSlots;
};

// coordinates in the 2-D space
struct Coordinate
{
    Coordinate(size_t x, size_t y);
    size_t x;
    size_t y;
};

struct Slot
{
    Slot(size_t x, size_t y);
    Slot(Coordinate&& c);
    Slot(const Coordinate& c);

    Coordinate c;
    boost::optional<Occupant*> occupant;
};

class Area
{
public:
    Area(size_t l, size_t w);
    bool isSlotTaken(const Slot& slot) const;

    std::set<Slot*> findSlots(size_t l, size_t w);
private:
    std::vector<std::vector<Slot>> slots;
    size_t length;
    size_t width;
};
