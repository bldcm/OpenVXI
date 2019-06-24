
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
 #include "SWIutilInternal.h"
 
 // SWIfilterOutputStream::SWIfilterOutputStream
 // Refer to SWIfilterOutputStream.hpp for doc.
 SWIfilterOutputStream::SWIfilterOutputStream(SWIoutputStream *outStream,
                                              bool ownStream):
   _stream(outStream), _ownStream(ownStream)
 {}
 
 // SWIfilterOutputStream::~SWIfilterOutputStream
 // Refer to SWIfilterOutputStream.hpp for doc.
 SWIfilterOutputStream::~SWIfilterOutputStream()
 {
   if (_ownStream)
   {
     close();
     delete _stream;
     _stream = NULL;
   }
 }
 
 int SWIfilterOutputStream::writeBytes(const void *data, int dataSize)
 {
   return _stream->writeBytes(data, dataSize);
 }
 
 
 bool SWIfilterOutputStream::isBuffered() const
 {
   return _stream->isBuffered();
 }
 
 
 SWIstream::Result SWIfilterOutputStream::printString(const char *s)
 {
   return _stream->printString(s);
 }
 
 
 SWIstream::Result SWIfilterOutputStream::printChar(char c)
 {
   return _stream->printChar(c);
 }
 
 
 SWIstream::Result SWIfilterOutputStream::flush()
 {
   return _stream->flush();
 }
 
 SWIstream::Result SWIfilterOutputStream::close()
 {
   return _stream->close();
 }
 
 SWIstream::Result SWIfilterOutputStream::waitReady(long timeoutMs)
 {
   return _stream->waitReady(timeoutMs);
 }
