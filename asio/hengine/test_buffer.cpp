#define BOOST_TEST_MODULE ringbuffer
#include <boost/test/unit_test.hpp>
#include "buffer.h"


BOOST_AUTO_TEST_CASE(test_linearbuffer)
{
    LinearBufferIdx lb(10);

    BOOST_TEST(lb.isEmpty());
    BOOST_TEST(!lb.isFull());

    BOOST_TEST(lb.capacity() == 10);

    BOOST_TEST(!lb.free(1));
    BOOST_TEST(lb.use(3));
    BOOST_TEST(lb.free(2));

    BOOST_TEST(lb.tailSpace() == 7);
    BOOST_TEST(lb.freeSpace() == 7);
    BOOST_TEST(lb.usedSpace() == 1);

    BOOST_TEST(!lb.use(8));
    BOOST_TEST(lb.free(1));

    BOOST_TEST(lb.isEmpty());
    BOOST_TEST(lb.tailSpace() == 10);
    
    BOOST_TEST(lb.use(10));
    BOOST_TEST(lb.free(3));
    BOOST_TEST(lb.free(7));
    BOOST_TEST(lb.isEmpty());
}

BOOST_AUTO_TEST_CASE(test_ringbuffer)
{
    RingBufferIdx rb(10);

    BOOST_TEST(rb.isEmpty());
    BOOST_TEST(!rb.isFull());

    BOOST_TEST(rb.capacity() == 10);

    BOOST_TEST(!rb.free(1));
    BOOST_TEST(rb.use(3));
    BOOST_TEST(rb.free(2));

    BOOST_TEST(rb.tailSpace() == 7);
    BOOST_TEST(rb.freeSpace() == 9);
    BOOST_TEST(rb.usedSpace() == 1);

    BOOST_TEST(rb.use(8));
    BOOST_TEST(rb.tailSpace() == 1);
    BOOST_TEST(rb.freeSpace() == 1);
    BOOST_TEST(rb.usedSpace() == 9);

    BOOST_TEST(!rb.free(10));
    BOOST_TEST(rb.free(3));
    BOOST_TEST(rb.free(6));

    BOOST_TEST(rb.isEmpty());
}
