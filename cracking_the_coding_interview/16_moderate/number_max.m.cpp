#include <bitset>
#include <iostream>

using namespace std;

int num_max(int a, int b)
{
    int diff = a - b;
    
    constexpr size_t size = sizeof(int)*8;
    int sign = diff >> (size-1);
    
    cout << bitset<size>(a) << endl;
    cout << bitset<size>(b) << endl;
    cout << bitset<size>(diff) << endl;
    cout << bitset<size>(sign) << endl;

    return 0;
}

int main(int argc, char* argv[])
{
    cout << num_max(10, -5) << endl;
    cout << num_max(25, 21) << endl;
    cout << num_max(12, -1221) << endl;
    cout << num_max(-3, 121) << endl;
    return 0;
}
