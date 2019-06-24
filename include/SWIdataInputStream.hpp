
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
 

 #ifndef SWIdataInputStream_HPP
 #define SWIdataInputStream_HPP
 
 #include <SWIinputStream.hpp>
 
 /**
  * Class reading from an input data buffer as a stream.
  *
  *
  * @doc <p>
  * Copyright 1998, 1999, 2000, 2001 SpeechWorks International, Inc.
  *  All Rights Reserved.
  **/
 class SWIUTIL_API_CLASS SWIdataInputStream: public SWIinputStream
 {
   // ................. CONSTRUCTORS, DESTRUCTOR  ............
   //
   // ------------------------------------------------------------
   /**
    * Creates an input stream on the specified buffer. The specified buffer is
    * not copied into local memory and should not be deleted by the caller.
    *
    * @param buffer [in] The buffer from which bytes are to be read.
    * @param bufferSize [in] The size of the buffer in number of bytes.
    **/
  public:
   SWIdataInputStream(const void* buffer,
                      int bufferSize):
     _buffer((const unsigned char *) buffer),
     _bufferSize(bufferSize),
     _offset(0)
   {}
 
 
   // ------------------------------------------------------------
   /// destructor
  public:
   virtual ~SWIdataInputStream()
   {}
 
   //
   // ......... METHODS ..........
   //
   /**
    * Reads some number of bytes from the input stream and stores them into the
    * buffer array <code>buffer</code>.  The number of bytes actually read
    * is returned as an integer.
    *
    * <p> If <code>data</code> is <code>null</code>, a
    * <code>SWI_STREAM_INVALID_ARGUMENT</code> is returned.  If
    * <code>dataSize</code> of is zero, then no bytes are read and
    * <code>0</code> is returned; otherwise, there is an attempt to read at
    * least one byte. If no byte is available because the stream is at end of
    * file, the value <code>SWI_STREAM_EOF</code> is returned; otherwise, at
    * least one byte is read and stored into <code>buffer</code>. If the stream
    * is not opened, <code>SWI_STREAM_ILLEGAL_STATE</code> is returned.
    *
    * @param      data   [out] the buffer into which the data is read.
    * @param      dataSize [in] the number of bits to be read.
    * @return the total number of bytes read into the buffer, or a negative
    * number indicating there is no more data because the end of the stream has
    * been reached, or to indicate an error.
    **/
  public:
   virtual int readBytes(void* data, int dataSize);
 
   virtual int read();
 
   /**
    * Skips over and discards <code>n</code> bytes of data from this
    * input stream.  @doc The <code>skip</code> method may end up skipping over
    * some smaller number of bytes, possibly <code>0</code>.  This may result
    * from reaching end of file before <code>n</code> bytes have been skipped.
    * The actual number of bytes skipped is returned.  If <code>n</code> is
    * negative, no bytes are skipped.
    *
    * @param      n   [in] the number of bytes to be skipped.
    * @return the actual number of bytes skipped, or a negative number
    * indicating an error.
    **/
  public:
   virtual int skip(int n);
 
  public:
   virtual int peek(int offset = 0) const;
 
   virtual int getLookAhead() const;
 
   /**
    * Returns true to indicate that it is a buffered stream.
    **/
  public:
   bool isBuffered() const;
 
   /**
    * Closes this input stream and releases any system resources associated
    * with the stream.
    *
    **/
  public:
   virtual Result close();
 
  private:
   /**
    * The internal buffer that is to be parsed.
    **/
   const unsigned char *_buffer;
 
   /**
    * The size of the buffer
    **/
   int _bufferSize;
 
   /**
    * The position of the next byte to read.
    **/
   int _offset;
 };
 #endif
