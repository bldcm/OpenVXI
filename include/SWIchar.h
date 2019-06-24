/* SWIchar, character conversion routines */

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
 

 #ifndef _SWICHAR_H_
 #define _SWICHAR_H_
 
 #ifdef __cplusplus
 #include <cwchar>
 #else
 #include <wchar.h>
 #endif
 
 #ifdef __cplusplus
 extern "C" {
 #endif
 
 typedef enum {
   SWIchar_CONVERSION_LOSS = -6,   // when wchar -> latin1
   SWIchar_BUFFER_OVERFLOW = -5,
   SWIchar_OUT_OF_MEMORY = -4,  // malloc returns null
   SWIchar_INVALID_INPUT = -3,  // null, empty string, unicode when should be ascii only
   SWIchar_FAIL = -2,    // unable to convert
   SWIchar_ERROR = -1,    // general error
   SWIchar_SUCCESS = 0
 } SWIcharResult;
 
 #define SWIchar_isspace(x) ((x) == ' ' || (x) == '\t' || (x) == '\n' || \
                         (x) == '\r' || (x) == '\013' || (x) == '\014')
 #define SWIchar_iswspace(x) ((x) == L' ' || (x) == L'\t' || (x) == L'\n' || \
                         (x) == L'\r' || (x) == L'\013' || (x) == L'\014')
 
 #define SWIchar_isdigit(x) ((x) >= '0' && (x) <= '9')
 #define SWIchar_iswdigit(x) ((x) >= L'0' && (x) <= L'9')
 
 #define SWIchar_isalpha(x) (((x) >= 'a' && (x) <= 'z') || \
     ((x) >= 'A' && (x) <= 'Z'))
 #define SWIchar_iswalpha(x) (((x) >= L'a' && (x) <= L'z') || \
     ((x) >= L'A' && (x) <= L'Z'))
 
 #define SWIBUFLEN(x) (sizeof(x)/sizeof(x[0]))
 
 #define SWIchar_MAXSTRLEN    64
 
 const wchar_t *SWIcharReturnCodeToWcs(SWIcharResult rc);
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif
 
 
