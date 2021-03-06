
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
 

 #ifndef SWIdataOutputStream_HPP
 #define SWIdataOutputStream_HPP
 
 #include "SWIoutputStream.hpp"
 
 #define SWI_BUFFER_OUTPUT_STREAM_DEFAULT_SIZE 512
 
 /**
  * This class represents an output stream that writes to a buffer.  The
  * <code>write</code> method causes the buffer to grow if it cannot hold the
  * new data.  Once the stream is completed, the buffer can be retrieved using
  * the <code>getBuffer</code> method and the number of bytes in the buffer is
  * retrieved using the <code>getSize</code> method.  The memory buffer is
  * deleted once the output stream is deleted.  Therefore, the buffer should be
  * copied, if it is required after the stream is deleted.
  *
  * @doc <p> Copyright 1998, 1999, 2000, 2001 SpeechWorks International, Inc.
  * All Rights Reserved.
  **/
 class SWIUTIL_API_CLASS SWIdataOutputStream: public SWIoutputStream
 {
   // ................. CONSTRUCTORS, DESTRUCTOR  ............
   //
   /**
    * Creates an empty SWIdataOutputStream whose initial capacity is equal to
    * the specified capacity.  If the capacity is less than 1, the initial
    * capacity is set to 1.
    *
    * @param capacity The initial capacity required for the internal buffer.
    **/
  public:
   SWIdataOutputStream(int capacity = SWI_BUFFER_OUTPUT_STREAM_DEFAULT_SIZE);
 
   // ------------------------------------------------------------
   /// destructor
  public:
   virtual ~SWIdataOutputStream();
 
   //
   // ......... METHODS ..........
   //
   /**
    * Writes <code>bufferSize</code> bytes from the specified buffer.
    *
    * <p> If <code>dataSize</code> is less than <code>0</code>, or
    * <code>data</code> is <code>null</code> and <code>dataSize</code> is not
    * <code>0</code>, then <code>SWI_STREAM_INVALID_ARGUMENT</code> is
    * returned.
    *
    * @param    data   [in] the data to be written.
    * @param    dataSize [in]  the number of bytes to write.
    * @return   the number of bytes written, or a negative number indicating failure.
    **/
  public:
   virtual int writeBytes(const void* data, int dataSize);
 
   /**
    * Returns a pointer to the internal buffer of the stream.  Since only a
    * pointer is returned, the pointer return should not be modified.
    *
    * @return a pointer to the internal buffer of the stream.
    **/
  public:
   const void *getBuffer() const
   {
     return _buffer;
   }
 
   /**
    * Returns the buffer as a C-string with a NULL character at the end.
    **/
   const char *getString() const;
 
  public:
   /**
    * Returns the number of bytes in the internal buffer of the stream.
    *
    * @return the number of bytes in the internal buffer of the stream.
    **/
   int getSize() const
   {
     return _bufferSize;
   }
 
  public:
   /**
    * Resets the size of the buffer to 0.
    **/
   void reset()
   {
     _bufferSize = 0;
   }
 
  public:
   virtual Result close();
   virtual bool isBuffered() const;
 
  private:
   Result ensureCapacity(int requiredCapacity) const;
 
   // members.
   /**
    * The internal buffer for the stream.
    **/
  private: mutable char *_buffer;
 
   /**
    *  The current capacity of the buffer.
    **/
  private: mutable int _capacity;
 
   /**
    * The number of bytes written so far into the buffer.
    **/
  private: mutable int _bufferSize;
 
   //
   // ......... operators  ........
   //
 
 };
 #endif
