#define BOOST_TEST_MODULE packing
#include <boost/test/unit_test.hpp>
#include "packing.h"

BOOST_AUTO_TEST_CASE(test_int8)
{
    Packet<32> packet;

    int8_t i0 = 6;
    int8_t i1 = -5;
    uint8_t i2 = 120;
    uint8_t i3 = 200;

    BOOST_TEST(packet.put(i0));
    BOOST_TEST(packet.put(i1));
    BOOST_TEST(packet.put(i2));
    BOOST_TEST(packet.put(i3));

    int8_t g0 = 0;
    BOOST_TEST(packet.get(g0));
    BOOST_TEST(g0 == i0);

    int8_t g1 = 0;
    BOOST_TEST(packet.get(g1));
    BOOST_TEST(g1 == i1);

    uint8_t g2 = 0;
    BOOST_TEST(packet.get(g2));
    BOOST_TEST(g2 == i2);

    uint8_t g3 = 0;
    BOOST_TEST(packet.get(g3));
    BOOST_TEST(g3 == i3);
}

BOOST_AUTO_TEST_CASE(test_int16)
{
    Packet<64> packet;

    int16_t i0 = 6;
    int16_t i1 = -5;
    uint16_t i2 = 12000;
    uint16_t i3 = 35000;

    BOOST_TEST(packet.put(i0));
    BOOST_TEST(packet.put(i1));
    BOOST_TEST(packet.put(i2));
    BOOST_TEST(packet.put(i3));

    int16_t g0 = 0;
    BOOST_TEST(packet.get(g0));
    BOOST_TEST(g0 == i0);

    int16_t g1 = 0;
    BOOST_TEST(packet.get(g1));
    BOOST_TEST(g1 == i1);

    uint16_t g2 = 0;
    BOOST_TEST(packet.get(g2));
    BOOST_TEST(g2 == i2);

    uint16_t g3 = 0;
    BOOST_TEST(packet.get(g3));
    BOOST_TEST(g3 == i3);
}

BOOST_AUTO_TEST_CASE(test_int32)
{
    Packet<128> packet;

    int32_t i0 = 6;
    int32_t i1 = -5;
    uint32_t i2 = 1000000000;
    uint32_t i3 = 3000000000;

    BOOST_TEST(packet.put(i0));
    BOOST_TEST(packet.put(i1));
    BOOST_TEST(packet.put(i2));
    BOOST_TEST(packet.put(i3));

    int32_t g0 = 0;
    BOOST_TEST(packet.get(g0));
    BOOST_TEST(g0 == i0);

    int32_t g1 = 0;
    BOOST_TEST(packet.get(g1));
    BOOST_TEST(g1 == i1);

    uint32_t g2 = 0;
    BOOST_TEST(packet.get(g2));
    BOOST_TEST(g2 == i2);

    uint32_t g3 = 0;
    BOOST_TEST(packet.get(g3));
    BOOST_TEST(g3 == i3);
}

BOOST_AUTO_TEST_CASE(test_int64)
{
    Packet<256> packet;

    int64_t i0 = 6;
    int64_t i1 = -5;
    uint64_t i2 = 0x3fffffffffffffffu;
    uint64_t i3 = 0xbfffffffffffffffu;

    BOOST_TEST(packet.put(i0));
    BOOST_TEST(packet.put(i1));
    BOOST_TEST(packet.put(i2));
    BOOST_TEST(packet.put(i3));

    int64_t g0 = 0;
    BOOST_TEST(packet.get(g0));
    BOOST_TEST(g0 == i0);

    int64_t g1 = 0;
    BOOST_TEST(packet.get(g1));
    BOOST_TEST(g1 == i1);

    uint64_t g2 = 0;
    BOOST_TEST(packet.get(g2));
    BOOST_TEST(g2 == i2);

    uint64_t g3 = 0;
    BOOST_TEST(packet.get(g3));
    BOOST_TEST(g3 == i3);
}

BOOST_AUTO_TEST_CASE(test_floats)
{
    Packet<256> packet;

    float f0 = 3.14;
    float f1 = -3.14;
    double f2 = 3.1415926;
    double f3 = -3.1415926;

    BOOST_TEST(packet.put(f0));
    BOOST_TEST(packet.put(f1));
    BOOST_TEST(packet.put(f2));
    BOOST_TEST(packet.put(f3));

    float g0 = 0.0;
    BOOST_TEST(packet.get(g0));
    BOOST_TEST(g0 == f0);

    float g1 = 0.0;
    BOOST_TEST(packet.get(g1));
    BOOST_TEST(g1 == f1);

    double g2 = 0.0;
    BOOST_TEST(packet.get(g2));
    BOOST_TEST(g2 == f2);

    double g3 = 0.0;
    BOOST_TEST(packet.get(g3));
    BOOST_TEST(g3 == f3);
}
