
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
 

 #ifndef _SBLOG_H                 /* Allows multiple inclusions */
 #define _SBLOG_H
 
 #include <VXIlog.h>              /* For VXIlog base interface */
 #include <VXIvalue.h>
 #include <time.h>
 
 #include "VXIheaderPrefix.h"
 #ifdef SBLOG_EXPORTS
 #define SBLOG_API SYMBOL_EXPORT_DECL
 #else
 #define SBLOG_API SYMBOL_IMPORT_DECL
 #endif
 
 /* forward declaration */
 #ifdef __cplusplus
 extern "C" struct SBlogInterface;
 extern "C" {
 #else
 struct SBlogInterface;
 #endif
 
 #define SBLOG_MAX_TAG  79999  /* 80,000 tags, range from 0 - 79,999 */
 
 /**
  * @name SBlog
  * @memo SBlog implementation of VXIlog
  *
  @doc
  SBlog is an implementation of the 
  <a href="VXIlogInterface.html">  VXIlog interface </a> for logging.
  The SBlogInterface extends the VXIlogInterface to support a listener
  registration mechanism for sending the logs to the final output
  stream. This mechanism allows multiple systems to tap on to a single
  log event so it can be sent both to  a file and to a central logging
  service is desired.  Listeners are invoked through callbacks.  The
  callback call is blocking.  If a listener is going to do signficant
  work or invoke functions which may block, for example a socket write,
  a messsage queue should be implemented so that callback is
  non-blocking. Failure to do this will impact overall system performance.<p>
  
   Across all streams, SBlog is responsible for automatically
   supplying the following information for end consumer use:
   <ul>
     <li> timestamp </li>
     <li> (Error only) error key/values, the error number,
       and the supplied module name. </li>
     <li> (Diagnostic only) tag number, and the
       supplied subtag name </li>
     <li> (Event only) key/values pairs to log in the event </li>
   </ul>
   <p>
  
   In addition, for diagnostic logging the SBlog defines a mechanism
   for enabling/disabling messages on an individual tag basis without
   requiring a recompile for use by consumers of the diagnostic
   log. Diagnostic( ) is highly efficient for cases when the tag is
   disabled: in other words, the lookup for seeing if a tag (an
   integer) is enabled should be done using a simple array or some
   other extremely low-overhead mechanism. SBlog provides a way to
   enable/disable tags on-the-fly (without needing to restart the
   software). <p>
 
   Each OSB PIK component provides an XML error file which integrators
   can choose to us (or rewrite) to map error numbers and error key/values
   to error text. An XSLT transform can do this in an error viewer.
 
  */
 
 /*@{*/
 
 /**
  ** @name SBlogStream
  ** @memo SBlog definition of a VXIlogStream
  ** @doc  SBlog defines a log stream as the following, which
  **       contains methods for writing data and closing the stream.
  */
 typedef struct SBlogStream
 {
   /**
    * @name Close
    * @memo Close the stream
    *
    * @param  stream   [IN/OUT] Handle to the stream to close, will be
    *                  set to NULL on success
    *
    * @return VXIlog_RESULT_SUCCESS on success
    */
   VXIlogResult (*Close)(struct SBlogStream  **stream);
 
   /**
    * @name Write
    * @memo Write content to the stream
    *
    * @param buffer   [IN]  Buffer of data to write to the stream
    * @param buflen   [IN]  Number of bytes to write
    * @param nwritten [OUT] Number of bytes actual written, may be less then
    *                 buflen if an error is returned
    * @param stream   [IN]  Handle to the stream to write to
    *
    * @return VXIlog_RESULT_SUCCESS on success
    */
   VXIlogResult (*Write)(struct SBlogStream  *stream,
 			const VXIbyte        *buffer,
 			VXIulong              buflen,
 			VXIulong             *nwritten);
 
 } SBlogStream;
 
 
 /**
  ** @name SBlogErrorListener
  ** @memo Prototype for error listener notification
  ** @doc  All error listener registrants must conform to this signature.
  **
  ** @param  pThis         [IN] pointer to the SBlogInterface that issued
  **                            the callback
  ** @param  moduleName    [IN] module name on which the error occurred
  ** @param  errorID       [IN] error number
  ** @param  timestamp     [IN] time_t for the time of the log event
  ** @param  timestampMsec [IN] Milliseconds for the time of the log event
  ** @param  keys          [IN] VXIVector of keys, all VXIString types
  ** @param  values        [IN] VXIVector of values, each a VXIInteger, 
  **                            VXIFloat, VXIString, or VXIPtr
  ** @param  userdata      [IN] User data that is delivered in the callback
  **
  */
 
 typedef void SBlogErrorListener(struct SBlogInterface *pThis,
 				const VXIchar         *moduleName,
 				VXIunsigned            errorID,
 				time_t                 timestamp,
 				VXIunsigned            timestampMsec,
 				const VXIVector       *keys,
 				const VXIVector       *values,
 				void                  *userdata);
 
 /**
  ** @name SBlogDiagnosticListener
  ** @memo Prototype for diagnostic listener notification
  ** @doc  All diagnostic listener registrants must conform to this
  ** signature.
  **
  ** @param  pThis         [IN] pointer to the SBlogInterface that issued
  **                            the callback
  ** @param  tagID         [IN] Identifier that classifies a group of logically
  **                            associated diagnostic messages (usually from a
  **                            single software module) that are desirable to
  **                            enable or disable as a single unit. See the top
  **                            of this file for tagID allocation rules.
  ** @param  subtag        [IN] Arbitrary string that may be used to subdivide
  **                            the diagnostic messages of that tagID, or 
  **                            provide additional standardized information
  **                            such as the source file, function, or method.
  **                            There are no rules for the content of this
  **                            field.
  ** @param  timestamp     [IN] time_t the time of the log event
  ** @param  timestampMsec [IN] Milliseconds for the time of the log event
  ** @param  printmsg      [IN] NULL terminated string to be printed.
  ** @param  userdata      [IN] User data that is delivered in the callback.
  **
  */
 
 typedef void SBlogDiagnosticListener(struct SBlogInterface *pThis,
 				     VXIunsigned            tagID,
 				     const VXIchar         *subtag,
 				     time_t                 timestamp,
 				     VXIunsigned            timestampMsec,
 				     const VXIchar         *printmsg,
 				     void                  *userdata);
 
 /**
  ** @name SBlogEventListener
  ** @memo Prototype for event listener notification
  ** @doc  All event listener registrants must conform to this
  ** signature.
  **
  ** @param pThis         [IN] pointer to the SBlogInterface that issued
  **                           the callback
  ** @param eventID       [IN] event number
  ** @param timestamp     [IN] time_t for the time of the log event
  ** @param timestampMsec [IN] Milliseconds for the time of the log event
  ** @param keys          [IN] VXIVector of keys, all VXIString types
  ** @param values        [IN] VXIVector of values, each a VXIInteger, VXIFloat,
  **                           VXIString, or VXIPtr
  ** @param userdata      [IN] User data that is delivered in the callback.
  **
  */
 
 typedef void SBlogEventListener(struct SBlogInterface *pThis,
 				VXIunsigned            eventID,
 				time_t                 timestamp,
 				VXIunsigned            timestampMsec,
 				const VXIVector       *keys,
 				const VXIVector       *values,
 				void                  *userdata);
 
 /**
  ** @name SBlogContentListener
  ** @memo Prototype for content listener notification
  ** @doc  All content listener registrants must conform to this
  ** signature.
  **
  ** @param pThis         [IN] pointer to the SBlogInterface that issued
  **                           the callback
  ** @param  moduleName   [IN] Name of the software module that is 
  **                           outputting the data
  ** @param  contentType  [IN] MIME content type for the data
  ** @param  userdata     [IN] User data that is delivered in the callback.
  ** @param  logKey       [OUT] Key name to cross-reference this content
  **                            in logging errors, events, and/or diagnostic 
  **                            messages. Ownership is passed on success, 
  **                            the user will call VXIStringDestroy( ) to
  **                            free this when no longer required.
  ** @param  logValue     [OUT] Value to cross-reference this content
  **                            in logging errors, events, and/or diagnostic 
  **                            messages. Ownership is passed on success, the
  **                            user will call VXIStringDestroy( ) to free
  **                            this when no longer required.
  ** @param  stream       [OUT] Handle for writing the content and
  **                            closing the stream
  **
  */
 
 typedef VXIlogResult SBlogContentListener(struct SBlogInterface *pThis,
 					  const VXIchar         *moduleName,
 					  const VXIchar         *contentType,
 					  void                  *userdata,
 					  VXIString            **logKey,
 					  VXIString            **logValue,
 					  SBlogStream          **stream);
 
 
 /**
  ** @name SBlog Interface
  ** @memo SBlog extension interface to the VXIlog interface.
  ** @doc  SBlog defines extensions to the VXIlog interface that support
  ** the tag mechanism and the listener registration 
  */
 typedef struct SBlogInterface
 {
   /** 
    ** Base interface, must appear first
    */
   VXIlogInterface vxilog;
   
   /**
   ** @name RegisterErrorListener
   ** @memo Subscribe the given listener for errors.
   ** @doc  The given listener will be notified for all errors (via
   ** VXIlog::Error or VError calls) as each calls is processed by
   ** SBlog. The call cannot be disabled.
   **
   ** @param alistener   [IN] Subscribing listener 
   ** @param userdata    [IN] User data that will be returned to the 
   **                         listener when notification occurs.
   **                         Note: the same listener may be 
   **                         registered multiple times, as long
   **                         as unique userdata is passed. In this
   **                         case, the listener will be called once
   **                         for each unique userdata.
   ** 
   ** @return    VXIlog_RESULT_SUCCESS:       success
   ** @return    VXIlog_RESULT_SYSTEM_ERROR:  internal error
   **
   */
   VXIlogResult (*RegisterErrorListener)(
     struct SBlogInterface *pThis,
     SBlogErrorListener    *alistener,   /* in */
     void                  *userdata);   /* in */
 
   /**
   ** @name UnregisterErrorListener
   ** @memo Unsubscribes the given listener.
   ** @doc  Unsubscribes the given listener/userdata combination.
   **
   ** @param alistener  [IN] The subscribing listener 
   ** @param userdata   [IN] User data that was passed in during registration.
   ** 
   ** @return   VXIlog_RESULT_SUCCESS:       success
   ** @return   VXIlog_RESULT_SYSTEM_ERROR:  internal error
   */
   VXIlogResult (*UnregisterErrorListener)(
     struct SBlogInterface *pThis,
     SBlogErrorListener    *alistener,  /* in */
     void                  *userdata);  /* in */
 
   /**
   ** @name RegisterDiagonsticListener
   ** @memo Subscribe the given listener for a diagnostic message.
   ** @doc  The given listener will be notified for all diagnostic
   ** messages (via VXIlog::Diagnostic or VDiagnostic calls) as each
   ** calls is processed by SBlog. The set of Diagnostics that are
   ** returned is controlled by ControlDiagnosticTag to either true or
   ** false.  By default all tags are assumed to be false when the
   ** listener is registered, and therefore off. A tag must be
   ** specifically turned to true (enabled) before any callbacks will
   ** be generated.  If a tag is enabled, all callbacks that registered
   ** on a given SBlog interface will be invoked. Generally, only one
   ** callback will be registered on each interface.
   **
   ** @param alistener  [IN] Subscribing listener 
   ** @param userdata   [IN] User data that will be returned to the 
   **                        listener when notification occurs.
   **                        Note: the same listener may be 
   **                        registered multiple times, as long
   **                        as unique userdata is passed. In this
   **                        case, the listener will be called once
   **                        for each unique userdata.
   ** 
   ** @return   VXIlog_RESULT_SUCCESS:       success
   ** @return   VXIlog_RESULT_SYSTEM_ERROR:  internal error
   ** 
   */
   VXIlogResult (*RegisterDiagnosticListener)(
     struct SBlogInterface   *pThis,
     SBlogDiagnosticListener *alistener,  /* in */
     void                    *userdata);  /* in */
 
   /**
   ** @name UnregisterDiagnosticListener
   ** @memo Unsubscribes the given listener.
   ** @doc  Unsubscribes the given listener/userdata combination.
   **
   ** @param alistener  [IN] The subscribing listener 
   ** @param userdata   [IN] User data that was passed in during registration.
   ** 
   ** @return   VXIlog_RESULT_SUCCESS:       success
   ** @return   VXIlog_RESULT_SYSTEM_ERROR:  internal error
   */
   VXIlogResult (*UnregisterDiagnosticListener)(
     struct SBlogInterface    *pThis,
     SBlogDiagnosticListener  *alistener,    /* in */
     void                     *userdata);    /* in */
 
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
     struct SBlogInterface *pThis,
     VXIunsigned            tagID,
     VXIbool                state);
 
   /**
   ** @name RegisterEventListener
   ** @memo Subscribe the given listener for events.
   ** @doc  The given listener will be notified for all events (via
   ** VXIlog::Event or VEvent calls) as each calls is processed by
   ** SBlog. The call cannot be disabled.
   **
   ** @param alistener  [IN] Subscribing listener 
   ** @param userdata   [IN] User data that will be returned to the 
   **                        listener when notification occurs.
   **                        Note: the same listener may be 
   **                        registered multiple times, as long
   **                        as unique userdata is passed. In this
   **                        case, the listener will be called once
   **                        for each unique userdata.
   ** 
   ** @return   VXIlog_RESULT_SUCCESS:       success
   ** @return   VXIlog_RESULT_SYSTEM_ERROR:  internal error
   **
   */
   VXIlogResult (*RegisterEventListener)(
     struct SBlogInterface *pThis,
     SBlogEventListener    *alistener,   /* in */
     void                  *userdata);   /* in */
 
   /**
   ** @name UnregisterEventListener
   ** @memo Unsubscribes the given listener.
   ** @doc  Unsubscribes the given listener/userdata combination.
   **
   ** @param alistener  [IN] The subscribing listener 
   ** @param userdata   [IN] User data that was passed in during registration.
   ** 
   ** @return   VXIlog_RESULT_SUCCESS:       success
   ** @return   VXIlog_RESULT_SYSTEM_ERROR:  internal error
   */
   VXIlogResult (*UnregisterEventListener)(
     struct SBlogInterface  *pThis,
     SBlogEventListener     *alistener,  /* in */
     void                   *userdata);  /* in */
 
   /**
   ** @name RegisterContentListener
   ** @memo Subscribe the given listener for content write requests
   ** @doc  The given listener will be notified for all content write
   ** requests (via VXIlog::ContentOpen( ) calls) as each request is 
   ** processed by SBlog. The call cannot be disabled.
   **
   ** @param alistener  [IN] Subscribing listener 
   ** @param userdata   [IN] User data that will be returned to the 
   **                        listener when notification occurs.
   **                        Note: the same listener may be 
   **                        registered multiple times, as long
   **                        as unique userdata is passed. In this
   **                        case, the listener will be called once
   **                        for each unique userdata.
   ** 
   ** @return   VXIlog_RESULT_SUCCESS:       success
   ** @return   VXIlog_RESULT_SYSTEM_ERROR:  internal error
   **
   */
   VXIlogResult (*RegisterContentListener)(
     struct SBlogInterface *pThis,
     SBlogContentListener  *alistener,   /* in */
     void                  *userdata);   /* in */
 
   /**
   ** @name UnregisterContentListener
   ** @memo Unsubscribes the given listener.
   ** @doc  Unsubscribes the given listener/userdata combination.
   **
   ** @param alistener  [IN] The subscribing listener 
   ** @param userdata   [IN] User data that was passed in during registration.
   ** 
   ** @return   VXIlog_RESULT_SUCCESS:       success
   ** @return   VXIlog_RESULT_SYSTEM_ERROR:  internal error
   */
   VXIlogResult (*UnregisterContentListener)(
     struct SBlogInterface  *pThis,
     SBlogContentListener   *alistener,  /* in */
     void                   *userdata);  /* in */
 
 } SBlogInterface;
 
 
 /**
  * Global platform initialization of SBlog
  *
  * @result VXIlog_RESULT_SUCCESS on success
  */
 SBLOG_API VXIlogResult SBlogInit(void);
 
 /**
  * Global platform shutdown of Log
  *
  * @result VXIlog_RESULT_SUCCESS on success
  */
 SBLOG_API VXIlogResult SBlogShutDown(void);
 
 /**
  * Create a new log service handle
  *
  * @result VXIlog_RESULT_SUCCESS on success 
  */
 SBLOG_API VXIlogResult SBlogCreateResource(VXIlogInterface **log);
 
 /**
  * Destroy the interface and free internal resources
  *
  * @result VXIlog_RESULT_SUCCESS on success 
  */
 SBLOG_API VXIlogResult SBlogDestroyResource(VXIlogInterface **log);
 
 /*@}*/
 
 #ifdef __cplusplus
 } /* extern "C" */
 #endif
 
 #include "VXIheaderSuffix.h"
 
 #endif  /* include guard */
