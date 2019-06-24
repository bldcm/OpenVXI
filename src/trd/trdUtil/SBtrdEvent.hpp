/* SBtrdEvent, utility class for managing events */

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
 

 /* $Id: SBtrdEvent.hpp,v 7.2.2.3 2003/10/06 17:59:33 mpanacci Exp $ */
 
 // -----1=0-------2=0-------3=0-------4=0-------5=0-------6=0-------7=0-------8
 
 #ifndef _SBTRD_EVENT_H__
 #define _SBTRD_EVENT_H__
 
 #include "VXIheaderPrefix.h"          // For SYMBOL_EXPORT_CPP_DECL
 #include "VXItrd.h"                   // For VXItrdTimer, VXItrdMutex
 
 #ifdef SBTRDUTIL_EXPORTS
 #define SBTRDUTIL_API_CLASS SYMBOL_EXPORT_CPP_CLASS_DECL
 #else
 #define SBTRDUTIL_API_CLASS SYMBOL_IMPORT_CPP_CLASS_DECL
 #endif
 
 extern "C" struct VXIlogInterface;
 
 class SBTRDUTIL_API_CLASS SBtrdEvent {
  public:
   // Constructor and destructor
   SBtrdEvent (VXIlogInterface *log = NULL, VXIunsigned diagTagBase = 0) :
     _diagTagBase(diagTagBase), _log(log), _timer(NULL), _sleepMutex(NULL),
     _alerted(FALSE) { }
   virtual ~SBtrdEvent( );
 
   // Creation method
   VXItrdResult Create( );
 
   // Reset the event
   VXItrdResult Reset( );
 
   // Set the event
   VXItrdResult Set( );
 
   // Wait on the event
   VXItrdResult Wait( );
 
  private:
   // Error logging
   void Error (VXIunsigned errorID, const VXIchar *format, ...) const;
 
   // Diagnostic logging
   void Diag (VXIunsigned tag, const VXIchar *subtag, 
 	     const VXIchar *format, ...) const;
 
   // Disable the copy constructor and equality operator to catch making
   // copies at compile or link time, neither is really implemented
   SBtrdEvent (const SBtrdEvent &);
   SBtrdEvent & operator= (const SBtrdEvent &);
 
  private:
   VXIunsigned       _diagTagBase;
   VXIlogInterface  *_log;
   VXItrdTimer      *_timer;
   VXItrdMutex      *_sleepMutex;
   volatile VXIbool  _alerted;
 };
 
 #include "VXIheaderSuffix.h"
 
 #endif  // _SBTRD_EVENT_H__
