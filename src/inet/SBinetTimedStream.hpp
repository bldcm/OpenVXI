
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
 
#ifndef SBINETTIMEDSTREAM_HPP
 #define SBINETTIMEDSTREAM_HPP
 
 /**
  * A SBinetTimedStream is a class that is a wrapper around a
  * SBinetStoppableStream.  The only purpose of the SBinetTimedStream is to set
  * time-out of the wrapped stream and then to invoke the request operation on
  * the wrapped stream.  The wrapped stream are actually responsible for
  * respecting time-outs.
  *
  * @doc <p>
  **/
 
 #include "VXIinetStream.hpp"
 #include "SWIutilLogger.hpp"
 #include "SBinetLog.h"
 #include "SBinetStoppable.hpp"
 class SBinetStoppableStream;
 
 class SBinetTimedStream: public VXIinetStream, public SWIutilLogger
 {
   // ................. CONSTRUCTORS, DESTRUCTOR  ............
   //
   // ------------------------------------------------------------
   /**
    * Default constructor.
    **/
  public:
   SBinetTimedStream(SBinetStoppableStream *aStream,
                     VXIlogInterface *log, VXIunsigned diagLogBase);
 
   /**
    * Destructor.
    **/
  public:
   virtual ~SBinetTimedStream();
 
   VXIinetResult Open(VXIint flags,
 		     const VXIMap* properties,
 		     VXIMap* streamInfo);
 
   VXIinetResult Read(/* [OUT] */ VXIbyte*         pBuffer,
                      /* [IN]  */ VXIulong         nBuflen,
                      /* [OUT] */ VXIulong*        pnRead );
 
   VXIinetResult Write(/* [IN]  */ const VXIbyte*   pBuffer,
                       /* [IN]  */ VXIulong         nBuflen,
                       /* [OUT] */ VXIulong*        pnWritten);
 
   VXIinetResult Close();
   /**
     * Disabled copy constructor.
    **/
  private:
   SBinetTimedStream(const SBinetTimedStream&);
 
   /**
     * Disabled assignment operator.
    **/
  private:
   SBinetTimedStream& operator=(const SBinetTimedStream&);
 
  private:
   void setDelay(VXIint32 delay);
 
  private:
   SBinetStoppableStream* _stream;
 
   VXIint32 _timeoutOpen;
   VXIint32 _timeoutIO;
   VXIint32 _timeoutDownload;
 
   SWITimeStamp *_finalTime;
 };
 #endif
