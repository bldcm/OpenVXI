
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
 
#ifndef SBINETSTREAM_H
 #define SBINETSTREAM_H
 
 #include "VXIinetStream.hpp"
 #include "SWIutilLogger.hpp"
 #include "SBinetURL.h"
 
 class SBinetStream: public VXIinetStream, public SWIutilLogger
 {
  public:
   SBinetStream(SBinetURL *url, VXIlogInterface *log, VXIunsigned diagLogBase);
 
  public:
   virtual ~SBinetStream();
 
  public:
   const SBinetURL *getURL() const
   {
     return _url;
   }
 
  public:
   virtual VXIinetResult Open(VXIint flags,
                              const VXIMap* properties,
                              VXIMap* streamInfo) = 0;
 
   virtual VXIinetResult Read(/* [OUT] */ VXIbyte*         pBuffer,
                              /* [IN]  */ VXIulong         nBuflen,
                              /* [OUT] */ VXIulong*        pnRead ) = 0;
 
   virtual VXIinetResult Write(/* [IN] */ const VXIbyte*   pBuffer,
                               /* [IN]  */ VXIulong         nBuflen,
                               /* [OUT] */ VXIulong*        pnWritten) = 0;
 
   virtual VXIinetResult Close() = 0;
 
  protected:
   void getCacheInfo(const VXIMap* properties, VXIint32& maxAge, VXIint32& maxStale);
   void getModInfo(const VXIMap *properties, time_t& lastModified, SBinetNString &etag);
   SBinetURL *_url;
 };
 
 #endif
