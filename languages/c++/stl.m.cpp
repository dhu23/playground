#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <memory>

using namespace std;

struct Object
{       
    Object():x_(0), y_(0) {}
    Object(int x, int y): x_(x), y_(y) {}
    int x_;
    int y_;

    ostream& print(ostream& os) const
    {
        os << "(" << x_ << "," << y_ << ")";
        return os;
    }
};
typedef shared_ptr<Object> ObjectPtr;

ostream& operator<<(ostream& os, const Object& obj)
{
    return obj.print(os);
}

ostream& operator<<(ostream& os, const ObjectPtr& ptr)
{
    return ptr->print(os);
}

istream& operator>>(istream& is, Object& obj)
{
    is >> obj.x_ >> obj.y_;
    return is;
}

struct CompareObjectX
{
    bool operator()(const Object& lhs, const Object& rhs)
    {
        return lhs.x_ < rhs.x_;
    }
};

struct CompareObject
{
    bool operator()(const Object& lhs, const Object& rhs)
    {
        if (lhs.x_ > rhs.x_) return false;

        return lhs.x_ < rhs.x_ || lhs.y_ < rhs.y_;
    }
};

template<typename T>
ostream& operator<<(ostream& os, const vector<T>& vec)
{
    for(const T& t : vec)
    {
        os << t;
    }
    return os;
}


void testSort()
{
    vector<Object> v;
    v.push_back(Object(1, 2));
    v.push_back(Object(1, 4));
    v.push_back(Object(2, 3));
    v.push_back(Object(1, 3));

    cout << v << endl;

    sort(v.begin(), v.end(), CompareObjectX());

    cout << v << endl;

    sort(v.begin(), v.end(), CompareObject());

    cout << v << endl;
}

struct RemoveCheck
{
    RemoveCheck(int goal):goal_(goal) {}

    bool operator()(const ObjectPtr& ptr)
    {
        return goal_ == (ptr->x_ + ptr->y_);
    }

    int goal_;
};

void testRemove()
{
    vector<ObjectPtr> v;
    v.emplace_back(ObjectPtr(new Object(5, 1)));
    v.emplace_back(ObjectPtr(new Object(1, 2)));
    v.emplace_back(ObjectPtr(new Object(1, 3)));
    v.emplace_back(ObjectPtr(new Object(2, 3)));
    v.emplace_back(ObjectPtr(new Object(2, 4)));
    v.emplace_back(ObjectPtr(new Object(3, 4)));
    v.emplace_back(ObjectPtr(new Object(5, 3)));
    v.emplace_back(ObjectPtr(new Object(4, 2)));

    cout << v << endl;

    v.erase(remove_if(v.begin(), v.end(), RemoveCheck(6)), v.end());

    cout << v << endl;
}

void testIterator()
{
    vector<Object> v;
    cout << "reading inputs from cin" << endl;
    
    stringstream ss("1 2 3 4 5 6");

    copy(
            istream_iterator<Object>(ss), 
            istream_iterator<Object>(), 
            back_inserter(v));
    cout << "done reading inputs" << endl;

    copy(v.begin(), v.end(), ostream_iterator<Object>(cout, "\n"));
}

void testCout()
{
    vector<int> vec{1, 3, 5, 7, 9, 11};
    for(auto i : vec)
    {
        cout << setfill(' ') << setw(4) << ' ';
        cout << i << endl;
    }
}

void show(map<int, int>& m)
{
    for (auto kv : m)
    {
        cout << kv.first << "-->" << kv.second << endl;
            //<< (kv.first ? *kv.first : 0) << " --> "
            //<< (kv.second ? *kv.second : 0) << endl;
    }
}

template<typename T> struct TD; // TypeDisplayer

void testMap()
{
    map<int, int> m; // will be mapped from x to 11*x for (1 to 9)
    for (int i = 1; i < 10; ++i)
    {
        m.emplace(i, 0);
    }
    show(m);
    for (auto kv : m)
    {
        // TD<decltype(kv)> x; // kv type is std::pair<const int, int>
        kv.second = kv.first*11;
    }
    show(m); // nothing happens
    for (auto& kv : m)
    {
        // TD<decltype(kv)> x; // kv type is std::pair<const int, int>&
        kv.second = kv.first*11;
    }
    show(m);
}

void testSharedPointer()
{
    // test shared pointer constructed from pointer to auto variable
    int x = 5;
    // shared_ptr<int> p(&x); // causes core dump
    shared_ptr<int> p(new int(5)); 
    cout << *p << endl;
}

void testFile()
{
    string a = "abcdef";
    ofstream f("test", ios::binary);
    f.write(a.c_str(), 7);
}

int main(int argc, char* argv[])
{
    // testSort();
    // testRemove();
    
    // testIterator();
    // testCout();
    // testMap();

    // testSharedPointer();

    // testFile();

    return 0;
}

// g++ -std=c++11 -o stl.tsk stl.m.cpp
