
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
 
#ifndef __SBCOOKIE_H_                   /* Allows multiple inclusions */
 #define __SBCOOKIE_H_
 
 #include "VXItypes.h"
 #include <string.h>
 #include <time.h>    // For time( )
 
 #include "SBinetString.hpp"
 class SWIutilLogger;
 class SBinetURL;
 
 class SBinetCookie
 {
 public: // CTOR/DTOR
   SBinetCookie(const char* domain,
                const char* path,
                const char* name,
                const char* value,
                const time_t nExpires,
                const bool fSecure ):
     _Name(name), _Value(value), _Domain(domain), _Path(path),
     _nExpires(nExpires), _fSecure(fSecure), _tTimeStamp(0)
   {
     // All values should be text so we can use assignments above.
     // We keep in small char values as this is what is needed most.
     // values are checked for NULL before we are called.
     updateTimeStamp();
   }
 
   virtual ~SBinetCookie( ) {}
 
  public:
   void update(const char* value,
               time_t nExpires,
               bool fSecure )
   {
     // All values should be text so we can use strcpy
     // We keep in small char values as this is what is needed most
     // values are checked for NULL before we are called
     _Value = value;
     _nExpires = nExpires;
     _fSecure = fSecure;
     updateTimeStamp();
   }
 
   bool matchExact(const SBinetCookie *cookie) const;
 
   bool matchRequest(const char *domain,
                     const char *path,
                     const SWIutilLogger *logger = NULL) const;
 
   void updateTimeStamp() { _tTimeStamp = time(0); }
 
   inline const char* getName() const     { return _Name.c_str( ); }
   inline const char* getValue() const    { return _Value.c_str( ); }
   inline const char* getDomain() const   { return _Domain.c_str( ); }
   inline const char* getPath() const    { return _Path.c_str( ); }
   inline time_t getExpires() const { return _nExpires; }
   inline bool getSecure() const { return _fSecure; }
   inline time_t getTimeStamp() const { return _tTimeStamp; }
 
  public:
   static SBinetCookie *parse(const SBinetURL *url,
                              const char *&cookiespec,
 			     const SWIutilLogger *logger);
 
   // Functions are made public so that they are available to
   // proxy matching code.
   static bool matchDomain(const char *cdomain, const char *domain,
                           const SWIutilLogger* logger = NULL);
   static bool matchPath(const char *cpath, const char *path,
                         const SWIutilLogger *logger = NULL);
 
  private:
   SBinetCookie():_Name(), _Value(), _Domain(), _Path(), _nExpires(0),
 		 _fSecure(false), _tTimeStamp(0)
   {}
 
   const char *parseAttributes(const char *attributeSpec,
                               const SWIutilLogger *logger);
 
   struct AttributeInfo;
 
   typedef void (*AttributeHandler)(AttributeInfo *info,
                                    const char *value,
                                    SBinetCookie *cookie,
                                    const SWIutilLogger *logger);
 
   struct AttributeInfo
   {
     const char *name;
     bool hasValue;
     AttributeHandler handler;
   };
 
   static void domainHandler(AttributeInfo *info,
                             const char *value,
                             SBinetCookie *cookie,
                             const SWIutilLogger *logger);
   static void pathHandler(AttributeInfo *info,
                           const char *value,
                           SBinetCookie *cookie,
                           const SWIutilLogger *logger);
   static void expiresHandler(AttributeInfo *info,
                              const char *value,
                              SBinetCookie *cookie,
                              const SWIutilLogger *logger);
   static void secureHandler(AttributeInfo *info,
                             const char *value,
                             SBinetCookie *cookie,
                             const SWIutilLogger *logger);
   static void maxAgeHandler(AttributeInfo *info,
                             const char *value,
                             SBinetCookie *cookie,
                             const SWIutilLogger *logger);
 
   static AttributeInfo attributeInfoMap[];
 
   SBinetNString _Name;
   SBinetNString _Value;
 
   SBinetNString _Domain;
   SBinetNString _Path;
   time_t _nExpires;
   bool _fSecure;
   time_t _tTimeStamp;
 };
 
 #endif // include guard
