/* misc_string, miscellaneous internal string utility macros */

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
 

 #ifndef __MISC_STRING_H
 #define __MISC_STRING_H
 
 #ifdef __cplusplus
 extern "C" {
 #endif
 
 #define BUFSIZE (4096 + 1024) // typical maxlen is 4096, want room over that
 
 #define BUFMALLOC(buf, len) \
   ((sizeof(buf) >= (len)) ? (buf) : (char *) malloc(len))
 
 #define BUFMALLOC2(buf, currlen, len) \
   ((currlen >= len) ? (buf) : (char *) malloc(len))
 
 #define BUFFREE(buf, val) \
   if ((val) != (buf)) free(val)
 
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif
