#include "b.h"
// #include "util.h"
#include <iostream>
/*
__attribute__((weak))    
void foo()
{
    int a, b, c, d, e, f, g;
    std::cout << "in basdffffffffffffffffffffffffffffffffffffffffffffffff" << std::endl;
    struct A
    {
        int array[1000];
    };
    A xxx;
}
*/

__attribute__((weak))   int a = 5000; 

B::B()
{
    // foo();
    std::cout << a << std::endl;
}
