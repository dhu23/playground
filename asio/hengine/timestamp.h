#ifndef _INCLUDED_TIMESTAMP_H_
#define _INCLUDED_TIMESTAMP_H_

#include <cstdint>
#include <chrono>
#include <iostream>
#include <iomanip>

class Timestamp
{
    uint64_t nano_;
public:

    constexpr static std::size_t sizeT()
    { 
        return sizeof(decltype(Timestamp::nano_)); 
    }

    constexpr static uint64_t NANO_PER_MICRO = 1000UL;
    constexpr static uint64_t MICRO_PER_MILLI = 1000UL;
    constexpr static uint64_t MILLI_PER_SEC = 1000UL;

    constexpr static uint64_t NANO_PER_MILLI = NANO_PER_MICRO * MICRO_PER_MILLI;
    constexpr static uint64_t NANO_PER_SEC = NANO_PER_MILLI * MILLI_PER_SEC;

    static uint64_t getTotalSeconds(std::tm t)
    {
        // convert tm as localtime to epoch time
        return std::max(static_cast<uint64_t>(std::mktime(&t)), 0UL);
        // std::time_t ts = std::mktime(&t);
        // std::cout 
        //     << "mktime result:" << ts
        //     << ",tm-year:" << t.tm_year 
        //     << ",tm_mon:" << t.tm_mon
        //     << ",tm_mday:" << t.tm_mday
        //     << ",tm_hour:" << t.tm_hour
        //     << ",tm_min:" << t.tm_min
        //     << ",tm_sec:" << t.tm_sec 
        //     << std::endl;
        // if (ts > 0) { return static_cast<uint64_t>(ts); }
        // else { return 0; }
    }

public:
    using system_clock = std::chrono::system_clock;
    using system_time_point = std::chrono::time_point<system_clock>;

    constexpr Timestamp():
        nano_(0)
    {}

    constexpr explicit Timestamp(uint64_t ns):
        nano_(ns)
    {}

    Timestamp(const system_time_point& tp):
        nano_(
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                // std::chrono::nanoseconds is equivalent to 
                // std::chrono::duration<system_clock::rep, std::nano>
                tp.time_since_epoch()).count())
    {}

    // Timestamp(std::tm ts, uint16_t millis=0, uint16_t micros=0, uint16_t nanos=0):
    //     nano_(
    //         getTotalSeconds(ts) * NANO_PER_SEC + 
    //         static_cast<uint64_t>(millis) * NANO_PER_MILLI + 
    //         static_cast<uint64_t>(micros) * NANO_PER_MICRO + 
    //         static_cast<uint64_t>(nanos))
    // {}

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
        // localtime_r(&ts, &t); // this would express tm based off localtime
        gmtime_r(&ts, &t); // this would express tm based off GMT
        return t;
    }

    bool operator==(const Timestamp& ts) const { return nano_ == ts.nano_; }

    bool operator<(const Timestamp& ts) const { return nano_ < ts.nano_; }
    
    template<typename R, typename P>
    Timestamp& operator+=(const std::chrono::duration<R, P>& dur)
    {
        nano_ += std::chrono::duration_cast<std::chrono::nanoseconds>(dur).count();
        return *this;
    }

    template<typename R, typename P>
    Timestamp& operator-=(const std::chrono::duration<R, P>& dur)
    {
        nano_ -= std::chrono::duration_cast<std::chrono::nanoseconds>(dur).count();
        return *this;
    }

    template<typename R, typename P>
    Timestamp inTheFuture(const std::chrono::duration<R, P>& dur) const
    {
        Timestamp ret(*this);
        ret += dur;
        return ret;
    }

    template<typename R, typename P>
    Timestamp inThePast(const std::chrono::duration<R, P>& dur) const
    {
        Timestamp ret(*this);
        ret -= dur;
        return ret;
    }

    std::ostream& print(std::ostream& os) const
    {
        std::tm t = this->tm();

        os 
            << std::setw(4) << std::setfill('0') << (t.tm_year+1900) << '-'
            << std::setw(2) << std::setfill('0') << (t.tm_mon+1) << '-'
            << std::setw(2) << std::setfill('0') << t.tm_mday << ' '
            << std::setw(2) << std::setfill('0') << t.tm_hour << ':'
            << std::setw(2) << std::setfill('0') << t.tm_min << ':'
            << std::setw(2) << std::setfill('0') << t.tm_sec << '.'
            << std::setw(3) << std::setfill('0') << this->milliseconds() << ','
            << std::setw(3) << std::setfill('0') << this->microseconds() << ','
            << std::setw(3) << std::setfill('0') << this->nanoseconds();
        return os;
    }

    std::string toString() const
    {
        std::ostringstream oss;
        this->print(oss);
        return oss.str();
    }

    explicit operator std::string() const
    {
        return this->toString();
    }
};

inline bool operator<(const Timestamp& lhs, const Timestamp& rhs)
{
    return lhs.operator<(rhs);
}

inline std::ostream& operator<<(std::ostream& os, const Timestamp& obj)
{
    return obj.print(os);
}

#endif
