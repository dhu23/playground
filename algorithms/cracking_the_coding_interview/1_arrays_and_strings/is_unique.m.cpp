#include <string>
#include <iostream>
#include <array>

// assuming ASCII characters
bool isUnique(const std::string& s)
{
    if (s.size() > 128) 
    {
        // pigeon hole principle
        return false;
    }

    std::array<int, 128> count;
    for (const char c : s)
    {
        count[c-'\0'] += 1;
        if (count[c-'\0'] > 1)
        {
            return false;
        }
    }
    return true;
}

int main(int argc, char* argv[])
{
    std::cout << isUnique("implementation") << std::endl;
    std::cout << isUnique("abcdefg") << std::endl;
    return 0;
}
