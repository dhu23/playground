#include "a.h"
#include <iostream>

int main(int argc, char* argv[])
{
    using std::cout;
    using std::endl;

    A a1;
    a1.x = 3;
    a1.y = 4;
    cout << "x=" << a1.x << ",y=" << a1.y << ",hypot=" << a1.hypot() << endl;
    cout << a1.i << endl;
    cout << A::i << endl;
    // cout << static_cast<int>(a1.e) << endl;
    cout << a1.e << endl;

    return 0;
}
