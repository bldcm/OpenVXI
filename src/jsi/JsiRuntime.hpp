/*****************************************************************************
  *****************************************************************************
  *
  * $Id: JsiRuntime.hpp,v 1.9.2.2 2003/10/06 19:11:33 mpanacci Exp $
  *
  * JsiRuntime, class for managing JavaScript runtime environments
  *
  * The JsiRuntime class represents a JavaScript interpreter runtime
  * environment, which is the overall JavaScript engine execution space
  * (compiler, interpreter, decompiler, garbage collector, etc.) which
  * can hold zero or more JavaScript contexts (JsiContext objects).
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
 

 // -----1=0-------2=0-------3=0-------4=0-------5=0-------6=0-------7=0-------8
 
 #ifndef _JSI_RUNTIME_H__
 #define _JSI_RUNTIME_H__
 
 #include "VXIjsi.h"              // For VXIjsiResult codes
 #include "SBjsiLogger.hpp"      // For SBjsiLogger base class
 
 #ifndef HAVE_SPIDERMONKEY
 #error Need Mozilla SpiderMonkey to build this ECMAScript integration
 #endif
 #include <jspubtd.h>             // SpiderMonkey JavaScript typedefs (JS...)
 
 #ifndef JS_DLL_CALLBACK
 #define JS_DLL_CALLBACK CRT_CALL // For SpiderMonkey 1.5 RC 3 and earlier
 #endif
 
 #ifndef JS_THREADSAFE
 extern "C" struct VXItrdMutex;
 #endif
 
 extern "C" struct VXIlogInterface;
 
 class JsiRuntime : SBjsiLogger {
  public:
   // Constructor and destructor
   JsiRuntime( );
   virtual ~JsiRuntime( );
 
   // Creation method
   VXIjsiResult Create (long              runtimeSize,
 		       VXIlogInterface  *log,
 		       VXIunsigned       diagTagBase);
   
  public:
   // These are only for JsiContext use!
 
   // Get a new JavaScript context for the runtime
   VXIjsiResult NewContext (long contextSize, JSContext **context);
 
   // Destroy a JavaScript context for the runtime
   VXIjsiResult DestroyContext (JSContext **context);
 
 #ifndef JS_THREADSAFE
   // Flag that we are beginning and ending access of this runtime
   // (including any access of a context within this runtime), used to
   // ensure thread safety. For simplicity, return true on success,
   // false on failure (mutex error).
   bool AccessBegin( ) const;
   bool AccessEnd( ) const;
 #endif
   
  private:
   // Static callback for diagnostic logging of garbage collection
   static JSBool JS_DLL_CALLBACK GCCallback (JSContext *cx, JSGCStatus status);
 
   // Disable the copy constructor and assignment operator
   JsiRuntime (const JsiRuntime &rt);
   JsiRuntime & operator= (const JsiRuntime &rt);
 
  private:
   VXIlogInterface  *log;         // For logging
   JSRuntime        *runtime;     // JavaScript runtime environment
 
 #ifndef JS_THREADSAFE
   VXItrdMutex      *mutex;       // For thread safe evals
 #endif
 };
 
 #endif  // _JSI_RUNTIME_H__
