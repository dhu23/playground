#define BOOST_TEST_MODULE ringbuffer
#include <boost/test/unit_test.hpp>
#include "buffer.h"

//BOOST_AUTO_TEST_CASE(test_ctor)
//{
//    RingBuffer<32, 4> rb;
//    BOOST_TEST(rb.isEmpty());
//    BOOST_TEST(!rb.isFull());
//}
//
//BOOST_AUTO_TEST_CASE(test_push)
//{
//    RingBuffer<32, 4> rb;
//    rb.push(int(5));
//    rb.push(int(5));
//    rb.push(int(5));
//    rb.push(int(5));
//}


BOOST_AUTO_TEST_CASE(test_linearbuffer)
{
    LinearBuffer lb(10);

    BOOST_TEST(lb.isEmpty());
    BOOST_TEST(!lb.isFull());

    BOOST_TEST(lb.capacity() == 10);

    BOOST_TEST(!lb.read(1));
    BOOST_TEST(lb.write(3));
    BOOST_TEST(lb.read(2));

    BOOST_TEST(lb.tailSpace() == 7);
    BOOST_TEST(lb.freeSpace() == 7);
    BOOST_TEST(lb.usedSpace() == 1);

    BOOST_TEST(!lb.write(8));
    BOOST_TEST(lb.read(1));

    BOOST_TEST(lb.isEmpty());
    BOOST_TEST(lb.tailSpace() == 10);
    
    BOOST_TEST(lb.write(10));
    BOOST_TEST(lb.read(3));
    BOOST_TEST(lb.read(7));
    BOOST_TEST(lb.isEmpty());
}
