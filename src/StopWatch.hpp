#ifndef _StopWatch_hpp_
#define _StopWatch_hpp_

class StopWatch
{
    unsigned long _start;

public:
    StopWatch(void)
    {
        reset();
    }
    void reset()
    {
        _start = millis();
    }
    unsigned long Elapsed()
    {
        return millis() - _start;
    }
    operator unsigned long()
    {
        return Elapsed();
    }
    operator bool()
    {
        return Elapsed() > 0;
    }
    operator int()
    {
        return Elapsed();
    }
    operator String(){
        return String(Elapsed());
    }
    bool isTimeup(unsigned long time)
    {
        return Elapsed() >= time;
    }
    // 秒を取得
    unsigned long Second()
    {
        return Elapsed() / 1000L;
    }
};

#endif