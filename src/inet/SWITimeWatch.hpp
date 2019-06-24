#ifndef SWITIMEWATCH_HPP
 #define SWITIMEWATCH_HPP
 

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
 
 #include "SWITimeStamp.hpp"
 
 /**
  * class to facilitate computing elapsed time of operations.
  * @doc <p>
  **/
 
 class SWIUTIL_API_CLASS SWITimeWatch
 {
   // ................. CONSTRUCTORS, DESTRUCTOR  ............
   //
   // ------------------------------------------------------------
   /**
    * Default constructor.
    **/
  public:
   SWITimeWatch();
 
   /**
    * Destructor.
    **/
  public:
   ~SWITimeWatch()
   {}
 
   // Starts the timer. This sets the reference time from which all new elapsed
   // time are computed.  This does not reset the elapsed time to 0.  This is
   // useful to pause the timer.
  public:
   void start();
 
   // Stops the timer.
  public:
   void stop();
 
   // Returns the elapsed time.  If the TimeWatch is in the stopped state,
   // successive calls to getElapsed() will always return the same value.  If
   // the TimeWatch is in the started state, successive calls will return the
   // elapsed time since start() was called.
  public:
   unsigned long getElapsed();
 
   // Sets the elapsed to 0 and resets the reference time to now.
  public:
   void reset();
 
  private:
   unsigned long _elapsed;
 
  private:
   SWITimeStamp _timestamp;
 };
 
 #endif
