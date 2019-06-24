
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
 

 /* (1) Platform specific macro which handles symbol exports & imports */
 
 #ifndef SWIUTIL_HEADER_PREFIX_H
 #define SWIUTIL_HEADER_PREFIX_H
 
 #ifdef _WIN32
 #ifdef __cplusplus
 #define SWIUTIL_EXPORT_DECL extern "C" __declspec(dllexport)
 #define SWIUTIL_IMPORT_DECL extern "C" __declspec(dllimport)
 #define SWIUTIL_EXPORT_CPP_DECL extern __declspec(dllexport)
 #define SWIUTIL_IMPORT_CPP_DECL extern __declspec(dllimport)
 #define SWIUTIL_EXPORT_CPP_CLASS_DECL __declspec(dllexport)
 #define SWIUTIL_IMPORT_CPP_CLASS_DECL __declspec(dllimport)
 #else
 #define SWIUTIL_EXPORT_DECL __declspec(dllexport)
 #define SWIUTIL_IMPORT_DECL __declspec(dllimport)
 #endif
 #else
 #ifdef __cplusplus
 #define SWIUTIL_EXPORT_DECL extern "C"
 #define SWIUTIL_IMPORT_DECL extern "C"
 #define SWIUTIL_EXPORT_CPP_DECL extern
 #define SWIUTIL_IMPORT_CPP_DECL extern
 #define SWIUTIL_EXPORT_CPP_CLASS_DECL
 #define SWIUTIL_IMPORT_CPP_CLASS_DECL
 #else
 #define SWIUTIL_EXPORT_DECL extern
 #define SWIUTIL_IMPORT_DECL extern
 #endif
 #endif
 
 #if !defined(SWIUTIL_EXPORT_DECL) || !defined(SWIUTIL_IMPORT_DECL)
 #error Symbol import/export pair not defined.
 #endif
 
 #ifdef SWIUTIL_NO_DLL
 
 #define SWIUTIL_API_CLASS
 #define SWIUTIL_API_CPP
 #define SWIUTIL_API
 
 #else  /* ! SWIUTIL_NO_DLL */
 
 #ifdef SWIUTIL_EXPORTS
 
 #ifdef __cplusplus
 #define SWIUTIL_API_CLASS SWIUTIL_EXPORT_CPP_CLASS_DECL
 #define SWIUTIL_API_CPP SWIUTIL_EXPORT_CPP_DECL
 #endif
 #define SWIUTIL_API SWIUTIL_EXPORT_DECL
 
 #else  /* ! SWIUTIL_EXPORTS */
 
 #ifdef __cplusplus
 #define SWIUTIL_API_CLASS SWIUTIL_IMPORT_CPP_CLASS_DECL
 #define SWIUTIL_API_CPP SWIUTIL_IMPORT_CPP_DECL
 #endif
 #define SWIUTIL_API SWIUTIL_IMPORT_DECL
 
 #endif /* SWIUTIL_EXPORTS */
 
 #endif /* SWIUTIL_NO_DLL */
 
 #endif /* include guard */
