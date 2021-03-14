#include "slots.h"
#include <stdexcept>

using std::string;
using std::set;

Occupant::Occupant(
        const string& name, size_t l, size_t w, 
        const std::vector<std::vector<Slot*>>& slots, Area& area):
    name(name), length(l), width(w), occupiedSlots()
{
    if (l == 0 || w == 0)
    {
        throw std::runtime_error("length or width cannot be 0");
    }
    // verify slots shape
    if (slots.size() != l)
    {
        throw std::runtime_error("slot length(horizontal) not matching");
    }
    // verify slot width
    size_t slotWidth = slots.front().size();
    if (slotWidth != w)
    {
        throw std::runtime_error("slot width(vertical) not matching");
    }
    // verify consistent width
    for (size_t i = 1; i < l; ++i)
    {
        if (slots[i].size() != w)
        {
            throw std::runtime_error("slot width(vertical) not matching");
        }
    }

    // verify slots in Area and they are not taken
    for (size_t i = 0; i < l; ++i)
    {
        for (size_t j = 0; j < w; ++j)
        {
            if (area.isSlotTaken(slots[i][j]))
            {
                throw std::runtime_error("some slots are already taken");
            }
        }
    }
    occupiedSlots.assign(slots.begin(), slots.end());

    for (size_t i = 0; i < l; ++i)
    {
        for (size_t j = 0; j < w; ++j)
        {
            occupiedSlots[i][j].occupant = this;
        }
    }
}

Coordinate::Coordinate(size_t x, size_t y): x(x), y(y) {}

Slot::Slot(size_t x, size_t y): Coordinate(x, y), occupant() {}
Slot::Slot(const Coordinate& c): Coordinate(c), occupant() {}
Slot::Slot(Coordinate&& c): Coordinate(c), occupant() {}

Area::Area(size_t l, size_t w):slots()
{
    slots.resize(l);

    for (size_t i = 0; i < l; ++i)
    {
        slots[i].reserve(w);
        for (size_t j = 0; j < w; ++j)
        {
            slots[i].emplace_back(i, j);
        }
    }
    length = l;
    width = w;
}

bool Area::isSlotTaken(const Slot& slot) const
{
    if (slot.c.x >= length || slot.c.y >= width)
    {
        throw std::runtime_error("out of boundary");
    }
    if (slot.c.x >= slots.size())
    {
        throw std::runtime_error("out of boundary");
    }
    if (slots.c.w >= slots[slot.c.x].size())
    {
        throw std::runtime_error("out of boundary");
    }
    return slots[slot.c.x][slot.c.y]
}
