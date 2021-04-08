#include "natural_number.h"

#include <algorithm>
#include <stdexcept>
#include <iostream>

namespace dhm{

using std::vector;
using std::string;
using std::pair;
using std::make_pair;

namespace{

char getCharNumber(const int number)
{
    if(9 < number || 0 > number)
    {
        throw std::runtime_error("Invalid Digit");
    }
    return '0'+number;
}

char getCharNumber(char value)
{
    return '0'+value;
}

char getValue(char numberChar)
{
    return numberChar-'0';
}

NaturalNumber multiplyBySingleDigit(const NaturalNumber& n, const char m)
{
    size_t len = n.numOfDigits();
    char mValue = getValue(m);

    vector<char> product;

    char carry = 0;
    for(size_t i = 1; ; ++i)
    {
        char digit = n.getDigit(i);

        if('0' == digit && 0 == carry && i > len)
        {
            break;
        }

        char total = getValue(digit)*mValue+carry;
        carry = total/10;
        total %= 10;
        product.push_back(getCharNumber(total));
    }
    /*
    NaturalNumber productNum(product);
    std::cout 
        << n.print() << "*" << m << "=" << productNum.print() 
        << std::endl;
    return productNum;
    */
    return NaturalNumber(product);
}

NaturalNumber multiplyByPowerOfTen(const NaturalNumber& n, const int power)
{
    vector<char> product(power, '0');
    size_t nLen = n.numOfDigits();
    for(size_t i = 1; i <= nLen; ++i)
    {
        product.push_back(n.getDigit(i, false));
    }
    /*
    NaturalNumber productNum(product);
    std::cout
        << n.print() << "*10^" << power << "=" << productNum.print() 
        << std::endl;
    return productNum;
    */
    return NaturalNumber(product);
}

// the largest multiple has to be a single digit number
int getLargestMultipleOfDivisor(
        const NaturalNumber& dividend, const NaturalNumber& divisor)
{
    if(0 == divisor)
    {
        throw std::runtime_error("CANNOT divide by ZERO");
    }
    else if(dividend < divisor)
    {
        return 0;
    }
    else if(multiplyByPowerOfTen(divisor, 1) <= dividend)
    {
        /*
        std::cout
            << "divisor = " << divisor.print() 
            << " dividend = " << dividend.print() << std::endl;
        */
        throw std::runtime_error("Dividend is too large");
    }
    else
    {
        int attempt = 9;
        while(divisor*attempt > dividend)
        {
            --attempt;
        }
        return attempt;
    }

}

// get a new number consisting of numbers from top to position k(including)
NaturalNumber getTop(const NaturalNumber& n, size_t k)
{
    size_t nLen = n.numOfDigits();
    if(k > nLen)
    {
        throw std::runtime_error("NOT enough digits");
    }

    vector<char> top;
    while(k <= nLen)
    {
        top.push_back(n.getDigit(k));
        ++k;
    }
    return NaturalNumber(top);
}

// division: returns quotient and remainder
pair<NaturalNumber, NaturalNumber> calculateDivision(
        const NaturalNumber& dividend, const NaturalNumber& divisor)
{
    size_t dividendLen = dividend.numOfDigits();
    vector<char> result;
    NaturalNumber d = dividend;
    for(size_t pos = dividendLen; pos >= 1; --pos)
    {
        // std::cout << "d = " << d.print() << std::endl;
        NaturalNumber top = getTop(d, pos);
        // std::cout << "top = " << top.print() << std::endl;

        int digit = getLargestMultipleOfDivisor(top, divisor);

        // std::cout << "digit = " << digit << std::endl;
        result.insert(result.begin(), getCharNumber(digit));

        // update dividend when the result is between 1 and 9
        if(0 != digit)
        {
            d -= multiplyByPowerOfTen(digit*divisor, pos-1);
            // std::cout << "d updated: " << d.print() << std::endl;
        }
    }
    return make_pair<NaturalNumber, NaturalNumber>(NaturalNumber(result), d);
}

} // end of anonymous namespace

NaturalNumber::NaturalNumber():
    m_digits(1, '0')
{
}

NaturalNumber::NaturalNumber(int number):
    m_digits()
{
    if(number < 0)
    {
        throw std::runtime_error("Negative Number is NOT Natural Number");
    }

    while(0 != number)
    {
        m_digits.push_back(getCharNumber(number%10));
        number = number/10;
    }
}

NaturalNumber::NaturalNumber(
        const vector<char>& digits, const bool isBackwards):
    m_digits(digits)
{
    if(!isBackwards)
    {
        std::reverse(m_digits.begin(), m_digits.end());
    }

    // remove '0's at the end
    while('0' == m_digits.back())
    {
        m_digits.pop_back();
    }

    if(m_digits.empty())
    {
        m_digits.push_back('0');
    }
}

NaturalNumber::NaturalNumber(const NaturalNumber& n):
    m_digits()
{
    size_t nLen = n.numOfDigits();
    for(size_t i = 1; i <= nLen; ++i)
    {
        m_digits.push_back(n.getDigit(i, false));    
    }
}

NaturalNumber& NaturalNumber::operator=(const NaturalNumber& n)
{
    if(&n != this)
    {
        this->m_digits = n.m_digits;
    }
    return *this;
}

NaturalNumber::~NaturalNumber()
{
}

string NaturalNumber::print() const
{
    return string(m_digits.rbegin(), m_digits.rend());
}

char NaturalNumber::getDigit(const size_t k, const bool fillWithZero) const
{
    if(!fillWithZero)
    {
        return m_digits.at(k-1);
    }
    else
    {
        return (k > m_digits.size()) ? '0' : m_digits[k-1];
    }
}

size_t NaturalNumber::numOfDigits() const
{
    return m_digits.size();
}

NaturalNumber& NaturalNumber::operator+=(const NaturalNumber& n)
{
    NaturalNumber temp(*this+n);
    this->m_digits = temp.m_digits;
}

NaturalNumber& NaturalNumber::operator-=(const NaturalNumber& n)
{
    NaturalNumber temp(*this-n);
    this->m_digits = temp.m_digits;
}

NaturalNumber& NaturalNumber::operator*=(const NaturalNumber& n)
{
    NaturalNumber temp(*this*n);
    this->m_digits = temp.m_digits;
}
 
NaturalNumber& NaturalNumber::operator/=(const NaturalNumber& n)
{
    NaturalNumber temp(*this/n);
    this->m_digits = temp.m_digits;
}

NaturalNumber& NaturalNumber::operator%=(const NaturalNumber& n)
{
    NaturalNumber temp(*this%n);
    this->m_digits = temp.m_digits;
}

bool operator<(const NaturalNumber& lhs, const NaturalNumber& rhs)
{
    if(&lhs == &rhs)
    {
        return false;
    }

    size_t numL = lhs.numOfDigits();
    size_t numR = rhs.numOfDigits();
    if(numL < numR)
    {
        return true;
    }
    else if(numL > numR)
    {
        return false;
    }
    else
    {
        for(size_t place = numL; 0 < place; --place)
        {
            char digitL = lhs.getDigit(place);
            char digitR = rhs.getDigit(place);
            if(digitL < digitR)
            {
                return true;
            }
            else if(digitL > digitR)
            {
                return false;
            }
        }
        return false;
    }
}

bool operator>(const NaturalNumber& lhs, const NaturalNumber& rhs)
{
    return rhs < lhs;
}

bool operator==(const NaturalNumber& lhs, const NaturalNumber& rhs)
{
    return !(rhs < lhs) && !(rhs > lhs);
}


bool operator<=(const NaturalNumber& lhs, const NaturalNumber& rhs)
{
    return lhs < rhs || lhs == rhs;
}

bool operator>=(const NaturalNumber& lhs, const NaturalNumber& rhs)
{
    return lhs > rhs || lhs == rhs;
}


NaturalNumber operator+(const NaturalNumber& lhs, const NaturalNumber& rhs)
{
    size_t lLen = lhs.numOfDigits();
    size_t rLen = rhs.numOfDigits();

    vector<char> sum;

    char carry = 0;
    for(size_t i = 1; ; ++i)
    {
        char lDigit = lhs.getDigit(i);
        char rDigit = rhs.getDigit(i);

        if('0' == lDigit && '0' == rDigit && 0 == carry &&
                i > lLen && i > rLen)
        {
            break;
        }

        char total = getValue(lDigit)+getValue(rDigit)+carry;
        carry = 0;

        if(total>=10)
        {
            total -= 10;
            ++carry;
        }
        /*
        std::cout 
            << "total " << getCharNumber(total) 
            << " carry " << getCharNumber(carry) << std::endl;
        */
        sum.push_back(getCharNumber(total));
    }

    /*
    NaturalNumber sumNum(sum);
    std::cout 
        << lhs.print() << "+" << rhs.print() << "=" << sumNum.print() 
        << std::endl;
    return sumNum;
    */
    return NaturalNumber(sum);
}

NaturalNumber operator-(const NaturalNumber& lhs, const NaturalNumber& rhs)
{
    if(lhs < rhs)
    {
        throw std::runtime_error("CANNOT subtract a larger number");
    }

    size_t lLen = lhs.numOfDigits();
    size_t rLen = rhs.numOfDigits();

    vector<char> diff;

    char carry = 0;
    for(size_t i = 1; ; ++i)
    {
        char lDigit = lhs.getDigit(i);
        char rDigit = rhs.getDigit(i);

        if('0' == lDigit && '0' == rDigit && 
                i > lLen && i > rLen)
        {
            if(0 == carry)
            {
                break;
            }
            else 
            {
                throw std::runtime_error("NOTHING to borrow");
            }
        }

        char total = getValue(lDigit)-carry-getValue(rDigit);
        carry = 0;
        if(total < 0)
        {
            total += 10;
            carry = 1;
        }

        // std::cout << getCharNumber(total) << std::endl;
        diff.push_back(getCharNumber(total));
    }
    /*
    NaturalNumber diffNum(diff);
    std::cout 
        << lhs.print() << "-" << rhs.print() << "=" << diffNum.print() 
        << std::endl;
    return diffNum;
    */
    return NaturalNumber(diff);
}


NaturalNumber operator*(const NaturalNumber& lhs, const NaturalNumber& rhs)
{
    // map<char, NaturalNumber> resultCache;
    
    // TODO can use some optimization
    size_t rLen = rhs.numOfDigits();

    NaturalNumber product;
    for(size_t i = 1; i <= rLen; ++i)
    {
        NaturalNumber result = multiplyBySingleDigit(lhs, rhs.getDigit(i));
        product += multiplyByPowerOfTen(result, i-1);
    }
    /*
    NaturalNumber productNum(product);
    std::cout 
        << lhs.print() << "*" << rhs.print() << "=" << productNum.print()
        << std::endl;
    return productNum;
    */
    return NaturalNumber(product);
    // return multiplyBySingleDigit(lhs, '7');
}

NaturalNumber operator/(const NaturalNumber& lhs, const NaturalNumber& rhs)
{ 
    return calculateDivision(lhs, rhs).first;
}

NaturalNumber operator%(const NaturalNumber& lhs, const NaturalNumber& rhs)
{
    return calculateDivision(lhs, rhs).second;
}

} // end of namespace dhm 
