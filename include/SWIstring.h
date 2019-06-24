/* SWIstring, character conversion routines */

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
 

 #ifndef _SWISTRING_H_
 #define _SWISTRING_H_
 
 #include <SWIchar.h>
 
 #ifdef __cplusplus
 extern "C" {
 #endif
 
   /* li_string.cpp */
 extern double SWIatof( const char *str);
 extern float SWIatofloat( const char *str);
 extern SWIcharResult SWIdtostr(double d, char *str, int len);
 extern SWIcharResult SWIdtowcs(double d, wchar_t *wstr, int len);
 
 
   /* SWIstring.c */
 double SWIwcstod( const wchar_t *wstr);
 float SWIwcstof( const wchar_t *wstr);
 SWIcharResult SWIwtof(const wchar_t *wstr, float *fval);
 
 SWIcharResult SWIstrtowcs(const char *str, wchar_t *wstr, int len);
 SWIcharResult SWIwcstostr(const wchar_t *wstr, char *str, int len);
 SWIcharResult SWIitowcs(int i, wchar_t *wstr, int len);
 SWIcharResult SWIwcstoi(const wchar_t *wstr, int *pi);
 int SWIwtoi(const wchar_t *wstr);
 int SWIwcsstrcmp(const wchar_t *w, const char *str);
 
 int SWIisascii(const wchar_t *wstr);
 int SWIislatin1(const wchar_t *wstr);
 int SWIisvalid_unicode(const wchar_t *wstr);
 
 wchar_t* SWIwcstok (wchar_t* wcs, const wchar_t* delim, wchar_t** ptr);
 char *SWIstrtok(char *str, const char *delim, char **ptr);
 #ifdef __cplusplus
 }
 #endif
 
 #endif
