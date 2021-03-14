// implement a function void reverse(char* str) in C or C++ which reverses a 
// null-terminated string

// assuming there is a \0 byte somewhere. However string length or array size
// are unknown to this function

#include <cstdio>
#include <utility>

using std::swap;

void reverse(char* str)
{
    if (*str == '\0') return;
    int end = 0;
    while (*(str+end+1) != '\0') ++end;

    int start = 0;
    while (start < end)
    {
        swap(*(str+start), *(str+end));
        ++start;
        --end;
    }
}

int main(int argc, char* argv[])
{
    char s1[10] = {'s', 'a', 'm', 'p', 'l', 'e', '\0', 'a', 'a', '\0'};
    printf("sample str1 = %s\n", s1);
    reverse(s1);
    printf("reversed str1 = %s\n", s1);
    return 0;
}
