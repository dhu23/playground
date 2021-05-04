#define BOOST_TEST_MODULE messages
#include <boost/test/unit_test.hpp>
#include "messages.h"

BOOST_AUTO_TEST_CASE(test_sizeof)
{
    BOOST_TEST(sizeof(ShutDown) == 64);
    BOOST_TEST(sizeof(LogOn) == 32);
    BOOST_TEST(sizeof(LogOut) == 64);
    BOOST_TEST(sizeof(QuoteUpdate) != 20); // due to alignment I guess?

    BOOST_TEST(
        sizeT<ShutDown>() == 
        sizeof(ByteArray<32>)+sizeof(ByteArray<32>));
    BOOST_TEST(sizeT<LogOn>() == sizeof(ByteArray<32>));
    BOOST_TEST(
        sizeT<LogOut>() == 
        sizeof(ByteArray<32>)+sizeof(ByteArray<32>));
    BOOST_TEST(
        sizeT<QuoteUpdate>() == 
        sizeof(uint32_t)+sizeof(uint64_t)+sizeof(ByteArray<8>));
}

BOOST_AUTO_TEST_CASE(test_packing)
{
    Packet<256, LinearBufferIdx> packet;
    
    LogOn lon { ByteArray<32>::makeFromArray("robot1") };
    LogOut lout {
        ByteArray<32>::makeFromArray("control"), 
        ByteArray<32>::makeFromArray("EOD")
    };
    ShutDown sd {
        ByteArray<32>::makeFromArray("control"), 
        ByteArray<32>::makeFromArray("EOD-shutdown")
    };
    
    BOOST_TEST(put(packet, lon));
    BOOST_TEST(put(packet, lout));
    BOOST_TEST(put(packet, sd));


    BOOST_TEST(peekM(packet));
    MType mt1;
    BOOST_TEST(get(packet, mt1));
    BOOST_TEST(mt1 == MType::LogOn);
    LogOn glon;
    BOOST_TEST(get(packet, glon));
    BOOST_TEST(glon == lon);

    BOOST_TEST(peekM(packet));
    MType mt2;
    BOOST_TEST(get(packet, mt2));
    BOOST_TEST(mt2 == MType::LogOut);
    LogOut glout;
    BOOST_TEST(get(packet, glout));
    BOOST_TEST(glout == lout);

    BOOST_TEST(peekM(packet));
    MType mt3;
    BOOST_TEST(get(packet, mt3));
    BOOST_TEST(mt3 == MType::ShutDown);
    ShutDown gsd;
    BOOST_TEST(get(packet, gsd));
    BOOST_TEST(gsd == sd);

    BOOST_TEST(!peekM(packet));

    // while (peekM(packet))
    // {
    //     MType mt;
    //     get(packet, mt); // it should work as peekM is successful
    //     switch(mt)
    //     {
    //     case MType::LogOn: 
    //         {
    //             LogOn glon;
    //             get(packet, glon);
    //             BOOST_TEST(glon == lon);
    //         }
    //         break;
    //     case MType::LogOut:
    //         {
    //             LogOut glout;
    //             get(packet, glout);
    //             BOOST_TEST(glout == lout);
    //         }
    //         break;
    //     case MType::ShutDown:
    //         {
    //             ShutDown gsd;
    //             get(packet, gsd);
    //             BOOST_TEST(gsd == sd);
    //         }
    //         break;
    //     default:
    //         break;
    //     }
    // }
}
