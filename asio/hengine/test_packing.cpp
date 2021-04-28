#define BOOST_TEST_MODULE packing
#include <boost/test/unit_test.hpp>
#include "packing.h"

BOOST_AUTO_TEST_CASE(test_int8)
{
    Packet<4> packet;

    // each has 1 byte
    int8_t i0 = 6;
    int8_t i1 = -5;
    uint8_t i2 = 120;
    uint8_t i3 = 200;

    int8_t i4 = 0; // no room

    BOOST_TEST(packet.put(i0));
    BOOST_TEST(packet.put(i1));
    BOOST_TEST(packet.put(i2));
    BOOST_TEST(packet.put(i3));

    BOOST_TEST(!packet.put(i4)); // not enough room

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

    int8_t g4 = 0;
    BOOST_TEST(!packet.get(g4));
}

BOOST_AUTO_TEST_CASE(test_int16)
{
    Packet<8> packet;

    // each has 2 bytes
    int16_t i0 = 6;
    int16_t i1 = -5;
    uint16_t i2 = 12000;
    uint16_t i3 = 35000;

    int16_t i4 = 0; // no room

    BOOST_TEST(packet.put(i0));
    BOOST_TEST(packet.put(i1));
    BOOST_TEST(packet.put(i2));
    BOOST_TEST(packet.put(i3));

    BOOST_TEST(!packet.put(i4)); // no room

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

    int16_t g4 = 0;
    BOOST_TEST(!packet.get(g4));
}

BOOST_AUTO_TEST_CASE(test_int32)
{
    Packet<16> packet;

    // each has 4 bytes
    int32_t i0 = 6;
    int32_t i1 = -5;
    uint32_t i2 = 1000000000;
    uint32_t i3 = 3000000000;
    
    int32_t i4 = 0; // no room

    BOOST_TEST(packet.put(i0));
    BOOST_TEST(packet.put(i1));
    BOOST_TEST(packet.put(i2));
    BOOST_TEST(packet.put(i3));

    BOOST_TEST(!packet.put(i4)); // no room

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

    int32_t g4 = 0;
    BOOST_TEST(!packet.get(g4));
}

BOOST_AUTO_TEST_CASE(test_int64)
{
    Packet<32> packet;

    // each has 8 bytes
    int64_t i0 = 6;
    int64_t i1 = -5;
    uint64_t i2 = 0x3fffffffffffffffu;
    uint64_t i3 = 0xbfffffffffffffffu;

    int64_t i4 = 0; // no room

    BOOST_TEST(packet.put(i0));
    BOOST_TEST(packet.put(i1));
    BOOST_TEST(packet.put(i2));
    BOOST_TEST(packet.put(i3));

    BOOST_TEST(!packet.put(i4));

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

    int64_t g4 = 0;
    BOOST_TEST(!packet.get(g4));
}

BOOST_AUTO_TEST_CASE(test_floats)
{
    Packet<28> packet;

    float f0 = 3.14; // 4 bytes
    float f1 = -3.14; // 4 bytes
    double f2 = 3.1415926; // 8 bytes
    double f3 = -3.1415926; // 8 bytes

    BOOST_TEST(packet.put(f0));
    BOOST_TEST(packet.put(f1));
    BOOST_TEST(packet.put(f2));
    BOOST_TEST(packet.put(f3));

    BOOST_TEST(!packet.put(f3)); // no room for another

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

    double g4 = 0.0;
    BOOST_TEST(!packet.get(g4));
}

BOOST_AUTO_TEST_CASE(test_bool)
{
    Packet<2> packet;

    BOOST_TEST(packet.put(true));
    BOOST_TEST(packet.put(false));
    BOOST_TEST(!packet.put(false)); // no room

    bool g0 = false;
    BOOST_TEST(packet.get(g0));
    BOOST_TEST(g0);

    bool g1 = true;
    BOOST_TEST(packet.get(g1));
    BOOST_TEST(!g1);

    BOOST_TEST(!packet.get(g1));
}
