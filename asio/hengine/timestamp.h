#ifndef _INCLUDED_TIMESTAMP_H_
#define _INCLUDED_TIMESTAMP_H_

#include <cstdint>
#include <chrono>


class Timestamp
{
    uint64_t nano_;
public:

    constexpr static uint64_t NANO_PER_MICRO = 1000UL;
    constexpr static uint64_t MICRO_PER_MILLI = 1000UL;
    constexpr static uint64_t MILLI_PER_SEC = 1000UL;

    constexpr static uint64_t NANO_PER_MILLI = NANO_PER_MICRO * MICRO_PER_MILLI;
    constexpr static uint64_t NANO_PER_SEC = NANO_PER_MILLI * MILLI_PER_SEC;

    static uint64_t getTotalSeconds(std::tm t)
    {
        return std::max(static_cast<uint64_t>(std::mktime(&t)), 0UL);
    }

public:
    using system_clock = std::chrono::system_clock;
    using system_time_point = std::chrono::time_point<system_clock>;

    constexpr Timestamp():
        nano_(0)
    {}

    constexpr Timestamp(uint64_t ns):
        nano_(ns)
    {}

    Timestamp(const system_time_point& tp):
        nano_(
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                // std::chrono::nanoseconds is equivalent to 
                // std::chrono::duration<system_clock::rep, std::nano>
                tp.time_since_epoch()).count())
    {}

    Timestamp(std::tm ts, uint16_t millis=0, uint16_t micros=0, uint16_t nanos=0):
        Timestamp(
            getTotalSeconds(ts) * NANO_PER_SEC + 
            static_cast<uint64_t>(millis) * NANO_PER_MILLI + 
            static_cast<uint64_t>(micros) * NANO_PER_MICRO + 
            static_cast<uint64_t>(nanos))
    {}

    static Timestamp now()
    {
        return Timestamp(system_clock::now());
    }

    uint64_t totalNanoseconds() const { return nano_; }
    uint64_t totalMicroseconds() const { return nano_ / NANO_PER_MICRO; }
    uint64_t totalMilliseconds() const { return nano_ / NANO_PER_MILLI; }
    uint64_t totalSeconds() const { return nano_ / NANO_PER_SEC; }

    uint16_t nanoseconds() const { return nano_ % 1000; }
    uint16_t microseconds() const { return (nano_ / 1000) % 1000; }
    uint16_t milliseconds() const { return (nano_ / 1000000) % 1000; }
    std::tm tm() const 
    {
        std::tm t;
        time_t ts = this->totalSeconds();
        gmtime_r(&ts, &t);
        return t;
    }

    bool operator<(const Timestamp& ts) const { return nano_ < ts.nano_; }
};

bool operator<(const Timestamp& lhs, const Timestamp& rhs)
{
    return lhs.operator<(rhs);
}

#endif
