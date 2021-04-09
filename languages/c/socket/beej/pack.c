#include <stdio.h>
#include <stdint.h>

// A bit background knowledge about floating-point arithmetic
//
// copied from stackexchange
//
//
// start by representing a floating-point value in a variant of scientific
// notation, using base 2 instead of base 10. For example, the value 
// 3.14159 can be represented as 
//
// 0.7853975 * 2^2
//
// 0.7853975 is the **significand**, a.k.a. the mantissa; it's the part 
// of the number containing the significant digits. This value is 
// multiplied by the base 2 raised to the power of 2 to get 3.14159.
//
// Floating-point numbers are encoded by storing the significand and
// the exponent (along with a sign bit).
//
// A typical 32-bit layout looks something like the following:
//
// 3 32222222 22211111111110000000000
// 1 09876543 21098765432109876543210
// +-+--------+-----------------------+
// | |        |                       |
// +-+--------+-----------------------+
//  ^    ^                ^
//  |    |                |
//  |    |                +-- significand (23-bit)
//  |    |
//  |    +------------------- exponent (8-bit)
//  |
//  +------------------------ sign bit
// 
//
// Like signed integer types, the high-order bit indicates sign;
// 0 indices a positive value, 1 indicates negative. 
// 
// The next 8 bits are used for the exponent. Exponents can be positive
// or negative, but instead of reserving another sign bit, they're 
// encoded such that 10000000 represents 0, so 00000000 represents -128
// and 11111111 represents 127
//
// The remaining bits are used for the signficand. Each bit represents
// a negative power of 2 counting from the left, so
//
// 01101 = 0 * 2^-1 + 1 * 2^-2 + 1 * 2^-3 + 0 * 2^-4 + 1 * 2^-5
//       = 0.25 + 0.125 + 0.03125
//       = 0.40625
//
//
//
//
// Some platforms assume a "hidden" leading bit in the significand that's
// always set to 1, so values in the significand are always between
// [0.5, 1). This allows these platforms to store values with a slightly
// greater precision (more on that below). My example doesn't do this. 
//
//
// So our value of 3.14159 would be represented as something like:
//
//    0 10000010 11001001000011111100111
//    ^     ^               ^
//    |     |               |
//    |     |               +--- significand = 0.7853975...
//    |     |
//    |     +------------------- exponent = 2 (130 - 128)
//    |
//    +------------------------- sign = 0 (positive)
//
// value = -1^(sign) * 2^(exponent) * (signficand)
//       = -1^0 * 2^2 * 0.7853975...
//       = 3.14159...
// 
//
//
// Now something you'll notice if you add up all the bits in the significand
// is that they don't total 0.7853975; they actually come out to 0.78539747. 
// There aren't quite enough bits to store the value exactly; we can only store 
// an approximation. The number of bits in the significand determines the
// precision, or how many significant digits you can store. 
//
// 23 bits give us roughly 6 decimal digits of precision. 
//
// 64-bit floating point types offer enough bits in the significand to give 
// roughly 12-15 digits of precision. But be aware that there are values that
// cannot be represented exactly no matter how many bits you use. Just as values
// like 1/3 cannot be represented in a finite number of decimal digits, values
// like 1/10 cannot be represented in a finte number of bits. Since values are 
// approximate, calculations with them are also approximate, and rounding 
// errors accumulate.
//
// The number of bits in the exponent determines the range (the minimum and 
// maximum values you can represent), but as you move towards your minimum and
// maximum values, the size of the gap between representable values increase. 
// That is, if you can't exactly present values between 0.785397 and 0.785398,
// then you can't exactly represent values between 7.85397 and 7.85398 either, 
// or values between 78.5397 and 78.5398, or values between 785397.0 and 
// 785398.0. 
//
// Be careful when **multiplying** 
// value large (in terms of magnitude) numbers by very small numbers


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
