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
 
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include "SWIchar.h"
 
 const wchar_t *SWIcharReturnCodeToWcs( SWIcharResult rc)
 {
   switch (rc) {
 		case SWIchar_SUCCESS:
 			return L"SUCCESS";
 		case SWIchar_ERROR:
 			return L"ERROR";
 		case SWIchar_FAIL:
 			return L"FAIL";
 		case SWIchar_INVALID_INPUT:
 			return L"INVALID INPUT";
 		case SWIchar_OUT_OF_MEMORY:
 			return L"OUT OF MEMORY";
 		case SWIchar_BUFFER_OVERFLOW:
 			return L"BUFFER OVERFLOW";
 		case SWIchar_CONVERSION_LOSS:
 			return L"CONVERSION LOSS";
 		default:
 			return L"UNKNOWN";
 	}
 }
