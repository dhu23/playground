#include "a.h"

#include <iostream>
// __attribute__((weak)) 
const double a = 1.0;
/*
void foo()
{
    std::cout << "in a.cpp foo()" << std::endl;    
}
*/
A::A()
{
    std::cout << a << std::endl;
}
