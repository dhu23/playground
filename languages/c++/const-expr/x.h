#include <iostream>
#include <string>

class X
{
public:
    X() {
        std::cout << "constructing X:" << X::val << std::endl;
    }
private:
    constexpr static const char* val = "--";
};
