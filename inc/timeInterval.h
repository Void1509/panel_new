#ifndef TIMEINTERVAL_H
#define TIMEINTERVAL_H

#include <linux/input.h>
#include <time.h>


// класс определения интервала времени в микросекундах или в миллисекундах
// запускаем таймер в конструкторе или через start()
// long mksec() возвращает продолжительность в микросекундах от start()
// long msec()  возвращает продолжительность в миллисекундах от start()
//--------------------------------------------------------------------------------
class TimeInterval
{
private:
#ifdef WIN32
    long ticks;
#else
    struct timeval tv1,tv2,dtv;
    struct timezone tz;
#endif
public:
    TimeInterval()
    {
        start();
    }
    void start()
    {
    #ifdef WIN32
        ticks = GetTickCount();
    #else
        gettimeofday(&tv1, &tz);
    #endif
    }
    // возвращает продолжительность в микросекундах от start()
    long mksec()
    {
    #ifdef WIN32
        return 1000*(GetTickCount() - ticks);
    #else
        gettimeofday(&tv2, &tz);
        dtv.tv_sec  = tv2.tv_sec -tv1.tv_sec;
        dtv.tv_usec = tv2.tv_usec-tv1.tv_usec;
        if (dtv.tv_usec < 0)    {   dtv.tv_sec--;   dtv.tv_usec+=1000000L;  }
        return dtv.tv_sec*1000000L+dtv.tv_usec;
    #endif
    }
    // возвращает продолжительность в миллисекундах от start()
    long msec()
    {
    #ifdef WIN32
        return (GetTickCount() - ticks);
    #else
        gettimeofday(&tv2, &tz);
        dtv.tv_sec  = tv2.tv_sec -tv1.tv_sec;
        dtv.tv_usec = tv2.tv_usec-tv1.tv_usec;
        if (dtv.tv_usec < 0)    {   dtv.tv_sec--;   dtv.tv_usec+=1000000L;  }
        return dtv.tv_sec*1000 + dtv.tv_usec/1000;
    #endif
    }
};

#endif // TIMEINTERVAL_H
