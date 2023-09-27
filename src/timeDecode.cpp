#include "time.h"
#include "timeDecode.h"


void    TimeDecode::decode(time_t timeSec)
{
    struct tm* lt = localtime(&timeSec);
    if (!lt) {
        timeSec = time(0);
        lt = localtime(&timeSec);
    }
    if (lt) {
        year    = lt->tm_year + 1900;
        mon     = lt->tm_mon + 1;
        day     = lt->tm_mday;

        hour    = lt->tm_hour;
        min     = lt->tm_min;
        sec     = lt->tm_sec;
    } else {
        year    = 1900;
        mon     = 1;
        day     = 1;

        hour    = 0;
        min     = 0;
        sec     = 0;
    }
}

TimeDecode :: TimeDecode(time_t timeSec)    {   decode(timeSec);    }
TimeDecode :: TimeDecode()                  {   decode(time(0));    }
int TimeDecode :: Year()                    {   return year;        }
int TimeDecode :: Mon()                     {   return mon;         }
int TimeDecode :: Day()                     {   return day;         }
int TimeDecode :: Hour()                    {   return hour;        }
int TimeDecode :: Min()                     {   return min;         }
int TimeDecode :: Sec()                     {   return sec;        }
