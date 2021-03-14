// write a smart pointer class. A smart pointer is a data type usually 
// implemented with templates, that simulates a pointer while also prividing
// automatic garbage collection. It automatically counts the number of 
// references to a SmartPointer<T*> object and frees the object of type T
// when the reference count hits zero.

#include "shared_pointer.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    SharedPointer<string> p1(new string("sample1"));
    cout << "p=" <<  p1.get() << ",content=" << *p1 << endl;
    p1.print(cout) << endl;
    
    SharedPointer<string> p2(p1);
    p1.print(cout) << endl;
    p2.print(cout) << endl;

    SharedPointer<string> p3(new string("sample2"));
    p1.print(cout) << endl;
    p2.print(cout) << endl;
    p3.print(cout) << endl;

    p2 = p3;
    p1.print(cout) << endl;
    p2.print(cout) << endl;
    p3.print(cout) << endl;

    return 0;
}
