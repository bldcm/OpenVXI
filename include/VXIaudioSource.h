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
 * $Id: VXIaudioSource.h,v 1.22.2.1 2002/08/06 20:18:13 dmeyer Exp $
 *
 ************************************************************************
 */

#ifndef _VXIAUDIOSOURCE_H 
#define _VXIAUDIOSOURCE_H 

#include "VXItypes.h"                  /* For VXIchar, VXIint32, etc. */
#include "VXIvalue.h"                  /* For VXIMap */

#include "VXIheaderPrefix.h"
#ifdef VXIAS_EXPORTS 
#define VXIAS_API SYMBOL_EXPORT_DECL 
#else 
#define VXIAS_API SYMBOL_IMPORT_DECL 
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct VXIasInterface;

/** 
 * @name VXIaudioSource
 * @memo Audio Source Interface
 * @version 1.0
 * @doc
 * Abstract interface for obtaining audio (speech) from the caller via
 * a telephony interface, where the audio is provided as in-memory
 * streaming audio delivered via events. <p>
 *
 * The Audio Source interface must be implemented for each new
 * underlying telephony interface, where this is frequently done by a
 * third party integrator.
 *
 * There is one Audio Source interface per telephony channel.
 *
 * The following table summarizes the events delivered by Audio Source
 * implementations.
 *
 * <table border=0>
 * <tr><td valign=top><b> AS_EVENT_ERROR </b></td>
 *
 * <td> Reports the Audio Source encountered an error in delivering
 * audio during a record operation, typically due to an underlying
 * telephony or I/O error. The error must have occurred after a call
 * to VXIasTriggerRecord( ) returned with a VXIas_RESULT_SUCCESS
 * result code, never during that call or after that call returned
 * with an error.  The Audio Source must be returned to an IDLE state
 * (the record operation halted) prior to delivering this event.
 *
 * The event includes a result code that indicates the cause of the
 * error. </td></tr>
 *
 * <tr><td valign=top><b> AS_EVENT_RECEIVED </b></td>
 *
 * <td> Reports audio (possibly caller speech) that has been received
 * from the telephony interface. This audio should be echo cancelled
 * audio (done by the telephony interface or an external device). It
 * can optionally be from a first-pass speech detector, which uses
 * the telephony interface or an external device to supporess audio
 * delivery until potential speech is identified within the audio
 * stream (either a noise detector or more intelligent speech detector). 
 *
 * However, if a first-pass speech detector is used (most commonly to
 * reduce CPU consuption by avoiding streaming silence that occurs
 * prior to speech), after it detects potential speech it must deliver
 * audio starting at 250 msec BEFORE the speech detector fired. This
 * allows a higher-level, more sophisticated endpointer integrated
 * above this interface to properly scan for low-energy speech that is
 * important for recognition that first-pass speech detectors commonly
 * chop off. </td></tr>
 *
 * <tr><td valign=top><b> AS_EVENT_STOPPED </b></td>
 *
 * <td> Reports that a record operation has terminated normally,
 * either due to the input operation successfully completing or the
 * record operation being stopped via TriggerStop( ). Audio Source
 * implementations may also choose to automatically stop the record
 * operation and return this event on hang-up, although this should
 * only be done when this is automatic functionality within the
 * underlying telephony library (the client is responsible for rapdily
 * calling TriggerStop( ) when hang-up occurs, which in most cases
 * simplifies the Audio Source implementation by avoiding session
 * control interactions).
 *
 * The Audio Source must move from the RECORDING state to an IDLE
 * state prior to delivering this event. </td></tr>
 *
 * <tr><td valign=top><b> AS_EVENT_OVERFLOW </b></td>
 *
 * <td> Reports the Audio Source has overflowed its internal buffers
 * while in a PAUSED state and is throwing out audio (the
 * implementation decides whether to discard audio first-in-first-out
 * or last-in-first-out). This will not be reported again until the
 * record operation is resumed and then paused again. The record
 * operation will otherwise proceed normally once resumed. The PAUSE
 * state was entered due to the event listener returning
 * VXIasCb_RESULT_PAUSE_AUDIO for an AS_EVENT_RECEIVED event.
 * </td></tr>
 *
 * <tr><td valign=top><b> AS_EVENT_LOST </b></td>
 *
 * <td> Reports the Audio Source has detected some audio data was
 * lost, such as dropped packets on a Voice over IP (VoIP) network. The
 * event reports the number of bytes lost and the sequence number of
 * the packet that was lost.
 * </td></tr>
 * </table> 
 */

/*@{*/

/**
 * @memo Macros to determine the availability of new methods
 */
#define AS_BEGIN_END_SESSION_SUPPORTED(asIntf) \
  ((asIntf->GetVersion( ) >= 0x00010002) &&    \
   (asIntf->BeginSession != NULL) && (asIntf->EndSession != NULL))

/**
 * Result codes for interface methods
 *
 * Result codes less then zero are severe errors (likely to be
 * platform faults), those greater then zero are warnings (likely to
 * be application issues)
 */
typedef enum VXIasResult {
  /** Fatal error, terminate call    */
  VXIas_RESULT_FATAL_ERROR       = -100,
  /** Record operation in progress   */
  VXIas_RESULT_BUSY              =  -51,
  /** Low-level telephony library error */
  VXIas_RESULT_DRIVER_ERROR      =  -50,
  /** I/O error                      */
  VXIas_RESULT_IO_ERROR          =   -8,
  /** Out of memory                  */
  VXIas_RESULT_OUT_OF_MEMORY     =   -7,
  /** System error, out of service   */
  VXIas_RESULT_SYSTEM_ERROR      =   -6,
  /** Errors from platform services  */
  VXIas_RESULT_PLATFORM_ERROR    =   -5,
  /** Return buffer too small        */
  VXIas_RESULT_BUFFER_TOO_SMALL  =   -4,
  /** Parameter name is not valid    */
  VXIas_RESULT_INVALID_PARAM_NAME  = -3,
  /** Parameter value is not valid   */
  VXIas_RESULT_INVALID_PARAM_VALUE = -2,
  /** Invalid function argument      */
  VXIas_RESULT_INVALID_ARGUMENT  =   -1,
  /* Success                         */
  VXIas_RESULT_SUCCESS           =    0,
  /** Normal failure, nothing logged */
  VXIas_RESULT_FAILURE           =    1,
  /** Non-fatal non-specific error   */
  VXIas_RESULT_NON_FATAL_ERROR   =    2,
  /** Unsupported MIME type          */
  VXIas_RESULT_BAD_MIME_TYPE     =   50,
  /** Operation is not supported     */
  VXIas_RESULT_UNSUPPORTED       =  100 
} VXIasResult; 


/**
 * Result codes for event listener callbacks
 */
typedef enum VXIasCallbackResult { 
  /** Event listener callback was successful */
  VXIasCb_RESULT_SUCCESS         = 1,
  /** Event listener callback requests pausing the delivery of
      additional audio until TriggerResume( ) is called, as the
      consumer is not ready for additional audio. The Audio Source may
      opt to ignore this (it should log an error to the operator
      console if doing so), or may support this by buffering the
      audio, AS_EVENT_ERROR, and AS_EVENT_STOPPED events in the
      meanwhile, delivering an AS_EVENT_OVERFLOW event if the Audio
      Source audio buffer overflows, and delivering all the buffered
      audio and events in the normal order if TriggerResume( ) is
      called. */
  VXIasCb_RESULT_PAUSE_AUDIO     = 2, 
} VXIasCallbackResult; 


/**
 * Events returned from the Audio Source as detailed in the Audio
 * Source interface description.
 */
#define AS_EVENT_ERROR                   0x0020 
#define AS_EVENT_RECEIVED                0x0021 
#define AS_EVENT_STOPPED                 0x0022 
#define AS_EVENT_OVERFLOW                0x0023 
#define AS_EVENT_LOST                    0x0024
#define _AS_EVENT_LAST_EVENT_MARKER      AS_EVENT_LOST


/**
 * Event structure for returning audio and/or detailed status information
 *
 * All Audio Source events return an event data structure for
 * providing audio from the record operation and/or detailed status
 * information.  This structure, along with all the data it points at,
 * is only valid for the duration of the event listener invocation.
 */
typedef struct VXIasEventData { 
  /** 
   * Size of the audio data in bytes, may be 0. For AS_EVENT_LOST and
   * AS_EVENT_OVERFLOW events, this is the amount of audio data lost.
   */
  VXIulong     dataSizeBytes;

  /** 
   * Pointer to the audio data, NULL if dataSizeBytes is 0 or when
   * this is related to an AS_EVENT_LOST or AS_EVENT_OVERFLOW event.
   */
  VXIptr       data;

  /** 
   * MIME content type for the audio data, must match an audio type
   * supported by the recognizer, and must be the same across the
   * entire record operation. Typical choices are "audio/basic"
   * (VXI_MIME_ULAW, Raw (headerless) 8kHz 8-bit mono u-law [PCM]
   * single channel audio [G.711]) and "audio/x-alaw-basic
   * (VXI_MIME_ALAW, Raw (headerless) 8kHz 8-bit mono A-law [PCM]
   * single channel audio [G.711]). When reporting AS_EVENT_LOST or
   * AS_EVENT_OVERFLOW events, the MIME content type must still
   * indicate the audio type used for the actual stream so the
   * dataSizeBytes value can be properly interpreted.
   */
  VXIchar     *mimetype;

  /**
   * When dataSizeBytes is greater then 0 (audio data is being
   * delivered), sequence number of the data packet, where the first
   * audio packet after TriggerRecord( ) is 1, and then each packet
   * afterwards increments the sequence number by one. (A TriggerStop( )
   * operation followed by a TriggerRecord( ) thus resets the
   * sequence number to 1.) If data packets are discarded due to
   * buffer overflows (when an AS_EVENT_OVERFLOW is delivered), or if
   * data packets were lost (when an AS_EVENT_LOST is delivered), the
   * sequence number should also be incremented by one for each packet
   * discarded or lost.
   */
  VXIulong     sequenceNum; 

  /**
   * Result code indicating details for AS_EVENT_ERROR events,
   * typically set to VXIas_RESULT_SUCCESS for other events.
   */
  VXIasResult  resultCode; 
} VXIasEventData; 

/* For backward compatibility */
#define VXIasAudioData VXIasEventData


/**
 * Signature for the event handler called to report Audio Source
 * events, as registered via RegisterEventListener( ).
 *
 * As a flow control mechanism, this may return
 * VXIasCb_RESULT_PAUSE_AUDIO to request the Audio Source to suspend
 * the delivery of audio packets, instead buffering the audio. Once
 * the consumer is ready for more audio, the audio packet delivery is
 * resumed by calling TriggerResume( ). However, this Audio Source
 * functionality is optional, and even Audio Source implementations
 * that support this may have limited buffer sizes and may thus return
 * an AS_EVENT_OVERFLOW error (throwing out audio warning) if the
 * stream is paused for long.
 *
 * @param pThis	     [IN] Audio source reporting the event
 * @param eventType  [IN] Event type that occurred
 * @param eventData  [IN] VXIasEventDataStatus structure that reports
 *                    the details of the event, where the structure
 *                    and its members are only valid for the duration
 *                    of the event listener call (audio must be deep
 *                    copied if required for a longer time period).
 * @param userData   [IN] User data for the listener, as specified
 *                    when registering the listener.  
 * 
 * @return VXIasCb_RESULT_SUCCESS on success
 * @return VXIasCb_RESULT_PAUSE_AUDIO to request the Audio Source to
 *          pause audio delivery (may get ignored, however, as that
 *          functionality is optional)
 */
typedef VXIasCallbackResult VXIasEventListener (
  struct VXIasInterface  *pThis,
  const VXIulong          eventType,
  const VXIasEventData   *eventData,
  const VXIptr            userData
);


/* 
** ================================================== 
** VXIasInterface Interface Definition 
** ================================================== 
*/ 
/** @name VXIasInterface 
 ** @memo Audio Source interface for audio input
 **
 */
typedef struct VXIasInterface { 
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
   *          Internet address in reverse order, such as com.xyz.audioSource
   *          for VXIaudioSource from xyz.com. This is similar to how
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
   * @return VXIas_RESULT_SUCCESS on success
   */
  VXIasResult (*RegisterEventListener)(struct VXIasInterface  *pThis,
				       VXIasEventListener     *eventListener,
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
   * @return VXIas_RESULT_SUCCESS on success
   */
  VXIasResult (*UnregisterEventListener)(struct VXIasInterface  *pThis,
					 VXIasEventListener     *eventListener,
					 const VXIptr            userData);


  /** 
   * @name TriggerRecord 
   * @memo Triggers delivery of audio input data to all listeners, non-blocking
   *
   * @doc 
   * This only triggers audio input data delivery. This routine is
   * expected to return after data delivery has been enabled (as
   * opposed to blocking until data delivery is complete).
   *
   * @param mimeType  [IN] MIME content type for the audio input data,
   *                   may be NULL or an empty string to indicate that
   *                   any MIME content type supported by the recognizer
   *                   is acceptable. 
   *
   * @return VXIas_RESULT_SUCCESS on success
   * @return VXIas_RESULT_BAD_MIME_TYPE, MIME type is not supported
   */
  VXIasResult (*TriggerRecord)(struct VXIasInterface  *pThis,
			       const VXIchar          *mimeType);
  
  /**
   * @name TriggerStop
   * @memo Stops the record operation
   *
   * @doc 
   * This routine is synchronous and returns when the record operation
   * is either stopped or there is an error. If success is returned,
   * an AS_EVENT_STOPPED event must be delivered before this returns.
   *
   * @return VXIas_RESULT_SUCCESS on success
   * @return VXIas_RESULT_NON_FATAL_ERROR if no record is in progress
   * @return Another VXIas_RESULT_[...] code for severe errors 
   */
  VXIasResult (*TriggerStop)(struct VXIasInterface  *pThis);

  /** 
   * @name TriggerResume   
   * @memo Resumes the delivery of audio input data
   *
   * @doc
   * If an event listener returns an VXIasCb_RESULT_PAUSE_AUDIO return
   * code, the Audio Source is requested to pause audio delivery. This
   * call resumes audio delivery, where the buffered audio will be
   * rapidly delivered until either it "catches up" with the real time
   * audio input feed, or until an event listener pauses delivery
   * again. (If the Audio Source audio buffer overflows, an
   * AS_EVENT_OVERFLOW event is delivered.)
   *
   * However, responding to VXIasCb_RESULT_PAUSE_AUDIO is optional.
   * If the audio source cannot support that functionality, all calls
   * to this should return VXIas_RESULT_UNSUPPORTED for symmetry.
   *
   * @return VXIas_RESULT_SUCCESS on success
   * @return VXIas_RESULT_NON_FATAL_ERROR if no record is in progress
   * @return VXIas_RESULT_UNSUPPORTED if pause/resume is not supported
   * @return Another VXIas_RESULT_[...] code for severe errors 
   */
  VXIasResult (*TriggerResume)(struct VXIasInterface  *pThis);

  /**
   * @name BeginSession
   * @memo Reset for a new session.
   *
   * @doc
   * NOTE: This is only available as of version 1.2 of the 
   *       VXIaudioSource interface, use AS_BEGIN_END_SESSION_SUPPORTED( )
   *       to determine availability.
   *
   * This must be called for each new session, allowing for
   * call specific handling to occur. For some implementations, this
   * can be a no-op.  For others runtime binding of resources or other
   * call start specific handling may be done on this call.
   *
   * @param args  [IN] Implementation defined input and output
   *                    arguments for the new session
   *
   * @result VXIas_RESULT_SUCCESS on success
   */
  VXIasResult (*BeginSession)(struct VXIasInterface  *pThis,
                              VXIMap                 *args);

  /**
   * @name EndSession
   * @memo Performs cleanup at the end of a call session.
   *
   * @doc
   * NOTE: This is only available as of version 1.2 of the 
   *       VXIaudioSource interface, use AS_BEGIN_END_SESSION_SUPPORTED( )
   *       to determine availability.
   *
   * This must be called at the termination of a call, allowing for
   * call specific termination to occur. For some implementations, this
   * can be a no-op. For others runtime resources may be released or
   * other adaptation may be completed.
   *
   * @param args  [IN] Implementation defined input and output
   *                    arguments for ending the session
   *
   * @result VXIas_RESULT_SUCCESS on success
   */
  VXIasResult (*EndSession)(struct VXIasInterface  *pThis,
                            VXIMap                 *args);

} VXIasInterface; 

/*@}*/

#ifdef __cplusplus
}
#endif

#include "VXIheaderSuffix.h"

#endif  /* include guard */
