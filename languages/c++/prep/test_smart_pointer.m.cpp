#include <iostream>
#include <sstream>

#include "smart_pointer.h"


struct Point2D {
    int x;
    int y;
    std::ostream& print(std::ostream& os) const {
        os << '(' << x << ',' << y << ')';
        return os; 
    }
    std::string toString() const {
        std::ostringstream oss;
        this->print(oss);
        return oss.str();
    }
};

std::ostream& operator<<(std::ostream& os, const Point2D& p) {
    p.print(os);
    return os;
}


int main(int argc, char* argv[]) {
    Point2D p{2, 3};
    UniquePointer<Point2D> ptr(p);

    std::cout << *ptr << std::endl; 

    return 0;
}