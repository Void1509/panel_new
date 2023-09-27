#ifndef AGRO_TYPES_H
#define AGRO_TYPES_H

#include <math.h>   // floor
#include <stdint.h>

typedef unsigned char   uchar;
typedef unsigned short  ushort;
typedef unsigned int    uint;
typedef unsigned long   ulong;

typedef uint    dword;
typedef ushort  word;
typedef uchar   byte;

typedef float qreal;

#define  nullptr    0

template <typename T>
inline T agro_Abs(const T &t) { return t >= 0 ? t : -t; }

inline int agro_Round(qreal d)
{
    return d >= qreal(0.0) ? int(d + qreal(0.5)) : int(d - int(d-1) + qreal(0.5)) + int(d-1);
}

inline int agro_Floor(qreal v)
{
    return int(floor(v));
}

#endif // AGRO_TYPES_H
