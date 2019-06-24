
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
 

 #if _MSC_VER >= 1100    // Visual C++ 5.x
 #pragma warning( disable : 4786 4503 )
 #endif
 
 #include "SWIoutputStream.hpp"
 #include <cstdio>
 #include <cstring>
 
 // SWIoutputStream::SWIoutputStream
 // Refer to SWIoutputStream.hpp for doc.
 SWIoutputStream::SWIoutputStream()
 {}
 
 // SWIoutputStream::~SWIoutputStream
 // Refer to SWIoutputStream.hpp for doc.
 SWIoutputStream::~SWIoutputStream()
 {}
 
 SWIstream::Result SWIoutputStream::printString(const char *s)
 {
   int len = strlen(s);
   int rc = writeBytes(s, len);
   if (rc < 0) return Result(rc);
   if (rc == len) return SUCCESS;
   return WRITE_ERROR;
 }
 
 SWIstream::Result SWIoutputStream::printChar(char c)
 {
   int rc = writeBytes(&c, sizeof c);
   if (rc < 0) return Result(rc);
   return rc == sizeof c ? SUCCESS : WRITE_ERROR;
 }
 
 
 SWIstream::Result SWIoutputStream::printInt(int i)
 {
   char tmp[20];  // should be more than enough for any 32 bit integer.
   ::sprintf(tmp, "%d", i);
   return printString(tmp);
 }
 
 SWIstream::Result SWIoutputStream::flush()
 {
   return SUCCESS;
 }
 
 bool SWIoutputStream::isBuffered() const
 {
   return false;
 }
 
 SWIstream::Result SWIoutputStream::waitReady(long timeoutMs)
 {
   return SUCCESS;
 }
