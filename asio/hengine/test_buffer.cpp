#define BOOST_TEST_MODULE ringbuffer
#include <boost/test/unit_test.hpp>
#include "buffer.h"

void testExclusive(const LinearBufferIdx& b)
{
    for (std::size_t i = b.head(); i < b.capacity(); ++i)
    {
        BOOST_TEST(b.isWritable(i) != b.isReadable(i));
    }
}

BOOST_AUTO_TEST_CASE(test_linearbuffer)
{
    LinearBufferIdx lb(10);

    BOOST_TEST(lb.isEmpty());
    BOOST_TEST(!lb.isFull());

    BOOST_TEST(lb.capacity() == 10);

    BOOST_TEST(!lb.read(1));

    BOOST_TEST(lb.write(3));
    testExclusive(lb);

    BOOST_TEST(lb.read(2));
    testExclusive(lb);

    BOOST_TEST(lb.nextMany(9, 1) == 10);
    BOOST_TEST(lb.isReadable(2));
    BOOST_TEST(!lb.isWritable(2));
    BOOST_TEST(lb.isWritable(3));
    BOOST_TEST(!lb.isReadable(3));

    BOOST_TEST(lb.tailSpace() == 7);
    BOOST_TEST(lb.freeSpace() == 7);
    BOOST_TEST(lb.usedSpace() == 1);

    BOOST_TEST(!lb.write(8));
    testExclusive(lb);

    BOOST_TEST(lb.read(1));
    testExclusive(lb);

    BOOST_TEST(lb.isEmpty());
    BOOST_TEST(lb.tailSpace() == 10);
    
    BOOST_TEST(lb.write(10));
    BOOST_TEST(lb.read(3));
    testExclusive(lb);

    BOOST_TEST(lb.read(7));
    testExclusive(lb);

    BOOST_TEST(lb.isEmpty());
}

void testExclusive(const RingBufferIdx& b)
{
    for (std::size_t i = 0; i < b.capacity(); ++i)
    {
        BOOST_TEST(b.isReadable(i) != b.isWritable(i));
    }
}

BOOST_AUTO_TEST_CASE(test_ringbuffer)
{
    RingBufferIdx rb(10);

    BOOST_TEST(rb.isEmpty());
    BOOST_TEST(!rb.isFull());

    BOOST_TEST(rb.capacity() == 10);

    BOOST_TEST(!rb.read(1));
    BOOST_TEST(rb.write(3));
    testExclusive(rb);
    
    BOOST_TEST(rb.read(2));
    testExclusive(rb);

    BOOST_TEST(rb.nextMany(9, 1) == 0); // wrapping
    BOOST_TEST(rb.isReadable(2));
    BOOST_TEST(!rb.isWritable(2));
    BOOST_TEST(rb.isWritable(3));
    BOOST_TEST(!rb.isReadable(3));

    BOOST_TEST(rb.tailSpace() == 7);
    BOOST_TEST(rb.freeSpace() == 9);
    BOOST_TEST(rb.usedSpace() == 1);

    BOOST_TEST(rb.write(8));
    testExclusive(rb);

    BOOST_TEST(rb.tailSpace() == 1);
    BOOST_TEST(rb.freeSpace() == 1);
    BOOST_TEST(rb.usedSpace() == 9);

    BOOST_TEST(rb.isWritable(1));
    BOOST_TEST(!rb.isReadable(1));
    BOOST_TEST(rb.isReadable(2));
    BOOST_TEST(!rb.isWritable(2));
    BOOST_TEST(rb.isReadable(0));
    BOOST_TEST(!rb.isWritable(0));

    BOOST_TEST(!rb.read(10));
    BOOST_TEST(rb.read(3));
    testExclusive(rb);

    BOOST_TEST(rb.read(6));
    testExclusive(rb);

    BOOST_TEST(rb.isEmpty());
}
