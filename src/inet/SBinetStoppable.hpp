
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
 
#ifndef SBINETEXPIRABLE_HPP
 #define SBINETEXPIRABLE_HPP
 
 // This class implements a simple interface to set time-out and to determine
 // whether they have expired.
 
 #include "SWITimeStamp.hpp"
 
 class SBinetStoppable
 {
  public:
   SBinetStoppable()
   {}
 
   /**
    * Destructor.
    **/
  public:
   virtual ~SBinetStoppable()
   {}
 
   /**
    * Sets the time-out.  A NULL value removes the current time-out.
    */
  public:
   virtual void setTimeOut(const SWITimeStamp *timeout);
 
  public:
   bool hasTimedOut() const;
 
  public:
   void getTimeOut(SWITimeStamp *timeout) const
   {
     *timeout = _expiration;
   }
 
  public:
   long getDelay() const;
 
  private:
   SWITimeStamp _expiration;
 };
 
 #endif
