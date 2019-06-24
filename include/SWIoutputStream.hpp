#ifndef SWIOUTPUTSTREAM_HPP
 #define SWIOUTPUTSTREAM_HPP

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
 

 #include "SWIstream.hpp"
 
 /**
  ** Abstract class representing output stream classes.
  ** @doc <p>
  **/
 class SWIUTIL_API_CLASS SWIoutputStream: public SWIstream
 {
   // ................. CONSTRUCTORS, DESTRUCTOR  ............
   //
   // ------------------------------------------------------------
   /// default constructor
 public:
   SWIoutputStream();
 
   // ------------------------------------------------------------
   /// destructor
 public:
   virtual ~SWIoutputStream();
 
   //
   // ......... METHODS ..........
   //
   /**
    * Writes <code>bufferSize</code> bytes from the specified buffer.
    *
    * <p>
    * If <code>buffer</code> is <code>null</code>,
    * <code>SWI_STREAM_INVALID_ARGUMENT</code> is returned.
    * <p>
    *
    * @param  buffer      [in] the data to be written.
    * @param  bufferSize  [in] the number of bytes to write.
    * @return the number of bytes written, or a negative number indicating failure.
    **/
 public:
   virtual int writeBytes(const void* buffer, int bufferSize) = 0;
 
   /**
    * Flushes this output stream and forces any buffered output bytes to be
    * written out. The general contract of <code>flush</code> is that calling
    * it is an indication that, if any bytes previously written have been
    * buffered by the implementation of the output stream, such bytes should
    * immediately be written to their intended destination.
 
    * The default implementation does nothing returns
    * <code>SWI_STREAM_SUCCESS</code> Sub-classes implementing buffering should
    * re-implement this method.  <p>
    *
    *
    * @return <code>SWI_STREAM_SUCCESS</code> if succes, negative value if
    * failure.
    **/
 public:
   virtual SWIstream::Result flush();
 
   /** Indicates whether this stream is buffered or not.  Default implementation
    * returns false.  Sub classes using buffering should reimplemnent this mehod
    * to return true.
    **/
  public:
   virtual bool isBuffered() const;
 
  public:
   virtual SWIstream::Result printString(const char *s);
 
  public:
   virtual SWIstream::Result printChar(char c);
 
  public:
   virtual SWIstream::Result printInt(int i);
 
   /**
    * Waits until the stream is ready for writing bytes or the specified
    * timeout expires.
    *
    * @param timeoutMs timeout, if less than 0, no timeout, a value of 0 means
    * polling, verify the state of the stream and return immediately.
    *
    * Returns SUCCESS if the stream is ready, TIMED_OUT if the delay has
    * expired or any other Stream::Result value to indicate a failure.
    *
    * Default implementation returns SWIstream::SUCCESS.
    **/
  public:
   virtual SWIstream::Result waitReady(long timeoutMs = -1);
 
   /**
    * Closes this output stream and releases any system resources
    * associated with this stream. The general contract of <code>close</code>
    * is that it closes the output stream. A closed stream cannot perform
    * output operations.
    * <p>
    * The default implementation just returns <code>SWI_STREAM_SUCCESS</code>
    *
    * @return SWI_STREAM_SUCCESS if success, negative value if failure.
    **/
 public:
   virtual Result close() = 0;
 };
 #endif
