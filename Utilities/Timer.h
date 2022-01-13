#ifndef TIMER_H
#define TIMER_H



// TODO:
// - Implement ITimer using ctime's functions, QueryPerformanceCounter() has very poor resolution on some CPUs


// ------------------------------------------------------------------------------------
// ----------------------------------Included headers----------------------------------
// ------------------------------------------------------------------------------------
#include <windows.h>


// ------------------------------------------------------------------------------------
// -------------------------------------Interfaces-------------------------------------
// ------------------------------------------------------------------------------------
class ITimer
{
public:
    virtual void Reset() = 0;
    virtual float GetElapsed() const = 0;
};


// ------------------------------------------------------------------------------------
// ---------------------------------------Classes--------------------------------------
// ------------------------------------------------------------------------------------

class PerformanceTimer : public ITimer
{
public:
    PerformanceTimer();

    void Reset();
    float GetElapsed() const;

private:
    LARGE_INTEGER StartTime;

    // Frequency inverse of timer
    float InvFrequency;
};



#endif