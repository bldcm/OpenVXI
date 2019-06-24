
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
 

 #include "SWIbufferedOutputStream.hpp"
 #include <cstdio>
 
 SWIbufferedOutputStream::SWIbufferedOutputStream(SWIoutputStream *outStream,
                                                  int bufferSize,
                                                  bool ownStream):
   SWIfilterOutputStream(outStream, ownStream),
   _buffer(NULL), _pos(NULL), _end(NULL)
 {
   if (bufferSize < 1) bufferSize = 1;
   _buffer = new char[bufferSize];
   _pos = _buffer;
   _end = _pos + bufferSize;
 }
 
 SWIbufferedOutputStream::~SWIbufferedOutputStream()
 {
   flush();
   if (isOwner()) close();
 }
 
 SWIstream::Result SWIbufferedOutputStream::close()
 {
   Result rc1 = flush();
   Result rc2 = SWIfilterOutputStream::close();
 
   delete [] _buffer;
   _pos = _end = _buffer = NULL;
   return rc1 == SUCCESS ? rc2 : rc1;
 }
 
 
 int SWIbufferedOutputStream::writeBytes(const void *data, int dataSize)
 {
   if (_buffer == NULL) return SWIstream::ILLEGAL_STATE;
   const char *p = static_cast<const char *>(data);
   const char *q = p + dataSize;
   int nbWritten = 0;
 
   while (p < q)
   {
     while (_pos < _end && p < q)
     {
       *_pos++ = *p++;
       nbWritten++;
     }
 
     SWIstream::Result rc;
     if (p < q && (rc = flushBuffer()) != SWIstream::SUCCESS)
       return rc;
   }
   return nbWritten;
 }
 
 SWIstream::Result SWIbufferedOutputStream::printString(const char *s)
 {
   if (_buffer == NULL) return SWIstream::ILLEGAL_STATE;
 
   const char *p = s;
 
   while (*p)
   {
     while (_pos < _end && *p) *_pos++ = *p++;
 
     SWIstream::Result rc;
     if (*p && (rc = flushBuffer()) != SWIstream::SUCCESS)
       return rc;
   }
   return SWIstream::SUCCESS;
 }
 
 SWIstream::Result  SWIbufferedOutputStream::printChar(char c)
 {
   if (_buffer == NULL) return SWIstream::ILLEGAL_STATE;
 
   SWIstream::Result rc;
   if (_pos == _end && (rc = flushBuffer()) != SWIstream::SUCCESS)
     return rc;
 
   *_pos++ = c;
   return SWIstream::SUCCESS;
 }
 
 SWIstream::Result SWIbufferedOutputStream::flush()
 {
   if (_buffer == NULL) return SWIstream::ILLEGAL_STATE;
   return flushBuffer();
 }
 
 bool SWIbufferedOutputStream::isBuffered() const
 {
   return true;
 }
 
 SWIstream::Result SWIbufferedOutputStream::flushBuffer()
 {
   int len = _pos - _buffer;
   _pos = _buffer;
 
   if (len == 0)
     return SWIstream::SUCCESS;
 
   int rc = SWIfilterOutputStream::writeBytes(_buffer, len);
 
   if (len == rc)
     return SWIstream::SUCCESS;
 
   if (rc >= 0)
     return SWIstream::WRITE_ERROR;
 
   return SWIstream::Result(rc);
 }
