#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>

#include <stdint.h> // defines int*_t, uint*_t types
#include <inttypes.h> // for PRIdxxx

// macros for packing floats and doubles:
#define pack754_16(f) (pack754((f), 16, 5))
#define pack754_32(f) (pack754((f), 32, 8))
#define pack754_64(f) (pack754((f), 64, 11))

#define unpack754_16(i) (unpack754((i), 16, 6))
#define unpack754_32(i) (unpack754((i), 32, 8))
#define unpack754_64(i) (unpack754((i), 64, 11))

// pack754() -- pack a floating point number into IEEE-754 format

unsigned long long int pack754(long double f, unsigned bits, unsigned expbits)
{
    long double fnorm;
    int shift;
    long long sign, exp, significand;
    unsigned significandbits = bits - expbits - 1; // -1 for sign bit

    if (f == 0.0) return 0; // get this special case out of the way

    // check sign and begin normalization
    if (f < 0) { sign = 1; fnorm = -f; }
    else { sign = 0; fnorm = f; }

    // get the normalized form of f and track the exponent
    shift = 0;
    while (fnorm >= 2.0) { fnorm /= 2.0; shift++; }
    while (fnorm < 1.0) { fnorm *= 2.0; shift--; }
    fnorm = fnorm - 1.0;

    // calculate the binary form (non-float) of the significand data
    significand = fnorm * ((1LL<<significandbits) + 0.5f);

    // get the biased exponent
    exp = shift + ((1<<(expbits-1)) - 1); // shift + bias

    return (sign<<(bits-1)) | (exp<<(bits-expbits-1)) | significand;
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
    result += 1.0f; //add the one back on

    // deal with the exponent
    bias = (1<<(expbits-1))-1;
    shift = ((i>>significandbits)&((1LL<<expbits)-1)) - bias;
    while (shift > 0) { result *= 2.0; shift--; }
    while (shift < 0) { result /= 2.0; shift++; }

    // sign it
    result *= (1>>(bits-1))&1 ? -1.0 : 1.0;

    return result;
}

// -----------------------------------------------------------
// I think maybe the author used a different architecture where
// int is 16-bit?? and long int is 32-bit??
// so I changed the signature of the functions, using int16, int32 etc


// packi16() -- store a 16-bit int into a char buffer like htons
void packi16(unsigned char *buf, uint16_t i)
{
    *buf++ = i>>8; 
    *buf++ = i;
}

// packi32 -- store a 32-bit int into a char buffer like htonl
void packi32(unsigned char *buf, uint32_t i)
{
    *buf++ = i>>24;
    *buf++ = i>>16;
    *buf++ = i>>8;
    *buf++ = i;
}

// packi64 -- store a 64-bit int into a char buff like htonl
void packi64(unsigned char *buf, uint64_t i)
{
    *buf++ = i>>56;
    *buf++ = i>>48;
    *buf++ = i>>40;
    *buf++ = i>>32;
    *buf++ = i>>24;
    *buf++ = i>>16;
    *buf++ = i>>8;
    *buf++ = i;
}

// I swapped the order of the definition to be slightly different from
// the original so functions get reusagable

// unpacku16 -- unpack a 16 bit int from a char buffer like ntohs
uint16_t unpacku16(unsigned char *buf)
{
    return ((uint16_t)buf[0] << 8) | buf[1];
}

// unpacki16 - unpack a 16 bit int from a char buffer like ntohs
int16_t unpacki16(unsigned char *buf)
{
    uint16_t i2 = unpacku16(buf);
    int16_t i;

    // change unsigned numbers to signed
    // 0x7fffu ==> 0111 1111 1111 1111
    if (i2 <= 0x7fffu) { i = i2; }
    else { i = -1 - (int16_t)(0xffffu - i2); }

    return i;
}

// unpacku32 - unpack a 32 bit unsigned from a char buffer like ntohl
uint32_t unpacku32(unsigned char *buf)
{
    return
        ((uint32_t)buf[0] << 24) |
        ((uint32_t)buf[1] << 16) |
        ((uint32_t)buf[2] << 8) |
        buf[3];
}

// unpacki32 - unpack a 32 bit signed int from a char buffer like ntohl
int32_t unpacki32(unsigned char *buf)
{
    uint32_t i2 = unpacku32(buf);

    int32_t i;
    //change unsigned numbers to signed
    if (i2 <= 0x7fffffffu) { i = i2; }
    else { i = -1 - (int32_t)(0xfffffffu - i2); }

    return i;
}

// unpacku64 - unpack a 64 bit unsigned from a char buffer like ntohl
uint64_t unpacku64(unsigned char *buf)
{
    return
        ((uint64_t)buf[0] << 56) |
        ((uint64_t)buf[0] << 48) |
        ((uint64_t)buf[0] << 40) |
        ((uint64_t)buf[0] << 32) |
        ((uint64_t)buf[0] << 24) |
        ((uint64_t)buf[0] << 16) |
        ((uint64_t)buf[0] << 8) |
        buf[7];
}

// unpacki64 - unpack a 64 bit int from a char buffer like ntohl
int64_t unpacki64(unsigned char *buf)
{
    uint64_t i2 = unpacku64(buf);
    
    int64_t i;

    // change unsigned numbers to signed
    if (i2 <= 0x7fffffffffffffffu) { i = i2; }
    else { i = -1 - (int64_t)(0x7fffffffffffffffu - i2); }

    return i;
}

// pack - store data dictated by the format string in the buffer
//
//  bits | signed    unsigned    float   string
//  -----+--------------------------------------
//     8 |      c           C       
//    16 |      h           H        
//    32 |      i           I        f
//    64 |      l           L        d
//   128 |                           g 
//     - |                                    s
//
// 16 bit unsigned length is automatically prepended to string


// need to see how it is used to make sense
unsigned int pack(unsigned char *buf, char *format, ...)
{
    va_list ap;

    signed char c; // 8 bit
    unsigned char C;

    short h; // 16 bit
    unsigned short H;

    int i; // 32 bit
    unsigned int I; // 32 bit

    long l; // 64 bit
    unsigned long L; 

    float f; // 32 bit
    double d; // 64 bit
    long double g; // 128 bit

    unsigned long fhold;

    char *s;
    unsigned short len; // 16-bit value for length
    unsigned int size = 0;

    va_start(ap, format);

    for (; *format != '\0'; format++) {
        printf("size=%d\n", size);
        switch (*format) {
        case 'c': // 8 bit
            size += 1;
            c = (signed char)va_arg(ap, int); // promoted
            *buf++ = c;
            break;

        case 'C': // 8 bit unsigned
            size += 1;
            C = (unsigned char)va_arg(ap, unsigned int); // promoted
            *buf++ = C;
            break;

        case 'h': // 16 bit unsigned
            size += 2;
            h = (short)va_arg(ap, int); 
            // h = va_arg(ap, short); 
            // when I used short, compiler says 
            // short int is promoted to int when passed through '...'
            packi16(buf, h);
            buf += 2;
            break;

        case 'H': // 16 bit unsigned
            size += 2;
            H = (unsigned short)va_arg(ap, unsigned int);
            // H = va_arg(ap, unsigned short);
            packi16(buf, H);
            buf += 2;
            break;

        case 'i': // 32-bit
            size += 4;
            i = va_arg(ap, int);
            packi32(buf, i);
            buf += 4;
            break;

        case 'I': // 32-bit unsigned
            size += 4;
            I = va_arg(ap, unsigned int);
            packi32(buf, I);
            buf += 4;
            break;

        case 'l': // 64 bit 
            size += 8;
            l = va_arg(ap, long);
            packi64(buf, l);
            buf += 8;
            break;

        case 'L': // 64 bit unsigned
            size += 8;
            L = va_arg(ap, unsigned long);
            packi64(buf, L);
            buf += 8;
            break;

        case 'f': // float 32
            size += 4;
            f = (float)va_arg(ap, double); // promoted
            fhold = pack754_32(f); // convert to IEEE 754
            packi32(buf, fhold);
            buf += 4;
            break;

        case 'd': // float 64
            size += 8;
            d = va_arg(ap, double);
            fhold = pack754_64(d); // convert to IEEE 754
            packi64(buf, fhold);
            buf += 8;
            break;

        case 'g': // float 128
            printf("cannot put 128 float into an int type\n");
            break;

        case 's': // string
            s = va_arg(ap, char*);
            len = strlen(s);
            size += len + 2; // 2 for the length, as a 16 bit int
            
            packi16(buf, len);
            buf += 2;
            memcpy(buf, s, len);
            buf += len;
            break;
        }
    }
    va_end(ap);

    return size;
}

// a bunch of points that has its memory allocated would get passed in
// need to see how it is used to make sense
void unpack(unsigned char *buf, char *format, ...)
{
    va_list ap;

    signed char *c; // 8 bit
    unsigned char *C;

    short *h; // 16 bit
    unsigned short *H; 

    int *i; // 32 bit
    unsigned int *I;

    long *l; // 64 bit
    unsigned long *L;

    float *f; // floats
    double *d;
    long double *g;

    unsigned long fhold;
    
    char *s;
    unsigned int len, maxstrlen=0, count;

    va_start(ap, format);

    for (; *format != '\0'; format++) {
        switch (*format) {
        case 'c': // 8 bit
            c = va_arg(ap, signed char*);
            if (*buf <= 0x7f) { *c = *buf; } // re-sign
            else { *c = -1 - (unsigned char)(0xffu - *buf); }
            buf++;
            break;

        case 'C': // 8 bit unsigned
            C = va_arg(ap, unsigned char*);
            *C = *buf++;
            break;

        case 'h': // 16-bit
            h = va_arg(ap, short*);
            *h = unpacki16(buf);
            buf += 2;
            break;

        case 'H': // 16 bit unsigned
            H = va_arg(ap, unsigned short*);
            *H = unpacku16(buf);
            buf += 2;
            break;

        case 'i': // 32 bit
            i = va_arg(ap, int*);
            *i = unpacki32(buf);
            buf += 4;
            break;

        case 'I': // 32-bit unsigned
            I = va_arg(ap, unsigned int*);
            *I = unpacku32(buf);
            buf += 4;
            break;

        case 'l': // 64 bit
            l = va_arg(ap, long*);
            *l = unpacki64(buf);
            buf += 8;
            break;

        case 'L': // 64 bit unsigned
            L = va_arg(ap, unsigned long*);
            *L = unpacku64(buf);
            buf += 8;
            break;

        case 'f': // float
            f = va_arg(ap, float*);
            fhold = unpacku32(buf);
            *f = unpack754_16(fhold);
            buf += 4;
            break;

        case 'd': // float 64
            d = va_arg(ap, double*);
            fhold = unpacku64(buf);
            *d = unpack754_64(fhold);
            buf += 8;
            break;

        case 's': // string
            s = va_arg(ap, char*);
            len = unpacku16(buf);
            buf += 2;

            if (maxstrlen > 0 && len >= maxstrlen) { count = maxstrlen; }
            else { count = len; }

            memcpy(s, buf, count);
            s[count] = '\0';
            buf += len;
            break;

        default:
            if (isdigit(*format)) { // track max str len
                maxstrlen = maxstrlen * 10 + (*format-'0');
            }
        }

        if (!isdigit(*format)) { maxstrlen = 0; }
    }
}


int main(void)
{
    // -------- print out int type length on my laptop -----------
    printf("signed char:%ld\n", sizeof(signed char)); // 1
    printf("unsigned char:%ld\n", sizeof(unsigned char)); // 1
    
    printf("short:%ld\n", sizeof(short)); // 2
    printf("unsigned short:%ld\n", sizeof(unsigned short)); // 2

    printf("int:%ld\n", sizeof(int)); // 4
    printf("unsigned int:%ld\n", sizeof(unsigned int)); // 4

    printf("long:%ld\n", sizeof(long)); // 8
    printf("unsigned long:%ld\n", sizeof(unsigned long)); // 8

    printf("long long:%ld\n", sizeof(long long)); // 8
    printf("unsigned long long:%ld\n", sizeof(unsigned long long)); // 8

    printf("int8_t:%ld\n", sizeof(int8_t)); // 1
    printf("uint8_t:%ld\n", sizeof(uint8_t)); // 1

    printf("int16_t:%ld\n", sizeof(int16_t)); // 2
    printf("uint16_t:%ld\n", sizeof(uint16_t)); // 2

    printf("int32_t:%ld\n", sizeof(int32_t)); // 4
    printf("uint32_t:%ld\n", sizeof(uint32_t)); // 4

    printf("int64_t:%ld\n", sizeof(int64_t)); // 8
    printf("uint64_t:%ld\n", sizeof(uint64_t)); // 8

    // ------- print out floating point type length on my laptop
    printf("float:%ld\n", sizeof(float)); // 4
    printf("double:%ld\n", sizeof(double)); // 8
    printf("long double:%ld\n", sizeof(long double)); // 16

    // packing and unpacking 
    unsigned char buf[1024];
    char *s = "Great unmitigated Zot! you've found the Runestaff!";
    int16_t packetsize, ps2;

    packetsize = pack(
        buf, "chhisf", 
        (int8_t)'B', (int16_t)0, (int16_t)37, (int32_t)-5, 
        s, (float)-3490.6677);

    packi16(buf+1, packetsize); // store packet size in packet for kicks
    printf("packet is %" PRId32 " bytes\n", packetsize); // should be 65
    
    // I am not following the old c style of declaring at the top...
    int8_t magic;
    int16_t monkeycount;
    int32_t altitude;
    float absurdityfactor;
    char s2[96];

    unpack(
        buf, "chhi96sf", 
        &magic, &ps2, &monkeycount, &altitude, s2, &absurdityfactor);
    printf(
        "'%c' %" PRId32" %" PRId16 " %" PRId32 " \"%s\" %f\n",
        magic, ps2, monkeycount, altitude, s2, absurdityfactor);
    // prints
    // 'B' 65 37 "Great unmitigated Zot! you've found the Runestaff!" 0.001310

    return 0;
}


