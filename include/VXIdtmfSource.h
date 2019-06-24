/****************License************************************************
 *
 * Copyright 2000-2001.  SpeechWorks International, Inc.
 *
 * Use of this software is subject to notices and obligations set forth
 * in the SpeechWorks Public License - Software Version 1.1 which is
 * included with this software.
 *
 * SpeechWorks is a registered trademark, and SpeechWorks Here,
 * DialogModules and the SpeechWorks logo are trademarks of SpeechWorks
 * International, Inc. in the United States and other countries.
 *
 ************************************************************************
 *
 * $Id: VXIdtmfSource.h,v 1.3 2002/07/26 19:34:48 dmeyer Exp $
 *
 ************************************************************************
 */

#ifndef _VXIDTMFSOURCE_H
#define _VXIDTMFSOURCE_H

#include "VXItypes.h"                  /* For VXIchar, VXIint32, etc. */
#include "VXIvalue.h"                  /* For VXIMap */

#include "VXIheaderPrefix.h"
#ifdef VXIDTMFSOURCE_EXPORTS
#define VXIDS_API SYMBOL_EXPORT_DECL
#else
#define VXIDS_API SYMBOL_IMPORT_DECL
#endif
#define VXIDTMFSOURCE_API VXIDS_API    /* For backward compatibility */

#ifdef __cplusplus
extern "C" {
#endif

struct VXIdsInterface;
#define VXIdtmfSource VXIdsInterface   /* For backward compatibility */

/** 
 * @name VXIdtmfSource
 * @memo DTMF Source Interface
 * @version 1.0
 * @doc
 * Abstract interface for obtaining DTMF input from the caller via
 * a telephony interface. <p>
 *
 * NOTE: CURRENTLY THIS IS NOT USED BY THE OPENSPEECH BROWSER! It
 *       is currently only an internal interface for the OpenSpeech
 *       Server product which required this functionality seperate
 *       of the VXIsessionControl interface.
 *
 * The DTMF Source interface must be implemented for each new
 * underlying telephony interface, where this is frequently done by a
 * third party integrator.
 *
 * There is one DTMF Source interface per telephony channel.
 *
 * The following table summarizes the events delivered by DTMF Source
 * implementations.
 *
 * <table border=0>
 * <tr><td valign=top><b> DS_EVENT_ERROR </b></td>
 *
 * <td> Reports the DTMF Source encountered an error in delivering
 * DTMF, typically due to an underlying telephony or I/O error. The
 * error must have occurred after a call to VXIdsTriggerStart( )
 * returned with a VXIds_RESULT_SUCCESS result code, never during that
 * call or after that call returned with an error.  The DTMF Source
 * must be returned to an IDLE state (DTMF delivery halted) prior to
 * delivering this event.
 *
 * The event includes a result code that indicates the cause of the
 * error. </td></tr>
 *
 * <tr><td valign=top><b> DS_EVENT_RECEIVED </b></td>
 *
 * <td> Reports a DTMF key press, with a separate event for each DTMF
 * received. If no event listeners are registered, these events should
 * be buffered and delivered as soon as a listener is registered.
 * </td></tr>
 *
 * <tr><td valign=top><b> DS_EVENT_STOPPED </b></td>
 *
 * <td> Reports that DTMF events have been disabled normally, either
 * due to the input operation successfully completing or the DTMF
 * being disabled via TriggerStop( ). DTMF Source implementations may
 * also choose to automatically disable DTMF return this event on
 * hang-up, although this should only be done when this is automatic
 * functionality within the underlying telephony library (the client
 * is responsible for rapdily calling TriggerStop( ) when hang-up
 * occurs, which in most cases simplifies the DTMF Source
 * implementation by avoiding session control interactions).
 *
 * The DTMF Source must move from the DTMF ENABLED state to an IDLE
 * state prior to delivering this event. </td></tr>
 * </table> 
 */

/*@{*/

/**
 * Result codes for interface methods
 *
 * Result codes less then zero are severe errors (likely to be
 * platform faults), those greater then zero are warnings (likely to
 * be application issues)
 */
typedef enum  VXIdsResult {
  /** Fatal error, terminate call    */
  VXIds_RESULT_FATAL_ERROR       = -100,
  /** Low-level telephony library error */
  VXIds_RESULT_DRIVER_ERROR      =  -50,
  /** I/O error                      */
  VXIds_RESULT_IO_ERROR          =   -8,
  /** Out of memory                  */
  VXIds_RESULT_OUT_OF_MEMORY     =   -7,
  /** System error, out of service   */
  VXIds_RESULT_SYSTEM_ERROR      =   -6,
  /** Errors from platform services  */
  VXIds_RESULT_PLATFORM_ERROR    =   -5,
  /** Return buffer too small        */
  VXIds_RESULT_BUFFER_TOO_SMALL  =   -4,
  /** Parameter name is not valid    */
  VXIds_RESULT_INVALID_PARAM_NAME  = -3,
  /** Parameter value is not valid   */
  VXIds_RESULT_INVALID_PARAM_VALUE = -2,
  /** Invalid function argument      */
  VXIds_RESULT_INVALID_ARGUMENT  =   -1,
  /* Success                         */
  VXIds_RESULT_SUCCESS           =    0,
  /** Normal failure, nothing logged */
  VXIds_RESULT_FAILURE           =    1,
  /** Non-fatal non-specific error   */
  VXIds_RESULT_NON_FATAL_ERROR   =    2,
  /** No DTMF digits available       */
  VXIds_RESULT_NO_DTMF           =    3,
  /** Operation is not supported     */
  VXIds_RESULT_UNSUPPORTED       =  100 
} VXIdsResult; 


/**
 * Events returned from the DTMF Source as detailed in the DTMF
 * Source interface description.
 */
#define DS_EVENT_ERROR                   0x0040
#define DS_EVENT_RECEIVED                0x0041
#define DS_EVENT_STOPPED                 0x0042
#define _DS_EVENT_LAST_EVENT_MARKER      DS_EVENT_STOPPED


/**
 * Signature for the event handler called to report DTMF Source
 * events, as registered via RegisterEventListener( ).
 *
 * @param pThis      [IN] DTMF source reporting the event
 * @param eventType  [IN] Event type that occurred
 * @param userData   [IN] User data for the listener, as specified
 *                    when registering the listener.  
 */
typedef void VXIdsEventListener (
  struct VXIdsInterface  *pThis,
  const VXIulong          eventType,
  const VXIptr            userData
);


/* 
** ================================================== 
** VXIdsInterface Interface Definition 
** ================================================== 
*/ 
/** @name VXIdsInterface 
 ** @memo DTMF Source interface for DTMF input
 **
 */
typedef struct VXIdsInterface { 
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
   *          Internet address in reverse order, such as com.xyz.dtmfSource
   *          for VXIdtmfSource from xyz.com. This is similar to how
   *          VoiceXML 1.0 recommends defining application specific error
   *          types.
   */
  const VXIchar* (*GetImplementationName)(void);

  /**
   * @name BeginSession
   * @memo Reset for a new session.
   *
   * @doc
   * This must be called for each new session, allowing for
   * call specific handling to occur. For some implementations, this
   * can be a no-op.  For others runtime binding of resources or other
   * call start specific handling may be done on this call.
   *
   * @param args  [IN] Implementation defined input and output
   *                    arguments for the new session
   *
   * @result VXIds_RESULT_SUCCESS on success
   */
  VXIdsResult (*BeginSession)(struct VXIdsInterface  *pThis,
                              VXIMap                 *args);

  /**
   * @name EndSession
   * @memo Performs cleanup at the end of a call session.
   *
   * @doc
   * This must be called at the termination of a call, allowing for
   * call specific termination to occur. For some implementations, this
   * can be a no-op. For others runtime resources may be released or
   * other adaptation may be completed.
   *
   * @param args  [IN] Implementation defined input and output
   *                    arguments for ending the session
   *
   * @result VXIds_RESULT_SUCCESS on success
   */
  VXIdsResult (*EndSession)(struct VXIdsInterface  *pThis,
                            VXIMap                 *args);

  /**
   * @name RegisterEventListener
   * @memo Register an event listener/user data pair for event notification
   *
   * @doc
   * Zero or more event listeners may be registered by calling this
   * method, each with their own user data. The combination of the
   * event listener function and user data pointer must be unique:
   * if the same pair is registered twice, an error is returned.
   *
   * @param eventListener  [IN] Event listener function that will be called
   *                        when events occur
   * @param userData       [IN] Opaque user defined data, passed to the
   *                        listener for each listener invocation to permit
   *                        the listener to access information it needs
   *                        to react to the event.
   *
   * @return VXIds_RESULT_SUCCESS on success
   */
  VXIdsResult (*RegisterEventListener)(struct VXIdsInterface  *pThis,
				       VXIdsEventListener     *eventListener,
				       const VXIptr            userData);

  /**
   * @name UnregisterEventListener
   * @memo Unregister an event listener/user data pair
   *
   * @param eventListener  [IN] Event listener function that was previously
   *                        registered to be called when events occur
   * @param userData       [IN] Opaque user defined data that was
   *                        previously registered with the event listener
   *                        function
   *
   * @return VXIds_RESULT_SUCCESS on success
   */
  VXIdsResult (*UnregisterEventListener)(struct VXIdsInterface  *pThis,
					 VXIdsEventListener     *eventListener,
					 const VXIptr            userData);

  /**
   * @name TriggerStart
   * @memo Enables delivery of DTMF input data to all listeners, non-blocking
   *
   * @doc 
   * This only enables DTMF input data delivery. This routine is
   * expected to return after data delivery has been enabled (as
   * opposed to blocking until data delivery is complete).
   *
   * @return VXIds_RESULT_SUCCESS on success
   */
  VXIdsResult (*TriggerStart)(struct VXIdsInterface  *pThis);

  /**
   * @name TriggerStop
   * @memo Stops the delivery of DTMF data
   *
   * @doc
   * This routine is synchronous and returns when the DTMF data
   * delivery is either stopped or there is an error. If success is
   * returned, a DS_EVENT_STOPPED event must be delivered before
   * this returns.
   *
   * @return VXIds_RESULT_SUCCESS on success
   * @return VXIds_RESULT_NON_FATAL_ERROR if DTMF delivery is not enabled
   * @return Another VXIds_RESULT_[...] code for severe errors 
   */
  VXIdsResult (*TriggerStop)(struct VXIdsInterface *pThis);

  /**
   * @name GetDTMF
   * @memo Read a single DTMF digit from the DTMF queue
   *
   * @doc
   * Warnings: There is only one DTMF queue and it is only 
   *           accessible via this routine and FlushDTMFQueue( ).
   *           In systems with multiple clients, they must negotiate
   *           among themselves for access to the queue.
   *
   * @param dtmfDigit  [OUT] Returns the DTMF digit, returned as an
   *                    ASCII character (0 - 9, *, #, A - D)
   *
   * @return VXIds_RESULT_SUCCESS on success
   * @return VXIds_RESULT_NON_FATAL_ERROR no DTMF digit available
   */
  VXIdsResult (*GetDTMF)(struct VXIdsInterface  *pThis,
			 VXIbyte                *dtmfDigit);

  /**
   * @name FlushDTMFQueue
   * @memo Remove all DTMF digits from the DTMF queue
   *
   * @doc
   * Warnings: There is only one DTMF queue and it is only 
   *           accessible via this routine and GetDTMF( ). In systems
   *           with multiple clients, they must negotiate among
   *           themselves for access to the queue.
   *
   * @param pThis	Pointer to the instance of the VXIdsInterface
   *
   *
   * @return VXIds_RESULT_SUCCESS on success
   */
  VXIdsResult (*FlushDTMFQueue)(struct VXIdsInterface  *pThis);

} VXIdsInterface;

/*@}*/

#ifdef __cplusplus
}
#endif

#include "VXIheaderSuffix.h"

#endif  /* include guard */
