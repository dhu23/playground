#include <iostream>
#include <sstream>
#include <format>

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


struct TestUniquePointer {
    static void print(const UniquePointer<Point2D>& ptr) {
        if (ptr) {
            std::cout << *ptr << std::endl;
        } else {
            std::cout << "nullptr" << std::endl;
        }
    }

    static void test_creation() {
        std::cout << "========= test creation =========" << std::endl;
        Point2D p{2, 3};

        UniquePointer<Point2D> ptr{p};
        print(ptr);
    }

    static void test_move_construction() {
        std::cout << "========= test move ctor =========" << std::endl;
        Point2D p{2, 3};

        UniquePointer<Point2D> ptr{UniquePointer<Point2D>{p}};
        print(ptr);
    }

    static void test_move_assignment() {
        std::cout << "========= test move assign =========" << std::endl;
        Point2D p{2, 3};

        UniquePointer<Point2D> ptr{UniquePointer<Point2D>{p}};
        UniquePointer<Point2D> another;
        
        std::cout << "newly created empty ptr: " << std::endl;
        print(another);
        another = std::move(ptr);

        std::cout << "after assignment:" << std::endl;
        print(another);

        std::cout << "the original:" << std::endl;
        print(ptr);
    }
};


int main(int argc, char* argv[]) {
    TestUniquePointer::test_creation();
    TestUniquePointer::test_move_construction();
    TestUniquePointer::test_move_assignment();

    return 0;
}