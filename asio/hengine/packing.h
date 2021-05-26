#include <cstring>
#include <cstdint> // defines int*_t, uint*_t types
#include <array>
#include <iostream>

#include "bytearray.h"
#include "timestamp.h"

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

// sizeT template
template<typename T>
struct SizeT
{
    constexpr static std::size_t value() { return sizeof(T); }
};

// specialization for ByteArray<N> type
template<std::size_t N>
struct SizeT<ByteArray<N>>
{
    constexpr static std::size_t value() { return 2+N; }
};

template<>
struct SizeT<Timestamp>
{
    constexpr static std::size_t value() { return Timestamp::sizeT(); }
};

template<std::size_t K, typename BufIdx>
class Packet
{
    BufIdx bufferIdx_;
    std::array<uint8_t, K> buffer_;

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

    explicit operator bool() const { return bufferIdx_.isEmpty(); }
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

    bool put(Timestamp ts) { return this->put(ts.totalNanoseconds()); }

    bool put(const char* buf, std::size_t size)
    {
        if (size > this->writableSize()) { return false; }
        for (std::size_t i = 0; i < size; ++i) { this->writeByte(buf[i]); };
        return true;
    }
    
    bool peek(uint8_t& u8, std::size_t skip=0) const;
    bool peek(uint16_t& u16, std::size_t skip=0) const;
    bool peek(uint32_t& u32, std::size_t skip=0) const;
    bool peek(uint64_t& u64, std::size_t skip=0) const;
    
    bool peek(int8_t& i8, std::size_t skip=0) const;
    bool peek(int16_t& i16, std::size_t skip=0) const;
    bool peek(int32_t& i32, std::size_t skip=0) const;
    bool peek(int64_t& i64, std::size_t skip=0) const;

    bool peek(float& f32, std::size_t skip=0) const;
    bool peek(double& f64, std::size_t skip=0) const;

    bool peek(bool& b, std::size_t skip=0) const;

    template<std::size_t N>
    bool peek(ByteArray<N>& obj, std::size_t skip=0) const
    {
        if (SizeT<ByteArray<N>>::value()+skip > this->readableSize()) { return false; }
        uint16_t size;
        if (!this->peek(size, skip)) { return false; }
        if (size != N) { return false; }
        obj.fromBuffer(
            reinterpret_cast<const char*>(&buffer_[bufferIdx_.head()+sizeof(size)+skip]),
            size);
        return true;
    }

    bool peek(Timestamp& ts, std::size_t skip=0) const
    {
        if (SizeT<Timestamp>::value()+skip > this->readableSize()) { return false; }
        uint64_t nano;
        if (!this->peek(nano, skip)) { return false; }
        ts = Timestamp(nano);
        return true;
    }

    template<typename T>
    bool get(T& x)
    {
        if (!this->peek(x)) { return false; }
        bufferIdx_.read(SizeT<T>::value());
        return true;
    }

    bool discard(std::size_t skip)
    {
        if (this->readableSize() < skip) { return false; }
        bufferIdx_.read(skip);
        return true;
    }

public:
    class Peeker
    {
        std::size_t seen_;
        const Packet* p_;

        Peeker(const Packet* p): 
            seen_(0),
            p_(p)
        {}
    public:
        template<typename T>
        bool peek(T& x)
        {
            if (!p_->peek(x, seen_)) { return false; }
            seen_ += SizeT<T>::value();
            return true;
        }
        bool discard(std::size_t skip)
        {
            if (p_->readableSize() < skip) { return false; }
            seen_ += skip;
            return true;
        }
        std::size_t skip() const { return seen_; }

        friend class Packet<K, BufIdx>;
    };

    Peeker peeker() const { return Peeker(this); }
    bool forward(const Peeker& p) 
    { 
        if (p.p_ != this) { return false; }
        bufferIdx_.read(p.seen_); 
        return true;
    }
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

template<std::size_t K, typename BufIdx>
bool Packet<K, BufIdx>::peek(uint8_t& u8, std::size_t skip) const
{
    if (sizeof(uint8_t)+skip > this->readableSize()) { return false; }
    u8 = this->peekByte(0+skip);
    return true;
}

template<std::size_t K, typename BufIdx>
bool Packet<K, BufIdx>::peek(uint16_t& u16, std::size_t skip) const
{
    if (sizeof(uint16_t)+skip > this->readableSize()) { return false; }
    u16 = 
        (static_cast<uint16_t>(this->peekByte(0+skip)) << 8) | 
        this->peekByte(1+skip);
    // u16 = static_cast<uint16_t>(buffer_[head_++]);
    // u16 <<= 8;
    // u16 |= buffer_[head_++];
    return true;
}

template<std::size_t K, typename BufIdx>
bool Packet<K, BufIdx>::peek(uint32_t& u32, std::size_t skip) const
{
    if (sizeof(uint32_t)+skip > this->readableSize()) { return false; }
    u32 = 
        (static_cast<uint32_t>(this->peekByte(0+skip)) << 24) |
        (static_cast<uint32_t>(this->peekByte(1+skip)) << 16) | 
        (static_cast<uint32_t>(this->peekByte(2+skip)) << 8) | 
        static_cast<uint32_t>(this->peekByte(3+skip));
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
bool Packet<K, BufIdx>::peek(uint64_t& u64, std::size_t skip) const
{
    if (sizeof(uint64_t)+skip > this->readableSize()) { return false; }
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
        (static_cast<uint64_t>(this->peekByte(0+skip)) << 56) |
        (static_cast<uint64_t>(this->peekByte(1+skip)) << 48) |
        (static_cast<uint64_t>(this->peekByte(2+skip)) << 40) |
        (static_cast<uint64_t>(this->peekByte(3+skip)) << 32) |
        (static_cast<uint64_t>(this->peekByte(4+skip)) << 24) |
        (static_cast<uint64_t>(this->peekByte(5+skip)) << 16) |
        (static_cast<uint64_t>(this->peekByte(6+skip)) << 8) |
        static_cast<uint64_t>(this->peekByte(7+skip));
    return true;
}

template<std::size_t K, typename BufIdx>
bool Packet<K, BufIdx>::peek(int8_t& i8, std::size_t skip) const
{
    uint8_t i = 0;
    if (!this->peek(i, skip)) { return false; }
    if (i <= 0x7fu) { i8 = i; }
    else { i8 = -1 - static_cast<int8_t>(0xffu-i); }
    return true;
}

template<std::size_t K, typename BufIdx>
bool Packet<K, BufIdx>::peek(int16_t& i16, std::size_t skip) const
{
    uint16_t i = 0;
    if (!this->peek(i, skip)) { return false; }
    if (i <= 0x7fffu) { i16 = i; }
    else { i16 = -1 - static_cast<int16_t>(0xffffu-i); }
    return true;
}

template<std::size_t K, typename BufIdx>
bool Packet<K, BufIdx>::peek(int32_t& i32, std::size_t skip) const
{
    uint32_t i = 0;
    if (!this->peek(i, skip)) { return false; }
    if (i <= 0x7fffffffu) { i32 = i; }
    else { i32 = -1 - static_cast<int32_t>(0xffffffffu-i); }
    return true;
}

template<std::size_t K, typename BufIdx>
bool Packet<K, BufIdx>::peek(int64_t& i64, std::size_t skip) const
{
    uint64_t i = 0;
    if (!this->peek(i, skip)) { return false; }
    if (i <= 0x7fffffffffffffffu) { i64 = i; }
    else { i64 = -1 - static_cast<int64_t>(0xffffffffffffffffu-i); }
    return true;
}

template<std::size_t K, typename BufIdx>
bool Packet<K, BufIdx>::peek(float& f32, std::size_t skip) const
{
    if (sizeof(float)+skip > this->readableSize()) { return false; }
    uint32_t i;
    if (!this->peek(i, skip)) { return false; }
    f32 = Packing::unpack754(i, 32, 8);
    // binrep(i, 32);
    return true;
}

template<std::size_t K, typename BufIdx>
bool Packet<K, BufIdx>::peek(double& f64, std::size_t skip) const
{
    if (sizeof(double)+skip > this->readableSize()) { return false; }
    uint64_t i;
    if (!this->peek(i, skip)) { return false; }
    f64 = Packing::unpack754(i, 64, 11);
    // binrep(i, 64);
    return true;
}

template<std::size_t K, typename BufIdx>
bool Packet<K, BufIdx>::peek(bool& b, std::size_t skip) const
{
    uint8_t i = 0;
    if (!this->peek(i, skip)) { return false; }
    b = i > 0;
    return true;
}
