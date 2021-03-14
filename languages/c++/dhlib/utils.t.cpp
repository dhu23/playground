// #define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE utils
#include <boost/test/unit_test.hpp>
#include "string_utils.h"
#include "time_utils.h"
#include <vector>
#include <unordered_set>

BOOST_AUTO_TEST_CASE(test_join_by)
{
    // test joinBy 
    auto ret = StringUtils::joinBy("--", 1, 2, 1.5, true, "abc");
    std::cout << ret << std::endl;
    BOOST_TEST(ret == "1--2--1.5--1--abc");
}

BOOST_AUTO_TEST_CASE(test_split_to_str)
{
    std::vector<std::string> ans{"abc", "def", "ghi"};
    auto ret = StringUtils::splitToStrBy("/", "abc/def/ghi/");
    for (auto& x : ret) { std::cout << "|" << x << "|\n"; }
    BOOST_TEST(ret == ans);
}

BOOST_AUTO_TEST_CASE(test_split_to_int)
{
    std::vector<int> ans{5712, 1215, 12};
    BOOST_TEST(StringUtils::splitToIntsBy(",", "5712,1215,12") == ans);
}

BOOST_AUTO_TEST_CASE(test_join_from)
{
    std::vector<int> x{1, 2, 12, 123};
    BOOST_TEST(StringUtils::joinFrom<int>(",", x) == "1,2,12,123,");
}

BOOST_AUTO_TEST_CASE(test_to_upper)
{
    std::string x("aaron@castlefield");
    BOOST_TEST(StringUtils::toUpper(x) == "AARON@CASTLEFIELD");
}

BOOST_AUTO_TEST_CASE(test_get_local_yyyymmdd)
{
    std::cout << getLocalTimeYYYYMMDD() << std::endl;
    std::cout << getLocalTimeYYYYMMDD_r() << std::endl;
}

// g++ utils.t.cpp time_utils.o -std=c++17 -I /home/daowen/boost_1_75_0 -L /home/daowen/boost_1_75_0/stage/lib -lboost_unit_test_framework -static -o utils_ut
