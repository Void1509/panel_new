#ifndef TIMEDECODE_H
#define TIMEDECODE_H

#include "sys/time.h"

class TimeDecode
{
private:
    int     year, mon, day, hour, min, sec;
    void    decode(time_t timeSec);
public:
    TimeDecode();
    TimeDecode(time_t timeSec);
    int Year();
    int Mon();
    int Day();
    int Hour();
    int Min();
    int Sec();
};

#endif // TIMEDECODE_H
