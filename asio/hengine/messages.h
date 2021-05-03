#ifndef _INCLUDED_MESSAGES_H_
#define _INCLUDED_MESSAGES_H_

#include <cstdint>
#include "bytearray.h"
#include "packing.h"

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

    // customized structures
    ShutDown,
    LogOn,
    LogOut
};

inline std::underlying_type<MType>::type fromMType(MType mt)
{
    return static_cast<std::underlying_type<MType>::type>(mt);
}


struct ShutDown
{
    ByteArray<32> from;
    ByteArray<32> reason;
};

struct LogOn
{
    ByteArray<32> from;
};

struct LogOut
{
    ByteArray<32> from;
    ByteArray<32> reason;
};

template<std::size_t K, typename BufIdx>
bool put(Packet<K, BufIdx>& packet, int8_t x)
{
    if (packet.freeSpace() < 1 + sizeof(int8_t)) { return false; }
    packet.put(fromMType(MType::Int8));
    packet.put(x);
    return true;
}

template<std::size_t K, typename BufIdx>
bool put(Packet<K, BufIdx>& packet, uint8_t x)
{
    if (packet.freeSpace() < 1 + sizeof(uint8_t)) { return false; }
    packet.put(fromMType(MType::UInt8));
    packet.put(x);
    return true;
}

template<std::size_t K, typename BufIdx>
bool put(Packet<K, BufIdx>& packet, int16_t x)
{
    if (packet.freeSpace() < 1 + sizeof(int16_t)) { return false; }
    packet.put(fromMType(MType::Int16));
    packet.put(x);
    return true;
}

template<std::size_t K, typename BufIdx>
bool put(Packet<K, BufIdx>& packet, uint16_t x)
{
    if (packet.freeSpace() < 1 + sizeof(uint16_t)) { return false; }
    packet.put(fromMType(MType::UInt16));
    packet.put(x);
    return true;
}

template<std::size_t K, typename BufIdx>
bool put(Packet<K, BufIdx>& packet, int32_t x)
{
    if (packet.freeSpace() < 1 + sizeof(int32_t)) { return false; }
    packet.put(fromMType(MType::Int32));
    packet.put(x);
    return true;
}

template<std::size_t K, typename BufIdx>
bool put(Packet<K, BufIdx>& packet, uint32_t x)
{
    if (packet.freeSpace() < 1 + sizeof(uint32_t)) { return false; }
    packet.put(fromMType(MType::UInt32));
    packet.put(x);
    return true;
}

template<std::size_t K, typename BufIdx>
bool put(Packet<K, BufIdx>& packet, int64_t x)
{
    if (packet.freeSpace() < 1 + sizeof(int64_t)) { return false; }
    packet.put(fromMType(MType::Int64));
    packet.put(x);
    return true;
}

template<std::size_t K, typename BufIdx>
bool put(Packet<K, BufIdx>& packet, uint64_t x)
{
    if (packet.freeSpace() < 1 + sizeof(uint64_t)) { return false; }
    packet.put(fromMType(MType::UInt64));
    packet.put(x);
    return true;
}

template<std::size_t K, typename BufIdx>
bool put(Packet<K, BufIdx>& packet, float x)
{
    if (packet.freeSpace() < 1 + sizeof(float)) { return false; }
    packet.put(fromMType(MType::Float32));
    packet.put(x);
    return true;
}

template<std::size_t K, typename BufIdx>
bool put(Packet<K, BufIdx>& packet, double x)
{
    if (packet.freeSpace() < 1 + sizeof(double)) { return false; }
    packet.put(fromMType(MType::Float64));
    packet.put(x);
    return true;
}

#endif
