#ifndef SBINETPROXYMATCHER_HPP
 #define SBINETPROXYMATCHER_HPP
 

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
 

 #include <SWIutilLogger.hpp>
 #include <SBinetString.hpp>
 #include <VXItypes.h>
 
 class SBinetProxyMatcher
 {
   // ................. CONSTRUCTORS, DESTRUCTOR  ............
   //
   // ------------------------------------------------------------
   /**
    * Default constructor.
    **/
  public:
   SBinetProxyMatcher(const char *domain, const char *path,
                      const char *proxyServer, int proxyPort);
 
   /**
    * Static constructor that parses a rule to generate a proxy matcher.
    **/
  public:
   static SBinetProxyMatcher *createMatcher(const char *rule, const SWIutilLogger *logger);
 
   /**
    * Destructor.
    **/
  public:
   virtual ~SBinetProxyMatcher();
 
  public:
   const char *getDomain() const
   {
     return _domain.c_str();
   }
 
   const char *getPath() const
   {
     return _path.c_str();
   }
 
   const char *getProxyServer() const
   {
     return _proxyServer.c_str();
   }
 
   int getProxyPort() const
   {
     return _proxyPort;
   }
 
   bool matches(const char *domain, const char *path) const;
 
  private:
   SBinetNString _domain;
   SBinetNString _path;
   SBinetNString _proxyServer;
   int _proxyPort;
 };
 
 #endif
