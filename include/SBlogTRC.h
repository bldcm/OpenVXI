
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
 

 #ifndef _SBLOGTRC_H              /* Allows multiple inclusions */
 #define _SBLOGTRC_H
 
 #include "VXIlog.h"              /* For VXIlog base interface */
 #include "VXIvalue.h"            /* For VXIMap */
 
 #include "VXIheaderPrefix.h"
 #ifdef SBLOGTRC_EXPORTS
 #define SBLOGTRC_API SYMBOL_EXPORT_DECL
 #else
 #define SBLOGTRC_API SYMBOL_IMPORT_DECL
 #endif
 
 #define IMPLEMENTATION_NAME L"com.speechworks.SBlogTrc"
 
   /* -- start docme interface -- */
 
 /**
  * Global platform initialization of SBlogTrc
  *
  * errorRangeMap    Map that defines the error range for each module
  *                  that will perform logging. Each key is a VXIString with
  *                  a module name as passed in the moduleName argument for
  *                  the VXIlog interface (see VXIlog.h). The value for each
  *                  key is a VXIInteger that specifies the base error ID for
  *                  that module, every error logged by that module will be
  *                  offset by this value.
  *
  * @result VXIlogResult 0 on success
  */
 SBLOGTRC_API VXIlogResult SBlogTrcInit(const VXIMap *errorRangeMap);
 
 /**
  * Global platform shutdown of Log
  *
  * @result VXIlogResult 0 on success
  */
 SBLOGTRC_API VXIlogResult SBlogTrcShutDown(void);
 
 /**
  * Create a new log service handle
  *
  * @result VXIlogResult 0 on success 
  */
 SBLOGTRC_API VXIlogResult SBlogTrcCreateResource(VXIlogInterface **log);
 
 /**
  * Destroy the interface and free internal resources
  *
  * @result VXIlogResult 0 on success 
  */
 SBLOGTRC_API VXIlogResult SBlogTrcDestroyResource(VXIlogInterface **log);
 
   /* -- end docme interface -- */
 
 #include "VXIheaderSuffix.h"
 
 #endif  /* include guard */
