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
 * $Id: VXIsessionControl.h,v 1.31 2002/03/06 19:25:54 dmeyer Exp $
 *
 ************************************************************************
 */

#ifndef _VXISESSIONCONTROL_H
#define _VXISESSIONCONTROL_H

#include "VXItypes.h"                  /* For VXIchar, VXIint32, etc. */

#include "VXIheaderPrefix.h"
#ifdef VXISC_EXPORTS
#define VXISC_API SYMBOL_EXPORT_DECL
#else
#define VXISC_API SYMBOL_IMPORT_DECL
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct VXIscInterface;

/**
 * @name VXIsessionControl
 * @memo Session Control Interface
 * @version 1.0
 * @doc
 * Abstract interface for call control and obtaining DTMF input from
 * the caller via a telephony interface. The call control
 * functionality includes detecting incoming calls, accepting incoming
 * calls, near-end disconnects (hang-up), detecting far-end
 * disconnects, and performing call transfers. <p>
 *
 * The Session Control interface must be implemented for each new
 * underlying telephony interface, where this is frequently done by a
 * third party integrator.
 *
 * There is one Session Control interface per telephony channel.
 *
 * The following table summarizes the events delivered by Session
 * Control implementations.
 *
 * <table border=0>
 * <tr><td valign=top><b> SC_EVENT_ERROR </b></td>
 *
 * <td> Reports the Session Control encountered an error on the line
 * during a call, typically due to an underlying telephony error. The
 * error must have occurred after a call to VXIscAcceptSession( )
 * returned with a VXIsc_RESULT_SUCCESS result code, never within that
 * function or after that function returned with an error. If the
 * error terminated the call, a SC_EVENT_END event must be delivered
 * after this event.
 *
 * The event includes a result code that indicates the cause of the
 * error. </td></tr>
 *
 * <tr><td valign=top><b> SC_EVENT_BEGIN </b></td>
 *
 * <td> Reports a call has started, as initiated by a successful call
 * to AcceptSession( ). </td></tr>
 *
 * <tr><td valign=top><b> SC_EVENT_END </b></td>
 *
 * <td> Reports a disconnect (hang-up, call terminated) due to either
 * a near-end disconnect (from a call to TerminateSession( )), a
 * far-end disconnect, or an error (in which case a SC_EVENT_ERROR
 * will be delivered prior to this event).
 *
 * <tr><td valign=top><b> SC_EVENT_TRANSFER_BEGIN </b></td>
 *
 * <td> Reports that a transfer has successfully started, as initiated
 * by a successful call to Transfer( ). For a bridge (supervised)
 * transfer this event should be reported when the far-end has
 * answered the call and the near-end party and far-end party are
 * able to hear each other.  For a blind transfer this should be
 * reported when the blind transfer has been successfully initiated in
 * the network. For a blind transfer, a SC_EVENT_END event should
 * immediately follow this event. </td></tr>
 *
 * <tr><td valign=top><b> SC_EVENT_TRANSFER_COMPLETE </b></td>
 *
 * <td> Reports that a bridge (supervised) transfer has completed with
 * the far-end party disconnected and the near-end party still
 * connected. This can occur either on a maxtime exceeded or far-end
 * party or network disconnect. </td></tr>
 *
 * <tr><td valign=top><b> SC_EVENT_INVITE </b></td>
 *
 * <td> Reports an incoming call request on the telephony channel.
 * For analog connections, should be generated on each
 * ring. </td></tr>
 *
 * <tr><td valign=top><b> SC_EVENT_DTMF_RECEIVED </b></td>
 *
 * <td> Reports a DTMF key press, with a separate event for each DTMF
 * received. If no event listeners are registered, these events should
 * be buffered and delivered as soon as a listener is registered.
 * </td></tr>
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
typedef enum VXIscResult {
  /** Fatal error, terminate call           */
  VXIsc_RESULT_FATAL_ERROR       = -100,
  /** The specified property does not exist */
  VXIsc_RESULT_INVALID_PROPERTY  =  -53,
  /** Session in progress                   */
  VXIsc_RESULT_BUSY              =  -51,
  /** Low-level telephony library error     */
  VXIsc_RESULT_DRIVER_ERROR      =  -50,
  /** I/O error                             */
  VXIsc_RESULT_IO_ERROR          =   -8,
  /** Out of memory                         */
  VXIsc_RESULT_OUT_OF_MEMORY     =   -7,
  /** System error, out of service          */
  VXIsc_RESULT_SYSTEM_ERROR      =   -6,
  /** Errors from platform services         */
  VXIsc_RESULT_PLATFORM_ERROR    =   -5,
  /** Return buffer too small               */
  VXIsc_RESULT_BUFFER_TOO_SMALL  =   -4,
  /** Parameter name is not valid           */
  VXIsc_RESULT_INVALID_PARAM_NAME  = -3,
  /** Parameter value is not valid          */
  VXIsc_RESULT_INVALID_PARAM_VALUE = -2,
  /** Invalid function argument             */
  VXIsc_RESULT_INVALID_ARGUMENT  =   -1,
  /* Success                                */
  VXIsc_RESULT_SUCCESS           =    0,
  /** Normal failure, nothing logged        */
  VXIsc_RESULT_FAILURE           =    1,
  /** Non-fatal non-specific error          */
  VXIsc_RESULT_NON_FATAL_ERROR   =    2,
  /** No DTMF digits available              */
  VXIsc_RESULT_NO_DTMF           =    3,
  /** Call Terminated by a local hangup     */
  VXIsc_RESULT_NEAR_END_DISCONNECT =  4,
  /** Call Terminated by a remote hangup    */
  VXIsc_RESULT_FAR_END_DISCONNECT  =  5,
  /** Call Terminated by a network hangup   */
  VXIsc_RESULT_NETWORK_DISCONNECT  =  6,
  /** Call failed, not answered             */
  VXIsc_RESULT_NOANSWER          =    7,
  /** Called party was busy                 */
  VXIsc_RESULT_FAR_END_BUSY      =    8,
  /** Network party was busy                */
  VXIsc_RESULT_NETWORK_BUSY      =    9,
  /** Transfer exceeded the maximum allowed time */
  VXIsc_RESULT_TIME_EXCEEDED     =   10,
  /** Operation is not supported            */
  VXIsc_RESULT_UNSUPPORTED       =  100
} VXIscResult;


/**
 * Types of transfers that can be performed
 */
typedef enum VXIscTransferType {
  /* Blind (unsupervised) transfer, automated session is terminated
     and the call is released to the network */
  VXIscXferType_BLIND       = 1,
  /* Bridge (supervised) transfer, automated session is restored when
     the call completes */
  VXIscXferType_BRIDGE,
} VXIscTransferType;


/**
 * Keys identifying standard read-only properties returned by
 * GetNthPropertyPair( ), support for these is optional but is highly
 * encouraged. Implementations may support other read-only properties
 * as well.
 */
#define SC_ANI_CALLER_NUMBER            L"ani"    /* VXIchar value */
#define SC_DNIS_DIALED_NUMBER           L"dnis"   /* VXIchar value */


/**
 * Keys identifying standard read/write properties that are set using
 * SetProperty( ). Support for these is required when Transfer( ) is
 * implemented, all time durations are in milliseconds.
 * Implementations may support other read/write properties as well.
 */
#define SC_XFER_CONNECT_TIMEOUT          L"connecttimeout"  /* VXIchar value */
#define SC_XFER_MAXTIME                  L"maxtime"         /* VXIchar value */


/**
 * Defaults for standard properties
 */
#define SC_ANI_CALLER_NUMBER_DEFAULT     L""
#define SC_DNIS_DIALED_NUMBER_DEFAULT    L""
#define SC_XFER_MAXTIME_DEFAULT          L"-1"
#define SC_XFER_CONNECT_TIMEOUT_DEFAULT  L"60000"


/**
 * Events returned from the Session Control as detailed in the Session
 * Control interface description.  
 */
#define SC_EVENT_ERROR		            0x0010
#define SC_EVENT_END		            0x0011
#define SC_EVENT_BEGIN		            0x0012
#define SC_EVENT_TRANSFER_BEGIN	            0x0013
#define SC_EVENT_TRANSFER_COMPLETE	    0x0014
#define SC_EVENT_INVITE		            0x0015
#define SC_EVENT_DTMF_RECEIVED	            0x0016
#define _SC_EVENT_LAST_EVENT_MARKER         SC_EVENT_DTMF_RECEIVED


/**
 * Event structure for returning detailed status information
 *
 * All Session Control events return a status structure for providing
 * detailed session and/or status information.  This structure, along
 * with all the data it points at, is only valid for the duration of
 * the event listener invocation.
 */
typedef struct VXIscEventData {
  /**
   * Size of the data in bytes, may be 0
   */
  VXIulong      dataSizeBytes;

  /**
   * Pointer to the data, NULL if dataSizeBytes is 0
   */
  VXIptr        data;

  /**
   * MIME content type of the data
   */
  VXIchar      *mimetype;

  /**
   * Result code indicating details for SC_EVENT_ERROR events,
   * typically set to VXIsc_RESULT_SUCCESS for other events.
   */
  VXIscResult   resultCode;
} VXIscEventData;

/* For backward compatibility */
#define VXIscSessionData VXIscEventData


/**
 * Signature for the event handler called to report Session Control
 * events, as registered via RegisterEventListener( ).
 *
 * @param pThis	     [IN] session control reporting the event
 * @param eventType  [IN] Event type that occurred
 * @param eventData  [IN] VXIscEventDataStatus structure that reports
 *                    the details of the event, where the structure
 *                    and its members are only valid for the duration
 *                    of the event listener call.
 * @param userData   [IN] User data for the listener, as specified
 *                    when registering the listener.
 */
typedef void VXIscEventListener (
  struct VXIscInterface       *pThis,
  const VXIulong               eventType,
  const VXIscEventData        *eventData,
  const VXIptr		       userData
);


/*
** ==================================================
** VXIscInterface Interface Definition
** ==================================================
*/
/** @name VXIscInterface
 ** @memo Session Control interface for call control and DTMF input
 **
 */
typedef struct VXIscInterface {
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
   *          Internet address in reverse order, such as com.xyz.sessionControl
   *          for VXIsessionControl from xyz.com. This is similar to how
   *          VoiceXML 1.0 recommends defining application specific error
   *          types.
   */
  const VXIchar* (*GetImplementationName)(void);

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
   * @return VXIsc_RESULT_SUCCESS on success
   */
  VXIscResult (*RegisterEventListener)(struct VXIscInterface  *pThis,
				       VXIscEventListener     *eventListener,
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
   * @return VXIsc_RESULT_SUCCESS on success
   */
  VXIscResult (*UnregisterEventListener)(struct VXIscInterface  *pThis,
					 VXIscEventListener     *eventListener,
					 const VXIptr            userData);

  /**
   * @name SetProperty
   * @memo Set a property associated with the caller session
   *
   * @doc
   * See the SC_[...] properties as defined above for the standard
   * properties, each implementation may define additional properties
   * as well.
   *
   * @param propertyName	[IN] Name of property to set
   * @param valueBuffSizeBytes	[IN] Size of value, in bytes. For the
   *                             standard properties, pass
   *                             wcslen(valueStr) * sizeof(wchar_t)
   * @param value		[IN] Value for the property to set. 
   *                             Note this is an untyped pointer, it
   *                             is up to the Session Control implementation
   *                             to define the appropriate type for
   *                             each property (all the standard properties
   *                             are VXIchar * based).
   *
   * @return VXIsc_RESULT_SUCCESS on success
   * @result VXIsc_RESULT_NON_FATAL_ERROR for an invalid property
   */
  VXIscResult (*SetProperty)(struct VXIscInterface  *pThis,
			     const VXIchar          *propertyName,
			     const VXIunsigned       valueBuffSizeBytes,
			     const VXIptr 	     value);

  /**
   * @name GetNthPropertyPair
   * @memo Get the Nth property pair
   *
   * @doc 
   * Used to iterate through the Session Control properties,
   * retrieving each key/value pair. This is most commonly used to
   * retrieve all the properties in order to expose them for
   * application use, allowing the Session Control implementation to
   * expose implementation specific properties to the application
   * without requiring voice processing platform changes.
   *
   * @param index               [IN] Property index to retrieve, where
   *                             the first property is index 0.
   * @param propertyName	[IN] Name of the property to retrieve
   * @param nameBuffSizeChars	[MODIFY] As input, the size of the
   *                             name buffer, in characters. This 
   *                             is then modified to return the actual
   *                             name size, in bytes. When the name 
   *                             buffer is too small, this modified 
   *                             name can be used to allocate a larger
   *                             buffer and re-try.
   * @param name		[OUT] Name for the property
   * @param valueBuffSizeChars	[MODIFY] As input, the size of the
   *                             value buffer, in characters. This 
   *                             is then modified to return the actual
   *                             value size, in bytes. When the value 
   *                             buffer is too small, this modified 
   *                             value can be used to allocate a larger
   *                             buffer and re-try.
   * @param value		[OUT] Value for the property
   *
   * @return VXIsc_RESULT_SUCCESS on success
   * @return VXIsc_RESULT_INVALID_PROPERTY when index is past the
   *            number of available properties
   * @return VXIsc_RESULT_FAILURE when the property has a non-VXIchar
   *            based value (can only occur for implementation defined
   *            properties, all standard properties are VXIchar based)
   * @return VXIsc_RESULT_BUFFER_TOO_SMALL when the name or value buffer
   *            is too small, look at the modified [...]BuffSizeChars
   *            values for the required sizes.
   */
  VXIscResult (*GetNthPropertyPair)(struct VXIscInterface  *pThis,
				    const VXIunsigned       index,
				    VXIunsigned            *nameBuffSizeChars,
				    VXIchar                *propertyName,
				    VXIunsigned            *valueBuffSizeChars,
				    VXIchar                *value);
  
  /**
   * @name WaitForCall
   * @memo Re-arm the telephony interface for new calls
   *
   * @doc
   * This call is blocking. When it returns the telephony interface
   * must be ready to accept calls and generate events. <p> 
   *
   * The hardware interface must start-up in an out-of-service (OOS)
   * state.  In this state, no events will be generated and calls
   * should be blocked (if possible) or rejected. The best
   * implementation is to arrange for the line to be out of service
   * back to the switch so that failover to the next line occurs at
   * the switch level. For some protocols this can be done by setting
   * the line to a busy state. <p>
   *
   * A hangup or error on the line should place the hardware back into
   * the OOS state so that no calls come into the line until it is
   * explictly re-armed by calling this function again. <p>
   *
   * @return VXIsc_RESULT_SUCCESS on success
   */
  VXIscResult (*WaitForCall)(struct VXIscInterface  *pThis);


  /** 
   * @name AcceptSession
   * @memo Used to accept an invite for a new call
   *
   * @doc 
   * After a SC_EVENT_INVITE, this is called to accept the session,
   * connecting the caller and starting the call. Some Session Control
   * implementations may support (or require) passing implementation
   * defined data when accepting a session.
   *
   * @param mimeType	   [IN] MIME content type of the implementation
   *                        defined data, typically NULL
   * @param dataSizeBytes  [IN] Size of the implementation defined data,
   *                        in bytes, typically 0
   * @param data	   [IN] Implementation defined data, typically NULL
   *
   * @return VXIsc_RESULT_SUCCESS on success
   */
  VXIscResult (*AcceptSession)(struct VXIscInterface  *pThis,
			       const VXIchar          *mimeType,
			       const VXIunsigned      *dataSizeBytes,
			       const VXIptr           *data);

  /**
   * @name TerminateSession
   * @memo Terminate (hangup) the call
   *
   * @doc 
   * Ends the current session.  The caller is responsible for also
   * stopping the Audio Player and Audio Source interfaces if they
   * are active.
   *
   * @return VXIsc_RESULT_SUCCESS on success
   */
  VXIscResult (*TerminateSession)(struct VXIscInterface  *pThis);

  /** 
   * @name Transfer
   * @memo Transfers the calling party to another party
   *
   * @doc 
   * This call is blocking. Two types of transfers are supported:
   * bridge (supervised) and blind (unsupervised). For both,
   * a SC_EVENT_TRANSFER_BEGIN event is delivered when the transfer
   * is successfully initiated. 
   *
   * For blind transfers, this will be followed by a SC_EVENT_END, as
   * the transfer operation terminates the session. There is no
   * gaurantee that the far-end party will actually answer the call
   * when performing a blind transfer.
   *
   * For bridging transfers, the transfer is fully supervised, with
   * the session being retained in a non-interactive state until the
   * transfer completes. If the near-end party disconnects, a
   * SC_EVENT_END is delivered, and the session terminates. If the
   * far-end party disconnects normally or is disconnected (due to
   * exceeding the maximum time limit), or the transfer failed (due to
   * a no answer, network failure, etc.) a SC_EVENT_TRANSFER_COMPLETE
   * event is delivered, this function returns, and the interactive
   * session may proceed.
   *
   * The SC_XFER_CONNECT_TIMEOUT property controls the amount of time
   * allowed for a transfer connect attempt before the transfer is
   * cancelled and the interactive session restored.
   *
   * The SC_XFER_MAXTIME property controls the maximum time that the
   * near-end and far-end parties may talk once they are
   * connected. When this is exceeded, the far-end party is
   * disconnected and the interactive session is restored.
   *
   * Some Session Control implementations may support (or require)
   * passing implementation defined data when performing a transfer.
   *
   * @param transferDest   [IN] URI that identifies the transfer
   *                        destination. Must support the tel:
   *                        URI syntax as defined in RFC 2806
   *                        (http://www.ietf.org/rfc/rfc2806.txt),
   *                        implementations can opt to support other
   *                        URI formats as well.
   * @param mimeType	   [IN] MIME content type of the implementation
   *                        defined data, typically NULL
   * @param dataSizeBytes  [IN] Size of the implementation defined data,
   *                        in bytes, typically 0
   * @param transferType   [IN] Type of transfer, VXIscXferType_BLIND or
   *                        VXIscXferType_BRIDGE
   * @param data	   [IN] Implementation defined data, typically NULL
   *
   * @return VXIsc_RESULT_SUCCESS on success
   * @return VXIsc_RESULT_NEAR_END_DISCONNECT on a near-end disconnect
   * @return VXIsc_RESULT_FAR_END_DISCONNECT on a far-end disconnect
   * @return VXIsc_RESULT_NETWORK_DISCONNECT on a network disconnect
   * @return VXIsc_RESULT_NOANSWER on a no answer
   * @return VXIsc_RESULT_FAR_END_BUSY on a far-end busy
   * @return VXIsc_RESULT_NETWORK_BUSY on a network busy
   * @return VXIsc_RESULT_TIME_EXCEEDED on maximum connection time exceeded
   */
  VXIscResult (*Transfer)(struct VXIscInterface   *pThis,
			  const VXIchar           *transferDest,
			  const VXIchar           *mimeType,
			  const VXIunsigned        dataSizeBytes,
			  const VXIscTransferType  transferType,
			  const VXIptr             data);
  
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
   * @return VXIsc_RESULT_SUCCESS on success
   * @return VXIsc_RESULT_NON_FATAL_ERROR no DTMF digit available
   */
  VXIscResult (*GetDTMF)(struct VXIscInterface  *pThis,
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
   * @return VXIsc_RESULT_SUCCESS on success
   */
  VXIscResult (*FlushDTMFQueue)(struct VXIscInterface  *pThis);

} VXIscInterface;

/*@}*/

#ifdef __cplusplus
}
#endif

#include "VXIheaderSuffix.h"

#endif  /* include guard */
