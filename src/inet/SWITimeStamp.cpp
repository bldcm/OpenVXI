
 /****************License************************************************
  *
  * Copyright 2000-2003.  ScanSoft, Inc.    
  *
  * Use of this software is subject to notices and obligations set forth 
  * in the SpeechWorks Public License - Software Version 1.2 which is 
  * included with this software. 
  *
  * ScanSoft is a registered trademark of ScanSoft, Inc., and OpenSpeech, 
  * SpeechWorks and the SpeechWorks logo are registered trademarks or 
  * trademarks of SpeechWorks International, Inc. in the United States 
  * and other countries.
  *
  ***********************************************************************/
 

 #include "SWIutilInternal.h"
 #include "SWITimeStamp.hpp"
 
 #ifdef _WIN32
 #include <sys/timeb.h>
 #else
 #include <sys/time.h>
 #endif
 
 SWITimeStamp::SWITimeStamp():_secs(0),_msecs(0)
 {}
 
 SWITimeStamp::~SWITimeStamp()
 {}
 
 time_t
 SWITimeStamp::getSecs() const
 {
   return _secs;
 }
 
 unsigned short
 SWITimeStamp::getMsecs() const
 {
   return _msecs;
 }
 
 void
 SWITimeStamp::reset()
 {
   _secs = 0;
   _msecs = 0;
 }
 
 /**
  * Assigns the current system time to this timestamp.
  **/
 void SWITimeStamp::setTimeStamp()
 {
 #ifdef _WIN32
   struct _timeb now;
 
   _ftime(&now);
   _secs = now.time;
   _msecs = now.millitm;
 #else
   struct timeval now;
   gettimeofday(&now, NULL);
   _secs = now.tv_sec;
   _msecs = now.tv_usec / 1000;
 #endif
 }
 
 
 /**
  * Adds this number of milliseconds to the current timestamp.
  **/
 void SWITimeStamp::addDelay(unsigned long msecs)
 {
   _secs += (time_t) msecs / 1000;
   _msecs += (unsigned short) (msecs % 1000);
 
   if (_msecs >= 1000)
   {
     _msecs -= 1000;
     _secs++;
   }
 }
 
 /**
  * Returns the number of milliseconds between this timestamp and the
  * timestamp passed as an argument.
  */
 long SWITimeStamp::delta(const SWITimeStamp& timestamp) const
 {
   long result = (_secs - timestamp._secs) * 1000;
   result += (_msecs - timestamp._msecs);
   return result;
 }
 
 int SWITimeStamp::compare(const SWITimeStamp& timestamp) const
 {
   if (_secs < timestamp._secs)
     return -1;
 
   if (_secs > timestamp._secs)
     return 1;
 
   if (_msecs < timestamp._msecs)
     return -1;
 
   if (_msecs > timestamp._msecs)
     return 1;
 
   return 0;
 }
