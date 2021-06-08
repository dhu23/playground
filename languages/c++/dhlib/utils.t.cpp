// #define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE utils
#include <boost/test/unit_test.hpp>
#include "time_utils.h"
#include <vector>
#include <unordered_set>


BOOST_AUTO_TEST_CASE(test_get_local_yyyymmdd)
{
    std::cout << getLocalTimeYYYYMMDD() << std::endl;
    std::cout << getLocalTimeYYYYMMDD_r() << std::endl;
}

// g++ utils.t.cpp time_utils.o -std=c++17 -I /home/daowen/boost_1_75_0 -L /home/daowen/boost_1_75_0/stage/lib -lboost_unit_test_framework -static -o utils_ut
