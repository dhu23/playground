#define BOOST_TEST_MODULE bytearray
#include <boost/test/unit_test.hpp>
#include "bytearray.h"
#include <iostream>

BOOST_AUTO_TEST_CASE(test_default_ctor)
{
    ByteArray<32> ba1;
    BOOST_TEST(ba1.empty());
    BOOST_TEST(ba1.size() == 0);
    BOOST_TEST(ba1.str() == std::string());
}

BOOST_AUTO_TEST_CASE(test_ctor)
{
    ByteArray<8> ba1;

    char input[16] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};
    ba1.fromArray(input, 10);
    BOOST_TEST(ba1.size() == 7);
    BOOST_TEST(ba1.str() == std::string("abcdefg"));

    ba1.fromArray(input, 4);
    BOOST_TEST(ba1.size() == 4);
    BOOST_TEST(ba1.str() == std::string("abcd"));

    ba1.fromCArray("abcdefghi");
    BOOST_TEST(ba1.size() == 7);
    BOOST_TEST(ba1.str() == std::string("abcdefg"));

    ba1.fromCArray("ab");
    BOOST_TEST(ba1.size() == 2);
    BOOST_TEST(ba1.str() == std::string("ab"));

    ba1.fromString(std::string("abcdefghi"));
    BOOST_TEST(ba1.size() == 7);
    BOOST_TEST(ba1.str() == std::string("abcdefg"));

    ba1.fromString(std::string("ab"));
    BOOST_TEST(ba1.size() == 2);
    BOOST_TEST(ba1.str() == std::string("ab"));
}
