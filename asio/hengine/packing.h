// should we be using one-sized packing buffer to hold all the out-going
// and in-coming messages or should we make accurate size? For instance,
// if we have two messages, A and B, and size of A is 24 and size of B is
// 256. Should we use a buffer size 256 for sending messages or should 
// we be sending two types of messages, one 24 and one 256? Or should we
// be somewhere in the middle? 

// #include <ctype.h>
#include <cstring>
#include <cstdint> // defines int*_t, uint*_t types
#include <array>
#include <iostream>

#include "bytearray.h"

struct Packing
{
    template<typename T>
    static void binrep(T x, uint64_t n)
    {
        std::ostringstream oss;
        for (T i = 1<<(n-1); i > 0; i /= 2)
        {
            if (x & i) { oss << "1"; }
            else { oss << "0"; }
        }
        std::cout << oss.str() << std::endl;
    }

    static uint64_t pack754(double f, int32_t bits, int32_t expbits)
    {
        double fnorm;
        int32_t shift;
        int64_t sign, exp, significand;
        uint32_t significandbits = bits - expbits - 1; // -1 for sign bit

        if (f == 0.0) return 0; // get this special case out of the way

        // check sign and begin normalization
        if (f < 0) { sign = 1; fnorm = -f; }
        else { sign = 0; fnorm = f; }

        // get the normalized form of f and track the exponent
        shift = 0;
        while (fnorm >= 2.0) { fnorm /= 2.0; shift++; }
        while (fnorm < 1.0) { fnorm *= 2.0; shift--; }
        fnorm = fnorm - 1.0;

        // calculate the binary form (non-float) of the significand data
        significand = fnorm * ((1LL<<significandbits) + 0.5f);

        // get the biased exponent
        exp = shift + ((1<<(expbits-1)) - 1); // shift + bias

        return (sign<<(bits-1)) | (exp<<(bits-expbits-1)) | significand;
    }

    static double unpack754(uint64_t i, int32_t bits, int32_t expbits)
    {
        double result;
        int64_t shift;
        uint32_t bias;
        uint32_t significandbits = bits - expbits - 1; // -1 for sign bit

        if (i == 0) return 0.0;

        // pull the significand
        result = (i&((1LL<<significandbits)-1)); // mask
        result /= (1LL<<significandbits); // convert back to float
        result += 1.0f; //add the one back on

        // deal with the exponent. meaning that 0 is not represented as all zeros
        bias = (1<<(expbits-1))-1;
        shift = ((i>>significandbits)&((1LL<<expbits)-1)) - bias;
        while (shift > 0) { result *= 2.0; shift--; }
        while (shift < 0) { result /= 2.0; shift++; }

        // sign it
        result *= (i>>(bits-1))&1 ? -1.0 : 1.0;

        return result;
    }

};


template<std::size_t K, typename BufIdx>
class Packet
{
    // doesn't look like we need this
    // static_assert(sizeof(char) == sizeof(int8_t));
    // static_assert(sizeof(unsigned char) == sizeof(uint8_t));
    // static_assert(sizeof(short) == sizeof(int16_t));
    // static_assert(sizeof(unsigned short) == sizeof(uint16_t));
    // static_assert(sizeof(int) == sizeof(int32_t));
    // static_assert(sizeof(unsigned int) == sizeof(uint32_t));
    // static_assert(sizeof(long) == sizeof(int64_t));
    // static_assert(sizeof(unsigned long) == sizeof(uint64_t));
    // static_assert(sizeof(long long) == sizeof(int64_t));
    // static_assert(sizeof(unsigned long long) == sizeof(uint64_t));

    BufIdx bufferIdx_;
    std::array<uint8_t, K> buffer_;
    // std::size_t head_; // indicates the readable location
    // std::size_t tail_; // indicates the writable location

    // no error check is done in this function. caller needs to make sure
    void writeByte(uint8_t x)
    {
        buffer_[bufferIdx_.tail()] = x;
        bufferIdx_.write(1);
    }

    uint8_t peekByte(std::size_t offset) const 
    { 
        return buffer_[bufferIdx_.head()+offset]; 
    }

public:
    Packet():
        bufferIdx_(K),
        buffer_()
    {}

    size_t writableSize() const { return bufferIdx_.freeSpace(); }
    size_t readableSize() const { return bufferIdx_.usedSpace(); }

    bool put(uint8_t u8);
    bool put(uint16_t u16);
    bool put(uint32_t u32);
    bool put(uint64_t u64);

    bool put(int8_t i8);
    bool put(int16_t i16);
    bool put(int32_t i32);
    bool put(int64_t i64);

    bool put(float f32);
    bool put(double f64);

    bool put(bool b);

    template<std::size_t N>
    bool put(const ByteArray<N>& obj)
    {
        // len takes an unsigned short
        if (2+N > this->writableSize()) { return false; }

        // this function should not fail from here
        if (!this->put(static_cast<uint16_t>(N))) { return false; }
        memcpy(&buffer_[bufferIdx_.tail()], obj.data(), N);
        bufferIdx_.write(N);
        return true;
    }
    
    // the followings are not needed
    // bool put(char c);
    // bool put(unsigned char c);
    // bool put(short s);
    // bool put(unsigned short s);
    // bool put(int i);
    // bool put(unsigned int i);
    // bool put(long l);
    // bool put(unsigned long l);
    // bool put(long long ll);
    // bool put(unsigned long long ll);
    
    bool peek(uint8_t& u8);
    bool peek(uint16_t& u16);
    bool peek(uint32_t& u32);
    bool peek(uint64_t& u64);
    
    bool peek(int8_t& i8);
    bool peek(int16_t& i16);
    bool peek(int32_t& i32);
    bool peek(int64_t& i64);

    bool peek(float& f32);
    bool peek(double& f64);

    bool peek(bool& b);

private:
    template<typename T>
    bool getT(T& x)
    {
        if (!this->peek(x)) { return false; }
        bufferIdx_.read(sizeof(T));
        return true;
    }

public:

    bool get(uint8_t& u8) { return getT(u8); }
    bool get(uint16_t& u16) { return getT(u16); }
    bool get(uint32_t& u32) { return getT(u32); }
    bool get(uint64_t& u64) { return getT(u64); }
    
    bool get(int8_t& i8) { return getT(i8); }
    bool get(int16_t& i16) { return getT(i16); }
    bool get(int32_t& i32) { return getT(i32); }
    bool get(int64_t& i64) { return getT(i64); }

    bool get(float& f32) { return getT(f32); }
    bool get(double& f64) { return getT(f64); }

    bool get(bool& b) { return getT(b); }

    template<std::size_t N>
    bool get(ByteArray<N>& obj)
    {
        if (2+N > this->readableSize()) { return false; }
        uint16_t size;
        if (!this->peek(size)) { return false; }
        if (size != N) { return false; }
        bufferIdx_.read(sizeof(uint16_t));
        obj.fromArray(reinterpret_cast<char*>(&buffer_[bufferIdx_.head()]), size);
        bufferIdx_.read(N);
        return true;
    }

    // the followings are not needed
    // bool get(char& c);
    // bool get(unsigned char& c);
    // bool get(short& s);
    // bool get(unsigned short& s);
    // bool get(int& i);
    // bool get(unsigned int& i);
    // bool get(long& l);
    // bool get(unsigned long& l);
    // bool get(long long& ll);
    // bool get(unsigned long long& ll);
    
};

template<std::size_t K, typename BufIdx>
bool Packet<K, BufIdx>::put(uint8_t u8)
{
    if (sizeof(uint8_t) > this->writableSize()) { return false; }
    this->writeByte(u8);
    return true;
}

template<std::size_t K, typename BufIdx>
bool Packet<K, BufIdx>::put(uint16_t u16)
{
    if (sizeof(uint16_t) > this->writableSize()) { return false; }
    this->writeByte(u16>>8);
    this->writeByte(u16);
    return true;
}

template<std::size_t K, typename BufIdx>
bool Packet<K, BufIdx>::put(uint32_t u32)
{
    if (sizeof(uint32_t) > this->writableSize()) { return false; }
    this->writeByte(u32>>24);
    this->writeByte(u32>>16);
    this->writeByte(u32>>8);
    this->writeByte(u32);
    return true;
}

template<std::size_t K, typename BufIdx>
bool Packet<K, BufIdx>::put(uint64_t u64)
{
    if (sizeof(uint64_t) > this->writableSize()) { return false; }
    this->writeByte(u64>>56);
    this->writeByte(u64>>48);
    this->writeByte(u64>>40);
    this->writeByte(u64>>32);
    this->writeByte(u64>>24);
    this->writeByte(u64>>16);
    this->writeByte(u64>>8);
    this->writeByte(u64);
    return true;
}

template<std::size_t K, typename BufIdx>
bool Packet<K, BufIdx>::put(int8_t i8)
{
    return this->put(static_cast<uint8_t>(i8));
}

template<std::size_t K, typename BufIdx>
bool Packet<K, BufIdx>::put(int16_t i16)
{
    return this->put(static_cast<uint16_t>(i16));
}

template<std::size_t K, typename BufIdx>
bool Packet<K, BufIdx>::put(int32_t i32)
{
    return this->put(static_cast<uint32_t>(i32));
}

template<std::size_t K, typename BufIdx>
bool Packet<K, BufIdx>::put(int64_t i64)
{
    return this->put(static_cast<uint64_t>(i64));
}

template<std::size_t K, typename BufIdx>
bool Packet<K, BufIdx>::put(float f32)
{
    if (sizeof(float) > this->writableSize()) { return false; }
    auto fhold = static_cast<uint32_t>(Packing::pack754(f32, 32, 8));
    return this->put(fhold);
}

template<std::size_t K, typename BufIdx>
bool Packet<K, BufIdx>::put(double f64)
{
    if (sizeof(double) > this->writableSize()) { return false; }
    auto fhold = static_cast<uint64_t>(Packing::pack754(f64, 64, 11));
    return this->put(fhold);
}

template<std::size_t K, typename BufIdx>
bool Packet<K, BufIdx>::put(bool b)
{
    return this->put(static_cast<uint8_t>(b ? 1 : 0));
}

// template<std::size_t K, typename BufIdx>
// bool Packet<K, BufIdx>::put(char c)
// {
//     return this->put(static_cast<int8_t>(c));
// }
// 
// template<std::size_t K, typename BufIdx>
// bool Packet<K, BufIdx>::put(unsigned char c)
// {
//     return this->put(static_cast<uint8_t>(c));
// }
// 
// template<std::size_t K, typename BufIdx>
// bool Packet<K, BufIdx>::put(short s)
// {
//     return this->put(static_cast<int16_t>(s));
// }
// 
// template<std::size_t K, typename BufIdx>
// bool Packet<K, BufIdx>::put(unsigned short s)
// {
//     return this->put(static_cast<uint16_t>(s));
// }
// 
// template<std::size_t K, typename BufIdx>
// bool Packet<K, BufIdx>::put(int i)
// {
//     return this->put(static_cast<int32_t>(i));
// }
// 
// template<std::size_t K, typename BufIdx>
// bool Packet<K, BufIdx>::put(unsigned int i)
// {
//     return this->put(static_cast<uint32_t>(i));
// }
// 
// template<std::size_t K, typename BufIdx>
// bool Packet<K, BufIdx>::put(long l)
// {
//     return this->put(static_cast<int64_t>(l));
// }
// 
// template<std::size_t K, typename BufIdx>
// bool Packet<K, BufIdx>::put(unsigned long l)
// {
//     return this->put(static_cast<uint64_t>(l));
// }
// 
// template<std::size_t K, typename BufIdx>
// bool Packet<K, BufIdx>::put(long long ll)
// {
//     return this->put(static_cast<int64_t>(ll));
// }
// 
// template<std::size_t K, typename BufIdx>
// bool Packet<K, BufIdx>::put(unsigned long long i)
// {
//     return this->put(static_cast<int64_t>(ll));
// }

template<std::size_t K, typename BufIdx>
bool Packet<K, BufIdx>::peek(uint8_t& u8)
{
    if (sizeof(uint8_t) > this->readableSize()) { return false; }
    u8 = this->peekByte(0);
    return true;
}

template<std::size_t K, typename BufIdx>
bool Packet<K, BufIdx>::peek(uint16_t& u16)
{
    if (sizeof(uint16_t) > this->readableSize()) { return false; }
    u16 = (static_cast<uint16_t>(this->peekByte(0)) << 8) | this->peekByte(1);
    // u16 = static_cast<uint16_t>(buffer_[head_++]);
    // u16 <<= 8;
    // u16 |= buffer_[head_++];
    return true;
}

template<std::size_t K, typename BufIdx>
bool Packet<K, BufIdx>::peek(uint32_t& u32)
{
    if (sizeof(uint32_t) > this->readableSize()) { return false; }
    u32 = 
        (static_cast<uint32_t>(this->peekByte(0)) << 24) |
        (static_cast<uint32_t>(this->peekByte(1)) << 16) | 
        (static_cast<uint32_t>(this->peekByte(2)) << 8) | 
        static_cast<uint32_t>(this->peekByte(3));
    // u32 = static_cast<uint32_t>(buffer_[head_++]);
    // u32 <<= 8;
    // u32 |= buffer_[head_++];
    // u32 <<= 8;
    // u32 |= buffer_[head_++];
    // u32 <<= 8;
    // u32 |= buffer_[head_++];
    return true;
}

template<std::size_t K, typename BufIdx>
bool Packet<K, BufIdx>::peek(uint64_t& u64)
{
    if (sizeof(uint64_t) > this->readableSize()) { return false; }
    // u64 = static_cast<uint64_t>(buffer_[head_++]);
    // u64 <<= 8;
    // u64 |= buffer_[head_++];
    // u64 <<= 8;
    // u64 |= buffer_[head_++];
    // u64 <<= 8;
    // u64 |= buffer_[head_++];
    // u64 <<= 8;
    // u64 |= buffer_[head_++];
    // u64 <<= 8;
    // u64 |= buffer_[head_++];
    // u64 <<= 8;
    // u64 |= buffer_[head_++];
    // u64 <<= 8;
    // u64 |= buffer_[head_++];
    u64 = 
        (static_cast<uint64_t>(this->peekByte(0)) << 56) |
        (static_cast<uint64_t>(this->peekByte(1)) << 48) |
        (static_cast<uint64_t>(this->peekByte(2)) << 40) |
        (static_cast<uint64_t>(this->peekByte(3)) << 32) |
        (static_cast<uint64_t>(this->peekByte(4)) << 24) |
        (static_cast<uint64_t>(this->peekByte(5)) << 16) |
        (static_cast<uint64_t>(this->peekByte(6)) << 8) |
        static_cast<uint64_t>(this->peekByte(7));
    return true;
}

template<std::size_t K, typename BufIdx>
bool Packet<K, BufIdx>::peek(int8_t& i8)
{
    uint8_t i = 0;
    if (!this->peek(i)) { return false; }
    if (i <= 0x7fu) { i8 = i; }
    else { i8 = -1 - static_cast<int8_t>(0xffu-i); }
    return true;
}

template<std::size_t K, typename BufIdx>
bool Packet<K, BufIdx>::peek(int16_t& i16)
{
    uint16_t i = 0;
    if (!this->peek(i)) { return false; }
    if (i <= 0x7fffu) { i16 = i; }
    else { i16 = -1 - static_cast<int16_t>(0xffffu-i); }
    return true;
}

template<std::size_t K, typename BufIdx>
bool Packet<K, BufIdx>::peek(int32_t& i32)
{
    uint32_t i = 0;
    if (!this->peek(i)) { return false; }
    if (i <= 0x7fffffffu) { i32 = i; }
    else { i32 = -1 - static_cast<int32_t>(0xffffffffu-i); }
    return true;
}

template<std::size_t K, typename BufIdx>
bool Packet<K, BufIdx>::peek(int64_t& i64)
{
    uint64_t i = 0;
    if (!this->peek(i)) { return false; }
    if (i <= 0x7fffffffffffffffu) { i64 = i; }
    else { i64 = -1 - static_cast<int64_t>(0xffffffffffffffffu-i); }
    return true;
}

template<std::size_t K, typename BufIdx>
bool Packet<K, BufIdx>::peek(float& f32)
{
    if (sizeof(float) > this->readableSize()) { return false; }
    uint32_t i;
    if (!this->peek(i)) { return false; }
    f32 = Packing::unpack754(i, 32, 8);
    // binrep(i, 32);
    return true;
}

template<std::size_t K, typename BufIdx>
bool Packet<K, BufIdx>::peek(double& f64)
{
    if (sizeof(double) > this->readableSize()) { return false; }
    uint64_t i;
    if (!this->peek(i)) { return false; }
    f64 = Packing::unpack754(i, 64, 11);
    // binrep(i, 64);
    return true;
}

template<std::size_t K, typename BufIdx>
bool Packet<K, BufIdx>::peek(bool& b)
{
    uint8_t i = 0;
    if (!this->peek(i)) { return false; }
    b = i > 0;
    return true;
}

// template<std::size_t K, typename BufIdx>
// bool Packet<K, BufIdx>::get(char& c)
// {
//     auto* p = reinterpret_cast<int8_t*>(&c);
//     return this->get(*p);
// }
// 
// template<std::size_t K, typename BufIdx>
// bool Packet<K, BufIdx>::get(unsigned char& c)
// {
//     auto* p = reinterpret_cast<uint8_t*>(&c);
//     return this->get(*p);
// }
// 
// template<std::size_t K, typename BufIdx>
// bool Packet<K, BufIdx>::get(short& s)
// {
//     auto* p = reinterpret_cast<int16_t*>(&s);
//     return this->get(*p);
// }
// 
// template<std::size_t K, typename BufIdx>
// bool Packet<K, BufIdx>::get(unsigned short& c)
// {
//     auto* p = reinterpret_cast<uint16_t*>(&s);
//     return this->get(*p);
// }
// 
// template<std::size_t K, typename BufIdx>
// bool Packet<K, BufIdx>::get(int& i)
// {
//     auto* p = reinterpret_cast<int32_t*>(&i);
//     return this->get(*p);
// }
// 
// template<std::size_t K, typename BufIdx>
// bool Packet<K, BufIdx>::get(unsigned int& i)
// {
//     auto* p = reinterpret_cast<uint32_t*>(&i);
//     return this->get(*p);
// }
// 
// template<std::size_t K, typename BufIdx>
// bool Packet<K, BufIdx>::get(long& l)
// {
//     auto* p = reinterpret_cast<int64_t*>(&l);
//     return this->get(*p)
// }
// 
// template<std::size_t K, typename BufIdx>
// bool Packet<K, BufIdx>::get(unsigned long& l)
// {
//     auto* p = reinterpret_cast<uint64_t*>(&l);
//     return this->get(*p);
// }
// 
// template<std::size_t K, typename BufIdx>
// bool Packet<K, BufIdx>::get(long long& ll)
// {
//     auto* p = reinterpret_cast<int64_t*>(&ll);
//     return this->get(*p);
// }
// 
// template<std::size_t K, typename BufIdx>
// bool Packet<K, BufIdx>::get(unsigned long long& ll)
// {
//     auto* p = reinterpret_cast<uint64_t*>(&ll);
//     return this->get(*p);
// }

