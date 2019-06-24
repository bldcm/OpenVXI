/*****************************************************************************
  *****************************************************************************
  *
  * $Id: SBjsi.h,v 1.1.2.2 2003/10/06 19:19:17 mpanacci Exp $
  *
  * SBjsi JavaScript (ECMAScript) Engine Interface
  *
  * SBjsi interface, an implementation of the VXIjsi abstract interface
  * for interacting with a JavaScript (ECMAScript) engine.  This
  * provides functionality for creating JavaScript execution contexts,
  * manipulating JavaScript scopes, manipulating variables within those
  * scopes, and evaluating JavaScript expressions/scripts.
  *
  * There is one JavaScript interface per thread/line.
  *
  *****************************************************************************
  ****************************************************************************/
 

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
 

 #ifndef _SBJSI_H
 #define _SBJSI_H
 
 #include "VXIjsi.h"                    /* For VXIjsi base interface */
 #include "VXIlog.h"                    /* For VXIlog interface */
 
 #include "VXIheaderPrefix.h"
 #ifdef SBJSI_EXPORTS
 #define SBJSI_API SYMBOL_EXPORT_DECL
 #else
 #define SBJSI_API SYMBOL_IMPORT_DECL
 #endif
 
   /* -- start docme interface -- */
 
 /**
  * @name SBjsi
  * @memo SBjsi implementation of VXIjsi
  * @doc
  * SBjsi interface, an implementation of the VXIjsi interface for
  * interacting with a ECMAScript (JavaScript) engine.  This provides
  * functionality for creating ECMAScript execution contexts,
  * manipulating ECMAScript scopes, manipulating variables within those
  * scopes, and evaluating ECMAScript expressions/scripts. <p>
  *
  * There is one VXIjsi interface per thread/line.  
  */
 
   /*@{*/
 
 
 /* Recommended defaults for SBjsiInit */
 #define JSI_RUNTIME_SIZE_DEFAULT     (1024 * 1024 * 16)
 #define JSI_CONTEXT_SIZE_DEFAULT     (1024 * 128)
 #define JSI_MAX_BRANCHES_DEFAULT     100000
 
 /**
  * Global platform initialization of JavaScript
  *
  * @param log             VXI Logging interface used for error/diagnostic 
  *                        logging, only used for the duration of this 
  *                        function call
  * @param  diagLogBase    Base tag number for diagnostic logging purposes.
  *                        All diagnostic tags for SBjsi will start at this
  *                        ID and increase upwards.
  * @param  runtimeSize    Size of the JavaScript runtime environment, in 
  *                        bytes. There is one runtime per process. See 
  *                        above for a recommended default.
  * @param  contextSize    Size of each JavaScript context, in bytes. There
  *                        may be multiple contexts per channel, although
  *                        the VXI typically only uses one per channel.
  *                        See above for a recommended default.
  * @param  maxBranches    Maximum number of JavaScript branches for each 
  *                        JavaScript evaluation, used to interrupt infinite
  *                        loops from (possibly malicious) scripts
  *
  * @result VXIjsiResult 0 on success
  */
 SBJSI_API VXIjsiResult SBjsiInit (VXIlogInterface  *log,
 				  VXIunsigned       diagLogBase,
 				  VXIlong           runtimeSize,
 				  VXIlong           contextSize,
 				  VXIlong           maxBranches);
 
 /**
  * Global platform shutdown of JavaScript
  *
  * @param log    VXI Logging interface used for error/diagnostic logging,
  *               only used for the duration of this function call
  *
  * @result VXIjsiResult 0 on success
  */
 SBJSI_API VXIjsiResult SBjsiShutDown (VXIlogInterface  *log);
 
 /**
  * Create a new JavaScript service handle
  *
  * @param log    VXI Logging interface used for error/diagnostic 
  *               logging, must remain a valid pointer throughout the 
  *               lifetime of the resource (until SBjsiDestroyResource( )
  *               is called)
  *
  * @result VXIjsiResult 0 on success 
  */
 SBJSI_API VXIjsiResult SBjsiCreateResource(VXIlogInterface   *log,
 					   VXIjsiInterface  **jsi);
 
 /**
  * Destroy the interface and free internal resources. Once this is
  *  called, the logging interface passed to SBjsiCreateResource( ) may
  *  be released as well.
  *
  * @result VXIjsiResult 0 on success 
  */
 SBJSI_API VXIjsiResult SBjsiDestroyResource(VXIjsiInterface **jsi);
 
   /* -- end docme interface -- */
 /*@}*/
 #include "VXIheaderSuffix.h"
 
 #endif  /* include guard */
