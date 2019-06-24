/* li_string, locale independant conversions */

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
 
 #include <math.h>
 #include <stdlib.h>
 #include <stdio.h>
 #include <wctype.h>
 #include <wchar.h>
 #include <string.h>
 #include <float.h>
 #include "SWIstring.h"
 
 const int    MAX_PRECISION = 6;
 const double VIRTUAL_ZERO = pow(10, -MAX_PRECISION);
 const double TOO_BIG      = pow(10, MAX_PRECISION) - 1;
 
 /*=========================================================================
 ** SWIatof(string) -- Converts character string to double
 **
 ** Arguments:
 **           string -- string to be converted to double 
 **
 ** Returns:  double representation of value in wstring
 **
 ** Description:
 **          Ignores leading whitespace in string of this form:
 **          [(+|-)[0123456789]+(.[0123456789])]
 **          charcater.  Converts the integer parts and the
 **          decimal part separately, adding them together at the
 **          end and applying the sign
 ** 
 **=========================================================================
 */
 double SWIatof(const char *str)
 {
    double result = 0.0;
    int negative = 0;
    char *cp = NULL;
 
    // NOTE:  unicode values for white space, '+', and '-'
    // are unambiguous
 
    // Skip over whitespace
    while (SWIchar_isspace(*str) )
      str++;
 
    // Handle sign
    if (*str == '+')
      *str++;
    else if (*str == '-')
    {
      negative = 1;
      *str++;
    }
 
    // NOTE: This is very clever code. It intentionally calls the
    // locale dependant C library string to double function, strtod( ),
    // in an attempt for very efficient conversion with locale
    // dependant fraction delimiters (, for some locales)
    // supported. But then it goes on to ensure the SWI cannonical
    // fraction delimiter (.) is supported as well by doing a secondary
    // conversion where required.
 
    result = strtod(str, &cp);   // integer part
    if ( (cp != 0) && (*cp == '.'))  // has fraction?
    {
      cp++;  // skip over radix point
      int decDigits = strspn(cp, "0123456789");
      if (decDigits > 0) /* avoid divide by zero */
        result += strtod(cp, &cp) / pow(10, decDigits); // add fraction
    }
 
    // Apply the sign 
    if (negative)
       result = -result;
 
    return(result);
 }
 
 /*=======================================================================
 SWIatofloat -- as above but converts a string to a float
 **=======================================================================
 */
 float SWIatofloat(const char *str)
 {
   double d = SWIatof(str);
   return( (float)d );
 }
 
 /*========================================================================
 ** SWIdtowcs -- converts floating point number to a string
 **
 ** Arguments:
 **              double toConvert -- number to be converted
 **              wchar_t* result  -- container for wide char representation
 **              int len          -- size in characters of output buffer
 **
 ** Returns:     void
 **
 ** Description:
 **              Notes the sign of the number, using only its absolute
 **              value.  Converts the integer and fractional parts
 **              separately, combining them later, stripping extraneous
 **              trailing zeros, and applying the sign character.
 **
 ** NOTE:        The conversion fails if the number of digits in the
 **              integer part exceeds MAX_PRECISION - 1 (there must
 **              be at least a single digit in the fraction.)
 **
 **========================================================================
 */
 SWIcharResult SWIdtowcs(const double toConvert, 
                              wchar_t *result,
                              int len)
 {
   wchar_t integerString[SWIchar_MAXSTRLEN];
   wchar_t fractionString[SWIchar_MAXSTRLEN] = {L'.', L'0'};
   double d = toConvert;
   int integerPart = (int)d;  // overflow caught by TOO_BIG check below.
  
   SWIcharResult status = (d > TOO_BIG) ? SWIchar_BUFFER_OVERFLOW : SWIitowcs(integerPart, integerString, MAX_PRECISION-2);
   if (status == SWIchar_SUCCESS)
   {
     d -= integerPart;  // fraction remaining
     if (d < 0)         // fraction should be positive 
       d = -d;
 
     if ( (d > 0.0) && (d > VIRTUAL_ZERO) ) // has fraction?
     {
       int decimalPlaces = MAX_PRECISION - wcslen(integerString);
       if (decimalPlaces >= 1)  // least one digit after the decimal
       {
         wchar_t* fp = &fractionString[1];  // after decimal.
         while (d < 0.1)  // account for leading zeros.
         {
           *fp++ = L'0';
           d *= 10.0;
           decimalPlaces--;
         }
         int integerFract = (int)((d * pow(10, decimalPlaces)) + 0.5);
         status = SWIitowcs(integerFract, fp, SWIchar_MAXSTRLEN);
         if (status == SWIchar_SUCCESS)
         {
           // Remove extraneous zeros
           wchar_t* wp = fractionString + wcslen(fractionString) - 1;
           while (  (*wp == L'0') && ( *(wp-1) != L'.'))
             *wp-- = L'\0';
         }
       }
       else // too small for handle fraction
         status = SWIchar_BUFFER_OVERFLOW;
     }
 
     // Combine the integer and fractional parts
     if (status == SWIchar_SUCCESS)
     {
       if (wcslen(integerString)+wcslen(fractionString)+1 <= (unsigned int)len)
       {
         wcscpy(result, integerString);
         wcscat(result, fractionString);
       }
       else
         status = SWIchar_BUFFER_OVERFLOW;
     }
   }
 
   return(status);
 }
 
 /*=========================================================================
 ** SWIdtostr -- as above but converts from double to character string
 **=========================================================================
 */
 SWIcharResult SWIdtostr(const double toConvert, char *str, int len)
 {
   wchar_t wideArray[SWIchar_MAXSTRLEN];
   SWIcharResult status = SWIdtowcs(toConvert, wideArray, len);
   if (status == SWIchar_SUCCESS)
   {
     const wchar_t* wp = wideArray;
     status = SWIwcstostr(wp, str, len); 
   }
 
   return(status);
 }
