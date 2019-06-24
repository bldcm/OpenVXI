
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
 

 #include "SBinetStoppable.hpp"
 #ifdef _WIN32
 #include <sys/timeb.h>
 #endif
 
 void SBinetStoppable::setTimeOut(const SWITimeStamp *timestamp)
 {
   if (timestamp == NULL)
   {
     _expiration.reset();
   }
   else
   {
     _expiration = *timestamp;
   }
 }
 
 bool SBinetStoppable::hasTimedOut() const
 {
   if (_expiration.getSecs() == (time_t) 0)
     return false;
 
   SWITimeStamp now;
   now.setTimeStamp();
 
   return _expiration.compare(now) <= 0;
 }
 
 long SBinetStoppable::getDelay() const
 {
   if (_expiration.getSecs() == (time_t) 0)
     return -1;
 
   SWITimeStamp now;
   now.setTimeStamp();
 
   long result = _expiration.delta(now);
   if (result < 0) result = 0;
   return result;
 }
