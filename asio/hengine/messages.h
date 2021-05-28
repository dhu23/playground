#ifndef _INCLUDED_MESSAGES_H_
#define _INCLUDED_MESSAGES_H_

#include <cstdint>
#include <optional>
#include "bytearray.h"
#include "packing.h"
#include "buffer.h"
#include "timestamp.h"
#include "templ.h"

// enum to underlying type conversion
template<typename E>
inline std::underlying_type<E>::type fromEnum(E e)
{
    return static_cast<std::underlying_type<E>::type>(e);
}

////////////////////// Message type enum class //////////////////////////
enum class MType : uint8_t
{
    Unknown = 0,
    ShutDown,
    LogOn,
    LogOut,
    QuoteUpdate,
    Acknowledgement,
    HeartBeat
};

inline std::string toString(MType obj)
{
    switch(obj)
    {
    case MType::Unknown:
        return "Unknown";
    case MType::ShutDown: 
        return "ShutDown";
    case MType::LogOn:
        return "LogOn";
    case MType::LogOut:
        return "LogOut";
    case MType::QuoteUpdate:
        return "QuoteUpdate";
    case MType::Acknowledgement:
        return "Acknowledgement";
    case MType::HeartBeat:
        return "HeartBeat";
    default:
        return "?";
    }
}

inline std::ostream& operator<<(std::ostream& os, MType obj)
{
    os << "MType(" << fromEnum(obj) << ")::" << toString(obj);
    return os;
}

///////////////////////////// Messages /////////////////////////////
struct ShutDown
{
    Timestamp ts;
    ByteArray<32> from;
    ByteArray<32> reason;

    bool operator==(const ShutDown& other) const
    {
        return equalTs(ts, other.ts, from, other.from, reason, other.reason);
    }

    std::ostream& print(std::ostream& os) const
    {
        os 
            << "ShutDown[ts=" << ts 
            << ",from=" << from << ",reason=" << reason << "]";
        return os;
    }
};

inline std::ostream& operator<<(std::ostream& os, const ShutDown& obj)
{
    return obj.print(os);
}

struct LogOn
{
    Timestamp ts;
    ByteArray<32> from;

    bool operator==(const LogOn& other) const
    {
        return equalTs(ts, other.ts, from, other.from);
    }

    std::ostream& print(std::ostream& os) const
    {
        os << "LogOn[ts=" << ts << "from=" << from << "]";
        return os;
    }
};

inline std::ostream& operator<<(std::ostream& os, const LogOn& obj)
{
    return obj.print(os);
}

struct LogOut
{
    Timestamp ts;
    ByteArray<32> from;
    ByteArray<32> reason;

    bool operator==(const LogOut& other) const
    {
        return equalTs(ts, other.ts, from, other.from, reason, other.reason);
    }

    std::ostream& print(std::ostream& os) const
    {
        os 
            << "LogOut[ts=" << ts 
            << ",from=" << from << ",reason=" << reason << "]";
        return os;
    }
};

inline std::ostream& operator<<(std::ostream& os, const LogOut& obj)
{
    return obj.print(os);
}

struct QuoteUpdate
{
    Timestamp ts;
    uint32_t quantity;
    uint64_t price;
    ByteArray<8> ticker;

    bool operator==(const QuoteUpdate& other) const
    {
        return equalTs(
            ts, other.ts,
            quantity, other.quantity, 
            price, other.price, 
            ticker, other.ticker);
    }

    std::ostream& print(std::ostream& os) const
    {
        os 
            << "QuoteUpdate[ts=" << ts
            << ",quantity=" << quantity << ",price=" << price 
            << ",ticker=" << ticker << "]";
        return os;
    }
};

inline std::ostream& operator<<(std::ostream& os, const QuoteUpdate& obj)
{
    return obj.print(os);
}

struct Acknowledgement
{
    Timestamp ts;
    ByteArray<32> from;
    MType received; // echo back the type of message to the sender

    bool operator==(const Acknowledgement& other) const
    {
        return equalTs(
            ts, other.ts, from, other.from, received, other.received);
    }

    std::ostream& print(std::ostream& os) const
    {
        os 
            << "Acknowledgement[ts=" << ts 
            << ",from=" << from << ",received=" << received;
        return os;
    }
};

inline std::ostream& operator<<(std::ostream& os, const Acknowledgement& obj)
{
    return obj.print(os);
}

struct HeartBeat
{
    Timestamp ts;
    ByteArray<32> from;

    bool operator==(const HeartBeat& other) const
    {
        return equalTs(ts, other.ts, from, other.from);
    }

    std::ostream& print(std::ostream& os) const
    {
        os << "HeartBeat[ts=" << ts << ",from=" << from << "]"; 
        return os;
    }
};

inline std::ostream& operator<<(std::ostream& os, const HeartBeat& obj)
{
    return obj.print(os);
}

/////////////////////// Message Packing Specialization //////////////////////

template<>
struct SizeT<MType>
{
    constexpr static std::size_t value() 
    {
        return sizeof(std::underlying_type<MType>::type);
    }
};

template<std::size_t K, typename BufIdx>
bool put(Packet<K, BufIdx>& packet, MType x)
{
    if (SizeT<MType>::value() > packet.writableSize()) { return false; }
    return packet.put(fromEnum(x));
}

template<std::size_t K, typename BufIdx>
auto peek(Packet<K, BufIdx>& packet, MType& x)
{
    auto p = packet.peeker();
    std::underlying_type<MType>::type val;
    auto ok = false;
    if (p.peek(val)) 
    { 
        x = MType(val); 
        ok = true;
    }
    return std::make_pair(ok, p);
}

template<>
struct SizeT<ShutDown>
{
    constexpr static std::size_t value()
    {
        return
            SizeT<decltype(ShutDown::ts)>::value() +
            SizeT<decltype(ShutDown::from)>::value() + 
            SizeT<decltype(ShutDown::reason)>::value();
    }
};

template<std::size_t K, typename BufIdx>
bool put(Packet<K, BufIdx>& packet, const ShutDown& x)
{
    if (packet.writableSize() < SizeT<ShutDown>::value()) { return false; }
    packet.put(x.ts);
    packet.put(x.from);
    packet.put(x.reason);
    return true;
}

template<std::size_t K, typename BufIdx>
auto peek(Packet<K, BufIdx>& packet, ShutDown& x)
{
    auto p = packet.peeker();
    auto ok = p.peek(x.ts) && p.peek(x.from) && p.peek(x.reason);
    return std::make_pair(ok, p);
}

template<>
struct SizeT<LogOn>
{
    constexpr static std::size_t value()
    {
        return
            SizeT<decltype(LogOn::ts)>::value() +
            SizeT<decltype(LogOn::from)>::value();
    }
};

template<std::size_t K, typename BufIdx>
bool put(Packet<K, BufIdx>& packet, const LogOn& x)
{
    if (packet.writableSize() < SizeT<LogOn>::value()) { return false; }
    packet.put(x.ts);
    packet.put(x.from);
    return true;
}

template<std::size_t K, typename BufIdx>
auto peek(Packet<K, BufIdx>& packet, LogOn& x)
{
    auto p = packet.peeker();
    auto ok = p.peek(x.ts) && p.peek(x.from);
    return std::make_pair(ok, p);
}

template<>
struct SizeT<LogOut>
{
    constexpr static std::size_t value()
    {
        return 
            SizeT<decltype(LogOut::ts)>::value() +
            SizeT<decltype(LogOut::from)>::value() + 
            SizeT<decltype(LogOut::reason)>::value();
    }
};

template<std::size_t K, typename BufIdx>
bool put(Packet<K, BufIdx>& packet, const LogOut& x)
{
    if (packet.writableSize() < SizeT<LogOut>::value()) { return false; }
    packet.put(x.ts);
    packet.put(x.from);
    packet.put(x.reason);
    return true;
}

template<std::size_t K, typename BufIdx>
auto peek(Packet<K, BufIdx>& packet, LogOut& x)
{
    auto p = packet.peeker();
    auto ok = p.peek(x.ts) && p.peek(x.from) && p.peek(x.reason);
    return std::make_pair(ok, p);
}

template<>
struct SizeT<QuoteUpdate>
{
    constexpr static std::size_t value()
    {
        return 
            SizeT<decltype(QuoteUpdate::ts)>::value() +
            SizeT<decltype(QuoteUpdate::quantity)>::value() +
            SizeT<decltype(QuoteUpdate::price)>::value() +
            SizeT<decltype(QuoteUpdate::ticker)>::value();
    }
};

template<std::size_t K, typename BufIdx>
bool put(Packet<K, BufIdx>& packet, const QuoteUpdate& x)
{
    if (packet.writableSize() < SizeT<QuoteUpdate>::value()) { return false; }
    packet.put(x.ts);
    packet.put(x.quantity);
    packet.put(x.price);
    packet.put(x.ticker);
    return true;
}

template<std::size_t K, typename BufIdx>
auto peek(Packet<K, BufIdx>& packet, QuoteUpdate& x)
{
    auto p = packet.peeker();
    auto ok = 
        p.peek(x.ts) && p.peek(x.quantity) && 
        p.peek(x.price) && p.peek(x.ticker);
    return std::make_pair(ok, p);
}

template<>
struct SizeT<Acknowledgement>
{
    constexpr static std::size_t value()
    {
        return
            SizeT<decltype(Acknowledgement::ts)>::value() +
            SizeT<decltype(Acknowledgement::from)>::value() + 
            SizeT<decltype(Acknowledgement::received)>::value();
    }
};

template<std::size_t K, typename BufIdx>
bool put(Packet<K, BufIdx>& packet, const Acknowledgement& x)
{
    if (packet.writableSize() < SizeT<Acknowledgement>::value()) { return false; }
    packet.put(x.ts);
    packet.put(x.from);
    put(packet, x.received);
    return true;
}

template<std::size_t K, typename BufIdx>
auto peek(Packet<K, BufIdx>& packet, Acknowledgement& x)
{
    auto p = packet.peeker();
    typename std::underlying_type<decltype(x.received)>::type val;
    auto ok = p.peek(x.ts) && p.peek(x.from) && p.peek(val);
    x.received = MType(val);
    return std::make_pair(ok, p);
}

template<>
struct SizeT<HeartBeat>
{
    constexpr static std::size_t value()
    {
        return
            SizeT<decltype(HeartBeat::ts)>::value() +
            SizeT<decltype(HeartBeat::from)>::value();
    }
};

template<std::size_t K, typename BufIdx>
bool put(Packet<K, BufIdx>& packet, const HeartBeat& x)
{
    if (packet.writableSize() < SizeT<HeartBeat>::value()) { return false; }
    packet.put(x.ts);
    packet.put(x.from);
    return true;
}

template<std::size_t K, typename BufIdx>
auto peek(Packet<K, BufIdx>& packet, HeartBeat& x)
{
    auto p = packet.peeker();
    auto ok = p.peek(x.ts) && p.peek(x.from);
    return std::make_pair(ok, p);
}


template<std::size_t K, typename BufIdx, typename T>
bool get(Packet<K, BufIdx>& packet, T& x)
{
    auto ret = peek(packet, x);
    if (!ret.first) { return false; }
    packet.forward(ret.second); 
    return true;
}

// template<std::size_t K, typename BufIdx>
// bool get(Packet<K, BufIdx>& packet, MType& x)
// {
//     auto 
// }

template<typename T>
struct CorrespondingMT
{
    constexpr static MType mtype() { return MType::Unknown; }
};

template<>
struct CorrespondingMT<ShutDown>
{
    constexpr static MType mtype() { return MType::ShutDown; }
};

template<>
struct CorrespondingMT<LogOn>
{
    constexpr static MType mtype() { return MType::LogOn; }
};

template<>
struct CorrespondingMT<LogOut>
{
    constexpr static MType mtype() { return MType::LogOut; }
};

template<>
struct CorrespondingMT<QuoteUpdate>
{
    constexpr static MType mtype() { return MType::QuoteUpdate; }
};

template<>
struct CorrespondingMT<Acknowledgement>
{
    constexpr static MType mtype() { return MType::Acknowledgement; }
};

template<>
struct CorrespondingMT<HeartBeat>
{
    constexpr static MType mtype() { return MType::HeartBeat; }
};

std::size_t requiredSize(MType mt)
{
    switch(mt)
    {
    case MType::ShutDown:
        return SizeT<ShutDown>::value();
    case MType::LogOn:
        return SizeT<LogOn>::value();
    case MType::LogOut:
        return SizeT<LogOut>::value();
    case MType::QuoteUpdate:
        return SizeT<QuoteUpdate>::value();
    case MType::Acknowledgement:
        return SizeT<Acknowledgement>::value();
    case MType::HeartBeat:
        return SizeT<HeartBeat>::value();
    case MType::Unknown:
    default:
        return 0;
    }
}

template<std::size_t K, typename BufIdx, typename T>
inline bool putM(Packet<K, BufIdx>& packet, const T& x)
{
    MType mt = CorrespondingMT<T>::mtype();
    if (mt == MType::Unknown) { return false; }

    if (packet.writableSize() < SizeT<MType>::value() + SizeT<T>::value())
    {
        return false;
    }
    put(packet, mt);
    put(packet, x);
    return true;
}

struct PeekMRes
{
    MType mt;
    bool complete;
};

template<std::size_t K, typename BufIdx>
inline std::optional<PeekMRes> peekM(Packet<K, BufIdx>& packet)
{
    MType mt;
    if (!peek(packet, mt).first) { return std::optional<PeekMRes>(); }

    auto size = requiredSize(mt);
    if (size == 0) { return std::optional<PeekMRes>(); }

    auto complete = packet.readableSize() >= SizeT<MType>::value() + size;
    return std::optional<PeekMRes>({mt, complete});
}


// Processor is an abstract class that provides interfaces to the business
// logic of any server or client processes. 
//
// Ideally it doesn't involve any network I/O itself and can be used as the 
// backbone for business logic unit test. However it is not entirely possible
// to completely decouple from I/O. For instance, a server that receives a log
// on and then starts sending heart beat message every one minute can't be 
// decoupled from the I/O. In other words, the function is innately of type
// onMessage :: IO Request -> IO Response or
// onMessage' :: Request -> IO Response
// very often, no response are needed, therefore :: Request -> IO ()
//
// The component that manages IO should reside within this class so that
// if swapped out for an identity/dummy IO component, it can be used for tests

// template<typename T>
// class Processor
// {
// public:
//     template<typename M, std::size_t K, typename BufIdx>
//     void onMessage(const M& msg, Packet<K, BufIdx>& output)
//     {
//         T& self = static_cast<T&>(*this);
//         self.onMessage(msg, output);
//     }
//     
//     template<std::size_t K, typename BufIdx>
//     virtual void onMessage(const LogOn& msg, Packet<K, BufIdx>& output)
//     {
//         T& self = static_cast<T&>(*this);
//         self.onMessage(msg, output);
//     }
// 
//     template<std::size_t K, typename BufIdx>
//     virtual void onMessage(const LogOut& msg, Packet<K, BufIdx>& output)
//     {
//         T& self = static_cast<T&>(*this);
//         self.onMessage(msg, output);
//     }
// 
//     template<std::size_t K, typename BufIdx>
//     virtual void onMessage(const QuoteUpdate& msg, Packet<K, BufIdx>& output)
//     {
//         T& self = static_cast<T&>(*this);
//         self.onMessage(
//     }
// 
//     template<std::size_t K, typename BufIdx>
//     virtual void onMessage(const Acknowledgement& msg, Packet<K, BufIdx>& output);
//     template<std::size_t K, typename BufIdx>
//     virtual void onMessage(const HeartBeat& msg, Packet<K, BufIdx>& output);
// };


enum class ProcMRes : uint8_t
{
    Error = 0,
    Imcomplete,
    Exhausted
};

inline std::string toString(ProcMRes obj)
{
    switch(obj)
    {
    case ProcMRes::Error: return "Error";
    case ProcMRes::Imcomplete: return "Imcomplete";
    case ProcMRes::Exhausted: return "Exhausted";
    default: return "?";
    }
}

inline std::ostream& operator<<(std::ostream& os, ProcMRes x)
{
    os << "ProcMRes(" << fromEnum(x) << ")::" << toString(x);
    return os;
}

// this template acts as the interface of the MProc class, therefore
// there is no absolute need to create an interface for it, unless
// for more readable compliation error messages
template<
    std::size_t KIn, typename BufIdxIn, 
    std::size_t KOut, typename BufIdxOut,
    typename MProc
>
ProcMRes procM(
    Packet<KIn, BufIdxIn>& packetIn, 
    Packet<KOut, BufIdxOut>& packetOut, 
    MProc& mp
    )
{
    while (packetIn.readableSize() > 0)
    {
        auto res = peekM(packetIn);
        if (!res) { return ProcMRes::Error; }
        else if (!res->complete) { return ProcMRes::Imcomplete; }
        else // res && res->complete
        {
            packetIn.discard(SizeT<MType>::value());
            switch(res->mt)
            {
            case MType::ShutDown:
                {
                    ShutDown x;
                    get(packetIn, x);
                    mp.onMessage(x, packetOut);
                }
                break;
            case MType::LogOn:
                {
                    LogOn x;
                    get(packetIn, x);
                    mp.onMessage(x, packetOut);
                }
                break;
            case MType::LogOut:
                {
                    LogOut x;
                    get(packetIn, x);
                    mp.onMessage(x, packetOut);
                }
                break;
            case MType::QuoteUpdate:
                {
                    QuoteUpdate x;
                    get(packetIn, x);
                    mp.onMessage(x, packetOut);
                }
                break;
            case MType::Acknowledgement:
                {
                    Acknowledgement x;
                    get(packetIn, x);
                    mp.onMessage(x, packetOut);
                }
                break;
            case MType::HeartBeat:
                {
                    HeartBeat x;
                    get(packetIn, x);
                    mp.onMessage(x, packetOut);
                }
                break;
            case MType::Unknown:
            default:
                break;
            }
        }
    }
    return ProcMRes::Exhausted;
}

#endif
