
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
 
 #include "VXIinet.h"
 #include "SBinetHttpCacheStream.hpp"
 #include "SBinetURL.h"
 #include "SBinetValidator.h"
 #include "SBinetChannel.h"
 #include "SBinetUtils.hpp"
 #include "SBinetString.hpp"
 #include "HttpUtils.hpp"
 
 VXIinetResult
 SBinetHttpCacheStream::cache2inetRc(VXIcacheResult result)
 {
   switch (result)
   {
    case VXIcache_RESULT_FATAL_ERROR:
      return VXIinet_RESULT_FATAL_ERROR;
    case VXIcache_RESULT_IO_ERROR:
      return VXIinet_RESULT_IO_ERROR;
    case VXIcache_RESULT_OUT_OF_MEMORY:
      return VXIinet_RESULT_OUT_OF_MEMORY;
    case VXIcache_RESULT_SYSTEM_ERROR:
      return VXIinet_RESULT_SYSTEM_ERROR;
    case VXIcache_RESULT_PLATFORM_ERROR:
      return VXIinet_RESULT_PLATFORM_ERROR;
    case VXIcache_RESULT_BUFFER_TOO_SMALL:
      return VXIinet_RESULT_BUFFER_TOO_SMALL;
    case VXIcache_RESULT_INVALID_PROP_NAME:
      return VXIinet_RESULT_INVALID_PROP_NAME;
    case VXIcache_RESULT_INVALID_PROP_VALUE:
      return VXIinet_RESULT_INVALID_PROP_VALUE;
    case VXIcache_RESULT_INVALID_ARGUMENT:
      return VXIinet_RESULT_INVALID_ARGUMENT;
    case VXIcache_RESULT_SUCCESS:
      return VXIinet_RESULT_SUCCESS;
    case VXIcache_RESULT_FAILURE:
      return VXIinet_RESULT_FAILURE;
    case VXIcache_RESULT_NON_FATAL_ERROR:
      return VXIinet_RESULT_NON_FATAL_ERROR;
    case VXIcache_RESULT_NOT_FOUND:
      return VXIinet_RESULT_NOT_FOUND;
    case VXIcache_RESULT_WOULD_BLOCK:
      return VXIinet_RESULT_WOULD_BLOCK;
    case VXIcache_RESULT_END_OF_STREAM:
      return VXIinet_RESULT_END_OF_STREAM;
    case VXIcache_RESULT_EXCEED_MAXSIZE:
      return VXIinet_RESULT_OUT_OF_MEMORY;
    case VXIcache_RESULT_ENTRY_LOCKED:
      return VXIinet_RESULT_FETCH_TIMEOUT;
    case VXIcache_RESULT_UNSUPPORTED:
      return VXIinet_RESULT_UNSUPPORTED;
    default: {
      Error(299, L"%s%i", L"VXIcacheResult", result);
      return VXIinet_RESULT_NON_FATAL_ERROR;
     }
   }
 }
 
 // Refer to SBinetHttpCacheStream.hpp for doc.
 SBinetHttpCacheStream::
 SBinetHttpCacheStream(SBinetURL* url,
                       SBinetHttpStream::SubmitMethod method,
                       SBinetChannel* ch,
                       VXIcacheInterface *cache,
                       VXIlogInterface *log,
                       VXIunsigned diagLogBase):
   SBinetStoppableStream(url, log, diagLogBase),
   _httpStream(NULL),
   _channel(ch),
   _cache(cache),
   _cacheStream(NULL),
   _method(method),
   _maxAge(-1),
   _maxStale(-1),
   _httpReadCompleted(false)
 {}
 
 // SBinetHttpCacheStream::~SBinetHttpCacheStream
 // Refer to SBinetHttpCacheStream.hpp for doc.
 SBinetHttpCacheStream::~SBinetHttpCacheStream()
 {
   Close();
 }
 
 VXIinetResult SBinetHttpCacheStream::Open(VXIint flags,
                                           const VXIMap* properties,
                                           VXIMap* streamInfo)
 {
   getCacheInfo(properties, _maxAge, _maxStale);
 
   // Determine if this is a conditional open with a validator,
   // irrelevant if caching is disabled
   const VXIValue* validatorVal =
     VXIMapGetProperty(properties, INET_OPEN_IF_MODIFIED);;
   SBinetValidator validator(GetLog(), GetDiagBase());
   if (validatorVal != NULL)
     validator.Create(validatorVal);
 
   if (!validator.isExpired(_maxAge, _maxStale))
   {
     if (streamInfo && !SBinetUtils::setValidatorInfo(streamInfo, &validator))
     {
       Error(103, NULL);
       return VXIinet_RESULT_OUT_OF_MEMORY;
     }
     return VXIinet_RESULT_NOT_MODIFIED;
   }
 
   // Check whether we can read this document from the cache.
   if (_maxAge != 0)
   {
     switch (openCacheRead(streamInfo))
     {
      case VXIcache_RESULT_SUCCESS:
        return VXIinet_RESULT_SUCCESS;
      case VXIcache_RESULT_ENTRY_LOCKED:
        Error(234, L"%s%s", L"URL", _url->getAbsolute());
        return VXIinet_RESULT_FETCH_TIMEOUT;
      default:
        break;
     }
   }
 
   SBinetURL *url = new SBinetURL(*_url);
 
   _httpStream =
     new SBinetHttpStream(url, _method, _channel, GetLog(), GetDiagBase());
 
   _httpReadCompleted = false;
 
   SWITimeStamp timeout;
   getTimeOut(&timeout);
   _httpStream->setTimeOut(&timeout);
 
   VXIinetResult rc = _httpStream->Open(flags, properties, streamInfo);
   const SBinetValidator *cacheValidator = _httpStream->getValidator();
 
   switch (rc)
   {
    case VXIinet_RESULT_SUCCESS:
      if (cacheValidator != NULL && !cacheValidator->isExpired())
        openCacheWrite(streamInfo, *cacheValidator, url);
      break;
    case VXIinet_RESULT_NOT_MODIFIED:
      // The remote server returned a NOT-MODIFIED, we have to return not
      // modified as well.  Also, if the validator returned by the HttpStream is
      // not strong or does not provide an expiration hint, we have to replace
      // it by a clone of the original one.
      if (streamInfo != NULL)
      {
        if (cacheValidator == NULL ||
            ((!cacheValidator->isStrong() &&
              cacheValidator->getExpiresTime() <= 0) &&
             (validator.isStrong() || validator.getExpiresTime() > 0)))
        {
          if (!SBinetUtils::setValidatorInfo(streamInfo, &validator))
          {
            Error(103, NULL);
            rc = VXIinet_RESULT_OUT_OF_MEMORY;
          }
        }
      }
      // no break: intentional
    default:
      Close();
      break;
   }
 
   return rc;
 }
 
 
 // This method writes the message body to the cache.
 VXIinetResult SBinetHttpCacheStream::Read(/* [OUT] */ VXIbyte*         pBuffer,
                                           /* [IN]  */ VXIulong         nBuflen,
                                           /* [OUT] */ VXIulong*        pnRead )
 {
   VXIinetResult rc = VXIinet_RESULT_NON_FATAL_ERROR;
   bool cleanCache = false;
 
   if (_httpStream != NULL)
   {
     VXIulong nbRead;
     rc = _httpStream->Read(pBuffer, nBuflen, &nbRead);
     if (pnRead != NULL) *pnRead = nbRead;
 
     if (_cacheStream != NULL)
     {
       switch (rc)
       {
        case VXIinet_RESULT_END_OF_STREAM:
          _httpReadCompleted = true;
          // no break: intentional
        case VXIinet_RESULT_SUCCESS:
          if (nbRead > 0)
          {
            VXIulong nbWritten = 0;
            // Now write the read data (message body) to the cache
            VXIcacheResult rcc = _cache->Write(_cache, pBuffer, nbRead,
                                               &nbWritten, _cacheStream);
 
            if ((rcc != VXIcache_RESULT_SUCCESS) || (nbWritten != nbRead))
            {
              if (rcc == VXIcache_RESULT_EXCEED_MAXSIZE)
              {
                Error(305, L"%s%s", L"URL", _url->getAbsolute());
                rc = VXIinet_RESULT_SUCCESS;
              }
              else
              {
                Error(232, L"%s%s%s%i%s%i%s%i", L"URL", _url->getAbsolute(),
                      L"rc", rcc, L"written", nbWritten, L"expected", nbRead);
                rc = VXIinet_RESULT_NON_FATAL_ERROR;
              }
              cleanCache = true;
            }
          }
          break;
        default:
          cleanCache = true;
          break;
       }
     }
   }
   else if (_cacheStream != NULL)
   {
     if (hasTimedOut())
     {
       Error(251, L"%s%s", L"URL", _url->getAbsolute());
       rc = VXIinet_RESULT_FETCH_TIMEOUT;
       cleanCache = true;
     }
     else
     {
       VXIcacheResult rcc = _cache->Read(_cache, pBuffer, nBuflen, pnRead, _cacheStream);
       if ((rcc != VXIcache_RESULT_SUCCESS) && (rcc != VXIcache_RESULT_END_OF_STREAM))
       {
          Error(231, L"%s%s%s%i%s%i%s%i", L"URL", _url->getAbsolute(),
                L"rc", rcc, L"read", *pnRead, L"expected", nBuflen);
          cleanCache = true;
       }
       rc = cache2inetRc(rcc);
     }
   }
   else
   {
     Error(218, L"%s%s", L"URL", _url->getAbsolute());
   }
 
   if (cleanCache)
   {
     VXIcacheResult rcc = _cache->CloseEx(_cache, FALSE, &_cacheStream);
     if (rcc != VXIcache_RESULT_SUCCESS)
       Error(239, L"%s%s%s%i", L"URL", _url->getAbsolute(), L"rc", rcc);
     _cacheStream = NULL;
   }
   return rc;
 }
 
 
 VXIinetResult SBinetHttpCacheStream::Write(/* [IN] */ const VXIbyte*   pBuffer,
                                            /* [IN]  */ VXIulong         nBuflen,
                                            /* [OUT] */ VXIulong*        pnWritten)
 {
   if (_httpStream != NULL)
   {
     return _httpStream->Write(pBuffer, nBuflen, pnWritten);
   }
   return VXIinet_RESULT_PLATFORM_ERROR;
 }
 
 
 VXIinetResult SBinetHttpCacheStream::Close()
 {
   VXIinetResult rc1 = VXIinet_RESULT_SUCCESS;
   VXIinetResult rc2 = VXIinet_RESULT_SUCCESS;
 
   VXIbool keepEntry = TRUE;
 
   if (_httpStream != NULL)
   {
     rc1 = _httpStream->Close();
     delete _httpStream;
     _httpStream = NULL;
     keepEntry = _httpReadCompleted;
   }
 
   if (_cacheStream != NULL)
   {
     VXIcacheResult rcc = _cache->CloseEx(_cache, keepEntry, &_cacheStream);
 
     if (rcc != VXIcache_RESULT_SUCCESS)
     {
       Error(239, L"%s%s%s%i", L"URL", _url->getAbsolute(), L"rc", rcc);
       rc2 = VXIinet_RESULT_FAILURE;
     }
     _cacheStream = NULL;
   }
 
   // If any of the close fails, return that one.
   return rc1 != VXIinet_RESULT_SUCCESS ? rc1 : rc2;
 }
 
 VXIcacheResult SBinetHttpCacheStream::openCacheRead(VXIMap *streamInfo)
 {
   if (_cache == NULL) return VXIcache_RESULT_FAILURE;
 
   // We need to put all the global variables at the beginning of the block
   // because of the goto for clean-up.
   const VXIchar* absoluteURL = NULL;
   VXIulong bytesRead = 0;
   VXIulong dataSize = 0;
   VXIulong dataSizeBytes = 0;
   VXIchar* mimeType = NULL;
   SBinetValidator validator(GetLog(), GetDiagBase());
   VXIbool keepEntry = TRUE;
   SBinetURL *ultimateURL = NULL;
   time_t expiresTimeStamp = (time_t) 0;
 
   VXIcacheResult rc = openCacheRead(validator, keepEntry, ultimateURL);
 
   if (rc != VXIcache_RESULT_SUCCESS)
     goto failure;
 
   absoluteURL = ultimateURL->getAbsolute();
 
   // Read the MIME type size (number of VXIchar), followed by the actual MIME
   // type
   bytesRead = 0;
   rc = _cache->Read(_cache, (VXIbyte *) &dataSize, sizeof(dataSize), &bytesRead, _cacheStream);
   if ((rc != VXIcache_RESULT_SUCCESS) || (bytesRead != sizeof(dataSize)))
   {
     Error(231, L"%s%s%s%i%s%i%s%i", "URL", absoluteURL, L"rc", rc,
           L"read", bytesRead, L"expected", sizeof(dataSize));
     rc = VXIcache_RESULT_FAILURE;
     goto failure;
   }
 
   // Now read mime type.
   mimeType = new VXIchar[dataSize + 1];
   dataSizeBytes = dataSize * sizeof(VXIchar);
 
   rc = _cache->Read(_cache, (VXIbyte *) mimeType, dataSizeBytes, &bytesRead, _cacheStream);
   if ((rc != VXIcache_RESULT_SUCCESS) || (bytesRead != dataSizeBytes))
   {
     Error(231, L"%s%s%s%i%s%i%s%i", L"URL", absoluteURL, L"rc", rc,
           L"read", bytesRead, L"expected", dataSizeBytes);
     rc = VXIcache_RESULT_FAILURE;
     goto failure;
   }
   mimeType[dataSize] = L'\0';
 
   VXIMapSetProperty(streamInfo, INET_INFO_HTTP_STATUS,
                     (VXIValue*)VXIIntegerCreate(SBinetHttpUtils::HTTP_NOT_MODIFIED));
 
   // Set the absolute URL property
   VXIMapSetProperty(streamInfo, INET_INFO_ABSOLUTE_NAME,
                     (VXIValue*)VXIStringCreate(absoluteURL));
 
   // Set the document size property
   VXIMapSetProperty(streamInfo, INET_INFO_SIZE_BYTES,
                     (VXIValue*)VXIIntegerCreate(validator.getSize()));
 
   // Set the MIME type property
   VXIMapSetProperty(streamInfo, INET_INFO_MIME_TYPE,
                     (VXIValue*)VXIStringCreate(mimeType));
 
   SBinetUtils::setValidatorInfo(streamInfo, &validator);
 
   // We leave the cache stream open until Read() or Close() is called
   rc = VXIcache_RESULT_SUCCESS;
   goto cleanup;
 
  failure:
   if (_cacheStream)
   {
     VXIcacheResult rcc = _cache->CloseEx(_cache, keepEntry, &_cacheStream);
     if (rcc != VXIcache_RESULT_SUCCESS)
       Error(239, L"%s%s%s%i", L"URL", absoluteURL, L"rc", rcc);
   }
   _cacheStream = NULL;
  cleanup:
   delete ultimateURL;
   delete [] mimeType;
   return rc;
 }
 
 // Write the document to cache. This method only stores the validator
 // and MIME type in the cache. The real document (message body) is
 // written later in Read().
 VXIinetResult
 SBinetHttpCacheStream::openCacheWrite(const VXIMap* streamInfo,
                                       const SBinetValidator& validator,
                                       const SBinetURL* ultimateURL)
 {
   // No caching.
   if (_cache == NULL) return VXIinet_RESULT_SUCCESS;
 
   const VXIchar* absoluteURL = _url->getAbsolute();
   VXIulong bytesWritten = 0;
   VXIulong dataSize = 0;
   VXIulong dataSizeBytes = 0;
   const VXIchar* mimeType = NULL;
 
   // Write the validator to cache
   VXIcacheResult rc = openCacheWrite(absoluteURL, validator);
   if (rc != VXIcache_RESULT_SUCCESS)
   {
     goto failure;
   }
 
   // Take care of redirection.
   if (*_url != *ultimateURL)
   {
     Diag(MODULE_SBINET_CACHE_TAGID, L"SBinetHttpCacheStream::OpenCacheWrite",
          L"Redirection detected, from %s to %s", absoluteURL, ultimateURL->getAbsolute());
 
     if (_cacheStream)
     {
       VXIcacheResult rcc = _cache->CloseEx(_cache, TRUE, &_cacheStream);
       if (rcc != VXIcache_RESULT_SUCCESS)
         Error(239, L"%s%s%s%i", L"URL", absoluteURL, L"rc", rcc);
     }
     _cacheStream = NULL;
     absoluteURL = ultimateURL->getAbsolute();
     // Write the validator to cache
     if ((rc = openCacheWrite(absoluteURL, validator)) != VXIcache_RESULT_SUCCESS)
       goto failure;
   }
 
   // Write the size of the of the mime-type (number of VXIchar).
   mimeType = SBinetUtils::getString(streamInfo, INET_INFO_MIME_TYPE);
   dataSize = mimeType == NULL ? 0 : ::wcslen(mimeType);
 
   bytesWritten = 0;
   rc = _cache->Write(_cache, (VXIbyte *) &dataSize, sizeof(dataSize), &bytesWritten, _cacheStream);
   if ((rc != VXIcache_RESULT_SUCCESS) || (bytesWritten != sizeof(dataSize)))
   {
     Error(232, L"%s%s%s%i%s%i%s%i", L"URL", absoluteURL, L"rc", rc,
           L"written", bytesWritten, L"expected", sizeof(dataSize));
     goto failure;
   }
 
   // Now write the mime-type.
   bytesWritten = 0;
   dataSizeBytes = dataSize * sizeof(VXIchar);
   rc = _cache->Write(_cache, (const VXIbyte *) mimeType, dataSizeBytes,
 		     &bytesWritten, _cacheStream);
   if ((rc != VXIcache_RESULT_SUCCESS) || (bytesWritten != dataSizeBytes))
   {
     Error(232, L"%s%s%s%i%s%i%s%i", L"URL", absoluteURL, L"rc", rc,
           L"written", bytesWritten, L"expected", dataSizeBytes);
     goto failure;
   }
 
   goto end;
 
  failure:
   if (_cacheStream)
   {
     VXIcacheResult rcc = _cache->CloseEx(_cache, FALSE, &_cacheStream);
     if (rcc != VXIcache_RESULT_SUCCESS)
       Error(239, L"%s%s%s%i", L"URL", absoluteURL, L"rc", rcc);
   }
   _cacheStream = NULL;
  end:
   if (rc == VXIcache_RESULT_ENTRY_LOCKED)
   {
     // This is because of a timeout on trying to lock the cache entry.  We just
     // return a a fetch timeout to cause our calling function to fail.
     Error(235, L"%s%s", L"URL", absoluteURL);
     return VXIinet_RESULT_FETCH_TIMEOUT;
   }
   else
   {
     // Something wrong occured while trying to write data to the cache entry.
     // We will assume success, but there will be no caching.
     return VXIinet_RESULT_SUCCESS;
   }
 }
 
 void SBinetHttpCacheStream::setTimeOut(const SWITimeStamp *timeOfDay)
 {
   if (_httpStream != NULL)
   {
     _httpStream->setTimeOut(timeOfDay);
   }
   SBinetStoppableStream::setTimeOut(timeOfDay);
 }
 
 VXIcacheResult
 SBinetHttpCacheStream::openCacheEntry(const VXIchar *absoluteURL,
                                       VXIcacheOpenMode mode,
                                       VXIMap *cacheStreamInfo)
 {
   VXIcacheResult rc;
   long delay;
   const long maxSleepTime = 20;
 
   static const VXIchar *moduleName = L"swi:" MODULE_SBINET;
 
   SBinetString cacheKey = moduleName;
   cacheKey += L':';
   cacheKey += absoluteURL;
 
   VXIMapHolder cacheProp; // will destroy the map for us
   VXIMapSetProperty(cacheProp.GetValue(), CACHE_CREATION_COST,
 		    (VXIValue *) VXIIntegerCreate(CACHE_CREATION_COST_FETCH));
 
   while ((rc = _cache->Open(_cache, MODULE_SBINET,
                             cacheKey.c_str(),
                             mode, CACHE_FLAG_NULL,
                             cacheProp.GetValue(), cacheStreamInfo,
 			    &_cacheStream))
          == VXIcache_RESULT_ENTRY_LOCKED)
   {
     //VXItrdThreadYield();
     delay = getDelay();
 
     if (delay == 0)
       break;
     else if (delay < 0 || delay > maxSleepTime)
       delay = maxSleepTime;
 
     SBinetHttpUtils::usleep(delay * 1000);
   }
 
   if (rc != VXIcache_RESULT_SUCCESS)
   {
     if (rc == VXIcache_RESULT_NOT_FOUND)
       Diag(MODULE_SBINET_CACHE_TAGID, L"SBinetHttpCacheStream",
            L"Not found in cache: %s", absoluteURL);
     else
       Error(230, L"%s%s%s%i", L"URL", absoluteURL, L"rc", rc);
   }
 
   return rc;
 }
 
 VXIcacheResult
 SBinetHttpCacheStream::openCacheRead(SBinetValidator& validator,
                                      VXIbool& keepEntry,
                                      SBinetURL *&ultimateURL)
 {
   VXIMap *cacheStreamInfo = VXIMapCreate();
   VXIbyte *validatorData = NULL;
   VXIcacheResult rc;
   VXIulong maxDataSize = 0;
   VXIulong bytesRead = 0;
   VXIint32 entrySizeBytes = 0;
   VXIulong dataSize = 0;
   SBinetURL *tmpURL = NULL;
   const VXIchar *absoluteURL = NULL;
 
   ultimateURL = new SBinetURL(*_url);
 
   for (;;)
   {
     absoluteURL = ultimateURL->getAbsolute();
 
     rc = openCacheEntry(absoluteURL, CACHE_MODE_READ, cacheStreamInfo);
 
     if (rc != VXIcache_RESULT_SUCCESS) break;
 
     // The document is in cache.
     Diag(MODULE_SBINET_CACHE_TAGID, L"SBinetHttpCacheStream",
          L"Found in cache: %s", absoluteURL);
 
     // Retrieve the total size of the cache entry
     if (!SBinetUtils::getInteger(cacheStreamInfo,
                                  CACHE_INFO_SIZE_BYTES, entrySizeBytes))
     {
       Error(233, L"%s%s%", L"URL", absoluteURL);
       rc = VXIcache_RESULT_FAILURE;
       break;
     }
 
     // Read the size of the validator info.
     rc = _cache->Read(_cache, (VXIbyte *) &dataSize,
                       sizeof(dataSize), &bytesRead, _cacheStream);
 
     if (rc != VXIcache_RESULT_SUCCESS || bytesRead != sizeof(dataSize))
     {
       Error(231, L"%s%s%s%i%s%i%s%i", L"URL", absoluteURL, L"rc", rc,
             L"read", bytesRead, L"expected", sizeof(dataSize));
       rc = VXIcache_RESULT_FAILURE;
       break;
     }
 
     if (dataSize > maxDataSize)
     {
       delete [] validatorData;
       validatorData = new VXIbyte[dataSize];
       maxDataSize = dataSize;
     }
 
     if (validatorData == NULL)
     {
       Error(103, NULL);
       rc = VXIcache_RESULT_FAILURE;
       break;
     }
 
     rc = _cache->Read(_cache, validatorData, dataSize, &bytesRead, _cacheStream);
     if (rc != VXIcache_RESULT_SUCCESS || bytesRead != dataSize)
     {
       Error(231, L"%s%s%s%i%s%i%s%i", L"URL", absoluteURL, L"rc", rc,
             L"read", bytesRead, L"expected", dataSize);
       rc = VXIcache_RESULT_FAILURE;
       break;
     }
 
     if (validator.Create(validatorData, dataSize) != VXIinet_RESULT_SUCCESS)
     {
       Error(215, L"%s%s", NULL);
       rc = VXIcache_RESULT_FAILURE;
       break;
     }
 
     if (validator.isExpired(_maxAge, _maxStale))
     {
       rc = VXIcache_RESULT_FAILURE;
       Diag(MODULE_SBINET_CACHE_TAGID, L"SBinetHttpCacheStream",
            L"Validator expired: %s", absoluteURL);
 
       // If the validator has expired, invalidate the cache entry.
       if (validator.isExpired())
         keepEntry = FALSE;
       break;
     }
 
     // If the validator's URL is not equals to the current ultimate URL, then
     // this is a redirect.
     absoluteURL = validator.getURL();
     if (tmpURL == NULL)
     {
       if (SBinetURL::create(absoluteURL, NULL, tmpURL) != VXIinet_RESULT_SUCCESS)
         rc = VXIcache_RESULT_FAILURE;
     }
     else
     {
       if (tmpURL->parse(absoluteURL, NULL) != VXIinet_RESULT_SUCCESS)
         rc = VXIcache_RESULT_FAILURE;
     }
     if (rc != VXIcache_RESULT_SUCCESS)
     {
       Diag(MODULE_SBINET_CACHE_TAGID,
            L"SBinetHttpCacheStream::OpenCacheRead",
            L"Invalid URL: <%s>", absoluteURL);
       break;
     }
 
     // Compare URLs.
     if (*ultimateURL == *tmpURL)
     {
       // No redirection.
       break;
     }
 
     // Redirection, close current stream and continue.
     Diag(MODULE_SBINET_CACHE_TAGID, L"SBinetHttpCacheStream::OpenCacheRead",
          L"Redirection detected, from %s to %s", ultimateURL->getAbsolute(), absoluteURL);
     *ultimateURL = *tmpURL;
     VXIcacheResult rcc = _cache->CloseEx(_cache, TRUE, &_cacheStream);
     if (rcc != VXIcache_RESULT_SUCCESS)
       Error(239, L"%s%s%s%i", L"URL", absoluteURL, L"rc", rcc);
     _cacheStream = NULL;
   }
 
   delete [] validatorData;
   delete tmpURL;
   if (cacheStreamInfo) VXIMapDestroy(&cacheStreamInfo);
   if (rc != VXIcache_RESULT_SUCCESS)
   {
     delete ultimateURL;
     ultimateURL = NULL;
   }
   return rc;
 }
 
 // This method writes the validator to the cache.
 VXIcacheResult
 SBinetHttpCacheStream::openCacheWrite(const VXIchar *absoluteURL,
                                       const SBinetValidator& validator)
 {
   // Check whether we already cached this URL.
   VXIcacheResult rc = openCacheEntry(absoluteURL, CACHE_MODE_WRITE, NULL);
   if (rc != VXIcache_RESULT_SUCCESS) return rc;
 
   VXIbyte *validatorData = NULL;
   VXIulong bytesWritten = 0;
   VXIulong dataSize = 0;
   if (!validator.serialize(validatorData, dataSize))
   {
     Error(215, L"%s%s", L"URL", absoluteURL);
     rc = VXIcache_RESULT_FAILURE;
     goto failure;
   }
 
   // Write the size of the validator.
   rc = _cache->Write(_cache, (VXIbyte *) &dataSize, sizeof(dataSize), &bytesWritten, _cacheStream);
   if (rc != VXIcache_RESULT_SUCCESS || (bytesWritten != sizeof(dataSize)))
   {
     if (rc == VXIcache_RESULT_EXCEED_MAXSIZE)
       Error(305, L"%s%s", L"URL", absoluteURL);
     else
       Error(232, L"%s%s%s%i%s%i%s%i", L"URL", absoluteURL, L"rc", rc,
             L"written", bytesWritten, L"expected", sizeof(dataSize));
     rc = VXIcache_RESULT_FAILURE;
     goto failure;
   }
 
   // Write the validator.
   bytesWritten = 0;
   rc = _cache->Write(_cache, validatorData, dataSize, &bytesWritten, _cacheStream);
   if (rc != VXIcache_RESULT_SUCCESS || bytesWritten != dataSize)
   {
     Error(232, L"%s%s%s%i%s%i%s%i", L"URL", absoluteURL, L"rc", rc,
           L"written", bytesWritten, L"expected", dataSize);
     rc = VXIcache_RESULT_FAILURE;
   }
 
  failure:
   delete [] validatorData;
   return rc;
 
 }
