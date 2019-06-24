
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
 
 #include "SBinetTimedStream.hpp"
 #include "SBinetStoppableStream.hpp"
 #include "SBinetUtils.hpp"
 #include "SBinetChannel.h"
 
 // SBinetTimedStream::SBinetTimedStream
 // Refer to SBinetTimedStream.hpp for doc.
 SBinetTimedStream::SBinetTimedStream(SBinetStoppableStream *aStream,
                                      VXIlogInterface *log,
                                      VXIunsigned diagLogBase):
   SWIutilLogger(MODULE_SBINET, log, diagLogBase),
   _stream(aStream),
   _timeoutOpen(-1),
   _timeoutIO(-1),
   _timeoutDownload(-1),
   _finalTime(NULL)
 {}
 
 // SBinetTimedStream::~SBinetTimedStream
 // Refer to SBinetTimedStream.hpp for doc.
 SBinetTimedStream::~SBinetTimedStream()
 {
   delete _finalTime;
   delete _stream;
 }
 
 VXIinetResult SBinetTimedStream::Open(VXIint flags,
                                       const VXIMap* properties,
                                       VXIMap* streamInfo)
 {
   if(!SBinetUtils::getInteger(properties,
                               INET_TIMEOUT_DOWNLOAD,
                               _timeoutDownload))
   {
     //_timeoutDownload = INET_TIMEOUT_DOWNLOAD_DEFAULT;
     _timeoutDownload = SBinetChannel::getPageLoadTimeout();
   }
 
   delete _finalTime;
 
   if (_timeoutDownload < 0)
   {
     _finalTime = NULL;
   }
   else
   {
     _finalTime = new SWITimeStamp;
     _finalTime->setTimeStamp();
     _finalTime->addDelay(_timeoutDownload);
   }
   setDelay(_timeoutOpen);
 
   VXIinetResult rc = _stream->Open(flags, properties, streamInfo);
   if (rc == VXIinet_RESULT_FETCH_TIMEOUT)
     Error(236, L"%s%i", L"Timeout", _timeoutDownload);
 
   _stream->setTimeOut(NULL);
   return rc;
 }
 
 
 VXIinetResult SBinetTimedStream::Read(/* [OUT] */ VXIbyte*         pBuffer,
                                       /* [IN]  */ VXIulong         nBuflen,
                                       /* [OUT] */ VXIulong*        pnRead )
 {
   setDelay(_timeoutIO);
   VXIinetResult rc =  _stream->Read(pBuffer, nBuflen, pnRead);
 
   if (rc == VXIinet_RESULT_FETCH_TIMEOUT)
     Error(237, L"%s%i", L"Timeout", _timeoutDownload);
 
   _stream->setTimeOut(NULL);
   return rc;
 }
 
 
 VXIinetResult SBinetTimedStream::Write(/* [IN]  */ const VXIbyte*   pBuffer,
                                        /* [IN]  */ VXIulong         nBuflen,
                                        /* [OUT] */ VXIulong*        pnWritten)
 {
   setDelay(_timeoutIO);
   VXIinetResult rc =  _stream->Write(pBuffer, nBuflen, pnWritten);
 
   if (rc == VXIinet_RESULT_FETCH_TIMEOUT)
     Error(238, L"%s%i", L"Timeout", _timeoutDownload);
 
   _stream->setTimeOut(NULL);
   return rc;
 }
 
 
 VXIinetResult SBinetTimedStream::Close()
 {
   return _stream->Close();
 }
 
 void SBinetTimedStream::setDelay(int timeoutFromNow)
 {
   if (timeoutFromNow < 0)
   {
     _stream->setTimeOut(_finalTime);
   }
   else
   {
     SWITimeStamp expirationTime;
     expirationTime.setTimeStamp();
     expirationTime.addDelay(timeoutFromNow);
 
     if (_finalTime == NULL ||
         _finalTime->compare(expirationTime) > 0)
       _stream->setTimeOut(&expirationTime);
     else
       _stream->setTimeOut(_finalTime);
   }
 }
