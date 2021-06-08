#define BOOST_TEST_MODULE generalutil 
#include <boost/test/unit_test.hpp>
#include <iostream>

#include "generalutil.h"


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
