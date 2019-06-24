
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
 

 #include <SWIbufferedInputStream.hpp>
 #include <SWIbufferedOutputStream.hpp>
 #include <SWIsocket.hpp>
 #include "SBinetSSLsocket.hpp"
 #include "SBinetChannel.h"
 #include "SBinetHttpConnection.hpp"
 
 SBinetHttpConnection::SBinetHttpConnection(SBinetURL::Protocol protocol,
                                            const SWIipAddress& ipAddress,
                                            bool usesProxy,
                                            SBinetChannel *channel,
                                            const char *connId):
   _remoteAddress(ipAddress),_socket(NULL),_inputStream(NULL),
   _outputStream(NULL),_channel(channel), _usesProxy(usesProxy),
   _protocol(protocol), _connId(NULL)
 {
   if (connId != NULL)
   {
     _connId = new char[strlen(connId) + 1];
     ::strcpy(_connId, connId);
   }
 }
 
 SBinetHttpConnection::~SBinetHttpConnection()
 {
   close();
   delete _connId;
 }
 
 
 SWIstream::Result SBinetHttpConnection::connect(long timeout)
 {
   if (_socket != NULL) return SWIstream::SUCCESS;
 
   switch (_protocol)
   {
    case SBinetURL::HTTP_PROTOCOL:
      _socket = new SWIsocket(SWIsocket::sock_stream, _channel);
      break;
    case SBinetURL::HTTPS_PROTOCOL:
      _socket = new SBinetSSLsocket(SWIsocket::sock_stream, _channel);
      break;
    default:
      _channel->Error(263, NULL);
      return SWIstream::INVALID_ARGUMENT;
   }
 
   SWIstream::Result rc = _socket->connect(_remoteAddress, timeout);
 
   if (rc != SWIstream::SUCCESS)
   {
     delete _socket;
     _socket = NULL;
   }
 
   return rc;
 }
 
 SWIinputStream *SBinetHttpConnection::getInputStream()
 {
   if (_socket != NULL && _inputStream == NULL)
   {
     SWIinputStream *s = _socket->getInputStream();
     if (s != NULL)
     {
       _inputStream = (s->isBuffered() ?
                       s :
                       new SWIbufferedInputStream(s));
     }
   }
   return _inputStream;
 }
 
 SWIoutputStream *SBinetHttpConnection::getOutputStream()
 {
   if (_socket != NULL && _outputStream == NULL)
   {
     SWIoutputStream *s = _socket->getOutputStream();
     if (s != NULL)
     {
       _outputStream = (s->isBuffered() ?
                        s :
                        new SWIbufferedOutputStream(s));
     }
   }
   return _outputStream;
 }
 
 SWIstream::Result SBinetHttpConnection::close()
 {
   if (_socket == NULL) return SWIstream::ILLEGAL_STATE;
 
   SWIstream::Result rc = _socket->close();
 
   delete _socket;
   delete _inputStream;
   delete _outputStream;
 
   _socket = NULL;
   _inputStream = NULL;
   _outputStream = NULL;
 
   return rc;
 }
 
 const SWIipAddress* SBinetHttpConnection::getRemoteAddress()
 {
   return &_remoteAddress;
 }
