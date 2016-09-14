#ifndef __ROCK_CLOCK_H__
#define __ROCK_CLOCK_H__

class RClock
{
public:
    RClock();
    ~RClock();
    void Cal(char* msg);
    void NowMs(ULONGLONG& now);   //���뼶

private:
    SYSTEMTIME m_startTime;
    SYSTEMTIME m_endTime;
};

#endif