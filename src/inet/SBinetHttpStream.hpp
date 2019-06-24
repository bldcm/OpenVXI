
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
 
#ifndef SBINETHTTPSTREAM_HPP
 #define SBINETHTTPSTREAM_HPP
 
 #include "SBinetStoppableStream.hpp"
 
 class SBinetChannel;
 class SBinetHttpConnection;
 class SWIinputStream;
 class SWIoutputStream;
 class SBinetValidator;
 
 /**
  * This class is responsible to fetch HTTP documents.
  **/
 class SBinetHttpStream : public SBinetStoppableStream
 {
  public:
   enum SubmitMethod { GET_METHOD = 1, POST_METHOD = 2 };
   enum EncodingType { TYPE_URL_ENCODED = 1, TYPE_MULTIPART = 2 };
 
  public:
   SBinetHttpStream(SBinetURL* url, SubmitMethod method,
                    SBinetChannel* ch,
                    VXIlogInterface *log, VXIunsigned diagLogBase);
 
   ~SBinetHttpStream();
 
   VXIinetResult Open(VXIint flags,
 		     const VXIMap* properties,
 		     VXIMap* streamInfo);
 
   VXIinetResult Read(/* [OUT] */ VXIbyte*         pBuffer,
                      /* [IN]  */ VXIulong         nBuflen,
                      /* [OUT] */ VXIulong*        pnRead );
 
   VXIinetResult Write(/* [IN]  */ const VXIbyte*   pBuffer,
                       /* [IN]  */ VXIulong         nBuflen,
                       /* [OUT] */ VXIulong*        pnWritten);
 
   VXIinetResult Close();
 
   SBinetChannel* getChannel()
   {
     return _channel;
   }
 
  public:
   int getHttpStatus() const
   {
     return _HttpStatus;
   }
 
  public:
   const SBinetValidator *getValidator() const
   {
     return _validator;
   }
 
  private:
   struct HeaderInfo;
 
   typedef void (*HeaderHandler)(HeaderInfo *,
                                 const char *value,
                                 SBinetHttpStream *,
                                 VXIMap *);
 
   struct HeaderInfo
   {
     const char *header;
     const VXIchar *inet_property;
     int value_type;
     HeaderHandler handler;
   };
 
   VXIinetResult getStatus();
 
   VXIinetResult parseHeaders(VXIMap *streamInfo);
 
   void processHeader(const char *header,
                      const char *value,
                      VXIMap *streamInfo);
 
   static void connectionHandler(HeaderInfo *headerInfo,
                                 const char *value,
                                 SBinetHttpStream *stream,
                                 VXIMap *streamInfo);
 
   static void setCookieHandler(HeaderInfo *headerInfo,
                                const char *value,
                                SBinetHttpStream *stream,
                                VXIMap *streamInfo);
 
   static void setCookie2Handler(HeaderInfo *headerInfo,
                                 const char *value,
                                 SBinetHttpStream *stream,
                                 VXIMap *streamInfo);
 
   static void defaultHeaderHandler(HeaderInfo *headerInfo,
                                    const char *value,
                                    SBinetHttpStream *stream,
                                    VXIMap *streamInfo);
 
   static void transferEncodingHandler(HeaderInfo *headerInfo,
                                       const char *value,
                                       SBinetHttpStream *stream,
                                       VXIMap *streamInfo);
 
   static void contentLengthHandler(HeaderInfo *headerInfo,
                                    const char *value,
                                    SBinetHttpStream *stream,
                                    VXIMap *streamInfo);
 
   VXIinetResult doGet(VXIint32 flags,
                       const VXIMap *properties,
                       VXIMap *streamInfo);
 
   VXIinetResult doPost(VXIint32 flags,
                        const VXIMap *properties,
                        VXIMap *streamInfo);
 
   VXIinetResult getHeaderInfo(VXIMap* streamInfo);
   void processHeaderInfo(VXIMap *streamInfo);
   void setValidatorInfo(VXIMap *streamInfo, time_t requestTime);
 
   bool handleRedirect(VXIMap *streamInfo);
   void sendCookies();
 
   VXIinetResult MapError(int ht_error, const VXIchar **errorDesc);
 
 
   VXIinetResult getChunkSize(VXIulong& chunkSize);
   int readChunked(/* [OUT] */ VXIbyte*         buffer,
                   /* [IN]  */ VXIulong         buflen);
 
   int readNormal(/* [OUT] */ VXIbyte*         buffer,
                  /* [IN]  */ VXIulong         buflen);
 
  private:
   VXIinetResult initSocket(SubmitMethod method,
                            const VXIMap *properties,
                            VXIMap *streamInfo);
 
   void writeDebugTimeStamp();
 
   int getValue(char *&value, unsigned long mask);
   int getHeader(char *&header, char delimiter);
   int getHeaderValue(char *&header, char *&value);
 
   int _HttpStatus;
 
   // The number of bytes to read in the current chunk if _chunked is true or
   // in the stream if _chunked is falsed.
   VXIulong _leftToRead;
   bool _chunked;
 
   SubmitMethod _method;
 
   SBinetChannel* _channel;
 
   SBinetHttpConnection *_connection;
   SWIinputStream *_inputStream;
   SWIoutputStream *_outputStream;
 
   VXIint32 _closeConnection;
   VXIint32 _connectionAborted;
 
   static HeaderInfo headerInfoMap[];
   static HeaderInfo* findHeaderInfo(const char *header);
 
   VXIinetResult waitStreamReady();
   EncodingType getEncodingType(const VXIMap *properties);
 
   VXIinetResult skipBody();
 
   SBinetValidator* _validator;
 };
 
 #endif
