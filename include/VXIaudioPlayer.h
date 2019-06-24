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
 * $Id: VXIaudioPlayer.h,v 1.27.2.2 2002/08/06 20:18:13 dmeyer Exp $
 *
 ************************************************************************
 */

#ifndef _VXIAUDIOPLAYER_H
#define _VXIAUDIOPLAYER_H

#include "VXItypes.h"                  /* For VXIchar, VXIint32, etc. */
#include "VXIvalue.h"                  /* For VXIMap */

#include "VXIheaderPrefix.h"
#ifdef VXIAP_EXPORTS
#define VXIAP_API SYMBOL_EXPORT_DECL
#else
#define VXIAP_API SYMBOL_IMPORT_DECL
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct VXIapInterface;

/**
 * @name VXIaudioPlayer
 * @memo Audio Player Interface
 * @version 1.0
 * @doc
 * Abstract interface for playing audio to the caller via a telephony
 * interface, where the audio may be on-disk audio files, in-memory
 * audio, or in-memory streaming audio. <p>
 *
 * The Audio Player interface must be implemented for each new
 * underlying telephony interface, where this is frequently done by a
 * third party integrator.
 *
 * There is one Audio Player interface per telephony channel.
 *
 * The following table summarizes the audio MIME content types that
 * all Audio Player implementations should support (they typically
 * support other MIME content types as well). The table after that
 * summarizes the events delivered by Audio Player implementations.
 *
 * <table border=0>
 * <tr><td valign=top><b> audio/basic (VXI_MIME_ULAW) </b></td>
 *
 * <td> Raw (headerless) 8kHz 8-bit mono u-law [PCM] single channel
 * audio (G.711) </td></tr>
 *
 * <tr><td valign=top><b> audio/x-alaw-basic (VXI_MIME_ALAW) </b></td>
 *
 * <td> Raw (headerless) 8kHz 8-bit G.711 mono A-law [PCM] single
 * channel audio (G.711) </td></tr>
 *
 * <tr><td valign=top><b> audio/x-wav (VXI_MIME_WAV) </b></td>
 *
 * <td> WAV (RIFF header) 8kHz 8-bit mono u-law or A-law [PCM]
 * single channel audio </td></tr>
 * </table>
 *
 * <table border=0>
 * <tr><td valign=top><b> AP_EVENT_STARTING </b></td>
 *
 * <td> Reports the Audio Player moved from the IDLE state to
 * the PLAYING or STREAMING state. Delivered only for the first play
 * node in a play or streaming play operation. </td></tr>
 *
 * <tr><td valign=top><b> AP_EVENT_ERROR </b></td>
 *
 * <td> Reports the Audio Player encountered an error in playing
 * a play node in the play list or a underlying telephony or I/O
 * error. The error must have occurred after a call to
 * VXIapTriggerPlay( ) or VXIapTriggerStreamingPlay( ) returned with a
 * VXIap_RESULT_SUCCESS result code, never during one of those calls
 * or after one of those calls return with an error.  The Audio Player
 * must be returned to an IDLE state (the play operation halted) prior
 * to delivering this event as the user may immediately trigger
 * additional play operations from within the event listener.
 *
 * The event includes a result code that indicates the cause of the
 * error and the playlist node that caused the error (when
 * available). </td></tr>
 *
 * <tr><td valign=top><b> AP_EVENT_WARNING </b></td>
 *
 * <td> Reports the Audio Player encountered a non-fatal error in
 * playing a play node in the play list or a underlying telephony or
 * I/O error that is recoverable.  The error must have occurred after
 * a call to VXIapTriggerPlay( ) or VXIapTriggerStreamingPlay( )
 * returned, never during one of those calls or after one of those
 * calls return with an error.  The warning has no implications on the
 * current state, if the result of the warning is that the play
 * operation completed an AP_EVENT_COMPLETED event must be delivered
 * after this event (or an AP_EVENT_ERROR delivered instead).
 *
 * The event includes a result code that indicates the cause of the
 * error and the playlist node that caused the error (when
 * available). </td></tr>
 *
 * <tr><td valign=top><b> AP_EVENT_COMPLETED </b></td>
 *
 * <td> Reports that a play operation has terminated normally, either
 * due to the output operation successfully completing or the play
 * operation being stopped via TriggerStop( ). Audio Player
 * implementations may also choose to automatically stop the play
 * operation and return this event on hang-up, although this should
 * only be done when this is automatic functionality within the
 * underlying telephony library (the client is responsible for rapdily
 * calling TriggerStop( ) when barge-in or hang-up occurs, which in
 * most cases simplifies the Audio Player implementation by avoiding
 * session control and audio source interactions).
 *
 * The Audio Player must move from the PLAYING or STREAMING state
 * to an IDLE state prior to delivering this event. </td></tr>
 *
 * <tr><td valign=top><b> AP_EVENT_RESUME_STREAM </b></td>
 *
 * <td> Reports the Audio Player is ready to resume a streaming audio
 * source that is currently paused, thus obtaining additional audio
 * for the current streaming audio operation. The source was paused
 * when an overflow occured while delivering audio for the streaming
 * play operation (TriggerStreamingPlay( ) returned
 * VXIap_RESULT_STREAM_OVERFLOW).  </td></tr>
 * </table>
 */

/*@{*/

/**
 * @memo Macros to determine the availability of new methods
 */
#define AP_BEGIN_END_SESSION_SUPPORTED(apIntf) \
  ((apIntf->GetVersion( ) >= 0x00010002) &&    \
   (apIntf->BeginSession != NULL) && (apIntf->EndSession != NULL))

/**
 * Result codes for interface methods
 *
 * Result codes less then zero are severe errors (likely to be
 * platform faults), those greater then zero are warnings (likely to
 * be application issues)
 */
typedef enum VXIapResult {
  /** Fatal error, terminate call    */
  VXIap_RESULT_FATAL_ERROR       = -100,
  /** Play operation in progress     */
  VXIap_RESULT_BUSY              =  -51,
  /** Low-level telephony library error */
  VXIap_RESULT_DRIVER_ERROR      =  -50,
  /** I/O error                      */
  VXIap_RESULT_IO_ERROR          =   -8,
  /** Out of memory                  */
  VXIap_RESULT_OUT_OF_MEMORY     =   -7,
  /** System error, out of service   */
  VXIap_RESULT_SYSTEM_ERROR      =   -6,
  /** Errors from platform services  */
  VXIap_RESULT_PLATFORM_ERROR    =   -5,
  /** Return buffer too small        */
  VXIap_RESULT_BUFFER_TOO_SMALL  =   -4,
  /** Parameter name is not valid    */
  VXIap_RESULT_INVALID_PARAM_NAME  = -3,
  /** Parameter value is not valid   */
  VXIap_RESULT_INVALID_PARAM_VALUE = -2,
  /** Invalid function argument      */
  VXIap_RESULT_INVALID_ARGUMENT  =   -1,
  /* Success                         */
  VXIap_RESULT_SUCCESS           =    0,
  /** Normal failure, nothing logged */
  VXIap_RESULT_FAILURE           =    1,
  /** Non-fatal non-specific error   */
  VXIap_RESULT_NON_FATAL_ERROR   =    2,
  /** File open error                */
  VXIap_RESULT_FILE_ERROR        =   50,
  /** There is nothing to play       */
  VXIap_RESULT_NOTHING_TO_PLAY   =   51,
  /** The stream has overflowed      */
  VXIap_RESULT_STREAM_OVERFLOW   =   52,
  /** The stream has run out of data to play */
  VXIap_RESULT_STREAM_UNDERFLOW  =   53,
  /** Unsupported MIME type          */
  VXIap_RESULT_BAD_MIME_TYPE =       54,
  /** Operation is not supported     */
  VXIap_RESULT_UNSUPPORTED       =  100
} VXIapResult;


/**
 * Definition of a play list node, used to deliver a linked list of
 * audio for playback
 */
typedef struct VXIapPlayListNode {
  /**
   * MIME content type for the data being passed, even when the data
   * is provided from a file. Audio Player implementations must use
   * this, not file extension mapping rules, to determine the audio
   * type for playback. The supported MIME content types are
   * determined by the Audio Player implementation.
   */
  VXIchar                      *contentType;

  /**
   * Full path to a local file that contains the content to
   * present. NULL if in-memory content is being supplied instead
   * (will be NULL for all streaming play operations).
   */
  VXIchar                      *filePath;

  /**
   * In-memory content to present. NULL if a file path is being
   * supplied instead.
   */
  VXIptr                        content;

  /**
   * Length, in bytes, of the content to present. 0 if a file path is
   * being supplied.
   */
  VXIlong                       contentLen;


  /**
   * Previous node in the play list. NULL if this is the head of the
   * list. The Audio Player implementation may freely modify this for
   * its own purposes whenever it accepts the node by returning
   * VXIap_RESULT_SUCCESS from TriggerPlay( ) or
   * TriggerStreamingPlay( ).
   */
  struct VXIapPlayListNode     *previous;

  /**
   * Next node in the play list.  NULL if this is the end of the
   * list. The Audio Player implementation may freely modify this for
   * its own purposes whenever it accepts the node by returning
   * VXIap_RESULT_SUCCESS from TriggerPlay( ) or
   * TriggerStreamingPlay( ).
   */
  struct VXIapPlayListNode     *next;

  /**
   * Destructor to destroy the node. The Audio Player implementation
   * must call this destructor on each individual node in the linked
   * list when it no longer requires the node, preferably as soon as
   * the data is no longer required. Destruction of a node does NOT
   * cause the destruction of the previous or next nodes.
   */
  void (*Destroy)(struct VXIapPlayListNode **node);

} VXIapPlayListNode;


/**
 * Events returned from the Audio Player as detailed in the Audio
 * Player interface description.
 */
#define AP_EVENT_STARTING	          0x0030
#define AP_EVENT_ERROR		          0x0031
#define AP_EVENT_WARNING                  0x0032
#define AP_EVENT_COMPLETED	          0x0033
#define AP_EVENT_RESUME_STREAM	          0x0034
#define _AP_EVENT_LAST_EVENT_MARKER	  AP_EVENT_RESUME_STREAM


/**
 * Event structure for returning detailed status information
 *
 * All Audio Player events return a status structure for providing
 * detailed status information along with the affected play list node.
 * This structure, along with all the data it points at, is only valid
 * for the duration of the event listener invocation.
 */
typedef struct VXIapEventDataStatus {
  /**
   * Exact node playing when the event occurred, such as the node that
   * caused an error condition for an error event. If the underlying
   * telephony API prevents determining the exact node, this may be
   * set to NULL.
   */
  VXIapPlayListNode  *playListNode;

  /**
   * Result code indicating details for AP_EVENT_ERROR and
   * AP_EVENT_WARNING events, typically set to VXIap_RESULT_SUCCESS
   * for other events.
   */
  VXIapResult         resultCode;
} VXIapEventDataStatus;


/**
 * Signature for the event handler called to report Audio Player
 * events, as registered via RegisterEventListener( ).
 *
 * @param pThis	     [IN] Audio player reporting the event
 * @param eventType  [IN] Event type that occurred
 * @param eventData  [IN] VXIapEventDataStatus structure that reports
 *                    the details of the event, where the structure
 *                    and its members are only valid for the duration
 *                    of the event listener call.
 * @param userData   [IN] User data for the listener, as specified
 *                    when registering the listener.
 */
typedef void VXIapEventListener (
  struct VXIapInterface       *pThis,
  const VXIulong               eventType,
  const VXIapEventDataStatus  *eventData,
  const VXIptr		       userData
);


/*
** ==================================================
** VXIapInterface Interface Definition
** ==================================================
*/
/** @name VXIapInterface
 ** @memo Audio Player interface for audio playback
 **
 */
typedef struct VXIapInterface {
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
   *          Internet address in reverse order, such as com.xyz.audioPlayer
   *          for VXIaudioPlayer from xyz.com. This is similar to how
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
   * @return VXIap_RESULT_SUCCESS on success
   */
  VXIapResult (*RegisterEventListener)(struct VXIapInterface  *pThis,
				       VXIapEventListener     *eventListener,
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
   * @return VXIap_RESULT_SUCCESS on success
   */
  VXIapResult (*UnregisterEventListener)(struct VXIapInterface  *pThis,
					 VXIapEventListener     *eventListener,
					 const VXIptr            userData);

  /**
   * @name GetOptimalAudioMimeType
   * @memo Retrieve the preferred MIME content type for play operations
   *
   * @doc
   * This retrieves the preferred MIME content type for play
   * operations, allowing clients to obtain that type whenever
   * possible from audio data sources such as text-to-speech
   * engines. Note that this value is only a hint, the client
   * will play audio with other MIME content types in many other
   * such as when the application developer specifies an audio file.
   *
   * @param contentType	    [OUT] Preferred MIME content type for audio
   * @param contentTypeLen  [IN] Size, in characters, of contentType
   *
   * @return VXIap_RESULT_SUCCESS on success
   */
  VXIapResult (*GetOptimalAudioMimeType)(struct VXIapInterface *pThis,
					 VXIchar 	       *contentType,
					 const VXIint	       contentTypeLen);

  /**
   * @name GetOptimalStreamBufferSizeRange
   * @memo Retrieve the preferred range of audio buffer sizes for streaming
   *
   * @doc
   * This retrieves the preferred audio buffer size range for streaming
   * play operations, allowing clients to obtain a buffer size within that
   * range whenever possible from streaming audio data sources such as
   * text-to-speech engines. Note that this value is only a hint, the
   * client will play audio with other buffer sizes in cases where the
   * desired range cannot be satisfied by the audio source.
   *
   * @param lowLimitBytes  [OUT] Lower limit of the optimal range, in bytes
   * @param highLimitBytes [OUT] High limit of the optimal range, in bytes
   *
   * @return VXIap_RESULT_SUCCESS on success
   */
  VXIapResult (*GetOptimalStreamBufferSizeRange)(struct VXIapInterface *pThis,
						 VXIulong     *lowLimitBytes,
						 VXIulong     *highLimitBytes);

  /**
   * @name TriggerPlay
   * @memo Triggers playing audio to the caller, non-blocking
   *
   * @doc
   * This routine is asynchronous, in that it does not block - it
   * only triggers the begin of a play operation. This interface only
   * supports one play operation at a time. If another play operation
   * is initiated via TriggerPlay( ) or TriggerStreamingPlay( ) before
   * this play operation completes, a VXIap_RESULT_BUSY error is returned.
   * <p>
   * When this function returns successfully, ownership of the entire
   * play list is passed to the Audio Player, which may then modify
   * the previous and next pointers in the play list as desired to
   * support the immplementation. The Audio Player is then responsible
   * for calling the Destroy( ) method of each play node as each
   * node is no longer needed. If this function returns an error,
   * however, this function must not have made any changes to the play
   * list (particularly the previous and next pointers), and ownership
   * is returned to the client.
   *
   * @param playList      [IN] List of play nodes to play to the caller,
   *                       on success ownership is passed
   * @param playListSize  [IN] Number of nodes in the play list
   * @param invalidNode   [OUT] On error, this is set to point at the
   *                       node in the play list that triggered the
   *                       error, or NULL if there is a generic error.
   *
   * @return VXIap_RESULT_SUCCESS on success
   * @return VXIap_RESULT_BUSY if a play is already in progress
   * @return VXIap_RESULT_NON_FATAL_ERROR if the MIME content
   *          type is not supported
   * @return Another VXIap_RESULT_[...] code for severe errors
   */
  VXIapResult (*TriggerPlay)(struct VXIapInterface  *pThis,
			     VXIapPlayListNode      *playList,
			     const VXIint	     playListSize,
			     VXIapPlayListNode     **invalidNode);

  /**
   * @name TriggerStop
   * @memo Stops the play operation
   *
   * @doc
   * This routine is synchronous and returns when the play is either
   * stopped or there is an error. If success is returned, an
   * AP_EVENT_COMPLETED event must be delivered before this returns.
   *
   * @return VXIap_RESULT_SUCCESS on success
   * @return VXIap_RESULT_NON_FATAL_ERROR if no play is in progress
   * @return Another VXIap_RESULT_[...] code for severe errors
   */
  VXIapResult (*TriggerStop)(struct VXIapInterface  *pThis);

  /**
   * @name TriggerStreamingPlay
   * @memo Triggers playing an audio stream to the caller, non-blocking
   *
   * @doc
   * This routine is asynchronous, in that it does not block - it only
   * triggers the begin of a play operation. This interface only
   * supports one play operation at a time, however for streaming
   * playback this function will be called multiple times by the
   * client in order to provide the streaming audio as it becomes
   * available from the streaming source. If another play operation is
   * initiated via TriggerPlay( ) before this operation completes, a
   * VXIap_RESULT_BUSY error is returned.
   * <p>
   * The first call to TriggerStreamingPlay( ) (while the Audio Player
   * is in an IDLE state) initiates a streaming play operation. For
   * streaming plays, every play node will always contain in-memory
   * audio, never a pointer to a file. The MIME content type and number
   * of bytes of audio within each play node will not vary within a
   * single streaming play operation.
   * <p>
   * The client merely delivers streaming audio as fast as possible,
   * making no attempt to recover from network delays (jitter) during
   * play operations, so the Audio Player should pre-buffer an
   * implementation defined quantity of audio before actually starting
   * playback to avoid underruns. The AP_EVENT_STARTING event must
   * only be delivered once the actual audio starts getting heard by
   * the caller (playback actually starts). If an underflow does
   * occur, the Audio Player must deliver an AP_EVENT_WARNING event
   * with a VXIap_RESULT_STREAM_UNDERFLOW status code to notify the
   * client, then re-build the pre-buffer before resuming playback
   * again.
   * <p>
   * For flow control, the Audio Player should maintain high and low
   * watermark thresholds. Whenever the amount of buffered audio is
   * less then the implementation defined high watermark, accept audio
   * from TriggerStreamingPlay( ) calls. When that is exceeded,
   * however return a VXIap_RESULT_STREAM_OVERFLOW error to instruct
   * the client to suppress additional audio until the Audio Player
   * delivers an AP_EVENT_RESUME_STREAM event. The Audio Player should
   * only do so when the amount of buffered audio falls below the low
   * watermark. This mechanism, where there is some distance between
   * the low and high watermarks, ensures the flow control is done
   * efficiently by minimizing the amount of times the audio stream is
   * paused and resumed, as those operations are relatively expensive.
   * <p>
   * When this function returns successfully, ownership of the play
   * node is passed to the Audio Player, which may then modify the
   * previous and next pointers as desired to support the
   * immplementation. The Audio Player is then responsible for calling
   * the Destroy( ) method of each play node as each node is no longer
   * needed. If this function returns an error, however, this function
   * must not have made any changes to the play node (particularly the
   * previous and next pointers), and ownership is returned to the
   * client.
   *
   * @param playList          [IN] Play node to play to the caller,
   *                           on success ownership is passed
   * @param lastPlayInStream  [IN] TRUE to indicate this is the last
   *                           play node in the stream, FALSE to indicate
   *                           additional play nodes will follow
   *
   * @return VXIap_RESULT_SUCCESS on success
   * @return   VXIap_RESULT_BUSY if a non-streaming play is already in
   *              progress
   * @return   VXIap_RESULT_SUCCESS on success
   * @return   VXIap_INVALID_ARGUMENT if a file path is supplied for
   *             any of the play nodes, all content must be passed in-memory
   * @return   VXIap_RESULT_NON_FATAL_ERROR if the MIME content
   *             type is not supported or if the MIME type does not match
   *             the MIME type of the current stream.
   * @return   VXIap_RESULT_STREAM_OVERFLOW if the VXIapInterface is
   *           temporarily unable accept any more playnodes. When it is
   *           ready for more plays the VXIapInterface will issue an
   *           AP_EVENT_RESUME_STREAM event and the application can continue
   *           to calling TriggerStreamingPlay starting with the playnode
   *           that first resulted in the VXIap_RESULT_STREAM_OVERFLOW.
   *
   * */
  VXIapResult (*TriggerStreamingPlay)(struct VXIapInterface *pThis,
				      VXIapPlayListNode     *playNode,
				      const VXIbool          lastPlayInStream);

  /**
   * @name BeginSession
   * @memo Reset for a new session.
   *
   * @doc
   * NOTE: This is only available as of version 1.2 of the
   *       VXIaudioPlayer interface, use AP_BEGIN_END_SESSION_SUPPORTED( )
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
   * @result VXIap_RESULT_SUCCESS on success
   */
  VXIapResult (*BeginSession)(struct VXIapInterface  *pThis,
                              VXIMap                 *args);

  /**
   * @name EndSession
   * @memo Performs cleanup at the end of a call session.
   *
   * @doc
   * NOTE: This is only available as of version 1.2 of the
   *       VXIaudioPlayer interface, use AP_BEGIN_END_SESSION_SUPPORTED( )
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
   * @result VXIap_RESULT_SUCCESS on success
   */
  VXIapResult (*EndSession)(struct VXIapInterface  *pThis,
                            VXIMap                 *args);
  /**
   ** @name TriggerPause()
   **
   ** @memo Pauses the play device.
   **
   ** @doc This routine is asynchronous.
   **
   ** @param	pThis		[*]in []out []modify	<br>
   **		        	pointer to the instance of the service
   **
   ** @return VXIap_RESULT_SUCCESS on success
   ** @return VXIap_RESULT_NON_FATAL_ERROR if no play is in progress or already paused.
   **          (or another VXIapResult code for severe errors)
   ** @see TriggerPause()
   */
  VXIapResult (*TriggerPause)(struct VXIapInterface* pThis /*in*/);

  /**
   ** @name TriggerResume()
   **
   ** @memo Resumes the play device.
   **
   ** @doc This routine is asynchronous.
   **
   ** @param	pThis		[*]in []out []modify	<br>
   **		        	pointer to the instance of the service
   **
   ** @return VXIap_RESULT_SUCCESS on success

   ** @return VXIap_RESULT_NON_FATAL_ERROR if no play is in progress or play
   ** in progress, but not paused.  (or another VXIapResult code for severe
   ** errors) @see TriggerPause()
   **/
  VXIapResult (*TriggerResume)(struct VXIapInterface* pThis /*in*/);

} VXIapInterface;

/*@}*/

#ifdef __cplusplus
}
#endif

#include "VXIheaderSuffix.h"

#endif  /* include guard */
