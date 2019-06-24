
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
 
 #include "SBinetProxyMatcher.hpp"
 #include "SBinetCookie.h"
 #include "HttpUtils.hpp"
 #include "ap_ctype.h"
 
 // SBinetProxyMatcher::SBinetProxyMatcher
 // Refer to SBinetProxyMatcher.hpp for doc.
 SBinetProxyMatcher::SBinetProxyMatcher(const char *domain, const char *path,
                                        const char *proxyServer, int proxyPort)
 {
   if (domain)
     _domain = domain;
 
   if (path && *path)
   {
     _path = path;
     int len = _path.length();
     if (--len > 0 && _path[len] == '/')
       _path.resize(len);
   }
   else
     _path = "/";
 
 
   if (proxyServer)
     _proxyServer = proxyServer;
 
   _proxyPort = proxyPort;
 }
 
 // SBinetProxyMatcher::~SBinetProxyMatcher
 // Refer to SBinetProxyMatcher.hpp for doc.
 SBinetProxyMatcher::~SBinetProxyMatcher()
 {}
 
 
 SBinetProxyMatcher *SBinetProxyMatcher::createMatcher(const char *rule, const SWIutilLogger *logger)
 {
   if (!rule)
   {
     return new SBinetProxyMatcher(NULL,NULL,NULL,0);
   }
 
   register const char *p = rule;
 
   // skip any leading whitespace.
   while (*p && ap_isspace(*p)) p++;
 
   // Extract domain.
   const char *domBeg = p;
 
   // Domain goes up to the / or |, but fail if we find any whitespace in
   // between.
   while (*p && *p != '/' && *p != '|' && !ap_isspace(*p)) p++;
   // Got end of domain.
   const char *domEnd = p;
 
   // If we stop because a whitespace, the next non whitespace character has to
   // be a |.
   if (*p && ap_isspace(*p))
   {
     do
     {
       p++;
     }
     while (*p && ap_isspace(*p));
 
     if (*p && *p != '|')
     {
       if (logger) logger->Error(600,NULL);
       return NULL;
     }
   }
 
   // Extract path if present.
   const char *pathBeg = p;
   const char *pathEnd = pathBeg;
 
   if (*p == '/')
   {
     // Path goes to the | or a whitespace character.
     while (*p && *p != '|' && !ap_isspace(*p)) p++;
     pathEnd = p;
 
     // Skip whitespaces in p.
     while (*p && ap_isspace(*p)) p++;
   }
 
   // Extract proxyServer if present.
   const char *proxyBeg = p;
   const char *proxyEnd = p;
   int proxyPort = 0;
 
   if (*p == '|')
   {
     // skip the vertical bar.
     p++;
 
     // Skip any leading character in the proxy server name.
     while (*p && ap_isspace(*p)) p++;
     proxyBeg = p;
 
     // Read the proxy server name.
     while (*p && *p != ':' && !ap_isspace(*p)) p++;
     proxyEnd = p;
 
     if (proxyEnd != proxyBeg)
     {
       // If we stopped because of any other reason than a ':', it is an error
       // as the port number is mandatory.
       if (*p != ':')
       {
         if (logger) logger->Error(601,NULL);
         return NULL;
       }
 
       // skip the colon.
       p++;
 
       while (*p && ap_isdigit(*p))
       {
         proxyPort *= 10;
 
         // overflow check.
         if (proxyPort < 0) break;
 
         proxyPort += *p - '0';
         p++;
       }
 
       // Check for invalid character right after the port spec.
       if (proxyPort <= 0 || (*p && !ap_isspace(*p)))
       {
         if (logger) logger->Error(602,NULL);
         return NULL;
       }
 
       // skip any whitespace left.
       while (*p && ap_isspace(*p)) p++;
     }
     else if (*p)
     {
       if (logger) logger->Error(604, NULL);
       return NULL;
     }
   }
 
   // Make sure there are no extra characters.
   if (*p)
   {
     if (logger) logger->Error(603,NULL);
     return NULL;
   }
 
 
   // If we get here, we have all the information to build the proxy matcher.
   SBinetNString domain(domBeg, domEnd - domBeg);
   SBinetNString path(pathBeg, pathEnd - pathBeg);
   SBinetNString proxyServer(proxyBeg, proxyEnd - proxyBeg);
 
   return new SBinetProxyMatcher(domain.c_str(), path.c_str(), proxyServer.c_str(), proxyPort);
 }
 
 bool SBinetProxyMatcher::matches(const char *domain, const char *path) const
 {
   return (SBinetCookie::matchDomain(_domain.c_str(), domain) &&
           SBinetCookie::matchPath(_path.c_str(), path));
 }
