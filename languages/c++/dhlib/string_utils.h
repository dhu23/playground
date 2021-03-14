#ifndef STRINGUTILS_H_
#define STRINGUTILS_H_

#include <cctype>
#include <vector>
#include <sstream>
#include <string>

#include <boost/tokenizer.hpp>

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

    template<typename T, typename UnaryFunc>
    static std::vector<T> splitBy(
        const char* delims, const std::string& s, UnaryFunc func)
    {
        using CharSep = boost::char_separator<char>;
        boost::tokenizer<CharSep> tokens(s, CharSep(delims));

        std::vector<T> ret;
        std::transform(
            tokens.begin(), tokens.end(), std::back_inserter(ret), func);
        return ret;
    }


    static std::vector<int> splitToIntsBy(
        const char* delims, const std::string& s)
    {
        auto toInt = [](const std::string& str){ return std::stoi(str); };
        return splitBy<int>(delims, s, toInt);
    }

    static std::vector<std::string> splitToStrBy(
        const char* delims, const std::string& s)
    {
        auto iden = [](const std::string& str) { return str; };
        return splitBy<std::string>(delims, s, iden);
    }
};


namespace qb
{
    static inline bool isNumber(std::string s){
        int d=0;
        int n=0;
        for(unsigned i=0; i<s.size(); i++){
            if(!std::isdigit(s[i])){
                if(s[i]=='.' && n==0){
                    n++;
                    continue;
                }else{
                    return false;
                }
            }else{
                d++;
            }
        }
        if(d==0){
            return false;
        }
        return true;
    }
}

#endif /* STRINGUTILS_H_ */
