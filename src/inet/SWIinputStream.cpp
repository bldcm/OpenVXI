
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
 #include "SWIoutputStream.hpp"
 
 int SWIinputStream::read()
 {
   unsigned char c;
   int rc = readBytes(&c, sizeof c);
   if (rc != sizeof c) return rc;
   return c;
 }
 
 int SWIinputStream::skip(int n)
 {
   if (n < 0)
     return INVALID_ARGUMENT;
 
   if (n == 0) return 0;
 
   char *tmp = new char[n];
   int nbSkip = readBytes(tmp, n);
   delete [] tmp;
   return nbSkip;
 }
 
 int SWIinputStream::readLine(SWIoutputStream& outstream)
 {
   int rc = getLookAhead();
 
   if (rc == 0)
     return NOT_SUPPORTED;
 
   if (rc < 0)
     return rc;
 
   int nbWritten = 0;
   int c = 0;
 
   for (;;)
   {
     if ((c = read()) < 0)
     {
       rc = c;
       break;
     }
 
     switch (c)
     {
      case '\r':
        if ((c = peek()) < 0 || (c == '\n' && (c = read()) < 0))
          rc = c;
        // no break: intentional
      case '\n':
        goto end;
        break;
      default:
        outstream.printChar(c);
        nbWritten++;
        break;
     }
   }
  end:
 
   if (nbWritten == 0)
     return rc > 0 ? 0 : rc;
 
   return nbWritten;
 }
 
 int SWIinputStream::readLine(char *buffer, int bufSize)
 {
   int rc = getLookAhead();
 
   if (rc == 0)
     return NOT_SUPPORTED;
 
   if (rc < 0)
     return rc;
 
   char *p = buffer;
   char *q = buffer + bufSize - 1; // keep one byte for '\0'
   int c = 0;
 
   while (p < q)
   {
     if ((c = read()) < 0)
     {
       rc = c;
       break;
     }
 
     switch (c)
     {
      case '\r':
       if ((c = peek()) < 0 || (c == '\n' && (c = read()) < 0))
         rc = c;
       // no break: intentional
      case '\n':
        goto end;
        break;
      default:
        *p++ = c;
        break;
     }
   }
  end:
   *p = '\0';
 
   if (p == q)
     return BUFFER_OVERFLOW;
 
   int len = p - buffer;
   if (len == 0)
     return rc > 0 ? 0 : rc;
 
   return len;
 }
 
 int SWIinputStream::peek(int offset) const
 {
   return NOT_SUPPORTED;
 }
 
 int SWIinputStream::getLookAhead() const
 {
   return 0;
 }
 
 bool SWIinputStream::isBuffered() const
 {
   return false;
 }
 
 SWIstream::Result SWIinputStream::waitReady(long timeoutMs)
 {
   return SUCCESS;
 }
