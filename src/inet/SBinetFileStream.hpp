
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
 
#ifndef SBINETFILESTREAM_HPP
 #define SBINETFILESTREAM_HPP
 
 #include "SBinetStream.hpp"
 #include "SBinetLog.h"
 #include "SBinetChannel.h"
 
 class SBinetFileStream : public SBinetStream
 {
  public:
   SBinetFileStream(SBinetURL* url, SBinetChannel* ch,
 		   VXIlogInterface *log, VXIunsigned diagLogBase);
   ~SBinetFileStream();
 
   VXIinetResult Open(VXIint flags,
 		     const VXIMap* properties,
 		     VXIMap* streamInfo);
 
   VXIinetResult Read( /* [OUT] */ VXIbyte*         pBuffer,
 		      /* [IN]  */ VXIulong         nBuflen,
 		      /* [OUT] */ VXIulong*        pnRead );
 
   VXIinetResult Write(/* [IN]  */ const VXIbyte*   pBuffer,
                       /* [IN]  */ VXIulong         nBuflen,
                       /* [OUT] */ VXIulong*        pnWritten);
 
   VXIinetResult Close();
 
  private:
   FILE* _pFile;
   VXIint _content_length;
   int _ReadSoFar;
 
   SBinetChannel* _channel;
 };
 
 #endif
