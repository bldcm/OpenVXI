
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
 

 #ifndef _SBCACHE_LOG_H
 #define _SBCACHE_LOG_H
 
 #include "VXIlog.h"          /* Logging interface */
 #ifdef __cplusplus
 #include "SBinetLogger.hpp"  /* Logging base class */
 #endif
 
 /* Module defines */
 #ifndef MODULE_PREFIX
 #define MODULE_PREFIX  COMPANY_DOMAIN L"."
 #endif
 
 #ifdef OPENVXI
 #define MODULE_SBCACHE                 MODULE_PREFIX L"OSBcache"
 #define SBCACHE_IMPLEMENTATION_NAME    COMPANY_DOMAIN L".OSBcache"
 #else
 #define MODULE_SBCACHE                 MODULE_PREFIX L"SBcache"
 #define SBCACHE_IMPLEMENTATION_NAME    COMPANY_DOMAIN L".SBcache"
 #endif
 
 #define SBCACHE_API_TAGID         0     /* API log */
 #define SBCACHE_MGR_TAGID         1     /* Cache manager log */
 #define SBCACHE_ENTRY_TAGID       2     /* Cache entry log */
 #define SBCACHE_STREAM_TAGID      3     /* Cache stream log */
 #define SBCACHE_ET_MUTEX_TAGID    4     /* Entry table mutex log */
 #define SBCACHE_E_MUTEX_TAGID     5     /* Entry mutex log */
 #define SBCACHE_CLEANUP_TAGID     6     /* Cache cleanup */
 
 #endif  /* _SBCACHE_LOG_H */
