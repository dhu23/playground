#define BOOST_TEST_MODULE messages
#include <boost/test/unit_test.hpp>
#include "messages.h"

BOOST_AUTO_TEST_CASE(test_sizeof)
{
    BOOST_TEST(sizeof(ShutDown) == 72);
    BOOST_TEST(sizeof(LogOn) == 40);
    BOOST_TEST(sizeof(LogOut) == 72);
    BOOST_TEST(sizeof(QuoteUpdate) != 28); // due to alignment I guess?

    BOOST_TEST(SizeT<Timestamp>::value() == 8);
    BOOST_TEST(SizeT<ByteArray<32>>::value() == 34);
    BOOST_TEST(SizeT<ShutDown>::value() == 76);
    BOOST_TEST(SizeT<LogOn>::value() == 42);
    BOOST_TEST(SizeT<LogOut>::value() == 76);
    BOOST_TEST(SizeT<QuoteUpdate>::value() == 30);
    BOOST_TEST(SizeT<Acknowledgement>::value() == 43);
    BOOST_TEST(SizeT<HeartBeat>::value() == 42);
}

BOOST_AUTO_TEST_CASE(test_packing)
{
    Packet<256, LinearBufferIdx> packet;

    Timestamp now = Timestamp::now();
    
    LogOn lon { now, ByteArray<32>::makeFromArray("robot1") };
    LogOut lout {
        now,
        ByteArray<32>::makeFromArray("control"), 
        ByteArray<32>::makeFromArray("EOD")
    };
    ShutDown sd {
        now,
        ByteArray<32>::makeFromArray("control"), 
        ByteArray<32>::makeFromArray("EOD-shutdown")
    };
    
    BOOST_TEST(putM(packet, lon));
    BOOST_TEST(putM(packet, lout));
    BOOST_TEST(putM(packet, sd));

    BOOST_TEST(
        packet.readableSize() == 
        SizeT<MType>::value()*3 + 
        SizeT<LogOn>::value() + SizeT<LogOut>::value() + SizeT<ShutDown>::value());

    auto res1 = peekM(packet);
    BOOST_TEST(static_cast<bool>(res1));
    BOOST_TEST(res1->mt == MType::LogOn);
    BOOST_TEST(res1->complete);

    struct TestProc : public Processor
    {
        ShutDown sd;
        LogOn lon;
        LogOut lout;
        void onMessage(const ShutDown& msg) { sd = msg; }
        void onMessage(const LogOn& msg) { lon = msg; }
        void onMessage(const LogOut& msg) { lout = msg; }
        void onMessage(const QuoteUpdate& msg) {}
        void onMessage(const Acknowledgement& msg) {}
        void onMessage(const HeartBeat& msg) {}
    };

    TestProc tp;
    BOOST_TEST(getM(packet, tp) == GetMRes::Exhausted);

    BOOST_TEST(tp.sd == sd);
    BOOST_TEST(tp.lon == lon);
    BOOST_TEST(tp.lout == lout);
}
