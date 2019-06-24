#ifndef SWIFILTEROUTPUTSTREAM_HPP
 #define SWIFILTEROUTPUTSTREAM_HPP
 

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
 

 /**
  * Class used to filter output stream.  The basic idea is that a
  * SWIfilterOutputstream is a wrapper around a SWIoutputStream.  This allows to
  * add functionality to existing SWInputStream classes.  The default
  * implementation of all methods of the SWIfilterOutputStream is to invoke the
  * corresponding method of the wrapped SWIoutputStream
  *
  *
  **/
 
 #include "SWIoutputStream.hpp"
 
 class SWIUTIL_API_CLASS SWIfilterOutputStream: public SWIoutputStream
 {
   // ................. CONSTRUCTORS, DESTRUCTOR  ............
   //
   // ------------------------------------------------------------
   /**
    * Default constructor.
    **/
  public:
   SWIfilterOutputStream(SWIoutputStream *outStream,
                         bool ownStream = true);
 
   /**
    * Destructor.
    **/
  public:
   virtual ~SWIfilterOutputStream();
 
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
 
  public:
   virtual SWIstream::Result waitReady(long timeoutMs = -1);
 
  protected:
   bool isOwner() const
   {
     return _ownStream;
   }
 
   /**
    * Disabled copy constructor.
    **/
  private:
   SWIfilterOutputStream(const SWIfilterOutputStream&);
 
   /**
    * Disabled assignment operator.
    **/
  private:
   SWIfilterOutputStream& operator=(const SWIfilterOutputStream&);
 
  private:
   const bool _ownStream;
   SWIoutputStream *_stream;
 };
 #endif
