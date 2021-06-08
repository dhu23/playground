#define BOOST_TEST_MODULE generalutil 
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <unordered_set>

#include "generalutil.h"

BOOST_AUTO_TEST_CASE(test_join_by)
{
    auto ret = StringUtils::joinBy("--", 1, 2, 1.5, true, "abc");
    BOOST_TEST(ret == "1--2--1.5--1--abc");
}

// BOOST_AUTO_TEST_CASE(test_split_to_str)
// {
//     std::vector<std::string> ans{"abc", "def", "ghi"};
//     auto ret = StringUtils::splitToStrBy("/", "abc/def/ghi/");
//     BOOST_TEST(ret == ans);
// }
// 
// BOOST_AUTO_TEST_CASE(test_split_to_int)
// {
//     std::vector<int> ans{5712, 1215, 12};
//     BOOST_TEST(StringUtils::splitToIntsBy(",", "5712,1215,12") == ans);
// }

BOOST_AUTO_TEST_CASE(test_join_from)
{
    std::vector<int> x{1, 2, 12, 123};
    BOOST_TEST(StringUtils::joinFrom<int>(",", x) == "1,2,12,123,");
}

BOOST_AUTO_TEST_CASE(test_to_upper)
{
    std::string x("abcdefg@hij.COM");
    BOOST_TEST(StringUtils::toUpper(x) == "ABCDEFG@HIJ.COM");
}

BOOST_AUTO_TEST_CASE(test_oscillator)
{
    Oscillator<int> o(0, 2, 5);

    BOOST_TEST(o.gen() == 0);
    BOOST_TEST(o.gen() == 2);
    BOOST_TEST(o.gen() == 4);
    BOOST_TEST(o.gen() == 6);
    BOOST_TEST(o.gen() == 8);
    BOOST_TEST(o.gen() == 10);
    BOOST_TEST(o.gen() == 8);
    BOOST_TEST(o.gen() == 6);
    BOOST_TEST(o.gen() == 4);
    BOOST_TEST(o.gen() == 2);
    BOOST_TEST(o.gen() == 0);
    BOOST_TEST(o.gen() == 2);

    Oscillator<int> o1(-1, 3, 5, false, 2);
    BOOST_TEST(o1.gen() == 5);
    BOOST_TEST(o1.gen() == 2);
    BOOST_TEST(o1.gen() == -1);
    BOOST_TEST(o1.gen() == 2);
    BOOST_TEST(o1.gen() == 5);
    BOOST_TEST(o1.gen() == 8);
    BOOST_TEST(o1.gen() == 11);
    BOOST_TEST(o1.gen() == 14);
    BOOST_TEST(o1.gen() == 11);
    BOOST_TEST(o1.gen() == 8);
    BOOST_TEST(o1.gen() == 5);
}
