/*****************************************************************************
  *****************************************************************************
  *
  * $Id: SBjsiString.hpp,v 1.1.2.2 2003/10/06 19:19:17 mpanacci Exp $
  *
  * SBinetString, string class that is a subset of STL wstring
  *
  * The SBinetString class stores a string in a grow-only buffer, a
  * functional subset of the STL wstring class. This header merely
  * exists to make it easy to eliminate the use of STL wstring which
  * does not exist on some Linux versions.
  *
  *****************************************************************************
  ****************************************************************************/
 

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
 

 // -----1=0-------2=0-------3=0-------4=0-------5=0-------6=0-------7=0-------8
 
 #ifndef _SBINET_STRING_H__
 #define _SBINET_STRING_H__
 
 #include "SBjsiInternal.h"
 #include "VXItypes.h"
 
 // Utility method for end users to convert wide to narrow characters
 // (characters that cannot be converted become a Latin-1 upside down ?
 // character), not used here
 inline char SBinetW2C(wchar_t w)
 {
   return ((w & 0xff00)?'\277':((unsigned char) (w & 0x00ff)));
 }
 
 #ifdef NO_STL
 #include "SBjsiStringNoSTL.hpp"
 #else
 #include "SBjsiStringSTL.hpp"
 #endif
 
 #endif  /* include guard */
