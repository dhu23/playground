#define BOOST_TEST_MODULE timestamp
#include <boost/test/unit_test.hpp>
#include "timestamp.h"

BOOST_AUTO_TEST_CASE(test_ctor)
{
    Timestamp ts;
    BOOST_TEST(true);
}
