#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>


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


#define pack754_32(f) (pack754((f), 32, 8))
#define pack754_64(f) (pack754((f), 64, 11))
#define unpack754_32(i) (unpack754((i), 32, 8))
#define unpack754_64(i) (unpack754((i), 64, 11))


uint64_t pack754(long double f, unsigned bits, unsigned expbits)
{
    long double fnorm;
    int shift;
    long long sign, exp, significand;
    unsigned significandbits = bits -expbits - 1; // -1 for sign bit

    if (f == 0.0) return 0; // get this special case out of the way

    // check sign and begin normalization
    if (f < 0) { sign = 1; fnorm = -f; }
    else { sign = 0; fnorm = f; }

    // get the normalized form of f and track the exponent
    shift = 0;

    // dividing by 2.0 is equivalent to right shift 1 bit. 
    // to compensate we need to shift left. 
    while (fnorm >= 2.0) { fnorm /= 2.0; shift++; }
    while (fnorm < 1.0) { fnorm *= 2.0; shift--; }

    fnorm = fnorm - 1.0;

    // normalized (fnorm + 1.0) * 2^shift ---> original
    

    // calculate the binary form (non-float) of the significand
    // fnorm * 2^significandbits would give exactly its binary form value
    significand = fnorm * ((1LL<<significandbits) + 0.5f);

    // get the biased exponent
    exp = shift + ((1<<(expbits-1)) - 1); //shift + bias

    // 1. put sign put at the top bit
    // 2. put exponent part in the middle
    // 3. put significand at last
    return (sign<<(bits-1)) | (exp<<significandbits) | significand;
}


long double unpack754(uint64_t i, unsigned bits, unsigned expbits)
{
    long double result;
    long long shift;
    unsigned bias;
    unsigned significandbits = bits - expbits - 1; // -1 for sign bit

    if (i == 0) return 0.0;

    // pull the significand
    result = (i&((1LL<<significandbits)-1)); // mask
    result /= (1LL<<significandbits); // convert back to float
    result += 1.0f; // add the one back on

    // deal with the exponent
    bias = (1<<(expbits-1)) - 1;
    shift = ((i>>significandbits)&(1LL<<expbits)-1) - bias;
    while (shift > 0) { result *= 2.0; shift--; }
    while (shift < 0) { result /= 2.0; shift++; }

    // sign it
    result *= (i>>(bits-1))&1 ? -1.0 : 1.0;

    return result;
}

int main(void)
{
    float f = 3.1415926, f2;
    double d = 3.14159265358979323, d2;

    uint32_t fi;
    uint64_t di;

    fi = pack754_32(f);
    f2 = unpack754_32(fi);


    di = pack754_64(d);
    d2 = unpack754_64(di);

    printf("float before : %.7f\n", f);
    printf("float encoded : 0x%08" PRIx32 "\n", fi);
    printf("float after : %.7f\n\n", f2);

    printf("double before : %.20lf\n", d);
    printf("double encoded : 0x%016" PRIx64 "\n", di);
    printf("double after : %.20lf\n", d2);

    // prints out on my Ubuntu laptop:
    //
    // float before : 3.1415925
    // float encoded : 0x40490fda
    // float after : 3.1415925
    // 
    // double before : 3.14159265358979311600
    // double encoded : 0x400921fb54442d18
    // double after : 3.14159265358979311600


    return 0;
}
