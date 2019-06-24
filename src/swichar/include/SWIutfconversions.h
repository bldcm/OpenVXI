/* SWIutfconversions, character conversion routines */

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
 

 #ifndef _SWIUTFCONVERSIONS_H_
 #define _SWIUTFCONVERSIONS_H_
 
 #include <stdio.h>
 #include <SWIchar.h>
 
 #ifdef __cplusplus
 extern "C" {
 #endif
 
   /***********************************************************************
    *  Character conversion routines
    ***********************************************************************/
 
   /* The following functions return:
   **   -1: Couldn't fit into buffer.
   **   -2: Decoding problem.  Seems to be invalid code!
   **   Else: The length of the resulting string (slen or wcslen; i.e. not 
   **   including null)
   **
   ** The maxdstlen parameter is also a len.  Therefore, it's assumed that 
   ** the actual buffer size
   **   is one greater than maxdstlen to hold the NULL!
   */
   SWIcharResult SWIwcstoutf8(const wchar_t *src, unsigned char *dst, int maxdstlen);
   SWIcharResult SWIutf8towcs( const unsigned char *src, wchar_t *dst, int maxdstlen );
 
   /* these are also str/wcslen's.  So, for allocation purposes, need to add 
    * 1 and multiply
    * by sizeof char/wchar_t
    */
   int SWIwcstoutf8len(const wchar_t* src);
   int SWIutf8towcslen(const unsigned char* src);
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif
