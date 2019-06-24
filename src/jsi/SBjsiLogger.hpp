/*****************************************************************************
  *****************************************************************************
  *
  * $Id: SBjsiLogger.hpp,v 1.1.2.2 2003/10/06 19:19:17 mpanacci Exp $
  *
  * SBjsiLogger - logging class for SBjsi
  *
  * This provides logging definitions for SBjsi use of VXIlog, along
  * with some convenience macros.
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
 

 /* -----1=0-------2=0-------3=0-------4=0-------5=0-------6=0-------7=0-------8
  */
 
 #ifndef _SBJSI_LOGGER_H__
 #define _SBJSI_LOGGER_H__
 
 #include "VXIheaderPrefix.h"          /* For SYMBOL_EXPORT_CPP_DECL */
 
 #include "VXIlog.h"          /* Logging engine */
 #include "SBjsiString.hpp"  /* For SBinetString */
 #include <stdarg.h>          /* For variable argument list */
 
 
 /* Base class for logging */
 class SBjsiLogger {
  protected:
   // Constructor and destructor
   SBjsiLogger (const VXIchar *moduleName, VXIlogInterface *log,
 		VXIunsigned diagTagBase) : 
     _moduleName(moduleName), _log(log), _diagTagBase(diagTagBase) { }
   virtual ~SBjsiLogger( ) { }
 
   // Set the log interface when unavailable at construct time
   void SetLog (VXIlogInterface *log, VXIunsigned diagTagBase) {
     _log = log; _diagTagBase = diagTagBase; }
 
  public:
   // Accessor
   VXIlogInterface *GetLog( ) { return _log; }
   VXIunsigned GetDiagBase( ) const { return _diagTagBase; }
 
   VXIbool DiagIsEnabled (VXIunsigned tag) const;
 
   // Logging methods
   VXIlogResult Error (VXIunsigned errorID, const VXIchar *format, ...) const;
   VXIlogResult Error (VXIlogInterface *log, VXIunsigned errorID, 
 		      const VXIchar *format, ...) const;
   static VXIlogResult Error (VXIlogInterface *log, const VXIchar *moduleName,
 			     VXIunsigned errorID, const VXIchar *format,
 			     ...);
 
   VXIlogResult Diag (VXIunsigned tag, const VXIchar *subtag, 
 		     const VXIchar *format, ...) const;
   VXIlogResult Diag (VXIlogInterface *log, VXIunsigned tag, 
 		     const VXIchar *subtag, const VXIchar *format,
 		     ...) const;
   static VXIlogResult Diag (VXIlogInterface *log, VXIunsigned diagTagBase,
 			    VXIunsigned tag, const VXIchar *subtag, 
 			    const VXIchar *format, ...);
 
   VXIlogResult VDiag (VXIunsigned tag, const VXIchar *subtag, 
 		      const VXIchar *format, va_list args) const;
 
  private:
   SBjsiString      _moduleName;
   VXIlogInterface  *_log;
   VXIunsigned       _diagTagBase;
 };
 
 
 /* Logging object for conveniently logging function entrance/exit */
 class SBjsiLogFunc {
  public:
   /* Constructor and destructor */
   SBjsiLogFunc (VXIlogInterface *log, VXIunsigned tag, const VXIchar *func, 
 		const int *rcvar, const VXIchar *format = NULL, ...) :
     _log(log), _tag(tag), _func(func), _rcvar(rcvar) {
     if ( _log ) {
       va_list ap;
       va_start (ap, format);
       _log->VDiagnostic (_log, _tag, _func, format, ap);
       va_end (ap);
     }
   }
 
   ~SBjsiLogFunc( ) {
     if ( _log ) {
       if ( _rcvar )
 	_log->Diagnostic (_log, _tag, _func, L"exiting, returned %d", *_rcvar);
       else
 	_log->Diagnostic (_log, _tag, _func, L"exiting");
     }
   }
 
  private:
   VXIlogInterface  *_log;
   VXIunsigned       _tag;
   const VXIchar    *_func;
   const int        *_rcvar;
 };
 
 #include "VXIheaderSuffix.h"
 
 #endif  /* define _SBJSI_LOGGER_H__ */
