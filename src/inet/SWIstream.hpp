
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
 
#ifndef SWIstream_H
 #define SWIstream_H
 
 #include "SWIutilHeaderPrefix.h"
 
 class SWIUTIL_API_CLASS SWIstream
 {
  public:
   enum Result
   {
     SUCCESS = 0,
     END_OF_FILE = -1,
     INVALID_ARGUMENT = -2,
     ILLEGAL_STATE = -3,
     READ_ERROR = -4,
     WRITE_ERROR = -5,
     OPEN_ERROR = -6,
     NOT_SUPPORTED = -7,
     PARSE_ERROR = -8,
     WOULD_BLOCK = -9,
     TIMED_OUT = -10,
     BUFFER_OVERFLOW = -11,
     FAILURE = -12,
     CONNECTION_ABORTED = -13
   };
 };
 
 #endif
