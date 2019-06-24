
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
 #include "SWITimeWatch.hpp"
 
 SWITimeWatch::SWITimeWatch():_elapsed(0L)
 {
 }
 
 void SWITimeWatch::start()
 {
   _timestamp.setTimeStamp();
 }
 
 void SWITimeWatch::reset()
 {
   _elapsed = 0;
   _timestamp.reset();
 }
 
 void SWITimeWatch::stop()
 {
   if (_timestamp.getSecs() == 0)
   {
     return;
   }
 
   SWITimeStamp now;
   now.setTimeStamp();
   _elapsed += now.delta(_timestamp);
   _timestamp.reset();
 }
 
 unsigned long SWITimeWatch::getElapsed()
 {
   if (_timestamp.getSecs() != 0)
   {
     SWITimeStamp now;
     now.setTimeStamp();
     return _elapsed + now.delta(_timestamp);
   }
   else
   {
     return _elapsed;
   }
 }
