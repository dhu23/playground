#define BOOST_TEST_MODULE ringbuffer
#include <boost/test/unit_test.hpp>
#include "buffer.h"


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

BOOST_AUTO_TEST_CASE(test_ringbuffer)
{
    RingBuffer rb(10);

    BOOST_TEST(rb.isEmpty());
    BOOST_TEST(!rb.isFull());

    BOOST_TEST(rb.capacity() == 10);

    BOOST_TEST(!rb.read(1));
    BOOST_TEST(rb.write(3));
    BOOST_TEST(rb.read(2));

    BOOST_TEST(rb.tailSpace() == 7);
    BOOST_TEST(rb.freeSpace() == 9);
    BOOST_TEST(rb.usedSpace() == 1);

    BOOST_TEST(rb.write(8));
    BOOST_TEST(rb.tailSpace() == 1);
    BOOST_TEST(rb.freeSpace() == 1);
    BOOST_TEST(rb.usedSpace() == 9);

    BOOST_TEST(!rb.read(10));
    BOOST_TEST(rb.read(3));
    BOOST_TEST(rb.read(6));

    BOOST_TEST(rb.isEmpty());
}
