// ------------------------------------------------------------------------------------
// ----------------------------------Included headers----------------------------------
// ------------------------------------------------------------------------------------
#include "Timer.h"


// ------------------------------------------------------------------------------------
// ---------------------------------Member definitions---------------------------------
// ------------------------------------------------------------------------------------

PerformanceTimer::PerformanceTimer()
{
    // Retrieve frequency of timer
    LARGE_INTEGER liTemp;
    QueryPerformanceFrequency( &liTemp );
    InvFrequency = 1.0f/static_cast<float>(liTemp.QuadPart);

    Reset();
}

void PerformanceTimer::Reset()
{
    QueryPerformanceCounter( &StartTime );
}

float PerformanceTimer::GetElapsed() const
{
    LARGE_INTEGER Time;
    QueryPerformanceCounter( &Time );

    return static_cast<float>(Time.QuadPart - StartTime.QuadPart) * InvFrequency;
}