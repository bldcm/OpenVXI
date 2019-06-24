
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
 

 /*****************************************************************************
  *****************************************************************************
  *
  * SBjsiLogger - logging class for SBinet
  *
  * This provides logging definitions for SBinet use of VXIlog, along
  * with some convenience macros.
  *
  *****************************************************************************
  ****************************************************************************/
 
 
 // -----1=0-------2=0-------3=0-------4=0-------5=0-------6=0-------7=0-------8
 
 #include "SBjsiInternal.h"
 
 #include "SBjsiLogger.hpp"                // For this class
 
 // -----1=0-------2=0-------3=0-------4=0-------5=0-------6=0-------7=0-------8
 
 
 // Determine if a tag is enabled
 VXIbool SBjsiLogger::DiagIsEnabled (VXIunsigned tagID) const
 {
   return (*_log->DiagnosticIsEnabled)(_log, _diagTagBase + tagID);
 }
 
 
 // Error logging
 VXIlogResult
 SBjsiLogger::Error (VXIunsigned errorID, const VXIchar *format, ...) const
 {
   va_list arguments;
 
   if ( ! _log )
     return VXIlog_RESULT_FAILURE;
   
   VXIlogResult rc;
   if ( format ) {
     va_start(arguments, format);
     rc = (*_log->VError)(_log, _moduleName.c_str( ), errorID, format, 
 			 arguments);
     va_end(arguments);
   } else {
     rc = (*_log->Error)(_log, _moduleName.c_str( ), errorID, NULL);
   }
 
   return rc;
 }
 
 
 // Error logging
 VXIlogResult
 SBjsiLogger::Error (VXIlogInterface *log, VXIunsigned errorID, 
 		     const VXIchar *format, ...) const
 {
   va_list arguments;
 
   if ( ! log )
     return VXIlog_RESULT_FAILURE;
   
   VXIlogResult rc;
   if ( format ) {
     va_start(arguments, format);
     rc = (*log->VError)(log, _moduleName.c_str( ), errorID, format, arguments);
     va_end(arguments);
   } else {
     rc = (*log->Error)(log, _moduleName.c_str( ), errorID, NULL);
   }
 
   return rc;
 }
 
 
 // Error logging, static
 VXIlogResult
 SBjsiLogger::Error (VXIlogInterface *log, const VXIchar *moduleName,
 		     VXIunsigned errorID, const VXIchar *format, ...)
 {
   va_list arguments;
 
   if ( ! log )
     return VXIlog_RESULT_FAILURE;
   
   VXIlogResult rc;
   if ( format ) {
     va_start(arguments, format);
     rc = (*log->VError)(log, moduleName, errorID, format, arguments);
     va_end(arguments);
   } else {
     rc = (*log->Error)(log, moduleName, errorID, NULL);
   }
 
   return rc;
 }
 
 
 // Diagnostic logging
 VXIlogResult 
 SBjsiLogger::Diag (VXIunsigned tag, const VXIchar *subtag, 
 		   const VXIchar *format, ...) const
 {
   va_list arguments;
 
   if ( ! _log )
     return VXIlog_RESULT_FAILURE;
 
   VXIlogResult rc;
   if ( format ) {
     va_start(arguments, format);
     rc = (*_log->VDiagnostic)(_log, tag + _diagTagBase, subtag, format, 
 			      arguments);
     va_end(arguments);
   } else {
     rc = (*_log->Diagnostic)(_log, tag + _diagTagBase, subtag, NULL);
   }
 
   return rc;
 }
 
 
 // Diagnostic logging
 VXIlogResult 
 SBjsiLogger::Diag (VXIlogInterface *log, VXIunsigned tag, 
 		    const VXIchar *subtag, const VXIchar *format, ...) const
 {
   va_list arguments;
 
   if ( ! log )
     return VXIlog_RESULT_FAILURE;
 
   VXIlogResult rc;
   if ( format ) {
     va_start(arguments, format);
     rc = (*log->VDiagnostic)(log, tag + _diagTagBase, subtag, format, 
 			     arguments);
     va_end(arguments);
   } else {
     rc = (*log->Diagnostic)(log, tag + _diagTagBase, subtag, NULL);
   }
 
   return rc;
 }
 
 
 // Diagnostic logging, static
 VXIlogResult 
 SBjsiLogger::Diag (VXIlogInterface *log, VXIunsigned diagTagBase,
 		    VXIunsigned tag, const VXIchar *subtag, 
 		    const VXIchar *format, ...)
 {
   va_list arguments;
 
   if ( ! log )
     return VXIlog_RESULT_FAILURE;
 
   VXIlogResult rc;
   if ( format ) {
     va_start(arguments, format);
     rc = (*log->VDiagnostic)(log, tag + diagTagBase, subtag, format, 
 			     arguments);
     va_end(arguments);
   } else {
     rc = (*log->Diagnostic)(log, tag + diagTagBase, subtag, NULL);
   }
 
   return rc;
 }
 
 
 // Diagnostic logging, va args
 VXIlogResult 
 SBjsiLogger::VDiag (VXIunsigned tag, const VXIchar *subtag, 
 		     const VXIchar *format, va_list args) const
 {
   if ( ! _log )
     return VXIlog_RESULT_FAILURE;
 
   VXIlogResult rc;
   if ( format ) {
     rc = (*_log->VDiagnostic)(_log, tag + _diagTagBase, subtag, format, args);
   } else {
     rc = (*_log->Diagnostic)(_log, tag + _diagTagBase, subtag, NULL);
   }
 
   return rc;
 }
