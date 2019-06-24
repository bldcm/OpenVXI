/* SWIstring, locale independant conversions */

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
 
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <limits.h>
 #include <SWIstring.h>
 #include "misc_string.h"
 
 #define INVALID_IUC_16BIT(C) \
    (C >= 0xD800 && C <= 0xDBFF) || \
    (C == 0xFFFE || C == 0xFFFF)
 
 #define INVALID_IUC_32BIT(C) \
    (C & 0xFFFF0000) || INVALID_IUC_16BIT(C)
 
 /*  SWIisvalid_unicode tests that the given wchar string
  *    - does not contain high surrogates (D800 to DBFF)
  *    - does not contain non-characters (FFFE and FFFF)
  *    - the top 16-bit of 32-bit wchar are 0
  */
 int SWIisvalid_unicode(const wchar_t *wstr)
 {
   int i, len = wcslen(wstr);
 
 #if defined(_linux_) && defined(__GNUC__)
     for (i = 0; i < len; i++, wstr++) {
       if ( INVALID_IUC_32BIT(*wstr) )
         return 0;
     }
 #elif defined (_win32_)
     for (i = 0; i < len; i++, wstr++) {
       if ( INVALID_IUC_16BIT(*wstr) )
         return 0;
     }
 #else
   if (sizeof(wchar_t) == 4) {
     for (i = 0; i < len; i++, wstr++) {
       if ( INVALID_IUC_32BIT(*wstr) )
         return 0;
     }
   }
   else {
     for (i = 0; i < len; i++, wstr++) {
       if ( INVALID_IUC_16BIT(*wstr) )
         return 0;
     }
   }
 #endif
   return 1;
 }
 
 /*  SWIisascii() function tests that the given wchar string
  *  contains only ASCII characters, which are any character
  *  with a value less than than or equal to 0x7F.
  */
 int SWIisascii(const wchar_t *wstr)
 {
   int i, len = wcslen(wstr);
 
   for (i = 0; i < len; i++) {
     if (*wstr++ > 0x7F)
       return 0;
   }
   return 1;
 }
 
 /*  SWIislatin1() function tests that the given wchar string
  *  contains only LATIN-1 characters, which are any character
  *  with a value less than than or equal to 0xFF.
  */
 int SWIislatin1(const wchar_t *wstr)
 {
   int i, len = wcslen(wstr);
 
   for (i = 0; i < len; i++) {
     if (*wstr++ > 0xFF)
       return 0;
   }
   return 1;
 }
 
   /* FIXME?  lots of error checking here - strlen, ascii;
    *  should we be less risk conscious but more efficient?
    */
 double SWIwcstod( const wchar_t *wstr)
 {
   int i;
   unsigned char tmpbuf[SWIchar_MAXSTRLEN];
   unsigned char *str;
   double dval;
 
   int len = wcslen(wstr);
   str = BUFMALLOC2(tmpbuf, SWIchar_MAXSTRLEN, len);
   if (!str)
     return 0.0;
 
   for (i = 0; i < len; i++) {
     if (*wstr > 0x7F)  /* is it ascii at least? */
       return 0.0;
     str[i] = (unsigned char) *wstr++;
   }
   str[i] = '\0';
   
   dval =  SWIatof(str);
   BUFFREE(tmpbuf, str);
   return dval;
 }
 
 
   /* FIXME?  lots of error checking here - strlen, ascii;
    *  should we be less risk conscious but more efficient?
    */
 float SWIwcstof( const wchar_t *wstr)
 {
   int i;
   unsigned char tmpbuf[SWIchar_MAXSTRLEN];
   unsigned char *str;
   float fval;
 
   int len = wcslen(wstr);
   str = BUFMALLOC2(tmpbuf, SWIchar_MAXSTRLEN, len);
   if (!str)
     return 0.0;
 
   for (i = 0; i < len; i++) {
     if (*wstr > 0x7F)  /* is it ascii at least? */
       return 0.0;
     str[i] = (unsigned char) *wstr++;
   }
   str[i] = '\0';
   
   fval =  SWIatofloat(str);
   BUFFREE(tmpbuf, str);
 
   return fval;
 }
 
   /* FIXME?  lots of error checking here - strlen, ascii;
    *  should we be less risk conscious but more efficient?
    */
 SWIcharResult SWIwtof( const wchar_t *wstr, float *fval)
 {
   int i;
   unsigned char tmpbuf[SWIchar_MAXSTRLEN];
   unsigned char *str;
 
   int len = wcslen(wstr);
   str = BUFMALLOC2(tmpbuf, SWIchar_MAXSTRLEN, len);
   if (!str)
     return SWIchar_BUFFER_OVERFLOW;
 
   for (i = 0; i < len; i++) {
     if (*wstr > 0x7F)  /* is it ascii at least? */
       return SWIchar_INVALID_INPUT;
     str[i] = (unsigned char) *wstr++;
   }
   str[i] = '\0';
   
   *fval =  SWIatofloat(str);
   BUFFREE(tmpbuf, str);
 
   return SWIchar_SUCCESS;
 }
 
 
 /* Converts char-based string to wchar-based string
  * - len is size of wstr
  * - returns SWIchar_BUFFER_OVERFLOW if len(str)>=len
  */
 SWIcharResult SWIstrtowcs(const char *str, wchar_t *wstr, int len)
 {
   const wchar_t *funcName = L"SWIstrtowcs";
   int l;
   int i;
 
   l = strlen(str);
 
   if (l >= len) {
     return SWIchar_BUFFER_OVERFLOW;
   }
 
   for (i = 0; i < l + 1; i++) {
     wstr[i] = (unsigned char)str[i];  // the high bit is not a sign bit //
   }
 
   return SWIchar_SUCCESS;
 }
 
 
 /* Converts wchar-based string to char-based string.
  * - when there is conversion loss, returns SWIchar_CONVERSION_LOSS
  *   and each loss-affected char is \277 (upside-down question mark)
  * - len is size of str
  * - returns SWIchar_BUFFER_OVERFLOW if len(wstr)>=len
 */
 SWIcharResult SWIwcstostr(const wchar_t *wstr, char *str, int len)
 {
   const wchar_t *funcName = L"wstrtostr";
   int l;
   int i;
   SWIcharResult rc = SWIchar_SUCCESS;
 
   l = wcslen(wstr);
 
   if (l > len) {
     return SWIchar_BUFFER_OVERFLOW;
   }
 
   for (i = 0; i < l + 1; i++) {
     str[i] = (unsigned char) wstr[i];
     if (str[i] != wstr[i]) {
       rc = SWIchar_CONVERSION_LOSS;
       str[i] = '\277';
     }
   }
   return rc;
 }
 
 
 SWIcharResult SWIitowcs(int i, wchar_t *wstr, int len)
 {
   const wchar_t *funcName = L"SWIitowcs";
   unsigned char buf[SWIchar_MAXSTRLEN];
   unsigned char *pend = buf+SWIchar_MAXSTRLEN;
   unsigned char *pbuf = pend;
   int sign = 1;
 
   if ( i < 0 ) {
     if ( i == INT_MIN ) {
       return SWIchar_INVALID_INPUT;
     }
 
     i = -i;
     sign = -1;
   }
 
   while(i) {
     int ch = i%10;
     i/=10;
 
     *--pbuf = ch+'0';
   }
 
   if (pbuf == pend)
     *--pbuf = '0';
 
   if ( sign == -1 ) {
     *--pbuf = '-';
   }
 
   if ( pend-pbuf > len ) {
     return SWIchar_BUFFER_OVERFLOW;
   }
 
   while (pbuf < pend) {
     *wstr++ = (wchar_t) *pbuf++;
   }
   *wstr = '\0';
   return SWIchar_SUCCESS;
 }
 
 
   /* The function stops reading the input string at the first character that it 
    * cannot recognize as part of a number. This character may be the null character 
    * (L'\0') terminating the string.
    * Returns SWIchar_INVALID_INPUT if string is empty, or
    *  does not start with [sign]digit
    */
 SWIcharResult SWIwcstoi(const wchar_t *wstr, int *pi)
 {
   int value = 0;
   int sign = 1;
   const wchar_t *start;
 
   while ( SWIchar_isspace(*wstr) && *wstr != '\0' ) wstr++;
 
   if ( *wstr == '-' ) {
     sign = -1;
     wstr++;
   }
   else if ( *wstr == '+' ) {
     wstr++;
   }
   start = wstr;
 
   while( *wstr ) {
     if ( *wstr < '0' || *wstr > '9' ) {
       break;
     }
     value = 10*value + *wstr++ - '0';
   }
 
   if (start == wstr)  /*  no digits in the string */
     return SWIchar_INVALID_INPUT;
 
   if ( sign == -1 ) value = -value;
 
   *pi = value;
   return SWIchar_SUCCESS;
 }
 
 
 int SWIwtoi(const wchar_t *wstr)
 {
   int val;
   SWIcharResult rc = SWIwcstoi( wstr, &val );
   if (rc != SWIchar_SUCCESS)
     return 0;
 
   return val;
 }
 
 
 int SWIwcsstrcmp(const wchar_t *w, const char *str)
 {
   while (*w && *str && *w == *str) {w++;str++;}
   return *w - *str;
 }
 
 #ifndef _win32_
 
 wchar_t *SWIwcstok(wchar_t *wcs, const wchar_t *delim,
 			      wchar_t **ptr)
 {
 #ifndef __GNUC__
   return wcstok_r(wcs,delim,ptr);
 #else
   return wcstok(wcs,delim,ptr);
 #endif
 }
 
 char *SWIstrtok(char *str, const char *delim, char **ptr)
 {
 #ifndef __GNUC__
   return strtok_r(str,delim,ptr);
 #else
   return strtok(str,delim);
 #endif
 }
 
 
 #elif (_win32_)
 
 wchar_t *SWIwcstok(wchar_t *wcs, const wchar_t *delim,
 			      wchar_t **ptr)
 {
   *ptr = NULL;
   return wcstok(wcs, delim);
 }
 
 char *SWIstrtok(char *str, const char *delim,
 			      char **ptr)
 {
   *ptr = NULL;
   return strtok(str, delim);
 }
 
 #endif
