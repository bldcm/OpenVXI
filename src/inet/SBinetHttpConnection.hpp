#ifndef SBINETHTTPCONNECTION_HPP
 #define SBINETHTTPCONNECTION_HPP
 

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
 

 #include <SWIstream.hpp>
 #include <SWIipAddress.hpp>
 
 class SWIsocket;
 class SWIinputStream;
 class SWIoutputStream;
 class SWIutilLogger;
 class SWIipAddress;
 class SBinetChannel;
 
 #include "SBinetURL.h"
 
 /**
  * class to encapsulate HTTP connection.
  * @doc <p>
  **/
 
 class SBinetHttpConnection
 {
 
   friend class SBinetChannel;
 
   // ................. CONSTRUCTORS, DESTRUCTOR  ............
   //
   // ------------------------------------------------------------
   /**
    * Constructor.
    **/
  private:
   SBinetHttpConnection(SBinetURL::Protocol protocol,
                        const SWIipAddress& ipAddress,
                        bool usesProxy,
                        SBinetChannel *channel,
                        const char *connId = NULL);
 
  public:
   SWIstream::Result connect(long timeout);
   SWIinputStream *getInputStream();
   SWIoutputStream *getOutputStream();
   SWIstream::Result close();
   const SWIipAddress *getRemoteAddress();
 
   bool usesProxy() const
   {
     return _usesProxy;
   }
 
   /**
    * Destructor.
    **/
  public:
   virtual ~SBinetHttpConnection();
 
  public:
   const char *getId() const
   {
     return _connId;
   }
 
   /**
    * Disabled copy constructor.
    **/
  private:
   SBinetHttpConnection(const SBinetHttpConnection&);
 
   /**
    * Disabled assignment operator.
    **/
  private:
   SBinetHttpConnection& operator=(const SBinetHttpConnection&);
 
  private:
   SWIsocket *_socket;
   SWIinputStream *_inputStream;
   SWIoutputStream *_outputStream;
   SBinetChannel *_channel;
   SWIipAddress _remoteAddress;
   bool _usesProxy;
   SBinetURL::Protocol _protocol;
   char *_connId;
 };
 #endif
