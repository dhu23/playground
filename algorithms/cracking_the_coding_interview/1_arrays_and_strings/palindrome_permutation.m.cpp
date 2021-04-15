#include <iostream>
#include <string>
#include <stdexcept>


// assuming the problem is only about checking low cases between a-z
bool checkPalindromPermutation(const std::string& s)
{
    // use one integer as a bit vector
    int count = 0; 
    for (char c : s)
    {
        if (c == ' ')
        {
            continue;
        }
        if (c > 'z' || c < 'a')
        {
            throw std::runtime_error("Can only handle a-z");
        }
        int mask = 1 << c-'a';
        count ^= mask;
    }
    // check if there is count bit wise has only 0 or one 1
    // 00000100000000 - 1 = 00000011111110
    return count == 0 || (count & (count-1)) == 0;
}


int main(int argc, char* argv[])
{
    std::cout << checkPalindromPermutation("tact coa") << std::endl;
    std::cout << checkPalindromPermutation("tacc coa") << std::endl;
    return 0;
}
