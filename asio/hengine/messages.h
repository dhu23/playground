#ifndef _INCLUDED_MESSAGES_H_
#define _INCLUDED_MESSAGES_H_

#include <cstdint>
#include "bytearray.h"
#include "packing.h"
#include "buffer.h"
#include "templ.h"

enum class MType : uint8_t
{
    Unknown = 0,

    // for native C types with known size
    Int8,
    UInt8,
    Int16,
    UInt16,
    Int32,
    UInt32,
    Int64,
    UInt64,
    Float32,
    Float64,
    Boolean,

    // customized structures
    ShutDown,
    LogOn,
    LogOut,
    QuoteUpdate,
    Acknowledgement,
    HeartBeat
};

inline std::underlying_type<MType>::type fromMType(MType mt)
{
    return static_cast<std::underlying_type<MType>::type>(mt);
}

inline std::string toString(MType obj)
{
    switch(obj)
    {
    case MType::Unknown:
        return "Unknown";
    case MType::Int8: 
        return "Int8";
    case MType::UInt8:
        return "UInt8";
    case MType::Int16:
        return "Int16";
    case MType::UInt16:
        return "UInt16";
    case MType::Int32: 
        return "Int32";
    case MType::UInt32: 
        return "UInt32";
    case MType::Int64: 
        return "Int64";
    case MType::UInt64:
        return "UInt64";
    case MType::Float32:
        return "Float32";
    case MType::Float64:
        return "Float64";
    case MType::Boolean:
        return "Boolean";
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
    os << "MType(" << fromMType(obj) << ")::" << toString(obj);
    return os;
}

struct ShutDown
{
    ByteArray<32> from;
    ByteArray<32> reason;

    bool operator==(const ShutDown& other) const
    {
        return equalTs(from, other.from, reason, other.reason);
    }

    std::ostream& print(std::ostream& os) const
    {
        os << "ShutDown[from=" << from << ",reason=" << reason << "]";
        return os;
    }
};

inline std::ostream& operator<<(std::ostream& os, const ShutDown& obj)
{
    return obj.print(os);
}

struct LogOn
{
    ByteArray<32> from;

    bool operator==(const LogOn& other) const
    {
        return equalTs(from, other.from);
    }

    std::ostream& print(std::ostream& os) const
    {
        os << "LogOn[from=" << from << "]";
        return os;
    }
};

inline std::ostream& operator<<(std::ostream& os, const LogOn& obj)
{
    return obj.print(os);
}

struct LogOut
{
    ByteArray<32> from;
    ByteArray<32> reason;

    bool operator==(const LogOut& other) const
    {
        return equalTs(from, other.from, reason, other.reason);
    }

    std::ostream& print(std::ostream& os) const
    {
        os << "LogOut[from=" << from << ",reason=" << reason << "]";
        return os;
    }
};

inline std::ostream& operator<<(std::ostream& os, const LogOut& obj)
{
    return obj.print(os);
}

struct QuoteUpdate
{
    uint32_t quantity;
    uint64_t price;
    ByteArray<8> ticker;

    bool operator==(const QuoteUpdate& other) const
    {
        return equalTs(
            quantity, other.quantity, 
            price, other.price, 
            ticker, other.ticker);
    }

    std::ostream& print(std::ostream& os) const
    {
        os 
            << "QuoteUpdate[quantity=" << quantity << ",price=" << price 
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
    ByteArray<32> from;
    MType received;

    bool operator==(const Acknowledgement& other) const
    {
        return equalTs(from, other.from, received, other.received);
    }

    std::ostream& print(std::ostream& os) const
    {
        os << "Acknowledgement[from=" << from << ",received=" << received;
        return os;
    }
};

inline std::ostream& operator<<(std::ostream& os, const Acknowledgement& obj)
{
    return obj.print(os);
}

struct HeartBeat
{
    ByteArray<32> from;

    bool operator==(const HeartBeat& other) const
    {
        return equalTs(from, other.from);
    }

    std::ostream& print(std::ostream& os) const
    {
        os << "HeartBeat[from=" << from << "]"; 
        return os;
    }
};

inline std::ostream& operator<<(std::ostream& os, const HeartBeat& obj)
{
    return obj.print(os);
}

/////////////////////// sizeT functions ////////////////////////////
template<typename T>
inline std::size_t sizeT() { return sizeof(T); }

// template<typename T, typename... Ts>
// inline std::size_t sizeT() { return sizeT<T>() + sizeT<Ts...>(); }

template<>
inline std::size_t sizeT<MType>()
{
    return sizeof(std::underlying_type<MType>::type);
}

template<>
inline std::size_t sizeT<ShutDown>()
{
    return 
        sizeT<decltype(ShutDown::from)>() + 
        sizeT<decltype(ShutDown::reason)>();
}
 
template<>
inline std::size_t sizeT<LogOn>()
{
    return sizeT<decltype(LogOn::from)>();
}

template<>
inline std::size_t sizeT<LogOut>()
{
    return 
        sizeT<decltype(LogOut::from)>() + 
        sizeT<decltype(LogOut::reason)>();
}

template<>
inline std::size_t sizeT<QuoteUpdate>()
{
    return 
        sizeT<decltype(QuoteUpdate::quantity)>() +
        sizeT<decltype(QuoteUpdate::price)>() + 
        sizeT<decltype(QuoteUpdate::ticker)>();
}

template<>
inline std::size_t sizeT<Acknowledgement>()
{
    return 
        sizeT<decltype(Acknowledgement::from)>() +
        sizeT<decltype(Acknowledgement::received)>();
}

template<>
inline std::size_t sizeT<HeartBeat>()
{
    return sizeT<decltype(HeartBeat::from)>();
}

std::size_t requiredSize(MType mt)
{
    switch(mt)
    {
    case MType::Int8:
        return sizeT<int8_t>();
    case MType::UInt8:
        return sizeT<uint8_t>();
    case MType::Int16:
        return sizeT<int16_t>();
    case MType::UInt16:
        return sizeT<uint16_t>();
    case MType::Int32:
        return sizeT<int32_t>();
    case MType::UInt32:
        return sizeT<uint32_t>();
    case MType::Int64:
        return sizeT<int64_t>();
    case MType::UInt64:
        return sizeT<uint64_t>();
    case MType::Float32:
        return sizeT<float>();
    case MType::Float64:
        return sizeT<double>();
    case MType::Boolean:
        return sizeT<bool>();
    case MType::ShutDown:
        return sizeT<ShutDown>();
    case MType::LogOn:
        return sizeT<LogOn>();
    case MType::LogOut:
        return sizeT<LogOut>();
    case MType::QuoteUpdate:
        return sizeT<QuoteUpdate>();
    case MType::Acknowledgement:
        return sizeT<Acknowledgement>();
    case MType::HeartBeat:
        return sizeT<HeartBeat>();
    case MType::Unknown:
    default:
        return 0;
    };
}

///////////////// helper functions for packing/unpacking /////////////////
template<std::size_t K, typename BufIdx, typename T>
inline void putT(Packet<K, BufIdx>& packet, T&& t)
{
    packet.put(t);
}

template<std::size_t K, typename BufIdx, typename T, typename... Ts>
inline void putT(Packet<K, BufIdx>& packet, T&& t, Ts&&... ts)
{
    packet.put(t);
    putT(packet, ts...);
}

template<std::size_t K, typename BufIdx, typename T>
inline void getT(Packet<K, BufIdx>& packet, T&& t)
{
    packet.get(t);
}

template<std::size_t K, typename BufIdx, typename T, typename... Ts>
inline void getT(Packet<K, BufIdx>& packet, T&& t, Ts&&... ts)
{
    packet.get(t);
    getT(packet, ts...);
}

template<std::size_t K, typename BufIdx>
inline bool peekM(Packet<K, BufIdx>& packet)
{
    std::underlying_type<MType>::type mt;
    if (!packet.peek(mt)) { return false; }

    std::size_t readsize = requiredSize(MType(mt));
    if (readsize == 0) { return false; }

    if (packet.readableSize() < 1+readsize) { return false; }
    return true;
}

//////////////////////// put and get functions ////////////////////////////
template<std::size_t K, typename BufIdx>
bool get(Packet<K, BufIdx>& packet, MType& x)
{
    if (packet.readableSize() < sizeT<MType>()) { return false; }
    std::underlying_type<MType>::type val;
    getT(packet, val);
    x = MType { val };
    return true;
}

template<std::size_t K, typename BufIdx>
bool put(Packet<K, BufIdx>& packet, int8_t x)
{
    if (packet.writableSize() < 1 + sizeT<int8_t>()) { return false; }
    putT(packet, fromMType(MType::Int8), x);
    return true;
}

template<std::size_t K, typename BufIdx>
bool get(Packet<K, BufIdx>& packet, int8_t& x)
{
    if (packet.readableSize() < sizeT<int8_t>()) { return false; }
    getT(packet, x);
    return true;
}

template<std::size_t K, typename BufIdx>
bool put(Packet<K, BufIdx>& packet, uint8_t x)
{
    if (packet.writableSize() < 1 + sizeT<uint8_t>()) { return false; }
    putT(packet, fromMType(MType::UInt8), x);
    return true;
}

template<std::size_t K, typename BufIdx>
bool get(Packet<K, BufIdx>& packet, uint8_t& x)
{
    if (packet.readableSize() < sizeT<uint8_t>()) { return false; }
    getT(packet, x);
    return true;
}

template<std::size_t K, typename BufIdx>
bool put(Packet<K, BufIdx>& packet, int16_t x)
{
    if (packet.writableSize() < 1 + sizeT<int16_t>()) { return false; }
    putT(packet, fromMType(MType::Int16), x);
    return true;
}

template<std::size_t K, typename BufIdx>
bool get(Packet<K, BufIdx>& packet, uint16_t& x)
{
    if (packet.readableSize() < sizeT<uint16_t>()) { return false; }
    getT(packet, x);
    return true;
}

template<std::size_t K, typename BufIdx>
bool put(Packet<K, BufIdx>& packet, int32_t x)
{
    if (packet.writableSize() < 1 + sizeT<int32_t>()) { return false; }
    putT(packet, fromMType(MType::Int32), x);
    return true;
}

template<std::size_t K, typename BufIdx>
bool get(Packet<K, BufIdx>& packet, uint32_t& x)
{
    if (packet.readableSize() < sizeT<uint32_t>()) { return false; }
    getT(packet, x);
    return true;
}

template<std::size_t K, typename BufIdx>
bool put(Packet<K, BufIdx>& packet, int64_t x)
{
    if (packet.writableSize() < 1 + sizeT<int64_t>()) { return false; }
    putT(packet, fromMType(MType::Int64), x);
    return true;
}

template<std::size_t K, typename BufIdx>
bool get(Packet<K, BufIdx>& packet, uint64_t& x)
{
    if (packet.readableSize() < sizeT<uint64_t>()) { return false; }
    getT(packet, x);
    return true;
}

template<std::size_t K, typename BufIdx>
bool put(Packet<K, BufIdx>& packet, float x)
{
    if (packet.writableSize() < 1 + sizeT<float>()) { return false; }
    putT(packet, fromMType(MType::Float32), x);
    return true;
}

template<std::size_t K, typename BufIdx>
bool get(Packet<K, BufIdx>& packet, float& x)
{
    if (packet.readableSize() < sizeT<float>()) { return false; }
    getT(packet, x);
    return true;
}

template<std::size_t K, typename BufIdx>
bool put(Packet<K, BufIdx>& packet, double x)
{
    if (packet.writableSize() < 1 + sizeT<double>()) { return false; }
    putT(packet, fromMType(MType::Float64), x);
    return true;
}

template<std::size_t K, typename BufIdx>
bool get(Packet<K, BufIdx>& packet, double& x)
{
    if (packet.readableSize() < sizeT<double>()) { return false; }
    getT(packet, x);
    return true;
}

template<std::size_t K, typename BufIdx>
bool put(Packet<K, BufIdx>& packet, bool x)
{
    if (packet.writableSize() < 1 + sizeT<bool>()) { return false; }
    putT(packet, fromMType(MType::Boolean), x);
    return true;
}

template<std::size_t K, typename BufIdx>
bool get(Packet<K, BufIdx>& packet, bool& x)
{
    if (packet.readableSize() < sizeT<bool>()) { return false; }
    getT(packet, x);
    return true;
}

template<std::size_t K, typename BufIdx>
bool put(Packet<K, BufIdx>& packet, const ShutDown& x)
{
    if (packet.writableSize() < 1 + sizeT<ShutDown>()) { return false; }
    putT(packet, fromMType(MType::ShutDown), x.from, x.reason);
    return true;
}

template<std::size_t K, typename BufIdx>
bool get(Packet<K, BufIdx>& packet, ShutDown& x)
{
    if (packet.readableSize() < sizeT<ShutDown>()) { return false; }
    getT(packet, x.from, x.reason);
    return true;
}

template<std::size_t K, typename BufIdx>
bool put(Packet<K, BufIdx>& packet, const LogOn& x)
{
    if (packet.writableSize() < 1 + sizeT<LogOn>()) { return false; }
    putT(packet, fromMType(MType::LogOn), x.from);
    return true;
}

template<std::size_t K, typename BufIdx>
bool get(Packet<K, BufIdx>& packet, LogOn& x)
{
    if (packet.readableSize() < sizeT<LogOn>()) { return false; }
    getT(packet, x.from);
    return true;
}

template<std::size_t K, typename BufIdx>
bool put(Packet<K, BufIdx>& packet, const LogOut& x)
{
    if (packet.writableSize() < 1 + sizeT<LogOut>()) { return false; }
    putT(packet, fromMType(MType::LogOut), x.from, x.reason);
    return true;
}

template<std::size_t K, typename BufIdx>
bool get(Packet<K, BufIdx>& packet, LogOut& x)
{
    if (packet.readableSize() < sizeT<LogOut>()) { return false; }
    getT(packet, x.from, x.reason);
    return true;
}

template<std::size_t K, typename BufIdx>
bool put(Packet<K, BufIdx>& packet, const QuoteUpdate& x)
{
    if (packet.writableSize() < 1 + sizeT<QuoteUpdate>()) { return false; }
    putT(packet, fromMType(MType::QuoteUpdate), x.quantity, x.price, x.ticker);
    return true;
}

template<std::size_t K, typename BufIdx>
bool get(Packet<K, BufIdx>& packet, QuoteUpdate& x)
{
    if (packet.readableSize() < sizeT<QuoteUpdate>()) { return false; }
    getT(packet, x.quantity, x.price, x.ticker);
    return true;
}

template<std::size_t K, typename BufIdx>
bool put(Packet<K, BufIdx>& packet, const Acknowledgement& x)
{
    if (packet.writableSize() < 1 + sizeT<Acknowledgement>()) { return false; }
    putT(packet, fromMType(MType::Acknowledgement), x.from, x.received);
    return true;
}

template<std::size_t K, typename BufIdx>
bool get(Packet<K, BufIdx>& packet, Acknowledgement& x)
{
    if (packet.readableSize() < sizeT<Acknowledgement>()) { return false; }
    getT(packet, x.from, x.received);
    return true;
}

template<std::size_t K, typename BufIdx>
bool put(Packet<K, BufIdx>& packet, const HeartBeat& x)
{
    if (packet.writableSize() < 1 + sizeT<HeartBeat>()) { return false; }
    putT(packet, fromMType(MType::HeartBeat), x.from);
    return true;
}

template<std::size_t K, typename BufIdx>
bool get(Packet<K, BufIdx>& packet, HeartBeat& x)
{
    if (packet.readableSize() < sizeT<HeartBeat>()) { return false; }
    getT(packet, x.from);
    return true;
}

#endif
