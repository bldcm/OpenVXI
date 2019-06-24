
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
 

 #ifndef SBINETHTTPCACHESTREAM_HPP
 #define SBINETHTTPCACHESTREAM_HPP
 
 /**
  * This class is responsible for reading information from a cache if
  * available and non-expired.  Otherwise, it uses an SBinetHttpStream
  * to fetch information and then caches it.
  *
  **/
 
 #include "SBinetStoppableStream.hpp"
 #include "SBinetHttpStream.hpp"
 #include "VXIcache.h"
 
 class SBinetChannel;
 class SBinetValidator;
 
 class SBinetHttpCacheStream: public SBinetStoppableStream
 {
   // ................. CONSTRUCTORS, DESTRUCTOR  ............
   //
   // ------------------------------------------------------------
   /**
    * Default constructor.
    **/
  public:
   SBinetHttpCacheStream(SBinetURL* url,
                         SBinetHttpStream::SubmitMethod method,
                         SBinetChannel* ch,
                         VXIcacheInterface *cache,
                         VXIlogInterface *log, VXIunsigned diagLogBase);
 
   /**
    * Destructor.
    **/
  public:
   virtual ~SBinetHttpCacheStream();
 
   virtual VXIinetResult Open(VXIint flags,
                              const VXIMap* properties,
                              VXIMap* streamInfo);
 
   virtual VXIinetResult Read(/* [OUT] */ VXIbyte*         pBuffer,
                              /* [IN]  */ VXIulong         nBuflen,
                              /* [OUT] */ VXIulong*        pnRead );
 
   virtual VXIinetResult Write(/* [IN] */ const VXIbyte*   pBuffer,
                               /* [IN]  */ VXIulong         nBuflen,
                               /* [OUT] */ VXIulong*        pnWritten);
 
   virtual VXIinetResult Close();
 
   virtual void setTimeOut(const SWITimeStamp *timeout);
 
   /**
     * Disabled copy constructor.
    **/
  private:
   SBinetHttpCacheStream(const SBinetHttpCacheStream&);
 
   /**
     * Disabled assignment operator.
    **/
  private:
   SBinetHttpCacheStream& operator=(const SBinetHttpCacheStream&);
 
  private:
   void setProperties(const VXIMap* properties);
   VXIcacheResult openCacheRead(VXIMap *streamInfo);
   VXIcacheResult openCacheRead(SBinetValidator& validator, VXIbool& keepEntry, SBinetURL* &ultimateURL);
   VXIinetResult openCacheWrite(const VXIMap *streamInfo, const SBinetValidator& validator, const SBinetURL *ultimateURL);
   VXIcacheResult openCacheWrite(const VXIchar *absoluteURL, const SBinetValidator& validator);
   VXIcacheResult openCacheEntry(const VXIchar *absoluteURL, VXIcacheOpenMode mode, VXIMap *cacheStreamInfo);
 
   VXIinetResult cache2inetRc(VXIcacheResult result);
 
  private:
   bool _httpReadCompleted;
   VXIint32 _maxAge;
   VXIint32 _maxStale;
   SBinetHttpStream *_httpStream;
   SBinetChannel *_channel;
   VXIcacheInterface *_cache;
   VXIcacheStream *_cacheStream;
   SBinetHttpStream::SubmitMethod _method;
 };
 #endif
