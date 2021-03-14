// given a real number between 0 and 1(e.g. 0.72) that is passed in as a
// double. print the binary representation. If the number cannot be 
// represented accurately in binary with at most 32 characters, print 'ERROR'

#include <string>
#include <vector>
#include <iterator>
#include <iostream>
#include <cassert>

// for number larger than 1, 
// 2^n * a(n) + 2^(n-1) * a(n-1) + ... + 2^2 * a(2) + 2 * a(1) + a(0)
// for number smaller than 1
// 1/2 * b(1) + 1/2^2 * b(2) + ... + 1/2^(n-1) * b(n-1) + 1/2^n * b(n)

std::string convertToBinary(double x)
{
    assert(x > 0 && x < 1.0);
    std::vector<char> ret;
    ret.reserve(34);
    ret.push_back('0');
    ret.push_back('.');
    while (x != 0 && ret.size() < 34)
    {
        int digit = static_cast<int>(2*x);
        x = 2*x-digit;
        ret.push_back('0'+digit);
    }
    if (x != 0)
    {
        return "ERROR";
    }
    else
    {
        return std::string(std::begin(ret), std::end(ret));
    }
}

double convertToDouble(const std::string& b)
{
    assert(b.size() > 2);
    assert(b[0] == '0' && b[1] == '.');

    double ret = 0.0;
    for (size_t i = b.size()-1; i > 1; --i) // backwards to idx = 2(inclusive)
    {
        ret += b[i] - '0';
        ret *= 0.5;
    }
    return ret;
}

int main(int argc, char* argv[])
{
    std::cout << convertToBinary(0.75) << std::endl;
    std::cout << convertToBinary(0.833) << std::endl;
    std::cout << convertToBinary(0.5+0.25+0.125) << std::endl;
    std::cout << convertToBinary(0.5+0.125) << std::endl;

    std::cout << convertToDouble("0.111") << std::endl;

    std::string b1 = "0.010101000101110101";
    std::cout << convertToBinary(convertToDouble(b1)) << std::endl;
    std::cout << b1 << std::endl;


    return 0;
}
