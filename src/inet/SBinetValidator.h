
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
 
#ifndef __SBVALIDATOR_H_                   /* Allows multiple inclusions */
 #define __SBVALIDATOR_H_
 
 #include <time.h>    // For time( )
 
 #include "VXIinet.h"          // For VXIinetResult
 #include "SWIutilLogger.hpp"   // Base class
 #include "SBinetString.hpp"   // For SBinetString
 
 #define VALIDATOR_MIME_TYPE L"application/x-vxi-SBinet-validator"
 
 class SBinetValidator : protected SWIutilLogger
 {
 public:
   SBinetValidator(VXIlogInterface *log, VXIunsigned diagTagBase);
   virtual ~SBinetValidator();
 
   // Creation
   VXIinetResult Create(const VXIchar *url, time_t requestTime, const VXIMap *streamInfo);
   VXIinetResult Create(const VXIchar *filename, VXIulong sizeBytes, time_t refTime);
   VXIinetResult Create(const VXIValue *value);
 
   VXIinetResult Create(const VXIbyte *content, VXIulong contentSize);
 
   // Serialization
   VXIContent *serialize() const;
   bool serialize(VXIbyte *&content, VXIulong& contentSize) const;
 
   // Determine if it is expired or modified
   bool isExpired() const;
 
   bool isModified(time_t lastModified, VXIulong sizeBytes) const
   {
     return lastModified != _lastModified || sizeBytes != _sizeBytes;
   }
 
   bool isExpired(const VXIint maxAge, const VXIint maxStale) const;
 
   time_t getLastModified() const
   {
     return _lastModified;
   }
 
   bool getMustRevalidateF() const
   {
     return _mustRevalidateF;
   }
 
   time_t getRefTime() const
   {
     return _refTime;
   }
 
   time_t getExpiresTime() const;
 
   time_t getCurrentAge() const
   {
     return time(NULL) - _refTime;
   }
 
 
   time_t getFreshnessLifetime() const
   {
     return _freshnessLifetime;
   }
 
   VXIulong getSize() const
   {
     return _sizeBytes;
   }
 
   const VXIchar *getETAG() const
   {
     return _eTag;
   }
 
   bool isStrong() const;
 
   const VXIchar *getURL() const
   {
     return _url;
   }
 
  private:
   // Log the validator to the diagnostic log
   void Log(const VXIchar *name) const;
 
   // VXIContent destructor
   static void ContentDestroy(VXIbyte **content, void *userData);
 
  private:
   time_t checkPragma(const VXIMap *streamInfo);
   time_t checkCacheControl(const VXIMap *streamInfo);
 
 
   VXIinetResult Create(const VXIchar *url, VXIulong sizeBytes,
                        time_t requestTime, const VXIMap *streamInfo);
   time_t  _refTime;
   time_t _freshnessLifetime;
   time_t _lastModified;
   VXIulong _sizeBytes;
   VXIchar *_eTag;
   VXIchar *_url;
   bool _mustRevalidateF;
 };
 
 #endif // include guard
