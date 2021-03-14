#include "a.h"
#include <cmath>

int A::hypot() const
{
    return std::sqrt(x*x + y*y);
}
