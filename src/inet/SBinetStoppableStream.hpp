
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
 
#ifndef SBINETEXPIRABLESTREAM_HPP
 #define SBINETEXPIRABLESTREAM_HPP
 
 /**
  * This class represents a SBinetStream that is stoppable.  This means that
  * time-out can be set and inspected for expiration.  However, this class only
  * provides a simple interface for stream to be stopped.  The actual detection
  * of time-out expiration has to be done within the derived classes.
  *
  * @doc <p>
  **/
 
 #include "SBinetStream.hpp"
 #include "SBinetStoppable.hpp"
 
 class SBinetStoppableStream: public SBinetStream, public SBinetStoppable
 {
   // ................. CONSTRUCTORS, DESTRUCTOR  ............
   //
   // ------------------------------------------------------------
   /**
    * Default constructor.
    **/
  public:
   SBinetStoppableStream(SBinetURL *url, VXIlogInterface *log, VXIunsigned diagLogBase):
   SBinetStream(url, log, diagLogBase),
   SBinetStoppable()
   {}
 
   /**
    * Destructor.
    **/
  public:
   virtual ~SBinetStoppableStream()
   {}
 
   /**
     * Disabled copy constructor.
    **/
  private:
   SBinetStoppableStream(const SBinetStoppableStream&);
 
   /**
     * Disabled assignment operator.
    **/
  private:
   SBinetStoppableStream& operator=(const SBinetStoppableStream&);
 
 };
 #endif
