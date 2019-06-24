#ifndef SBINETSSLSOCKET_HPP
 #define SBINETSSLSOCKET_HPP
 

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
 

 #include "SWIsocket.hpp"
 #include <openssl/ssl.h>
 
 /**
  * This class encapsulates client-side SSL operations.
  **/
 class SBinetSSLsocket: public SWIsocket
 {
  public:
   static int initialize();
   static int shutdown();
 
   // ................. CONSTRUCTORS, DESTRUCTOR  ............
   //
   // ------------------------------------------------------------
   /**
    * Default constructor.
    **/
  public:
   SBinetSSLsocket(Type socktype = sock_stream, SWIutilLogger *logger = NULL);
 
   /**
    * Destructor.
    **/
  public:
   virtual ~SBinetSSLsocket();
 
   virtual SWIstream::Result connect(const SWIipAddress& endpoint, long timeout = -1);
 
   virtual int read(void* buf, int len);
   virtual int recv(void* buf, int len, int msgf=0);
   virtual int recvfrom(SWIipAddress& sa, void* buf, int len, int msgf=0);
 
   virtual int write	(const void* buf, int len);
   virtual int send	(const void* buf, int len, int msgf=0);
   virtual int sendto	(const SWIipAddress& sa, const void* buf, int len, int msgf=0);
 
   virtual SWIstream::Result shutdown(shuthow sh);
   virtual SWIstream::Result SBinetSSLsocket::close();
 
   /**
    * Disabled copy constructor.
    **/
  private:
   SBinetSSLsocket(const SBinetSSLsocket&);
 
   /**
    * Disabled assignment operator.
    **/
  private:
   SBinetSSLsocket& operator=(const SBinetSSLsocket&);
 
  private:
   static SSL_CTX *_ctx;
   static SSL_METHOD *_meth;
   SSL *_ssl;
 
 };
 #endif
