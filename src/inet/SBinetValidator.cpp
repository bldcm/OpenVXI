
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
 
#ifndef _SB_USE_STD_NAMESPACE
 #define _SB_USE_STD_NAMESPACE
 #endif
 
 #include <wchar.h>
 #include <stdio.h>
 
 #include "SBinetValidator.h"     // For this class
 #include "SBinetLog.h"           // For logging definitions
 #include "HttpUtils.hpp"
 #include "SBinetUtils.hpp"
 #include "SBinetChannel.h"
 
 using namespace std;
 
 /*****************************************************************************
  *****************************************************************************
  * SBinetValidator Implementation
  *****************************************************************************
  *****************************************************************************
  */
 
 #if defined(_decunix_) || defined(_solaris_)
 
 // On solaris,
 // wsncasecmp is defined in widec.h but we don't have the right comp. flags to get the prototype.
 // I'm not sure about decunix though.
 //
 extern "C" wsncasecmp(const wchar_t *, const wchar_t *, size_t);
 #define wcsncasecmp(s1,s2,n) wsncasecmp(s1,s2,n)
 
 #if 0
 static int my_wcsncasecmp(const wchar_t *s1, const wchar_t *s2,
 			  register size_t n)
 {
   register unsigned int u1, u2;
 
   for (; n != 0; --n) {
     u1 = (unsigned int) *s1++;
     u2 = (unsigned int) *s2++;
     if (HttpUtils::toUpper(u1) != HttpUtils::toUpper(u2)) {
       return HttpUtils::toUpper(u1) - HttpUtils::toUpper(u2);
     }
     if (u1 == '\0') {
       return 0;
     }
   }
   return 0;
 }
 #endif
 #endif
 
 SBinetValidator::SBinetValidator(VXIlogInterface *log, VXIunsigned diagTagBase)
   : SWIutilLogger(MODULE_SBINET, log, diagTagBase), _freshnessLifetime((time_t) -1),
     _refTime((time_t) -1), _sizeBytes(0), _eTag(NULL), _mustRevalidateF(false),
     _url(NULL), _lastModified((time_t) -1)
 {}
 
 SBinetValidator::~SBinetValidator()
 {
   delete [] _eTag;
   delete [] _url;
 }
 
 VXIinetResult SBinetValidator::Create(const VXIchar *filename, VXIulong sizeBytes,
                                       time_t refTime)
 {
   _lastModified = refTime;
   _refTime = refTime;
   return Create(filename, sizeBytes, (time_t) -1, (VXIMap *) NULL);
 }
 
 VXIinetResult SBinetValidator::Create(const VXIchar *url,
                                       time_t requestTime,
                                       const VXIMap *streamInfo)
 {
   if (url == NULL || !*url || streamInfo == NULL)
   {
     return VXIinet_RESULT_INVALID_ARGUMENT;
   }
 
   VXIint32 sizeBytes = 0;
   SBinetUtils::getInteger(streamInfo, INET_INFO_SIZE_BYTES, sizeBytes);
 
   return Create(url, (VXIulong) sizeBytes, requestTime, streamInfo);
 }
 
 static time_t computeRefTime(time_t requestTime,
                              const VXIMap* streamInfo,
                              time_t& dateTime)
 {
   // The algorithm is based on the RFC section 13.2.3 but instead of computing
   // an initial age, it computes a reference time that can later be used to
   // retrieve the age.
 
   // The algo in the RFC to compute current-age is as follows.
 
   //  response_delay = response_time - request_time;
   //  corrected_initial_age = corrected_received_age + response_delay;
   //  resident_time = now - response_time;
   //  current_age   = corrected_initial_age + resident_time;
 
   // Which can be rewritten as follows:
 
   //  current_age = (corrected_received_age + response_delay) + (now - response_time)
   //              = (corrected_received_age + response_time - request_time) + (now - response_time)
   //              = corrected_received_age + now - request_time
   //              = now - (request_time - corrected_received_age)
   // This algorithm computes the ref. time to be equal to request_time - corrected_received_age.
 
   // Using this algorithm has two advantages:
   //
   // 1) More efficient: less computation required to compute the ref. time
   // than to compute the corrected initial age. When computing the current
   //
   // 2) Only requires to store the ref. time in the validator rather than the
   // response time and the corrected initial age.
 
   VXIint32 tmp;
   time_t responseTime = time(NULL);
   time_t computed_age = (time_t) 0;
 
   if (SBinetUtils::getInteger(streamInfo, L"Date", tmp))
   {
     dateTime = (time_t) tmp;
     computed_age = responseTime - dateTime;
     if (computed_age < (time_t) 0) computed_age = (time_t) 0;
   }
   else
   {
     dateTime = responseTime;
   }
 
   if (SBinetUtils::getInteger(streamInfo, L"Age", tmp))
   {
     time_t age = (time_t) tmp;
     if (age > computed_age) computed_age = age;
   }
   return requestTime - computed_age;
 }
 
 VXIinetResult SBinetValidator::Create(const VXIchar *url, VXIulong sizeBytes,
                                       time_t requestTime,
                                       const VXIMap *streamInfo)
 {
   if (url == NULL || !*url)
   {
     return VXIinet_RESULT_INVALID_ARGUMENT;
   }
 
   if (_url)
     delete [] _url;
   _url = ::wcscpy(new VXIchar[::wcslen(url) + 1], url);
 
   _sizeBytes = sizeBytes;
   _freshnessLifetime = (time_t) -1;
 
   const VXIchar *eTag = NULL;
 
   delete [] _eTag;
 
   //   // Sanity check
   //   if (_expiresTime >= 0 && _lastModifiedTime >= _expiresTime)
   //     _expiresTime = 0;
 
   //   time_t now = time(NULL);
   //   if (_lastModifiedTime > now)
   //     _lastModifiedTime = now;
 
   if (streamInfo != NULL)
   {
     time_t dateTime;
     _refTime = computeRefTime(requestTime, streamInfo, dateTime);
 
     VXIint32 lastModified = -1;
     SBinetUtils::getInteger(streamInfo, L"Last-Modified", lastModified);
     _lastModified = (time_t) lastModified;
 
     eTag = SBinetUtils::getString(streamInfo, L"ETag");
 
     // Inspect the headers for other relevant directives ('no-cache' etc.)
     _freshnessLifetime = checkPragma(streamInfo);
     if (_freshnessLifetime == (time_t) -1)
     {
       _freshnessLifetime = checkCacheControl(streamInfo);
     }
 
     if (_freshnessLifetime == (time_t) -1)
     {
       VXIint32 expiresTime = -1;
 
       if (SBinetUtils::getInteger(streamInfo, L"Expires", expiresTime))
       {
         _freshnessLifetime = ((time_t) expiresTime) - dateTime;
       }
       else if (_lastModified != (time_t) -1)
       {
         // Use the last modified time from the map to determine a freshness lifetime.
         _freshnessLifetime = (time_t) ((dateTime - _lastModified) *
                                        SBinetChannel::getFreshnessFraction());
         time_t maxLifetime = SBinetChannel::getFreshnessLifetime();
         if (_freshnessLifetime > maxLifetime)
           _freshnessLifetime = maxLifetime;
       }
       else
       {
         Error(302, L"%s%s", L"URL", url);
         _freshnessLifetime = SBinetChannel::getMaxLifetime();
       }
     }
     if (_freshnessLifetime < (time_t) 0)
       _freshnessLifetime = (time_t) 0;
   }
 
   if (eTag == NULL || !*eTag)
   {
     _eTag = NULL;
   }
   else
   {
     _eTag = ::wcscpy(new VXIchar[::wcslen(eTag) + 1], eTag);
   }
 
   Log(L"SBinetValidator::Create");
   return VXIinet_RESULT_SUCCESS;
 }
 
 
 VXIinetResult SBinetValidator::Create(const VXIValue *content)
 {
   if (VXIValueGetType(content) != VALUE_CONTENT)
   {
     Error(214, L"Type: %d", VXIValueGetType(content));
     return VXIinet_RESULT_INVALID_ARGUMENT;
   }
 
   // Get the content
   const VXIchar *contentType;
   const VXIbyte *contentData;
   VXIulong contentSizeBytes;
   if ( VXIContentValue((const VXIContent*) content,
                        &contentType, &contentData,
 		       &contentSizeBytes) != VXIvalue_RESULT_SUCCESS )
   {
     Error(215, NULL);
     return VXIinet_RESULT_INVALID_ARGUMENT;
 
   }
   else if ( ::wcscmp(contentType, VALIDATOR_MIME_TYPE) != 0 )
   {
     Error(216, L"mimeType: %s", contentType);
     return VXIinet_RESULT_INVALID_ARGUMENT;
   }
 
   return Create(contentData, contentSizeBytes);
 }
 
 
 VXIinetResult SBinetValidator::Create(const VXIbyte *data, VXIulong contentSizeBytes)
 {
   // Unpack the data
   const VXIbyte *ptr = data;
 
   ::memcpy(&_refTime, ptr, sizeof _refTime);
   ptr += sizeof _refTime;
 
   ::memcpy(&_freshnessLifetime, ptr, sizeof _freshnessLifetime);
   ptr += sizeof _freshnessLifetime;
 
   ::memcpy(&_lastModified, ptr, sizeof _lastModified);
   ptr += sizeof _lastModified;
 
   ::memcpy(&_mustRevalidateF, ptr, sizeof _mustRevalidateF);
   ptr += sizeof _mustRevalidateF;
 
   ::memcpy(&_sizeBytes, ptr, sizeof _sizeBytes);
   ptr += sizeof _sizeBytes;
 
   int len;
   ::memcpy(&len, ptr, sizeof len);
   ptr += sizeof len;
 
   delete [] _eTag;
   if (len > 0)
   {
     int size = len * sizeof(VXIchar);
     _eTag = new VXIchar[len + 1];
     ::memcpy(_eTag, ptr, size);
     ptr += size;
     _eTag[len] = L'\0';
   }
   else
     _eTag = NULL;
 
   ::memcpy(&len, ptr, sizeof len);
   ptr += sizeof len;
 
   delete [] _url;
   if (len > 0)
   {
     int size = len * sizeof(VXIchar);
     _url = new VXIchar[len + 1];
     ::memcpy(_url, ptr, size);
     ptr += size;
     _url[len] = L'\0';
   }
   else
     _url = NULL;
 
   Log(L"SBinetValidator::Deserialize");
   return VXIinet_RESULT_SUCCESS;
 }
 
 VXIContent *SBinetValidator::serialize() const
 {
   VXIbyte *data = NULL;
   VXIulong dataSize;
   VXIContent *content;
 
   if (serialize(data, dataSize))
   {
     // Create the content
     content = VXIContentCreate(VALIDATOR_MIME_TYPE, data, dataSize,
                                ContentDestroy, NULL);
 
     if (content == NULL)
       delete [] data;
   }
 
   return content;
 }
 
 bool SBinetValidator::serialize(VXIbyte*& content, VXIulong& contentSize) const
 {
   // Pack the data
   int urlLen = 0;
   int eTagLen = 0;
 
   contentSize = ((sizeof _refTime) +
                  (sizeof _freshnessLifetime) +
                  (sizeof _lastModified) +
                  (sizeof _mustRevalidateF) +
                  (sizeof _sizeBytes) +
                  (sizeof eTagLen) +
                  (sizeof urlLen));
 
   if (_eTag != NULL)
   {
     eTagLen = ::wcslen(_eTag);
     contentSize += eTagLen * sizeof(VXIchar);
   }
 
   if (_url != NULL)
   {
     urlLen = ::wcslen(_url);
     contentSize += urlLen * sizeof(VXIchar);
   }
 
   content = new VXIbyte[contentSize];
   VXIbyte *ptr = content;
 
   ::memcpy(ptr, &_refTime, sizeof _refTime);
   ptr += sizeof _refTime;
 
   ::memcpy(ptr, &_freshnessLifetime, sizeof _freshnessLifetime);
   ptr += sizeof _freshnessLifetime;
 
   ::memcpy(ptr, &_lastModified, sizeof _lastModified);
   ptr += sizeof _lastModified;
 
   ::memcpy(ptr, &_mustRevalidateF, sizeof _mustRevalidateF);
   ptr += sizeof _mustRevalidateF;
 
   ::memcpy(ptr, &_sizeBytes, sizeof _sizeBytes);
   ptr += sizeof _sizeBytes;
 
   ::memcpy(ptr, &eTagLen, sizeof eTagLen);
   ptr += sizeof eTagLen;
   if (eTagLen > 0)
   {
     ::memcpy(ptr, _eTag, eTagLen * sizeof(VXIchar));
     ptr += eTagLen * sizeof(VXIchar);
   }
 
   ::memcpy(ptr, &urlLen, sizeof urlLen);
   ptr += sizeof urlLen;
   if (urlLen > 0)
   {
     ::memcpy(ptr, _url, urlLen * sizeof(VXIchar));
     ptr += urlLen * sizeof(VXIchar);
   }
 
   return true;
 }
 
 time_t SBinetValidator::getExpiresTime() const
 {
   if (_freshnessLifetime <= (time_t) 0)
     return (time_t) 0;
   return _refTime + _freshnessLifetime;
 }
 
 bool SBinetValidator::isExpired() const
 {
   return _freshnessLifetime == (time_t) 0 ||
     _freshnessLifetime < time(NULL) - _refTime;
 }
 
 bool SBinetValidator::isExpired(const VXIint maxAge,
                                 const VXIint maxStale) const
 {
   if (maxAge == 0) return true;
 
   time_t now = time(NULL);
   time_t currentAge = now - _refTime;
 
   if (maxAge > 0 && currentAge > maxAge) return true;
 
   time_t staleness = currentAge - _freshnessLifetime;
 
   if (staleness < 0) return false;
   if (_mustRevalidateF) return true;
   if (staleness > maxStale) return true;
   if (_freshnessLifetime == (time_t) 0) return true;
 
   return false;
 }
 
 bool SBinetValidator::isStrong() const
 {
   return _eTag != NULL && wcsncasecmp(_eTag, L"w/", 2) != 0;
 }
 
 void SBinetValidator::Log(const VXIchar *name) const
 {
   if (DiagIsEnabled(MODULE_SBINET_VALIDATOR_TAGID))
   {
     char refTimeStr[64], expireStr[64], lastModifiedStr[64];
 
     if (_refTime == (time_t) -1)
       ::strcpy(refTimeStr, "-1");
     else
       SBinetUtils::getTimeStampStr(_refTime, refTimeStr);
 
     SBinetUtils::getTimeStampStr(getExpiresTime(), expireStr);
 
     if (_lastModified == (time_t) -1)
       ::strcpy(lastModifiedStr, "-1");
     else
       SBinetUtils::getTimeStampStr(_lastModified, lastModifiedStr);
 
     Diag(MODULE_SBINET_VALIDATOR_TAGID, name,
          L"URL: '%s', refTime %S, last modified %S, expires %S, must revalidate %s, size %lu, etag '%s'",
          _url, refTimeStr, lastModifiedStr, expireStr,
          _mustRevalidateF ? L"true" : L"false",
          _sizeBytes, _eTag);
   }
 }
 
 
 void SBinetValidator::ContentDestroy(VXIbyte **content, void *userData)
 {
   if (content)
   {
     delete [] *content;
     *content = NULL;
   }
 }
 
 time_t SBinetValidator::checkPragma(const VXIMap *streamInfo)
 {
   const VXIchar *pragmaDirective = SBinetUtils::getString(streamInfo, L"Pragma");
 
   if (pragmaDirective == NULL)
   {
     Diag(MODULE_SBINET_VALIDATOR_TAGID, L"SBinetValidator::checkPragma",
          L"No Pragma directive.");
     return (time_t) -1;
   }
 
   Diag(MODULE_SBINET_VALIDATOR_TAGID, L"SBinetValidator::checkPragma",
        L"Pragma directive: '%s'", pragmaDirective);
 
   const VXIchar *p = pragmaDirective;
   while (*p && SBinetHttpUtils::isSpace(*p)) p++;
   if (wcsncasecmp(p, L"no-cache", 8) == 0)
   {
     Diag(MODULE_SBINET_VALIDATOR_TAGID, L"SBinetValidator::checkPragma",
          L"Got Pragma: no-cache directive.");
     return (time_t) 0;
   }
   return (time_t) -1;
 }
 
 time_t SBinetValidator::checkCacheControl(const VXIMap *streamInfo)
 {
   time_t maxAge = (time_t) -1;
 
   const VXIchar *cacheControlDirective = SBinetUtils::getString(streamInfo, L"Cache-Control");
 
   if (cacheControlDirective == NULL)
   {
     Diag(MODULE_SBINET_VALIDATOR_TAGID, L"SBinetValidator::checkCacheControl",
          L"No Cache-Control directive.");
     return maxAge;
   }
 
   // We need to convert to narrow string in order to take advantage of the
   // HttpUtils function that are written in terms of narrow character.
 
   SBinetNString tmp = cacheControlDirective;
   const char *content = tmp.c_str();
 
   Diag(MODULE_SBINET_VALIDATOR_TAGID, L"SBinetValidator::checkCacheControl",
        L"Cache-Control directive: '%S'", content);
 
   int priority = 0;
   static const int maxAgePriority = 0;
   static const int sMaxAgePriority = 1;
   static const int cacheControlPriority = 2;
 
   SBinetNString attrib;
   SBinetNString val;
 
   for (;;)
   {
     content = SBinetHttpUtils::getValue(content, attrib);
 
     if (content == NULL)
     {
       Diag(MODULE_SBINET_VALIDATOR_TAGID,
            L"SBinetValidator::checkCacheControl",
            L"Could not get value.");
       break;
     }
 
     content = SBinetHttpUtils::expectChar(content, "=,");
 
     if (content == NULL)
     {
       Diag(MODULE_SBINET_VALIDATOR_TAGID,
            L"SBinetValidator::checkCacheControl",
            L"Expecting '=' or ','.");
       break;
     }
 
     const char *attribute = attrib.c_str();
     const char *value = NULL;
 
     if (*content == '=')
     {
       content = SBinetHttpUtils::getValue(++content, val);
       if (content == NULL)
       {
         Diag(MODULE_SBINET_VALIDATOR_TAGID,
              L"SBinetValidator::checkCacheControl",
              L"Could not extract value for attribute",
              attribute);
         break;
       }
       value = val.c_str();
     }
 
     Diag(MODULE_SBINET_VALIDATOR_TAGID, L"SBinetValidator::checkCacheControl",
          L"attribute = '%S', value = '%S'", attribute, value);
 
     if (strcasecmp(attribute, "no-cache") == 0)
     {
       // Ignore no-cache="something" as we have one cookie jar per channel.
       // So they are not shared.
       if (value == NULL && priority <= cacheControlPriority)
       {
         maxAge = (time_t) 0;
         priority = cacheControlPriority;
       }
     }
     else if ((strcasecmp(attribute, "no-store") == 0 ||
               strcasecmp(attribute, "private") == 0)
              && priority <= cacheControlPriority)
 
     {
       maxAge = (time_t) 0;
       priority = cacheControlPriority;
     }
     else if (strcasecmp(attribute, "must-revalidate") == 0)
     {
       // This directive does not affect priority nor does it depend on a
       // priority as it does not modify the maxAge value.
       _mustRevalidateF = true;
     }
     else if (strcasecmp(attribute, "max-age") == 0)
     {
       if (value != NULL &&  priority <= maxAgePriority)
       {
         maxAge = (time_t) atoi(value);
         priority = maxAgePriority;
       }
     }
     else if (strcasecmp(attribute, "s-maxage") == 0)
     {
       if (value != NULL && priority <= sMaxAgePriority)
       {
         maxAge = atoi(value);
         priority = sMaxAgePriority;
       }
     }
 
     content = SBinetHttpUtils::expectChar(content, ",");
 
     if (content == NULL)
     {
       Diag(MODULE_SBINET_VALIDATOR_TAGID,
            L"SBinetValidator::checkCacheControl",
            L"Expecting ','.");
       break;
     }
 
     if (!*content)
     {
       // end of string. No error.
       break;
     }
 
     content++;
   }
 
   return maxAge;
 }
