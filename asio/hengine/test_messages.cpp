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

// with template member function, this class cannot be defined locally 
// inside of the test function
struct TestProc
{
    ShutDown sd;
    LogOn lon;
    LogOut lout;

    Acknowledgement makeAck(Timestamp ts, MType mt)
    {
        return Acknowledgement
        {
            ts.inTheFuture(std::chrono::seconds(1)),
            ByteArray<32>::makeFromArray("testproc"),
            mt
        };
    }

    template<std::size_t K, typename BufIdx>
    void onMessage(const ShutDown& msg, Packet<K, BufIdx>& out) 
    { 
        sd = msg; 
        putM(out, makeAck(msg.ts, MType::ShutDown));
    }
    
    template<std::size_t K, typename BufIdx>
    void onMessage(const LogOn& msg, Packet<K, BufIdx>& out) 
    { 
        lon = msg; 
        putM(out, makeAck(msg.ts, MType::LogOn));
    }

    template<std::size_t K, typename BufIdx>
    void onMessage(const LogOut& msg, Packet<K, BufIdx>& out) 
    { 
        lout = msg; 
        putM(out, makeAck(msg.ts, MType::LogOut));
    }

    template<std::size_t K, typename BufIdx>
    void onMessage(const QuoteUpdate& msg, Packet<K, BufIdx>& out) 
    {}

    template<std::size_t K, typename BufIdx>
    void onMessage(const Acknowledgement& msg, Packet<K, BufIdx>& out) 
    {}

    template<std::size_t K, typename BufIdx>
    void onMessage(const HeartBeat& msg, Packet<K, BufIdx>& out) 
    {}
};

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

    auto nextOneSec = now.inTheFuture(std::chrono::seconds(1));

    TestProc tp;
    Packet<2048, LinearBufferIdx> outPacket;
    BOOST_TEST(procM(packet, outPacket, tp) == ProcMRes::Exhausted);

    BOOST_TEST(tp.sd == sd);
    BOOST_TEST(tp.lon == lon);
    BOOST_TEST(tp.lout == lout);

    MType mt1;
    Acknowledgement ack1;

    MType mt2;
    Acknowledgement ack2;

    MType mt3;
    Acknowledgement ack3;
    BOOST_TEST(get(outPacket, mt1));
    BOOST_TEST(get(outPacket, ack1));
    BOOST_TEST(get(outPacket, mt2));
    BOOST_TEST(get(outPacket, ack2));
    BOOST_TEST(get(outPacket, mt3));
    BOOST_TEST(get(outPacket, ack3));

    BOOST_TEST(mt1 == MType::Acknowledgement);
    BOOST_TEST(mt2 == MType::Acknowledgement);
    BOOST_TEST(mt3 == MType::Acknowledgement);
}
