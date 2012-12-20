#ifndef TIME_H
#define TIME_H

#include <time.h>

inline timespec
timespecAdd(timespec lhs, timespec rhs)
{
    lhs.tv_nsec += rhs.tv_nsec;
    if(lhs.tv_nsec > 1000000000)
        lhs.tv_sec += rhs.tv_sec + 1;
    else
        lhs.tv_sec += rhs.tv_sec;

    return lhs;
}

inline timespec
timespecSub(timespec lhs, timespec rhs)
{
    lhs.tv_nsec -= rhs.tv_nsec;
    if(lhs.tv_nsec < 0)
    {
        lhs.tv_nsec += 1000000000;
        lhs.tv_sec = lhs.tv_sec - rhs.tv_sec - 1;
    }
    else
        lhs.tv_sec -= rhs.tv_sec;

    return lhs;
}

inline bool
timespecGreaterThan(timespec lhs, timespec rhs)
{
    return lhs.tv_sec > rhs.tv_sec || (lhs.tv_sec == rhs.tv_sec && lhs.tv_nsec > rhs.tv_nsec);
}

inline bool
timespecGreaterThanZero(timespec lhs)
{
    return lhs.tv_sec >= 0 && lhs.tv_nsec > 0;
}

inline timespec
timspecAddMiliseconds(timespec lhs, unsigned int ms)
{
    lhs.tv_nsec += (ms % 1000) * 1000000;
    if(lhs.tv_nsec >= 1000000000)
        lhs.tv_sec += ms / 1000 + 1;
    else
        lhs.tv_sec += ms / 1000;

    return lhs;
}

#endif // TIME_H
