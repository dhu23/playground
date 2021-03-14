#ifndef INCLUDED_CITY_STORAGE_H
#define INCLUDED_CITY_STORAGE_H

#include "city_building.h"
#include <queue>

namespace dhc {

class Storage : public Building
{
    public: 
        Storage(const int length, const int width, const Location& bottomLeft, 
                const RoadSystem& roadSystem);
        virtual ~Storage() = 0;
        
        enum OrderType 
        {
            DO_NOT_ACCEPT = 0,
            ACCEPT = 1,
            GET = 2,
            EMPTY = 3
        }

        void setLimit(const ItemType item, const int limit);
        void takeInItem(const ItemType item, const int amount);
        void sendOutItem(const ItemType item, const int amount);
        void changeOrder(const ItemType item, const OrderType order);

    protected:
        struct ItemInfo
        {
            ItemType m_itemType;
            OrderType m_order;
            int m_limit;
            int m_stored;
        };

        const int m_capacity;
        
        std::vector<ItemInfo> m_items;
};

}

#endif
