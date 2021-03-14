#include "data_structure.h"

#include <iostream>

using ds::Stack;
using ds::Queue;
using std::cout;
using std::endl;


void testStack()
{
    cout << "----- stack test -----" << endl;
    Stack<int> s;
    cout << s.isEmpty() << endl; // print 1

    s.push(5);
    s.push(6);
    s.push(7);

    cout << s.peek() << endl; // print 7
    s.pop(); // remove 7
    s.pop(); // remove 6
    cout << s.peek() << endl; // print 5
    s.pop();
    cout << s.isEmpty() << endl; // print 1
    
    try
    {
        s.pop();
    }
    catch(std::exception& e)
    {
        cout << e.what() << endl;
    }
}

void testQueue()
{
    cout << "----- queue test ------" << endl;
    Queue<int> q;
    cout << q.isEmpty() << endl; // print 1

    q.add(1);
    q.add(2);
    q.add(3);

    cout << q.peek() << endl; // print 1
    q.remove();
    cout << q.peek() << endl; // print 2
    q.remove();
    q.remove();
    try
    {
        q.remove();
    }
    catch(std::exception& e)
    {
        cout << e.what() << endl;
    }
}

int main(int argc, char* argv[])
{
    testStack();
    testQueue();
    return 0;
}


// g++ --std=c++11 -o data_structure.tsk data_structure.cpp data_structure.m.cpp
