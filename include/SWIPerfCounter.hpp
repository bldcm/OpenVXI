#ifndef SWIPERFCOUNTER_H
#define SWIPERFCOUNTER_H

#ifdef _WIN32
#include <windows.h>
#include <limits.h>

class SWIPerfCounter
{

public:

  SWIPerfCounter():initialized(false)
  {
    hThread = GetCurrentThread();
    if (QueryPerformanceFrequency( &PerformanceFreq) != 0)
      initialized = true;
  }

  double GetTime()
  {
    LARGE_INTEGER Count;

    if (initialized)
    {
      QueryPerformanceCounter(&Count);
      return ((double)Count.HighPart *
              ((UINT_MAX + 1.0) / (double)PerformanceFreq.LowPart))
        + (double)Count.LowPart/(double)PerformanceFreq.LowPart;

    }
    else
    {
      return(-1.0);
    }
  }

  double GetCPU()
  {
    FILETIME CreationTime;
    FILETIME ExitTime;
    FILETIME KernelTime;
    FILETIME UserTime;
    double kTime;
    double uTime;


    if ( initialized && GetThreadTimes(hThread,&CreationTime,&ExitTime,&KernelTime,&UserTime)) {
      kTime = ((double)KernelTime.dwHighDateTime *
              ((UINT_MAX + 1.0) / 10000000.0))
            + (double)KernelTime.dwLowDateTime/10000000.0;
      uTime = ((double)UserTime.dwHighDateTime *
              ((UINT_MAX + 1.0) / 10000000.0))
            + (double)UserTime.dwLowDateTime/10000000.0;

      return (kTime + uTime);
    } else {
      return(-1.0);
    }

  }

 private:
  HANDLE hThread;
  LARGE_INTEGER PerformanceFreq;
  LARGE_INTEGER CountStop;
  bool initialized;


};
#endif
#endif
