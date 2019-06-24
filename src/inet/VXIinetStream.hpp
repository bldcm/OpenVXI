
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
 
#ifndef VXIINETSTREAM_HPP
 #define VXIINETSTREAM_HPP
 
 #include "VXIvalue.h"
 #include "VXIinet.h"
 
 struct VXIinetStream
 {
  public:
   virtual ~VXIinetStream()
   {}
 
   virtual VXIinetResult Open(VXIint flags,
                              const VXIMap* properties,
                              VXIMap* streamInfo) = 0;
 
   virtual VXIinetResult Read(/* [OUT] */ VXIbyte*         pBuffer,
                              /* [IN]  */ VXIulong         nBuflen,
                              /* [OUT] */ VXIulong*        pnRead ) = 0;
 
   virtual VXIinetResult Write(/* [IN] */ const VXIbyte*   pBuffer,
                               /* [IN]  */ VXIulong         nBuflen,
                               /* [OUT] */ VXIulong*        pnWritten) = 0;
 
   virtual VXIinetResult Close() = 0;
 };
 
 #endif
