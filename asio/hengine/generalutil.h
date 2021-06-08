#ifndef __INCLUDED_GENERAL_UTIL_H_
#define __INCLUDED_GENERAL_UTIL_H_

#include <cctype>
#include <vector>
#include <sstream>
#include <string>
#include <iterator>

// #include <boost/tokenizer.hpp>

namespace details // not for client code usage
{

template<typename T> 
inline std::ostream& concatStream(std::ostream& os, T&& t)
{
    os << t;
    return os;
}

template<typename T, typename... Ts>
inline std::ostream& concatStream(std::ostream& os, T&& t, Ts&&... ts)
{
    os << t;
    return concatStream(os, ts...);
}

template<typename D, typename T>
inline std::ostream& joinStreamBy(std::ostream& os, D&& delim, T&& t)
{
    return concatStream(os, t); // ignore delim for one item
}

template<typename D, typename T, typename... Ts>
inline std::ostream& joinStreamBy(std::ostream& os, D&& delim, T&& t, Ts&&... ts)
{
    os << t << delim;
    return joinStreamBy(os, delim, ts...);
}

} // end of details namespace for implementation-only templates


struct StringUtils
{
    template<typename... Ts>
    inline static std::string concat(Ts&&... ts)
    {
        std::ostringstream oss;
        details::concatStream(oss, ts...);
        return oss.str();
    }

    template<typename D, typename... Ts> 
    inline static std::string joinBy(D&& delim, Ts&&... ts)
    {
        std::ostringstream oss;
        details::joinStreamBy(oss, delim, ts...);
        return oss.str();
    }

    template<typename T, typename Container>
    inline static std::string joinFrom(
        const char* sep, const Container& c)
    {
        std::ostringstream oss;
        std::copy(c.begin(), c.end(), std::ostream_iterator<T>(oss, sep));
        return oss.str();
    }

    static inline std::string toUpper(const std::string& str)
    {
        std::string res = str;
        std::transform(str.begin(), str.end(), res.begin(), ::toupper);
        return res;
    }

    static inline std::string toLower(std::string& str)
    {
        std::string res = str;
        std::transform(str.begin(), str.end(), res.begin(), ::tolower);
        return res;
    }

    // template<typename T, typename UnaryFunc>
    // static std::vector<T> splitBy(
    //     const char* delims, const std::string& s, UnaryFunc func)
    // {
    //     using CharSep = boost::char_separator<char>;
    //     boost::tokenizer<CharSep> tokens(s, CharSep(delims));

    //     std::vector<T> ret;
    //     std::transform(
    //         tokens.begin(), tokens.end(), std::back_inserter(ret), func);
    //     return ret;
    // }


    // static std::vector<int> splitToIntsBy(
    //     const char* delims, const std::string& s)
    // {
    //     auto toInt = [](const std::string& str){ return std::stoi(str); };
    //     return splitBy<int>(delims, s, toInt);
    // }

    // static std::vector<std::string> splitToStrBy(
    //     const char* delims, const std::string& s)
    // {
    //     auto iden = [](const std::string& str) { return str; };
    //     return splitBy<std::string>(delims, s, iden);
    // }
};

template<typename T>
class Oscillator
{
    T min_;
    T step_;
    std::size_t range_;

    bool direction_;
    std::size_t pos_;

public:
    Oscillator(T tmin, T tstep, std::size_t range, bool dir=true, std::size_t pos=0):
        min_(tmin),
        step_(tstep),
        range_(range),
        direction_(dir),
        pos_(pos)
    {}

    T gen()
    {
        auto ret = pos_*step_ + min_;
        if (pos_ == range_) 
        {
            direction_ = false;
            --pos_;
        }
        else if (pos_ == 0)
        {
            direction_ = true;
            pos_ = 1;
        }
        else
        {
            pos_ = direction_ ? pos_+1 : pos_-1;
        }
        return ret;
    }
};

#endif
