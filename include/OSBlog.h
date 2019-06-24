
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
 

 #ifndef _OSBLOG_H                /* Allows multiple inclusions */
 #define _OSBLOG_H
 
 #include <VXIlog.h>              /* For VXIlog base interface */
 #include <VXItypes.h>
 
 #include "VXIheaderPrefix.h"
 #ifdef OSBLOG_EXPORTS
 #define OSBLOG_API SYMBOL_EXPORT_DECL
 #else
 #define OSBLOG_API SYMBOL_IMPORT_DECL
 #endif
 
 #ifdef __cplusplus
 extern "C" {
 #endif
 
 #define OSBLOG_MAX_TAG  79999  /* 80,000 tags, range from 0 - 79,999 */
 
 /**
  * Log file formats, currently the SpeechWorks proprietary log file
  * format with Latin 1 (ISO-8859-1) encoded characters or Unicode
  * (UTF-8) encoded characters.
  */
 #define OSBLOG_MIME_TEXT_LATIN1  L"text/plain;charset=ISO-8859-1"
 #define OSBLOG_MIME_TEXT_UTF8    L"text/plain;charset=UTF-8"
 #define OSBLOG_MIME_DEFAULT      OSBLOG_MIME_TEXT_LATIN1
 
 /**
  * @name OSBlog
  * @memo OSBlog implementation for logging errors, diagnostic logging, and
  * event logging.  
  *
  @doc
  OSBlog is an implementation of the 
  <a href="VXIlogInterface.html">  VXIlog interface </a> for logging.
  The OSBlogInterface extends the VXIlogInterface to support 
   enabling/disabling messages on an individual tag basis without
   requiring a recompile for use by consumers of the diagnostic
   log. Diagnostic( ) is highly efficient for cases when the tag is
   disabled: in other words, the lookup for seeing if a tag (an
   integer) is enabled should be done using a simple array or some
   other extremely low-overhead mechanism. OSBlog provides a way to
   enable/disable tags on-the-fly (without needing to restart the
   software). <p>
  
   Across all streams, OSBlog is responsible for automatically
   supplying the following information for end consumer use:
   <ul>
     <li> timestamp
     <li> (Error only) error key/values, the error number,
       and the supplied module name.
     <li> (Diagnostic only) tag number, and the
       supplied subtag name
     <li> (Event only) key/values pairs to log in the event
   <ul>
   <p>
  
   Each OpenVXI component provides an XML error file which integrators
   can choose to us (or rewrite) to map error numbers and error key/values
   to error text. An XSLT transform can do this in an error viewer.
 
  */
 
 /*@{*/
 
 /**
  ** @name OSBlog Interface
  ** @memo OSBlog extension interface to the VXIlog interface.
  ** @doc  OSBlog defines extensions to the VXIlog interface that support
  ** the tag mechanism
  */
 typedef struct OSBlogInterface
 {
   /** Include the a VXIlog interface structure.  */
   VXIlogInterface vxilog;
   
   /**
   ** @name ControlDiagnosticTag
   ** @memo Turn the diagnostic tag on (true) or off (false).
   ** @doc  All diagnostic log tags are assumed to be off unless
   ** specifically enabled.  This must be done by calling this function
   ** and setting the given tag to true.
   **
   ** @param  tagID    [IN] Identifier that classifies a group of logically
   **                  associated diagnostic messages (usually from a single
   **                  software module) that are desirable to enable or
   **                  disable as a single unit. See the top of this file 
   **                  for tagID allocation rules.
   ** @param  state    [IN] Boolean flag to turn the tag on (TRUE) or
   **                  off (FALSE).
   **
   ** @return   VXIlog_RESULT_SUCCESS:      success
   ** @return   VXIlog_RESULT_SYSTEM_ERROR: internal error
   **
   */
   VXIlogResult (*ControlDiagnosticTag)(
     struct OSBlogInterface *pThis,
     VXIunsigned             tagID,
     VXIbool                 state);
 
 } OSBlogInterface;
 
 
 /**
  * Global platform initialization of OSBlog
  *
  * @param  filename          Name of the file where diagnostic, error, and 
  *                           event information will be written. Pass NULL 
  *                           to disable logging to a file.
  * @param  fileMimeType      MIME content type for the log file, currently
  *                           LOG_MIME_TEXT_LATIN1 or LOG_MIME_TEXT_UTF8
  *                           as defined above, with LOG_MIME_TEXT_LATIN1
  *                           used if NULL or an empty string is passed.
  * @param  logToStdout       TRUE to log diagnostic and error messages to
  *                           standard out, FALSE to disable. Event reports
  *                           are never logged to standard out.
  *
  * @result VXIlog_RESULT_SUCCESS on success
  */
 OSBLOG_API VXIlogResult OSBlogInit(const char    *logFileName,
 				   const VXIchar *fileMimeType,
 				   VXIbool        logToStdout);
 
 /**
  * Global platform shutdown of Log
  *
  * @result VXIlog_RESULT_SUCCESS on success
  */
 OSBLOG_API VXIlogResult OSBlogShutDown(void);
 
 /**
  * Create a new log service handle
  *
  * @param channelNum      [IN] Logical channel number
  *
  * @result VXIlog_RESULT_SUCCESS on success 
  */
 OSBLOG_API VXIlogResult OSBlogCreateResource(VXIint            channelNum,
 					     VXIlogInterface **log);
 					     
 
 /**
  * Destroy the interface and free internal resources
  *
  * @result VXIlog_RESULT_SUCCESS on success 
  */
 OSBLOG_API VXIlogResult OSBlogDestroyResource(VXIlogInterface **log);
 
 /*@}*/
 
 #ifdef __cplusplus
 } /* extern "C" */
 #endif
 
 #include "VXIheaderSuffix.h"
 
 #endif  /* include guard */
