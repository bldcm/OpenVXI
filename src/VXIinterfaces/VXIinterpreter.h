
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
 

 #ifndef _VXIINTERPRETER_H
 #define _VXIINTERPRETER_H
 
 #include "VXItypes.h"                  /* For VXIchar */
 #include "VXIvalue.h"                  /* For VXIValue */
 
 #ifdef __cplusplus
 struct VXIcacheInterface;
 struct VXIinetInterface;
 struct VXIjsiInterface;
 struct VXIlogInterface;
 struct VXIobjectInterface;
 
 struct VXIpromptInterface;
 struct VXIrecInterface;
 struct VXItelInterface;
 #else
 #include "VXIcache.h"
 #include "VXIinet.h"
 #include "VXIjsi.h"
 #include "VXIlog.h"
 #include "VXIobject.h"
 #include "VXIprompt.h"
 #include "VXIrec.h"
 #include "VXItel.h"
 #endif
 
 #include "VXIheaderPrefix.h"
 #ifdef VXI_EXPORTS
 #define VXI_INTERPRETER SYMBOL_EXPORT_DECL
 #else
 #define VXI_INTERPRETER SYMBOL_IMPORT_DECL
 #endif
 
 #ifdef __cplusplus
 extern "C" {
 #endif
 
 /**
   @name VXI 
   @memo VXI interpreter interface
   @doc The OpenSpeech Browser core is the OpenVXI.  The
   VXIinterpreter interface implements the VXI interface function to run
   the interface. In addition a set of process and thread initialization
   routines are provided to set-up and destroy the interpreter per thread.
   */
 
 /*@{*/
 
 /**
  * Keys identifying properties for SetProperties.
  */
 #define VXI_BEEP_AUDIO        L"vxi.property.beep.uri"          /* VXIString */
 #define VXI_PLATFORM_DEFAULTS L"vxi.property.platform.defaults" /* VXIString */
 
 
 /**
  * Keys identifying properties for SetProperties.
  */
 #define VXI_DOC_MEMORY_CACHE  L"vxi.property.cache.size"        /* VXIInteger, size in kB. */
 
 /**
  * @name VXIinterpreterResult
  * @memo Result codes for interface methods
  * @doc
  * Result codes less then zero are severe errors (likely to be
  * platform faults), those greater then zero are warnings (likely to
  * be application issues)
  */
 typedef enum VXIinterpreterResult {
   /** Fatal error, terminate call    */
   VXIinterp_RESULT_FATAL_ERROR          = -100,
   /** Out of memory                  */
   VXIinterp_RESULT_OUT_OF_MEMORY        =   -7,
   /** Errors from platform services  */
   VXIinterp_RESULT_PLATFORM_ERROR       =   -5,
   /** Property name is not valid     */
   VXIinterp_RESULT_INVALID_PROP_NAME    =   -3,
   /** Property value is not valid    */
   VXIinterp_RESULT_INVALID_PROP_VALUE   =   -2,
   /** Invalid function argument      */
   VXIinterp_RESULT_INVALID_ARGUMENT     =   -1,
   /** Success                        */
   VXIinterp_RESULT_SUCCESS              =    0,
   /** Normal failure */
   VXIinterp_RESULT_FAILURE              =    1,
   /** Run call aborted */
   VXIinterp_RESULT_STOPPED              =    3,
   /** Document fetch timeout         */
   VXIinterp_RESULT_FETCH_TIMEOUT        =   51,
   /** Unable to open or read from URI */
   VXIinterp_RESULT_FETCH_ERROR          =   52,
   /** Not a valid VoiceXML document  */
   VXIinterp_RESULT_INVALID_DOCUMENT     =   53,
   /** Operation is not supported     */
   VXIinterp_RESULT_UNSUPPORTED          =  100
 } VXIinterpreterResult;
 
 
 /**
  * @name VXIresources
  * @memo Structure containing all the interfaces required by the VXI.
  * @doc This structure must be allocated and all the pointers filled
  * with created and initialized resources before creating the VXI
  * interface.
  */
 typedef struct VXIresources {
   /** log interface */
   VXIlogInterface    * log;
   /** Internet interface */
   VXIinetInterface   * inet;
   /** ECMAScript interface */
   VXIjsiInterface    * jsi;
   /** Recognizer interface */
   VXIrecInterface    * rec;
   /** Prompt interface */
   VXIpromptInterface * prompt;
   /** Telephony interface */
   VXItelInterface    * tel;
   /** Object interface. May be NULL in which case objects will not function. */
   VXIobjectInterface * object;
   /** Cache interface. May be NULL, but VXI performance will be reduced. */
   VXIcacheInterface  * cache;
 } VXIresources;
 
 
 /*
 ** ==================================================
 ** VXIinterpreterInterface Interface definition
 ** ==================================================
 */
 
 /**
  * @name VXIinterpreterInterface
  * @memo VXIinterpreter interface for VoiceXML execution
  * @version 1.0 <br>
  * @doc
  * Abstract interface for the VoiceXML intepreter, simply provides a
  * single method for running the intepreter on a document and getting
  * the document result.<p>
  *
  * There is one intepreter interface per thread/line.
  */
 typedef struct VXIinterpreterInterface {
   /**
    * @name GetVersion
    * @memo Get the VXI interface version implemented
    *
    * @return  VXIint32 for the version number. The high high word is
    *          the major version number, the low word is the minor version
    *          number, using the native CPU/OS byte order. The current
    *          version is VXI_CURRENT_VERSION as defined in VXItypes.h.
    */
   VXIint32 (*GetVersion)(void);
 
   /**
    * @name GetImplementationName
    * @memo Get the name of the implementation
    *
    * @return  Implementation defined string that must be different from
    *          all other implementations. The recommended name is one
    *          where the interface name is prefixed by the implementator's
    *          Internet address in reverse order, such as com.xyz.rec for
    *          VXIrec from xyz.com. This is similar to how VoiceXML 1.0
    *          recommends defining application specific error types.
    */
   const VXIchar* (*GetImplementationName)(void);
 
   /**
    * Run a VoiceXML document and optionally return the result
    *
    * @param name    [IN] Name of the VoiceXML document to fetch and
    *                  execute, may be a URL or a platform dependant path.
    *                  See the Open() method in VXIinet.h for details
    *                  about supported names, however for URLs this
    *                  must always be an absolute URL and any query arguments
    *                  must be embedded.
    * @param sessionScript [IN] A series of ECMAScript expressions which will
    *                  be evaluated by the VXI to populate the session scope 
    *                  in ECMAScript.
    * @param result  [OUT] (Optional, pass NULL if not desired.) Return
    *                  value for the VoiceXML document (from <exit/>), this
    *                  is allocated on success and when there is an
    *                  exit value (a NULL pointer is returned otherwise),
    *                  the caller is responsible for destroying the returned
    *                  value by calling VXIValueDestroy().
    *
    * @return         [From normal operation]
    *                 VXIinterp_RESULT_SUCCESS on success
    *                 VXIinterp_RESULT_FAILURE if normal error occured
    *                 VXIinterp_RESULT_STOPPED if aborted by Stop
    *                 [During initialization from defaults]
    *                 VXIinterp_RESULT_FETCH_TIMEOUT
    *                 VXIinterp_RESULT_FETCH_ERROR
    *                 VXIinterp_RESULT_INVALID_DOCUMENT
    *                 [Serious errors]
    *                 VXIinterp_RESULT_FATAL_ERROR
    *                 VXIinterp_RESULT_OUT_OF_MEMORY
    *                 VXIinterp_RESULT_PLATFORM_ERROR
    *                 VXIinterp_RESULT_INVALID_ARGUMENT
    */
   VXIinterpreterResult (*Run)(struct VXIinterpreterInterface  *pThis,
                               const VXIchar                   *name,
                               const VXIchar                   *sessionScript,
                               VXIValue                       **result);
 
   /**
    * Specify runtime properties for the VoiceXML interpreter.
    *
    * @param props   [IN] Map containing a list of properties.  Currently there
    *                  are two of interest:
    *                  * VXI_BEEP_AUDIO           URI for the beep audio
    *                  * VXI_PLATFORM_DEFAULTS    URI for the platform defaults
    *
    * @return         VXIinterp_RESULT_SUCCESS on success
    *                 VXIinterp_RESULT_INVALID_PROP_NAME
    *                 VXIinterp_RESULT_INVALID_PROP_VALUE
    *                 VXIinterp_RESULT_INVALID_ARGUMENT
    */
   VXIinterpreterResult (*SetProperties)(struct VXIinterpreterInterface *pThis,
                                         const VXIMap                   *props);
 
   /**
    * Load and parse an VXML document.  This tests the validity.
    *
    * @param name    [IN] Name of the VoiceXML document to fetch and
    *                  execute, may be a URL or a platform dependant path.
    *                  See the Open( ) method in VXIinet.h for details
    *                  about supported names, however for URLs this
    *                  must always be an absolute URL and any query arguments
    *                  must be embedded.
    *
    * @return        VXIinterp_RESULT_SUCCESS if document exists and is valid
    *                VXIinterp_RESULT_FAILURE if document is invalid VXML
    *                VXIinterp_RESULT_FETCH_ERROR if document retrieval failed
    *                VXIinterp_RESULT_FETCH_TIMEOUT
    *                VXIinterp_RESULT_INVALID_ARGUMENT
    *                VXIinterp_RESULT_FATAL_ERROR
    *                VXIinterp_RESULT_OUT_OF_MEMORY
    */
   VXIinterpreterResult (*Validate)(struct VXIinterpreterInterface  *pThis,
                                    const VXIchar *name);
 
   /**
    * In the interpreter is running and doStop == TRUE, this will cause the
    * in progress Run to return as soon as possible with 
    * VXIinterp_RESULT_STOPPED.  If doStop == FALSE, this clears any pending
    * stop request.
    *
    * NOTE: if the interpreter encounters an error before noticing the request
    * or while servicing the request, the actual return value from Run may not
    * something other than VXIinterp_RESULT_STOPPED.
    *
    * @return         VXIinterp_RESULT_SUCCESS on success
    *                 VXIinterp_RESULT_INVALID_ARGUMENT
    */
   VXIinterpreterResult (*RequestStop)(struct VXIinterpreterInterface  *pThis,
                                       VXIbool doStop);
 
   /**
    * Trigger an event 
    *
    * @param event    [IN] VoiceXML event to generate during Run.
    * @param message  [IN] Corresponding message string; may be NULL.
    *
    * @return         VXIinterp_RESULT_SUCCESS on success
    *                 VXIinterp_RESULT_INVALID_ARGUMENT
    */
   VXIinterpreterResult (*InsertEvent)(struct VXIinterpreterInterface *pThis,
                                       const VXIchar                  *event,
                                       const VXIchar                  *message);
 
   /**
    * Clear the event queue
    *
    * NOTE: this function must be called by the integration to clear the event(s)
    * in case of error occurs and the event(s) is no longer valid.  The interpreter
    * will not clear the queue to avoid race condition.
    *
    * @return         VXIinterp_RESULT_SUCCESS on success
    *                 VXIinterp_RESULT_INVALID_ARGUMENT
    */
   VXIinterpreterResult (*ClearEventQueue)(struct VXIinterpreterInterface *pThis);
 
 } VXIinterpreterInterface;
 
 
 /**
  * @name VXIinterpreterInit
  * @memo Per-process initialization for VXIinterpreter.
  * @doc This function should be called once at process startup.
  *
  * @param log            [IN] VXI Logging interface used for error/diagnostic
  *                             logging, only used for the duration of this
  *                             function call
  * @param  diagLogBase   [IN] Base tag number for diagnostic logging purposes.
  *                             All diagnostic tags for the VXI will start at
  *                             this ID and increase upwards.
  *
  * @return     VXIinterp_RESULT_SUCCESS if resources may be created.
  * @return     VXIinterp_RESULT_FAILURE if interface is unavailable.
  */
 VXI_INTERPRETER
 VXIinterpreterResult VXIinterpreterInit(VXIlogInterface  *log,
                                         VXIunsigned       diagLogBase,
                                         const VXIMap     *props);
 
 
 /**
  * @name VXIinterpreterShutDown
  * @memo  Per-process de-initialization for VXIinterpreter.
  * @doc This function should be called once per process shutdown, after
  * all the interfaces for the process are destroyed.
  *
  * @param log [IN] VXI Logging interface used for error/diagnostic logging,
  *                  only used for the duration of this function call
  */
 VXI_INTERPRETER void VXIinterpreterShutDown(VXIlogInterface  *log);
 
 
 /**
  * @name VXIinterpreterCreateResource
  * @memo  Create an interface to the VoiceXML interpreter.
  * @doc Create a VXI interface given an interface structure that
  * contains all the resources required for the VXI.
  *
  * @param resource [IN] A pointer to a structure containing all the
  *                       interfaces requires by the VXI
  * @param pThis    [IN] A pointer to the VXI interface that is to be
  *                       allocated.  The pointer will be set if this call
  *                       is successful.
  *
  * @return     VXIinterp_RESULT_SUCCESS if interface is available for use
  *             VXIinterp_RESULT_OUT_OF_MEMORY if low memory is suspected
  *             VXIinterp_RESULT_INVALID_ARGUMENT
  */
 VXI_INTERPRETER VXIinterpreterResult
 VXIinterpreterCreateResource(VXIresources *resource,
                              VXIinterpreterInterface ** pThis);
 
 /**
  * @name VXIinterpreterDestroyResource
  * @memo Destroy and de-allocate a VXI interface
  * @doc
  * Destroy an interface returned from VXIinterpreterCreateResource.
  * The pointer is set to NULL on success.
  *
  * @param pThis [IN] The pointer to the interface to be destroyed.
  */
 VXI_INTERPRETER void
 VXIinterpreterDestroyResource(VXIinterpreterInterface ** pThis);
 
 /*@}*/
 
 #ifdef __cplusplus
 }
 #endif
 
 #include "VXIheaderSuffix.h"
 
 #endif  /* include guard */
