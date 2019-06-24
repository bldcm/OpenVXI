
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
 

 #ifndef _SBINET_LOG_H
 #define _SBINET_LOG_H
 
 #include "VXIlog.h"          /* Logging interface */
 #ifdef __cplusplus
 #include "SBinetLogger.hpp"  /* Logging base class */
 #endif
 
 /* Module defines */
 #ifndef MODULE_PREFIX
 #define MODULE_PREFIX  COMPANY_DOMAIN L"."
 #endif
 
 #ifdef OPENVXI
 #define MODULE_SBINET                  MODULE_PREFIX L"OSBinet"
 #define SBINET_IMPLEMENTATION_NAME     COMPANY_DOMAIN L".OSBinet"
 #else
 #define MODULE_SBINET                  MODULE_PREFIX L"SBinet"
 #define SBINET_IMPLEMENTATION_NAME     COMPANY_DOMAIN L".SBinet"
 #endif
 
 #define MODULE_SBINET_API_TAGID             0
 #define MODULE_SBINET_CHANNEL_TAGID         1
 #define MODULE_SBINET_STREAM_TAGID          2
 #define MODULE_SBINET_COOKIE_TAGID          3
 #define MODULE_SBINET_VALIDATOR_TAGID       4
 #define MODULE_SBINET_CACHE_TAGID           5
 #define MODULE_SBINET_TIMING_TAGID          6
 #define MODULE_SBINET_PROXY_TAGID           7
 #define MODULE_SBINET_DUMP_HTTP_MSGS        10
 
 /* Only for SBinetLibwww, preserved for OpenVXI distributions */
 #define MODULE_SBINET_HTALERT_TAGID         100
 #define MODULE_SBINET_HTPRINT_TAGID         101
 #define MODULE_SBINET_HTTRACE_TAGID         102
 #define MODULE_SBINET_HTTRACEDATA_TAGID     103
 #define MODULE_SBINET_HTTP_HEADERS_TAGID    104
 
 #endif  /* _SBINET_LOG_H */
