#include <cstdio>
#include <iostream>
#include <ostream>
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

struct TestUniquePointer {
    static void print(const UniquePointer<Point2D>& ptr) {
        if (ptr) {
            std::cout << *ptr << std::endl;
        } else {
            std::cout << "nullptr" << std::endl;
        }
    }

    static void testEmptyCreation() {
        std::cout << "========= test creation =========" << std::endl;
        UniquePointer<Point2D> ptr{};
        print(ptr);
    }

    static void testMoveConstruction() {
        std::cout << "========= test move ctor =========" << std::endl;
        UniquePointer<Point2D> ptr1{UniquePointer<Point2D>{new Point2D{2, 3}}};
        print(ptr1);

        UniquePointer<Point2D> ptr2(std::move(ptr1));
        print(ptr1);
        print(ptr2);
    }

    static void testMoveAssignment() {
        std::cout << "========= test move assign =========" << std::endl;
        UniquePointer<Point2D> ptr{new Point2D{2, 3}};
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


struct TestSharedPointer {
    static void print(const SharedPointer<Point2D>& ptr) {
        if (ptr) {
            std::cout << *ptr << '[';
            if (ptr.getRefCount()) {
                std::cout << *(ptr.getRefCount()) << '(' << ptr.getRefCount() << ')';       
            } else {
                std::cout << "nothing(" << ptr.getRefCount() << ')';
            }
            std::cout << ']';
        } else {
            std::cout << "nullptr";
        }    
        std::cout << std::endl;
    }

    static void print(std::string name, const SharedPointer<Point2D>& ptr) {
        std::cout << name << ':';
        print(ptr);
    }

    static void testEmptyCreation() {
        std::cout << "============= test empty creation =============" << std::endl;
        SharedPointer<Point2D> ptr{};
        print(ptr);
    }

    static void testNormalConstructor() {
        std::cout << "============ test normal construction =========" << std::endl;
        SharedPointer<Point2D> ptr(new Point2D{3, 4});
        print(ptr);
    }

    static void testCopyConstructor() {
        std::cout << "=========== test Copy constructor ===========" << std::endl;
        SharedPointer<Point2D> ptr(new Point2D{3, 4});

        SharedPointer<Point2D> another(ptr);

        std::cout << "newly copy-constructed:" << std::endl;
        print(another);
        std::cout << "original:" << std::endl;
        print(ptr);
    }

    static void testCopyAssignment() {
        std::cout << "============ test copy assignment =============" << std::endl;
        const SharedPointer<Point2D> emptyPtr;
        const SharedPointer<Point2D> sourcePtr1{new Point2D{3, 4}};
        const SharedPointer<Point2D> sourcePtr2{new Point2D{4, 5}};
        
        print(emptyPtr);
        print(sourcePtr1);
        print(sourcePtr2);

        // assign empty to empty shared pointers
        std::cout << "assigning empty to empty ==>" << std::endl;
        SharedPointer<Point2D> assignee;
        assignee = emptyPtr;

        print("assignee", assignee);
        print("emptyPtr", emptyPtr);

        // assign something to empty shared pointers
        std::cout << "assigning something to empty ==>" << std::endl;
        assignee = sourcePtr1;
        print("assignee", assignee);
        print("emptyPtr", sourcePtr1);

        // assign something else to something
        std::cout << "assigning something else to something ==>" << std::endl;
        assignee = sourcePtr2;
        print("assignee", assignee);
        print("emptyPtr", emptyPtr);
        print("sourcePtr1",  sourcePtr1);
        print("sourcePtr2", sourcePtr2);

        // assign something to empty
        std::cout << "assigning null to something ==>" << std::endl;
        assignee = emptyPtr;
        print("assignee", assignee);
        print("sourcePtr1", sourcePtr1);
        print("sourcePtr2", sourcePtr2);
    }

    static void testMoveConstructor() {
        std::cout << "============= Test Move constructor" << std::endl;
        SharedPointer<Point2D> ptr{
            SharedPointer<Point2D>{new Point2D{3, 4}}
        };

        print("ptr", ptr);

        SharedPointer<Point2D> another(std::move(ptr));
        print("another", another);
        print("ptr", ptr);
    }

    static void testMoveAssignment() {
        std::cout << "============ Test Move assignment ============" << std::endl;
        SharedPointer<Point2D> assignee;
        SharedPointer<Point2D> sourcePtr{new Point2D{3, 4}};

        std::cout << "move something into null" << std::endl;
        assignee = SharedPointer<Point2D>{new Point2D{5, 6}};
        print("assignee", assignee);

        std::cout << "move something else into something" << std::endl;
        assignee = std::move(sourcePtr);
        print("assignee", assignee);
        print("sourcePtr", sourcePtr);
    }
};


int main(int argc, char* argv[]) {
    std::cout << "------------- UNIQUE POINTER ---------------" << std::endl;
    TestUniquePointer::testEmptyCreation();
    TestUniquePointer::testMoveConstruction();
    TestUniquePointer::testMoveAssignment();

    std::cout << "------------- SHARED POINTER --------------" << std::endl;
    TestSharedPointer::testEmptyCreation();
    TestSharedPointer::testNormalConstructor();
    TestSharedPointer::testCopyConstructor();
    TestSharedPointer::testCopyAssignment();
    TestSharedPointer::testMoveConstructor();
    TestSharedPointer::testMoveAssignment();
    
    return 0;
}