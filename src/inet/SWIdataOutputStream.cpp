
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
 

 #include <SWIdataOutputStream.hpp>
 #include <cstddef>
 #include <memory.h>
 
 SWIdataOutputStream::SWIdataOutputStream(int initialCapacity):
   _bufferSize(0)
 {
   _capacity = initialCapacity;
   if (_capacity < 1) _capacity = 1;
   _buffer = new char[_capacity];
 }
 
 SWIdataOutputStream::~SWIdataOutputStream()
 {
   delete [] _buffer;
 }
 
 /**
  * @see SWIdataOutputStream::write documentation.  Warning, this method
  * is not thread-safe.  Only one thread will write to the stream.  If it is
  * not the case, one should derive a sub-class from this one, and re-implement
  * its write method.
  **/
 int SWIdataOutputStream::writeBytes(const void* data,
                                     int dataSize)
 {
   // Nothing to write.
   if (dataSize == 0)
     return 0;
 
   // Inconsistent data and dataSize
   if (dataSize < 0 || data == NULL)
     return INVALID_ARGUMENT;
 
   // No existing buffer.  Can't write.
   if (_buffer == NULL)
   {
     return WRITE_ERROR;
   }
 
   int requiredCapacity = dataSize + _bufferSize;
 
   Result rc = ensureCapacity(requiredCapacity);
   if (rc != SUCCESS) return rc;
 
   // copy the bytes from data into the dataBuffer.
   ::memcpy(_buffer + _bufferSize, data, dataSize);
 
   // set the new size of the buffer.
   _bufferSize = requiredCapacity;
 
   return dataSize;
 }
 
 SWIstream::Result SWIdataOutputStream::ensureCapacity(int requiredCapacity) const
 {
   if (requiredCapacity > _capacity)
   {
     // Make the capacity twice as much as the required capacity.  This
     // ensures an amortized linear running time.
     int oldCapacity = _capacity;
     _capacity = requiredCapacity * 2;
 
     // Create a new buffer to hold the data.
     char *tmpBuffer = new char[_capacity];
     if (tmpBuffer == NULL)
     {
       _capacity = oldCapacity;
       return WRITE_ERROR;
     }
 
     // transfer the data from the existing buffer to the new buffer, and make
     // the new buffer the current one.
     ::memcpy(tmpBuffer, _buffer, _bufferSize);
     delete [] _buffer;
     _buffer = tmpBuffer;
   }
   return SUCCESS;
 }
 
 const char * SWIdataOutputStream::getString() const
 {
   if (ensureCapacity(_bufferSize + 1) != SUCCESS)
     return NULL;
 
   _buffer[_bufferSize] = '\0';
 
   return _buffer;
 }
 
 SWIstream::Result SWIdataOutputStream::close()
 {
   return SUCCESS;
 }
 
 bool SWIdataOutputStream::isBuffered() const
 {
   return true;
 }
