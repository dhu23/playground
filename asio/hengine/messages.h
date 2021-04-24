#ifndef _INCLUDED_MESSAGES_H_
#define _INCLUDED_MESSAGES_H_

#include <cstdint>
#include "bytearray.h"

enum class MType : char
{
    Unknown = 0,

    // for native C types with known size
    Int8,
    Int16,
    Int32,
    Int64,
    Float32,
    Float64,

    // customized structures
    ShutDown,
    LogOn,
    LogOut
};

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

// packing and unpacking code copied over from socke/beej/pack2.c

#endif
