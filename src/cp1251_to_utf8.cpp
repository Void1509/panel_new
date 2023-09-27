#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "cp1251_to_utf8.h"
#include "utf8_to_cp1251.h"

#include <string>

static const short utf[ 256 ] = {
    0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf,0x10,0x11,0x12,0x13,0x14,0x15,0x16,
    0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,0x20,0x21,0x22,0x23,0x24,0x25,0x26,
    0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,0x30,0x31,0x32,0x33,0x34,0x35,0x36,
    0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,0x40,0x41,0x42,0x43,0x44,0x45,0x46,
    0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,
    0x57,0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f,0x60,0x61,0x62,0x63,0x64,0x65,0x66,
    0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,0x75,0x76,
    0x77,0x78,0x79,0x7a,0x7b,0x7c,0x7d,0x7e,0x7f,0x402,0x403,0x201a,0x453,0x201e,
    0x2026,0x2020,0x2021,0x20ac,0x2030,0x409,0x2039,0x40a,0x40c,0x40b,0x40f,0x452,
    0x2018,0x2019,0x201c,0x201d,0x2022,0x2013,0x2014,0,0x2122,0x459,0x203a,0x45a,
    0x45c,0x45b,0x45f,0xa0,0x40e,0x45e,0x408,0xa4,0x490,0xa6,0xa7,0x401,0xa9,0x404,
    0xab,0xac,0xad,0xae,0x407,0xb0,0xb1,0x406,0x456,0x491,0xb5,0xb6,0xb7,0x451,
    0x2116,0x454,0xbb,0x458,0x405,0x455,0x457,0x410,0x411,0x412,0x413,0x414,0x415,
    0x416,0x417,0x418,0x419,0x41a,0x41b,0x41c,0x41d,0x41e,0x41f,0x420,0x421,0x422,
    0x423,0x424,0x425,0x426,0x427,0x428,0x429,0x42a,0x42b,0x42c,0x42d,0x42e,0x42f,
    0x430,0x431,0x432,0x433,0x434,0x435,0x436,0x437,0x438,0x439,0x43a,0x43b,0x43c,
    0x43d,0x43e,0x43f,0x440,0x441,0x442,0x443,0x444,0x445,0x446,0x447,0x448,0x449,
    0x44a,0x44b,0x44c,0x44d,0x44e,0x44f
};

char* cp1251_to_utf8(const char* str, char* res, int size)
{
    int j = 0;
    while (*str) {
        unsigned short c = utf[(unsigned char)*str++];
        if (c < 0x80) {
            if (j + 2 >= size) {    // следующий символ не входит
                break;
            }
            res[ j++ ] = c;
        }
        else if(c < 0x800)
        {
            if (j + 3 >= size) {    // следующий символ не входит
                break;
            }
            res[ j++ ] = (c >> 6) | 0xc0;
            res[ j++ ] = (c & 0x3f) | 0x80;
        }
    }
    res[ j ] = '\0';
    return res;
}

std::string std_cp1251_to_utf8(const char *ps)
{
    std::string ss;
    if (ps) {
        int size = 4*strlen(ps);
        char* res = new char[size];
        cp1251_to_utf8(ps, res, size);
        ss = res;
        delete[] res;
    }
    return ss;
}

std::string std_utf8_to_cp1251(const char* ps)
{
    std::string ss;
    if (ps) {
        int size = strlen(ps);
        char* cp1251 = new char[size];
        utf8_to_cp1251(ps, cp1251);
        ss = cp1251;
        delete[] cp1251;
    }
    return ss;
}


