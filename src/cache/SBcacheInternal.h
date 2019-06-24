
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
 

 #ifndef _SBcache_INTERNAL_H__
 #define _SBcache_INTERNAL_H__
 
 #ifndef NULL
 #ifdef  __cplusplus
 #define NULL    0
 #else
 #define NULL    ((void *)0)
 #endif
 #endif
 
 #ifdef _MSC_VER           /* Microsoft Visual C++ */
 #ifdef __cplusplus
 
 namespace std { };
 using namespace std;
 
 /* Suppress/disable warnings triggered by STL headers */
 #pragma warning(4 : 4786) /* identifier was truncated to '255' 
 			     characters in the debug information */
 
 #endif /* __cplusplus */
 #endif /* _MSC_VER */
 
 #endif /* _SBcache_INTERNAL_H__ */
