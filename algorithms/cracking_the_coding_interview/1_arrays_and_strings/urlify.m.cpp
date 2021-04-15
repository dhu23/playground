#include <iostream>
#include <string>
//#include <algorithm>
//#include <iterator>

void urlify(char input[100], int size)
{
    int numSpace = 0;
    for(int i = 0; i < size; ++i)
    {
        if(' ' == input[i])
        {
            ++numSpace;
        }
    }
    // numSpace = std::count(std::begin(input), std::begin(input)+size, ' ');
    int outputPos = size+2*numSpace; // each space turns into %20, 2 more char
    input[outputPos--] = '\0';
    
    if(0 == numSpace)
    {
        return;
    }

    for(int i = size-1; i >= 0; --i)
    {
        if(numSpace == 0)
        {
            // no more spaces left to work on.  
            break;
        }
        if(' ' != input[i])
        {
            input[outputPos--] = input[i];
        }
        else
        {
            input[outputPos--] = '0';
            input[outputPos--] = '2';
            input[outputPos--] = '%';
            --numSpace;
        }
    }
}

int main(int argc, char* argv[])
{
    char input[100] = "Mr John Smith    ";
    std::cout << std::string(input) << std::endl;
    urlify(input, 13);
    std::cout << std::string(input) << std::endl;
    return 0;
}
