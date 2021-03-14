#include <boost/serialization/base_object.hpp>
#include <boost/serialization/list.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <string>
#include <iostream>
#include <fstream>

class Point
{
    int x_;
    int y_;
public:
    Point(int x, int y): x_(x), y_(y) {}

    int x() const { return x_; }
    int y() const { return y_; }
    
};

namespace boost { namespace serialization {
    
template<class Archive>
void save(Archive& ar, const Point& p, const unsigned int version)
{
    ar << p.x();
    ar << p.y();
}

template<class Archive>
void load(Archive& ar, Point& p, const unsigned int version)
{
    int x;
    int y;
    ar >> x;
    ar >> y;
    p = Point(x, y);
}

}}


class Rectangle
{
    Point bl; // bottom left
    Point tr; // top right
public:
    int x1() const { return bl.x(); }
    int y1() const { return bl.y(); }
    int x2() const { return tr.x(); }
    int y2() const { return tr.y(); }

    Rectangle(int x1, int y1, int x2, int y2): bl(x1, y1), tr(x2, y2) {}
    int area() const { return std::abs(x2() - x1()) * std::abs(y2() - y1()); }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        boost::serialization::split_free(ar, bl, version);
        boost::serialization::split_free(ar, tr, version);
    }
};


int main(int argc, char* argv[])
{
    Rectangle r(0, 0, 1, 2);
    std::cout << r.area() << std::endl;
    
    std::string filename = "./ser.txt";

    std::ofstream ofs(filename);
    boost::archive::text_oarchive oa(ofs);
    oa << r;



    return 0;
}
