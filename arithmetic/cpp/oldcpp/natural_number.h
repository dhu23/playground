#ifndef INCLUDED_DHM_NATURAL_NUMBER_H
#define INCLUDED_DHM_NATURAL_NUMBER_H

#include <string>
#include <vector>
#include <utility>

namespace dhm{

class NaturalNumber
{
    public:
        NaturalNumber();
        NaturalNumber(int number);
        // NaturalNumber(const longlong number);
        // NaturalNumber(const std::string& numberString);
        explicit NaturalNumber(
                const std::vector<char>& digits, const bool isBackwards=true); 
        
        NaturalNumber(const NaturalNumber& n);
        NaturalNumber& operator=(const NaturalNumber& n);
        ~NaturalNumber();

        std::string print() const;

        // i.e. for number 1234, digit at the 3rd place would be 2

        NaturalNumber& operator+=(const NaturalNumber& m);
        NaturalNumber& operator-=(const NaturalNumber& m);
        
        NaturalNumber& operator*=(const NaturalNumber& m);
        NaturalNumber& operator/=(const NaturalNumber& m);

        NaturalNumber& operator%=(const NaturalNumber& m);


        friend bool operator<(
                const NaturalNumber& lhs, const NaturalNumber& rhs);
        friend bool operator>(
                const NaturalNumber& lhs, const NaturalNumber& rhs);
        friend bool operator==(
                const NaturalNumber& lhs, const NaturalNumber& rhs);
        friend bool operator<=(
                const NaturalNumber& lhs, const NaturalNumber& rhs);
        friend bool operator>=(
                const NaturalNumber& lhs, const NaturalNumber& rhs);

        friend NaturalNumber operator+(
                const NaturalNumber& lhs, const NaturalNumber& rhs);
        friend NaturalNumber operator-(
                const NaturalNumber& lhs, const NaturalNumber& rhs);
        friend NaturalNumber operator*(
                const NaturalNumber& lhs, const NaturalNumber& rhs);
        friend NaturalNumber operator/(
                const NaturalNumber& lhs, const NaturalNumber& rhs);
        friend NaturalNumber operator%(
                const NaturalNumber& lhs, const NaturalNumber& rhs);

        char getDigit(const size_t k, const bool fillWithZero=true) const;
        size_t numOfDigits() const;
        
    private:
        // digits vector are initalized backwards from 
        // how a number is normally written. 
        // i.e. number 1234 is expressed as {'4', '3', '2', '1'} 
        std::vector<char> m_digits;
};


bool operator<(const NaturalNumber& lhs, const NaturalNumber& rhs);
bool operator>(const NaturalNumber& lhs, const NaturalNumber& rhs);
bool operator==(const NaturalNumber& lhs, const NaturalNumber& rhs);
bool operator<=(const NaturalNumber& lhs, const NaturalNumber& rhs);
bool operator>=(const NaturalNumber& lhs, const NaturalNumber& rhs);

NaturalNumber operator+(const NaturalNumber& lhs, const NaturalNumber& rhs);
NaturalNumber operator-(const NaturalNumber& lhs, const NaturalNumber& rhs);

NaturalNumber operator*(const NaturalNumber& lhs, const NaturalNumber& rhs);
NaturalNumber operator/(const NaturalNumber& lhs, const NaturalNumber& rhs);

NaturalNumber operator%(const NaturalNumber& lhs, const NaturalNumber& rhs);

std::pair<NaturalNumber, NaturalNumber> 
getLCDAndLCM(const NaturalNumber& a, const NaturalNumber& b);

} // end of namesapce dhm 

#endif
