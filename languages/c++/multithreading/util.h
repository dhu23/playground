#ifndef __UTIL_H__
#define __UTIL_H__

#include <sstream>
#include <iterator>

namespace details
{

template<typename T>
std::ostream& concatStream(std::ostream& os, T&& t)
{
    os << t;
    return os;
}

template<typename T, typename... Ts>
std::ostream& concatStream(std::ostream& os, T&& t, Ts&& ...ts)
{
    os << t;
    return concatStream(os, ts...);
}


template<typename D, typename T>
std::ostream& joinStreamBy(std::ostream& os, D&& d, T&& t)
{
    return concatStream(os, t); // ignore d for one item
}

template<typename D, typename T, typename... Ts>
std::ostream& joinStreamBy(std::ostream& os, D&& delim, T&& t, Ts&&... ts)
{
    os << t << delim;
    return joinStreamBy(os, delim, ts...);
}

}

template<typename... Ts>
std::string concat(Ts&& ...ts)
{
    std::ostringstream oss;
    details::concatStream(oss, ts...);
    return oss.str();
}

template<typename D, typename... Ts>
std::string joinBy(D&& delim, Ts&&... ts)
{
    std::ostringstream oss;
    details::joinStreamBy(oss, delim, ts...);
    return oss.str();
}


#endif
