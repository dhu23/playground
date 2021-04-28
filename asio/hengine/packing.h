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

// packi16() -- store a 16-bit int into a char buffer like htons
void packi16(uint8_t *buf, uint16_t i)
{
    *buf++ = i>>8; 
    *buf++ = i;
}

// packi32 -- store a 32-bit int into a char buffer like htonl
void packi32(uint8_t *buf, uint32_t i)
{
    *buf++ = i>>24;
    *buf++ = i>>16;
    *buf++ = i>>8;
    *buf++ = i;
}

// packi64 -- store a 64-bit int into a char buff like htonl
void packi64(uint8_t *buf, uint64_t i)
{
    *buf++ = i>>56;
    *buf++ = i>>48;
    *buf++ = i>>40;
    *buf++ = i>>32;
    *buf++ = i>>24;
    *buf++ = i>>16;
    *buf++ = i>>8;
    *buf++ = i;
}

// I swapped the order of the definition to be slightly different from
// the original so functions get reusagable

// unpacku16 -- unpack a 16 bit int from a char buffer like ntohs
uint16_t unpacku16(uint8_t *buf)
{
    return ((uint16_t)buf[0] << 8) | buf[1];
}

// unpacki16 - unpack a 16 bit int from a char buffer like ntohs
int16_t unpacki16(uint8_t *buf)
{
    uint16_t i2 = unpacku16(buf);
    int16_t i;

    // change unsigned numbers to signed
    // 0x7fffu ==> 0111 1111 1111 1111
    if (i2 <= 0x7fffu) { i = i2; }
    else { i = -1 - (int16_t)(0xffffu - i2); }

    return i;
}

// unpacku32 - unpack a 32 bit unsigned from a char buffer like ntohl
uint32_t unpacku32(uint8_t *buf)
{
    return
        ((uint32_t)buf[0] << 24) |
        ((uint32_t)buf[1] << 16) |
        ((uint32_t)buf[2] << 8) |
        buf[3];
}

// unpacki32 - unpack a 32 bit signed int from a char buffer like ntohl
int32_t unpacki32(uint8_t *buf)
{
    uint32_t i2 = unpacku32(buf);

    int32_t i;
    //change unsigned numbers to signed
    if (i2 <= 0x7fffffffu) { i = i2; }
    else { i = -1 - (int32_t)(0xffffffffu - i2); }

    return i;
}

// unpacku64 - unpack a 64 bit unsigned from a char buffer like ntohl
uint64_t unpacku64(uint8_t *buf)
{
    return
        ((uint64_t)buf[0] << 56) |
        ((uint64_t)buf[0] << 48) |
        ((uint64_t)buf[0] << 40) |
        ((uint64_t)buf[0] << 32) |
        ((uint64_t)buf[0] << 24) |
        ((uint64_t)buf[0] << 16) |
        ((uint64_t)buf[0] << 8) |
        buf[7];
}

// unpacki64 - unpack a 64 bit int from a char buffer like ntohl
int64_t unpacki64(uint8_t *buf)
{
    uint64_t i2 = unpacku64(buf);
    
    int64_t i;

    // change unsigned numbers to signed
    if (i2 <= 0x7fffffffffffffffu) { i = i2; }
    else { i = -1 - (int64_t)(0xffffffffffffffffu - i2); }

    return i;
}

// pack - store data dictated by the format string in the buffer
//
//  bits | signed    unsigned    float   string
//  -----+--------------------------------------
//     8 |      c           C       
//    16 |      h           H        
//    32 |      i           I        f
//    64 |      l           L        d
//   128 |                           g 
//     - |                                    s
//
// 16 bit unsigned length is automatically prepended to string

// pack754() -- pack a floating point number into IEEE-754 format

uint64_t pack754(double f, int32_t bits, int32_t expbits)
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

double unpack754(uint64_t i, int32_t bits, int32_t expbits)
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

uint64_t pack754_32(float f) { return pack754(f, 32, 8); }
uint64_t pack754_64(double d) { return pack754(d, 64, 11); }

float unpack754_32(uint64_t i) { return unpack754(i, 32, 8); }
double unpack754_64(uint64_t i) { return unpack754(i, 64, 11); }

template<std::size_t K>
struct Packet
{
    Packet():
        buffer_(),
        head_(0),
        tail_(0)
    {}

    std::array<uint8_t, K> buffer_;
    std::size_t head_; // indicates the readable location
    std::size_t tail_; // indicates the writable location

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

    bool get(uint8_t& u8);
    bool get(uint16_t& u16);
    bool get(uint32_t& u32);
    bool get(uint64_t& u64);
    
    bool get(int8_t& i8);
    bool get(int16_t& i16);
    bool get(int32_t& i32);
    bool get(int64_t& i64);

    bool get(float& f32);
    bool get(double& f64);
};

template<std::size_t K>
bool Packet<K>::put(uint8_t u8)
{
    if (sizeof(uint8_t) + tail_ > K) { return false; }
    buffer_[tail_++] = u8;
    return true;
}

template<std::size_t K>
bool Packet<K>::put(uint16_t u16)
{
    if (sizeof(uint16_t) + tail_ > K) { return false; }
    buffer_[tail_++] = u16>>8;
    buffer_[tail_++] = u16;
    return true;
}

template<std::size_t K>
bool Packet<K>::put(uint32_t u32)
{
    if (sizeof(uint32_t) + tail_ > K) { return false; }
    buffer_[tail_++] = u32>>24;
    buffer_[tail_++] = u32>>16;
    buffer_[tail_++] = u32>>8;
    buffer_[tail_++] = u32;
    return true;
}

template<std::size_t K>
bool Packet<K>::put(uint64_t u64)
{
    if (sizeof(uint64_t) + tail_ > K) { return false; }
    buffer_[tail_++] = u64>>56;
    buffer_[tail_++] = u64>>48;
    buffer_[tail_++] = u64>>40;
    buffer_[tail_++] = u64>>32;
    buffer_[tail_++] = u64>>24;
    buffer_[tail_++] = u64>>16;
    buffer_[tail_++] = u64>>8;
    buffer_[tail_++] = u64;
    return true;
}

template<std::size_t K>
bool Packet<K>::put(int8_t i8)
{
    return this->put(static_cast<uint8_t>(i8));
}

template<std::size_t K>
bool Packet<K>::put(int16_t i16)
{
    return this->put(static_cast<uint16_t>(i16));
}

template<std::size_t K>
bool Packet<K>::put(int32_t i32)
{
    return this->put(static_cast<uint32_t>(i32));
}

template<std::size_t K>
bool Packet<K>::put(int64_t i64)
{
    return this->put(static_cast<uint64_t>(i64));
}

template<std::size_t K>
bool Packet<K>::put(float f32)
{
    if (sizeof(float) + tail_ > K) { return false; }
    auto fhold = static_cast<uint32_t>(pack754(f32, 32, 8));
    return this->put(fhold);
}

template<std::size_t K>
bool Packet<K>::put(double f64)
{
    if (sizeof(double) + tail_ > K) { return false; }
    auto fhold = static_cast<uint64_t>(pack754(f64, 64, 11));
    return this->put(fhold);
}

template<std::size_t K>
bool Packet<K>::get(uint8_t& u8)
{
    if (sizeof(uint8_t) + head_ > K) { return false; }
    u8 = buffer_[head_];
    head_ += 1;
    return true;
}

template<std::size_t K>
bool Packet<K>::get(uint16_t& u16)
{
    if (sizeof(uint16_t) + head_ > K) { return false; }
    u16 = (static_cast<uint16_t>(buffer_[head_]) << 8) | buffer_[head_+1];
    head_ += 2;
    // u16 = static_cast<uint16_t>(buffer_[head_++]);
    // u16 <<= 8;
    // u16 |= buffer_[head_++];
    return true;
}

template<std::size_t K>
bool Packet<K>::get(uint32_t& u32)
{
    if (sizeof(uint32_t) + head_ > K) { return false; }
    u32 = 
        (static_cast<uint32_t>(buffer_[head_]) << 24) |
        (static_cast<uint32_t>(buffer_[head_+1]) << 16) | 
        (static_cast<uint32_t>(buffer_[head_+2]) << 8) |
        static_cast<uint32_t>(buffer_[head_+3]);
    head_ += 4;
    // u32 = static_cast<uint32_t>(buffer_[head_++]);
    // u32 <<= 8;
    // u32 |= buffer_[head_++];
    // u32 <<= 8;
    // u32 |= buffer_[head_++];
    // u32 <<= 8;
    // u32 |= buffer_[head_++];
    return true;
}

template<std::size_t K>
bool Packet<K>::get(uint64_t& u64)
{
    if (sizeof(uint64_t) + head_ > K) { return false; }
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
        (static_cast<uint64_t>(buffer_[head_]) << 56) |
        (static_cast<uint64_t>(buffer_[head_+1]) << 48) |
        (static_cast<uint64_t>(buffer_[head_+2]) << 40) |
        (static_cast<uint64_t>(buffer_[head_+3]) << 32) |
        (static_cast<uint64_t>(buffer_[head_+4]) << 24) |
        (static_cast<uint64_t>(buffer_[head_+5]) << 16) |
        (static_cast<uint64_t>(buffer_[head_+6]) << 8) |
        static_cast<uint64_t>(buffer_[head_+7]);
    head_ += 8;
    return true;
}

template<std::size_t K>
bool Packet<K>::get(int8_t& i8)
{
    uint8_t i = 0;
    if (!this->get(i)) { return false; }
    if (i <= 0x7fu) { i8 = i; }
    else { i8 = -1 - static_cast<int8_t>(0xffu-i); }
    return true;
}

template<std::size_t K>
bool Packet<K>::get(int16_t& i16)
{
    uint16_t i = 0;
    if (!this->get(i)) { return false; }
    if (i <= 0x7fffu) { i16 = i; }
    else { i16 = -1 - static_cast<int16_t>(0xffffu-i); }
    return true;
}

template<std::size_t K>
bool Packet<K>::get(int32_t& i32)
{
    uint32_t i = 0;
    if (!this->get(i)) { return false; }
    if (i <= 0x7fffffffu) { i32 = i; }
    else { i32 = -1 - static_cast<int32_t>(0xffffffffu-i); }
    return true;
}

template<std::size_t K>
bool Packet<K>::get(int64_t& i64)
{
    uint64_t i = 0;
    if (!this->get(i)) { return false; }
    if (i <= 0x7fffffffffffffffu) { i64 = i; }
    else { i64 = -1 - static_cast<int64_t>(0xffffffffffffffffu-i); }
    return true;
}

template<std::size_t K>
bool Packet<K>::get(float& f32)
{
    if (sizeof(float) + head_ > K) { return false; }
    uint32_t i;
    if (!this->get(i)) { return false; }
    f32 = pack754(i, 32, 8);
    return true;
}

template<std::size_t K>
bool Packet<K>::get(double& f64)
{
    if (sizeof(double) + head_ > K) { return false; }
    uint64_t i;
    if (!this->get(i)) { return false; }
    f64 = pack754(i, 64, 11);
    return true;
}
