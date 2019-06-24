
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
 
 #include "SBinetStream.hpp"
 #include "SBinetUtils.hpp"
 #include "SBinetValidator.h"
 #include "SBinetLog.h"
 
 SBinetStream::SBinetStream(SBinetURL *url, VXIlogInterface *log, VXIunsigned diagLogBase):
   SWIutilLogger(MODULE_SBINET, log, diagLogBase),
   _url(url)
 {}
 
 SBinetStream::~SBinetStream()
 {
   delete _url;
 }
 
 void SBinetStream::getCacheInfo(const VXIMap* properties, VXIint32& maxAge, VXIint32& maxStale)
 {
   // Determine whether safe caching was requested (for proxy use)
   const VXIchar* cMode = SBinetUtils::getString(properties, INET_CACHING);
 
   if (cMode != NULL && ::wcscmp(cMode, INET_CACHING_SAFE) == 0)
   {
     maxStale = maxAge = 0;
   }
   else
   {
     maxStale = maxAge = -1;
     SBinetUtils::getInteger(properties,
                             INET_CACHE_CONTROL_MAX_AGE,
                             maxAge);
 
     SBinetUtils::getInteger(properties,
                             INET_CACHE_CONTROL_MAX_STALE,
                             maxStale);
   }
 }
 
 void SBinetStream::getModInfo(const VXIMap *properties,
                               time_t& lastModified, SBinetNString &etag)
 {
   lastModified = (time_t) -1;
   etag.clear();
 
   // Determine attributes for conditional fetching.
   const VXIValue *val = VXIMapGetProperty(properties, INET_OPEN_IF_MODIFIED);
 
   if (val != NULL)
   {
     SBinetValidator validator(GetLog(), GetDiagBase());
 
     if (validator.Create(val) == VXIinet_RESULT_SUCCESS)
     {
       // Only keep the Etag if it is not a weak one.
       if (validator.isStrong())
         etag = validator.getETAG();
 
       lastModified = validator.getLastModified();
     }
   }
 }
