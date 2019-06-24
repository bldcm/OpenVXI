
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
 

 /* $Id: SWIutilInternal.h,v 1.3.2.2.6.1 2003/10/20 13:28:37 mpanacci Exp $ */
 
 #ifndef SWIUTIL_INTERNAL_H
 #define SWIUTIL_INTERNAL_H
 
 #ifndef NULL
 #ifdef  __cplusplus
 #define NULL    0
 #else
 #define NULL    ((void *)0)
 #endif
 #endif
 
 #ifdef WIN32
 #define wcsncasecmp(s1,s2,n) _wcsnicmp(s1,s2,n)
 #define wcscasecmp(s1,s2) _wcsicmp(s1,s2)
 #define strcasecmp(a,b) _stricmp(a,b)
 #endif
 
 #ifdef __cplusplus
 
 namespace std { };
 using namespace std;
 
 #ifdef _MSC_VER           /* Microsoft Visual C++ */
 
 /* Suppress/disable warnings triggered by STL headers */
 #pragma warning(4 : 4786) /* identifier was truncated to '255' 
 			     characters in the debug information */
 
 #endif /* _MSC_VER */
 
 #if defined(__GNUC__) && (! defined(_GLIBCPP_USE_WCHAR_T))
 
 #include <string>
 
 namespace std
 {
   typedef basic_string<wchar_t> wstring;
 }
 
 #endif
 
 #endif /* __cplusplus */
 
 #endif /* include guard */
