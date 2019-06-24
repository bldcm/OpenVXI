#ifndef SWITIMESTAMP_HPP
 #define SWITIMESTAMP_HPP
 

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
 

 #include "SWIutilHeaderPrefix.h"
 
 #include <time.h>
 
 class SWIUTIL_API_CLASS SWITimeStamp
 {
   // ................. CONSTRUCTORS, DESTRUCTOR  ............
   //
   // ------------------------------------------------------------
   /**
    * Default constructor.  The current timestamp is initialized to 0.0.
    **/
  public:
   SWITimeStamp();
 
   /**
    * Destructor.
    **/
  public:
   ~SWITimeStamp();
 
   /**
    * Assigns the current system time to this timestamp.
    **/
  public:
    void setTimeStamp();
 
   /**
    * Adds this number of milliseconds to the current timestamp.
    **/
  public:
   void addDelay(unsigned long msecs);
 
   /**
    * Returns the number of milliseconds between this timestamp and the
    * timestamp passed as an argument.
    */
  public:
   long delta(const SWITimeStamp& timestamp) const;
 
  public:
   time_t getSecs() const;
 
   /**
    * Returns -1,0,1 depending on whether this timestamp is smaller than, equal
    * to, or greater than the specified timestamp.
    **/
  public:
   int compare(const SWITimeStamp& timestamp) const;
 
  public:
   unsigned short getMsecs() const;
 
  public:
   void reset();
 
  private:
   time_t _secs;
   unsigned short _msecs;
 };
 
 #endif
