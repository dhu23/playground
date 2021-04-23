#define BOOST_TEST_MODULE ringbuffer
#include <boost/test/unit_test.hpp>
#include "ringbuffer.h"

BOOST_AUTO_TEST_CASE(test_ctor)
{
    RingBuffer<32, 4> rb;
    BOOST_TEST(rb.isEmpty());
    BOOST_TEST(!rb.isFull());
}
