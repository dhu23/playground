#ifndef _INCLUDED_BYTEARRAY_H_
#define _INCLUDED_BYTEARRAY_H_

#include <array>
#include <string>
#include <cstring>

// null-byte-terminated and fixed-sized array


template<std::size_t N>
class ByteArray
{
    std::array<char, N> arr_;

public:
    ByteArray()
    {
        memset(arr_.data(), 0, N);
    }

    ByteArray<N>& fromArray(const char* from, std::size_t len)
    {
        std::size_t slen = std::min(len, N-1);
        memcpy(arr_.data(), from, slen);
        memset(arr_.data()+slen, 0, N-slen);
        return *this;
    }

    ByteArray<N>& fromCArray(const char* from)
    {
        return this->fromArray(from, std::strlen(from));
    }

    ByteArray<N>& fromString(const std::string& from)
    {
        return this->fromCArray(from.c_str());
    }

    const char* data() const { return arr_.data(); }
    std::string str() const { return std::string(arr_.data()); }
    // std::string(arr_.begin(), arr_.end()); returns a string with null bytes
    std::size_t size() const { return std::strlen(arr_.data()); }
    bool empty() const { return this->size() == 0; }
};

#endif
