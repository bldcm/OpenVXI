
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
 
#ifndef __SBINETURL_H_                   /* Allows multiple inclusions */
 #define __SBINETURL_H_
 
 
 #include "VXItypes.h"
 #include "VXIvalue.h"
 #include "VXIinet.h"
 
 #include "SBinetString.hpp"
 
 #define SB_BOUNDARY "osb_inet_multipart_boundary"
 #define SB_MULTIPART "multipart/form-data; boundary=osb_inet_multipart_boundary"
 #define SB_URLENCODED "application/x-www-form-urlencoded"
 #define CRLF "\r\n"
 
 class SBinetURL
 {
 public:
   enum Protocol {
     UNKNOWN_PROTOCOL = 0,
     FILE_PROTOCOL = 1,
     HTTP_PROTOCOL = 2,
     HTTPS_PROTOCOL = 3
   };
 
  public:
   SBinetURL(const SBinetURL& url)
   {
     operator=(url);
   }
 
   ~SBinetURL()
   {}
 
   Protocol getProtocol() const
   {
     return _protocol;
   }
 
   const VXIchar * getAbsolute() const
   {
     return _absoluteURL.c_str();
   }
 
   const VXIchar * getBase() const
   {
     return _baseURL.c_str();
   }
 
   const VXIchar * getPath() const
   {
     return _strPath.c_str();
   }
 
   const VXIchar * getHost() const
   {
     return _host.c_str();
   }
 
   const char * getNAbsolute() const
   {
     return N_absoluteURL.c_str();
   }
 
   const char * getNBase() const
   {
     return N_baseURL.c_str();
   }
 
   const char * getNPath() const
   {
     return N_strPath.c_str();
   }
 
   const char * getNHost() const
   {
     return N_host.c_str();
   }
 
   SBinetURL& operator=(const SBinetURL& url);
 
   bool operator==(const SBinetURL& url);
   bool operator!=(const SBinetURL& url)
   {
     return !operator==(url);
   }
 
   static VXIinetResult create(const VXIchar* pszName,
                               const VXIchar* pszUrlBase,
                               SBinetURL *& url);
 
   VXIinetResult parse(const VXIchar* pszName,
                       const VXIchar* pszUrlBase);
 
   void appendQueryArgsToURL(const VXIMap* _queryArgs);
 
   VXIString *getContentTypeFromUrl() const;
 
   SBinetNString queryArgsToNString(const VXIMap* queryArgs) const;
 
   static bool requiresMultipart(const VXIMap* queryArgs);
 
   SBinetNString queryArgsToMultipart(const VXIMap* queryArgs);
 
  public:
   int getPort() const
   {
     return _port;
   }
 
  private:
   SBinetURL():
     _absoluteURL(), _baseURL(), _host(), _strPath(), _protocol(UNKNOWN_PROTOCOL),
     _port(-1)
   {}
 
   static SBinetNString valueToNString(const VXIValue* value);
 
   // Multipart stuff.
   static bool requiresMultipart(const VXIValue* value);
   static bool requiresMultipart(const VXIVector* vxivector);
 
   static void appendKeyToMultipart(SBinetNString& result, const char *key);
 
   static void appendValueToMultipart(SBinetNString& result,
 				     const SBinetNString& value);
 
   static void appendQueryArgsMapToMultipart(SBinetNString& result,
                                             const VXIMap *vximap,
                                             SBinetNString& fieldName);
 
   static void appendQueryArgsVectorToMultipart(SBinetNString& result,
                                                const VXIVector *vxivector,
                                                SBinetNString& fieldName);
 
   static void appendQueryArgsToMultipart(SBinetNString& result,
                                          const VXIValue *value,
                                          SBinetNString& fieldName);
 
  private:
   SBinetString _absoluteURL;
   SBinetString _baseURL;
   SBinetString _host;
   SBinetString _strPath;
   SBinetNString N_absoluteURL;
   SBinetNString N_baseURL;
   SBinetNString N_host;
   SBinetNString N_strPath;
   Protocol _protocol;
   int _port;
 };
 #endif
