#ifndef SWIBUFFEREDOUTPUTSTREAM_HPP
 #define SWIBUFFEREDOUTPUTSTREAM_HPP
 

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
 

 #include "SWIfilterOutputStream.hpp"
 
 class SWIUTIL_API_CLASS SWIbufferedOutputStream: public SWIfilterOutputStream
 {
   // ................. CONSTRUCTORS, DESTRUCTOR  ............
   //
   // ------------------------------------------------------------
   /**
    * Creates a Writer wrapped around the stream.
    *
    * @param stream The OutputStream to be wrapped by the writer.
    * @param the size of the internal buffer to
 
    **/
  public:
   SWIbufferedOutputStream(SWIoutputStream *outStream,
                           int bufferSize = 1024,
                           bool ownStream = true);
 
   /**
    * Destructor.
    **/
  public:
   virtual ~SWIbufferedOutputStream();
 
  public:
   virtual int writeBytes(const void *data, int dataSize);
 
  public:
   bool isBuffered() const;
 
  public:
   virtual SWIstream::Result printString(const char *s);
 
  public:
   virtual SWIstream::Result printChar(char c);
 
  public:
   virtual SWIstream::Result flush();
 
  public:
   virtual SWIstream::Result close();
 
  private:
   SWIstream::Result flushBuffer();
 
   /**
    * Disabled copy constructor.
    **/
  private:
   SWIbufferedOutputStream(const SWIbufferedOutputStream&);
 
   /**
    * Disabled assignment operator.
    **/
  private:
   SWIbufferedOutputStream& operator=(const SWIbufferedOutputStream&);
 
  private:
   char *_buffer;
   char *_end;
   char *_pos;
 };
 #endif
