#ifndef __UTIL_H__
#define __UTIL_H__

#include <sstream>

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

template<typename... Ts>
std::string concat(Ts&& ...ts)
{
    std::ostringstream oss;
    concatStream(oss, ts...);
    return oss.str();
}

#endif
