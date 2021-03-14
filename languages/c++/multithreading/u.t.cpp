// #define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE utils
#include <boost/test/unit_test.hpp>
#include "util.h"


BOOST_AUTO_TEST_CASE(parse_to_ints)
{
    BOOST_TEST(1 == 1);

    // test joinBy 
    auto ret = joinBy("--", 1, 2, 1.5, true, "abc");
    std::cout << ret << std::endl;
    BOOST_TEST(ret == "1--2--1.5--1--abc");
}


// g++ -std=c++17 -I /home/daowen/boost_1_75_0 -L /home/daowen/boost_1_75_0/stage/lib -lboost_unit_test_framework -static -o ut u.t.cpp
