
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
 
/************************************************************************
  *
  * $Id: VXIheaderSuffix.h,v 1.4.24.1 2003/10/06 19:01:44 mpanacci Exp $
  *
  * Settings which should end all public headers
  *
  ************************************************************************
  */
 
 /* Reset the structure packing conventions */
 
 #if defined(_MSC_VER)            /* Microsoft Visual C++ */
   #pragma pack(pop)
 #elif defined(__BORLANDC__)      /* Borland C++ */
   #pragma option -a.
 #elif defined(__WATCOMC__)       /* Watcom C++ */
   #pragma pack(pop)
 #endif
