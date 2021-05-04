#ifndef _INCLUDED_TEMPL_H_
#define _INCLUDED_TEMPL_H_

template<typename T>
bool equalTs(T&& t1, T&& t2)
{
    return t1 == t2;
}

template<typename T, typename... Ts>
bool equalTs(T&& t1, T&& t2, Ts&&... ts)
{
    if (!equalTs(t1, t2)) { return false; }
    return equalTs(ts...);
}

template<typename T>
bool lexiLessTs(T&& t1, T&& t2)
{
    return t1 < t2;
}

template<typename T, typename... Ts>
bool lexiLessTs(T&& t1, T&& t2, Ts&&... ts)
{
    if (lexiLessTs(t1, t2)) { return true; }
    else if (lexiLessTs(t2, t1)) { return false; }
    else { return lexiLessTs(ts...); }
}

#endif
