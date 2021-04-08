#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <cmath>

template<typename T>
T convert(const std::string& s)
{
    std::vector<char> vec;
    int dec = 0;
    bool countDec = false;
    for (auto c : s)
    {
        if (c == '.') {
            countDec = true;
        }
        else
        {
            vec.push_back(c);
            if (countDec) ++dec;
        }
    }
    // pading 10-dec zeros
    std::vector<char> padding(10-dec, '0');
    vec.insert(vec.end(), padding.begin(), padding.end());
    int64_t bigInt = std::stoll(std::string(vec.begin(), vec.end()));
    return bigInt * 1e-10; // ?
}

bool valid(char c)
{
    return (c >= '0' && c <= '9') || c == '-' || c == '.';
}

template<typename T>
bool convert2(const std::string& s, T& ret)
{
    int dec = 0;
    bool countDec = false;
    int64_t bigInt = 0;
    bool neg = s.front() == '-';

    for (auto c : s)
    {
        if (!valid(c)) return false;
        if (c == '-') 
        {
            neg = true;
            continue;
        }

        if (c == '.') 
        {
            countDec = true;
        }
        else
        {
            bigInt = bigInt*10 + c-'0';
            if (countDec) ++dec;
        }
        if (dec >= 9) break;
    }
    // x*10^d + 10^(d-7)    / 10^d
    /*
    double bigDouble = bigInt + std::pow(10, dec-9);
    ret = bigDouble / std::pow(10, dec);
    if (neg) ret *= -1;
    return true;
    */

    // padding 10-dec zeros   1.23 --> 123 dec=2
    dec = 9 - dec;
    while (dec > 0)
    {
        bigInt = bigInt*10;
        --dec;
    }
    bigInt += 1;
    ret = bigInt * 1e-9;
    if (neg) ret *= (-1);
    return true;
}

void print(const std::string& s)
{
    std::cout << s << std::endl;

    /*
    std::cout << "convert --------" << std::endl;
    std::cout << std::setprecision(20) << convert<float>(s) << std::endl;
    std::cout << std::setprecision(20) << convert<double>(s) << std::endl;
    */

    // std::cout << "convert2 --------" << std::endl;
    /*
    float ret = 0.0;
    if (convert2<float>(s, ret))
    {
        std::cout << std::setprecision(20) << ret << std::endl;
    }
    */

    double retd = 0.0;
    if (convert2<double>(s, retd))
    {
        std::cout << std::setprecision(30) << retd << std::endl;
    }
    // std::cout << "stof/d--------" << std::endl;
    // std::cout << std::setprecision(20) << stof(s) << std::endl;
    // std::cout << std::setprecision(20) << stod(s) << std::endl;
    //
    //
    std::cout << std::endl << std::endl; 
}

std::string keepDecimal(const std::string& number, int numOfDecimal)
{
    if (numOfDecimal < 0) return number;
    size_t pos = number.find(".");
    if (std::string::npos == pos) return number;
    size_t end = pos+numOfDecimal+1;
    if (number.size() <= end ) return number;
    return std::string(number.begin(), number.begin()+end);
}

void testDecimal(const std::string& p, int numOfDecimal)
{
    std::cout << p << "====>" << keepDecimal(p, numOfDecimal) << std::endl;
}

int main(int argc, char* argv[])
{
    print("2.5495");
    print("3.01");
    print("-223.32");
    print("-22323.32");
    print("-2121223.32");
    print("0.125");
    print(".12115");
    print("1213.12124213511");

    std::cout << std::stod("1e-5") << std::endl;
    // std::cout << std::stod("") << std::endl;
    
    testDecimal("1.2", 4);
    testDecimal("12", 4);
    testDecimal("-1213.1", 4);
    testDecimal("-123.123", 4);
    testDecimal("-1234.5123", 4);
    testDecimal("-1215.43256", 4);

    return 0;
}
