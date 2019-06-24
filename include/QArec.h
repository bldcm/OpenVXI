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
 ***********************************************************************
 *
 * $Id: QArec.h,v 1.1.2.4 2003/07/28 16:19:24 ccao Exp $
 * 
 ************************************************************************
 */

#ifndef _QAREC_H
#define _QAREC_H

#include "VXIrec.h"                 /* For VXIrec base interface */
#include "VXIlog.h"                 /* For VXIlog interface */
#include "VXIcache.h"               /* For VXIcache interface */
#include "VXIinet.h"                /* For VXIinet interface */
#include "VXIprompt.h"                /* For VXIprompt interface */

#include "VXIheaderPrefix.h"
#ifdef QAREC_EXPORTS
#define QAREC_API SYMBOL_EXPORT_DECL
#else
#define QAREC_API SYMBOL_IMPORT_DECL
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name QArec
 * @memo QArec Interface
 * @doc
 * QArec provides a simulator implementation of the VXIrec abstract interface
 * for recognition functionality.   Recognition is done with all
 * blocking calls, because VoiceXML is essentially a blocking
 * protocol.  One VXIrec interface should be constructed per line.
 */

/*@{*/

/**
 * Global platform initialization of QArec
 *
 * @param log            VXI Logging interface used for error/diagnostic 
 *                       logging, only used for the duration of this 
 *                       function call
 * @param diagLogBase    Base tag number for diagnostic logging purposes.
 *                       All diagnostic tags for QArec will start at this
 *                       ID and increase upwards.
 * @param parameters     Any external parameters which may be needed to
 *                       initialize this layer.
 * 
 * @result VXIrec_RESULT_SUCCESS on success
 */
QAREC_API VXIrecResult QArecInit(VXIlogInterface *log,
				 VXIunsigned      diagLogBase,
				 VXIMap          *parameters);

/**
 * Global platform shutdown of Recognizer
 *
 * @param log    VXI Logging interface used for error/diagnostic logging,
 *               only used for the duration of this function call
 *
 * @result VXIrec_RESULT_SUCCESS on success
 */
QAREC_API VXIrecResult QArecShutDown(VXIlogInterface *log);

/**
 * Create a new recognizer service handle
 *
 * @param log    VXI Logging interface used for error/diagnostic logging,
 *               must remain a valid pointer throughout the lifetime of
 *               the resource (until QArecDestroyResource( ) is called)
 * @param inet   VXI Internet interface used for URL fetches,
 *               must remain a valid pointer throughout the lifetime of
 *               the resource (until QArecDestroyResource( ) is called)
 * @param prompt VXI prompt interface used for synthesizing and recording 
 *               simulated speech input.  Can pass as NULL if recording of 
 *               speech dialog is not supported by the implementation.
 * @param rec    VXIrecInterface pointer that will be allocated within this
 *               function. Call QArecDestroyResource( ) to delete the 
 *               resource.
 *
 * @result VXIrec_RESULT_SUCCESS on success 
 */
QAREC_API VXIrecResult QArecCreateResource(VXIlogInterface   *log,
					   VXIinetInterface  *inet,
					   VXIcacheInterface *cache,
					   VXIpromptInterface *prompt,
					   VXItelInterface *tel,
					   VXIrecInterface  **rec, 
					   VXIbool useInetWithSWIrec);

/**
 * Destroy the interface and free internal resources. Once this is
 *  called, the logging and Internet interfaces passed to
 *  VXIrecognizerCreateResource( ) may be released as well.
 *
 * @param rec    VXIrecInterface pointer that will be deallocated.  It will
 *               be set to NULL when deallocated.
 *
 * @result VXIrec_RESULT_SUCCESS on success 
 */
QAREC_API VXIrecResult QArecDestroyResource(VXIrecInterface **rec);

/*@}*/

#ifdef __cplusplus
}
#endif

#include "VXIheaderSuffix.h"

#endif  /* include guard */
