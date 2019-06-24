
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
 

 #include "SWIfilterInputStream.hpp"
 #include "SWIinputStream.hpp"
 #include "SWIoutputStream.hpp"
 #include <string.h>
 
 SWIfilterInputStream::SWIfilterInputStream(SWIinputStream *stream,
                                            bool ownStream):
   _stream(stream),_ownStream(ownStream)
 {}
 
 SWIfilterInputStream::~SWIfilterInputStream()
 {
   _stream->close();
   if (_ownStream)
   {
     delete _stream;
     _stream = NULL;
   }
 }
 
 int SWIfilterInputStream::readBytes(void *buffer, int dataSize)
 {
   return _stream->readBytes(buffer, dataSize);
 }
 
 int SWIfilterInputStream::read()
 {
   return _stream->read();
 }
 
 int SWIfilterInputStream::skip(int n)
 {
   return _stream->skip(n);
 }
 
 int SWIfilterInputStream::readLine(SWIoutputStream& outstream)
 {
   return _stream->readLine(outstream);
 }
 
 int SWIfilterInputStream::readLine(char *buffer, int bufSize)
 {
   return _stream->readLine(buffer, bufSize);
 }
 
 int SWIfilterInputStream::peek(int offset) const
 {
   return _stream->peek(offset);
 }
 
 int SWIfilterInputStream::getLookAhead() const
 {
   return _stream->getLookAhead();
 }
 
 bool SWIfilterInputStream::isBuffered() const
 {
   return _stream->isBuffered();
 }
 
 SWIstream::Result SWIfilterInputStream::close()
 {
   return _stream->close();
 }
 
 SWIstream::Result SWIfilterInputStream::waitReady(long timeoutMs)
 {
   return _stream->waitReady(timeoutMs);
 }
