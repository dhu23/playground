#include <stdio.h>
#include <stdint.h>


uint32_t htonf(float f)
{
    uint32_t p;
    uint32_t sign;

    if (f < 0) { sign = 1; f = -f; }
    else { sign = 0; }

    p = ((((uint32_t)f)&0x7fff) << 16) | (sign << 31); // whole part and sign
    p |= (uint32_t)(((f - (int)f) * 65536.0f)) & 0xffff; // fraction

    return p;
}

float ntohf(uint32_t p)
{
    float f = ((p>>16)&0x7fff); // whole part
    f += (p&0xffff) / 65536.0f; // fraction

    if (((p>>31)&0x1) == 0x1) { f = -f; } // sign bit set

    return f;
}

// This is sort of a naive implementation that stores a float in a 32-bit number
// The high bit (31) is used to store the sign of the number (1 means negative)
// and the next seven bits (30-16) are used to store the whole number portion
// of the float. Finally the remaining bits (15-0) are used to store the 
// fractional portion of the number. 
// 
// 65536 = 2^16

int main(void)
{
    float f = 3.1415926, f2;
    uint32_t netf;

    netf = htonf(f); // convert to "network" form
    f2 = ntohf(netf); // convert back to test

    printf("original:%f\n", f); // 3.141593
    printf("Network: 0x%08X\n", netf); // 0x0003243F
    printf("Unpacked: %f\n", f2); // 3.141586

    return 0;
}
