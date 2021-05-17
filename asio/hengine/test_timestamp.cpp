#define BOOST_TEST_MODULE timestamp
#include <boost/test/unit_test.hpp>
#include "timestamp.h"

BOOST_AUTO_TEST_CASE(test_ctor)
{
    // 1619376270: 2021.04.25 18:44:30 GMT
    // 1619390670: 2021.04.25 18:44:30 NY localtime
    Timestamp ts1(1619376270UL*1000000000UL);
    BOOST_TEST(static_cast<std::string>(ts1) == "2021-04-25 18:44:30.000,000,000");

    // std::tm t; // as localtime
    // t.tm_year = 121; // 2021
    // t.tm_mon = 3; // April
    // t.tm_mday = 25; 
    // t.tm_hour = 18;
    // t.tm_min = 44;
    // t.tm_sec = 30;
    // Timestamp ts2(t);

    // BOOST_TEST(ts1 == ts2);

    // BOOST_TEST(ts2.toString() == "2021-04-25 18:44:30.000,000,000");
}

BOOST_AUTO_TEST_CASE(test_operation)
{
    Timestamp t1;
    BOOST_TEST(t1.toString() == "1970-01-01 00:00:00.000,000,000");

    Timestamp t2 = t1.inTheFuture(std::chrono::hours(3));
    BOOST_TEST(t2.toString() == "1970-01-01 03:00:00.000,000,000");
    
    Timestamp t3 = t2.inThePast(std::chrono::hours(3));
    BOOST_TEST(t3.toString() == "1970-01-01 00:00:00.000,000,000");

    // Timestamp t4 = Timestamp::now();
    // std::cout << Timestamp::now() << std::endl;
}
