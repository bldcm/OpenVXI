#ifndef SWIFILTERINPUTSTREAM_HPP
 #define SWIFILTERINPUTSTREAM_HPP
 

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
 

 #include "SWIinputStream.hpp"
 
 /**
  * Class used to filter input stream.  The basic idea is that a
  * SWIfilterInputstream is a wrapper around a SWIinputStream.  This allows to
  * add functionality to existing SWInputStream classes.  The default
  * implementation of all methods of the SWIfilterInputStream is to invoke the
  * corresponding method of the wrapped SWIinputStream
 
  **/
 
 class SWIUTIL_API_CLASS SWIfilterInputStream: public SWIinputStream
 {
   // ................. CONSTRUCTORS, DESTRUCTOR  ............
   //
   // ------------------------------------------------------------
   /**
    * Default constructor.
    **/
  public:
   SWIfilterInputStream(SWIinputStream *stream, bool ownStream = true);
 
   /**
    * Destructor.
    **/
  public:
   virtual ~SWIfilterInputStream();
 
  public:
   virtual int readBytes(void* data, int dataSize);
 
  public:
   virtual int read();
 
  public:
   virtual int skip(int n);
 
  public:
   virtual int readLine(char *buffer, int bufSize);
 
  public:
   virtual int readLine(SWIoutputStream& out);
 
  public:
   virtual int peek(int offset = 0) const;
 
  public:
   virtual int getLookAhead() const;
 
  public:
   virtual bool isBuffered() const;
 
  public:
   virtual Result waitReady(long timeoutMs = -1);
 
  public:
   virtual Result close();
 
   /**
    * Disabled copy constructor.
    **/
  private:
   SWIfilterInputStream(const SWIfilterInputStream&);
 
   /**
    * Disabled assignment operator.
    **/
  private:
   SWIfilterInputStream& operator=(const SWIfilterInputStream&);
 
  private:
   SWIinputStream *_stream;
   bool _ownStream;
 };
 #endif
