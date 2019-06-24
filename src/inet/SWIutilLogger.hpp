/*****************************************************************************
  *****************************************************************************
  *
  * $Id: SWIutilLogger.hpp,v 1.1.6.1.6.3 2003/10/20 13:28:37 mpanacci Exp $
  *
  * SWIutilLogger - logging class for SWIutil
  *
  * This provides logging definitions for SWIutil use of VXIlog, along
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
 
 #ifndef _SWIUTIL_LOGGER_H__
 #define _SWIUTIL_LOGGER_H__
 
 #include "SWIutilHeaderPrefix.h"
 
 #include "VXIlog.h"          /* Logging engine */
 #include <stdarg.h>          /* For variable argument list */
 
 /* Base class for logging */
 class SWIUTIL_API_CLASS SWIutilLogger
 {
  protected:
   // Constructor and destructor
   SWIutilLogger(const VXIchar *moduleName, VXIlogInterface *log,
 		VXIunsigned diagTagBase);
   SWIutilLogger(const SWIutilLogger& logger);
   virtual ~SWIutilLogger();
 
   // Set the log interface when unavailable at construct time
   void SetLog(VXIlogInterface *log, VXIunsigned diagTagBase);
 
  public:
   // Accessor
   const VXIchar *GetModuleName() const;
 
   VXIlogInterface *GetLog() const;
   VXIunsigned GetDiagBase() const;
 
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
   VXIchar*          _moduleName;
   VXIlogInterface  *_log;
   VXIunsigned       _diagTagBase;
 
   // Disabled assignment operator.
   SWIutilLogger &operator=(const SWIutilLogger &logger);
 };
 
 
 #endif  /* define _SWIUTIL_LOGGER_H__ */
