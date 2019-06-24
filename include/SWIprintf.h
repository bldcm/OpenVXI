
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
 

 /* -----1=0-------2=0-------3=0-------4=0-------5=0-------6=0-------7=0-------8
  */
 
 #ifndef _SWI_PRINTF_H__
 #define _SWI_PRINTF_H__
 
 #include <stdio.h>
 #include <stdarg.h>
 #include <SWIchar.h>
 
 /* This is required for OSB reference source distributions of this
  * file, where we have to get these functions exported without a def
  * file. */
 #ifdef SWIPRINTF_DLL_EXPORT
 #ifdef __cplusplus
 #define SWIPRINTF_API extern "C" __declspec(dllexport)
 #else
 #define SWIPRINTF_API __declspec(dllexport)
 #endif
 #else
 #ifdef __cplusplus
 #define SWIPRINTF_API extern "C"
 #else
 #define SWIPRINTF_API extern
 #endif
 #endif
 
 SWIPRINTF_API
 int SWIfprintf (FILE* file, const char* format, ...);
 
 SWIPRINTF_API
 int SWIfwprintf (FILE* file, const wchar_t* format, ...);
 
 SWIPRINTF_API
 int SWIsprintf (char* str, size_t maxlen, const char* format, ...);
 
 SWIPRINTF_API
 int SWIswprintf (wchar_t* wcs, size_t maxlen, const wchar_t* format, ...);
 
 SWIPRINTF_API
 int SWIvsprintf (char* str, size_t maxlen, const char* format, va_list args);
 
 SWIPRINTF_API
 int SWIvswprintf (wchar_t* wcs, size_t maxlen, const wchar_t* format,
 		  va_list args);
 
 SWIPRINTF_API
 wchar_t* SWIfgetws (wchar_t* ws, int n, FILE* stream);
 
 /* yatman: added solaris to the mix */
 #ifdef _linux_
 SWIPRINTF_API
 int SWIswscanf(const wchar_t* buffer, const wchar_t* format, ... );
 #elif defined(_win32_)
 #define SWIswscanf swscanf
 #elif defined(_solaris_)
 #define SWIswscanf swscanf
 #else
 /* Assume we have swscanf() and see what happens */
 #define SWIswscanf swscanf
 #endif
 
 #endif  /* _SWI_PRINTF_H__ */
