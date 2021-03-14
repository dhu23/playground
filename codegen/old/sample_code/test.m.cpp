#include "dhu_exampleschema.h"
#include <iostream>

int main(int argc, char* argv[])
{
    dhu::TypeOne t1;
    t1.stringData() = "abcde";
    t1.doubleData() = 1.25;
    t1.print(std::cout, 10, 4);
    std::cout << std::endl;

    dhu::TypeTwo t2;
    t2.stringVecData().emplace_back("a");
    t2.stringVecData().emplace_back("b");
    t2.stringVecData().emplace_back("c");
    t2.typeOneData().stringData() = "xyz";
    t2.typeOneData().doubleData() = 10.5;

    t2.typeOneVecData().emplace_back();
    t2.typeOneVecData().back().stringData() = "aaa";
    t2.typeOneVecData().back().doubleData() = 1.0;

    t2.typeOneVecData().emplace_back();
    t2.typeOneVecData().back().stringData() = "bbb";
    t2.typeOneVecData().back().doubleData() = 2.0;

    t2.print(std::cout, 10, 4);

    return 0;
}
