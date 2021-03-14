#include "city_map.h"

#include <iostream>

int main(int argc, char* argv[])
{
    using namespace dhc;
    Location l1(3, 5);
    Location l2(3, 20);

    Location l3(3, 5);

    StraightRoad r1(l1, l2);
    std::cout << l3 << " on road:" << r1.onRoad(l3) << std::endl;
}
