
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
 
/***********************************************************************
  *
  * Top level API implementation and VXI class methods
  *
  ***********************************************************************/
 
 #define VXI_EXPORTS
 #include "VXI.hpp"
 #include "VXIinterpreter.h"
 #include "SimpleLogger.hpp"
 #include "DocumentParser.hpp"
 #include "VXIjsi.h"
 #include "VXIcache.h"                 // for version check on interface
 
 /******************************************************************
  * Init / ShutDown
  ******************************************************************/
 
 static unsigned int DEFAULT_DOCUMENT_CACHE_SIZE = 1024; // 1024 kB = 1 MB
 
 // VXIinterp_RESULT_SUCCESS
 // VXIinterp_RESULT_FAILURE
 VXI_INTERPRETER 
 VXIinterpreterResult VXIinterpreterInit(VXIlogInterface * log,
 					VXIunsigned       diagLogBase,
                                         const VXIMap    * props)
 {
   SimpleLogger::SetMessageBase(diagLogBase);
 
   unsigned int cacheSize = DEFAULT_DOCUMENT_CACHE_SIZE;
   if (props != NULL) {
     const VXIValue * v = VXIMapGetProperty(props, VXI_DOC_MEMORY_CACHE);
     if (v != NULL && VXIValueGetType(v) == VALUE_INTEGER)
       cacheSize = VXIIntegerValue(reinterpret_cast<const VXIInteger *>(v));
   }
 
   if (!DocumentParser::Initialize(cacheSize))
     return VXIinterp_RESULT_FAILURE;
 
   return VXIinterp_RESULT_SUCCESS;
 }
 
 
 VXI_INTERPRETER void VXIinterpreterShutDown(VXIlogInterface  *log)
 {
   DocumentParser::Deinitialize();
 }
 
 
 /******************************************************************
  * Interface definition
  ******************************************************************/
 
 // This is an interesting and legal hack.  The alignment of a structure in C
 // must follow that of the first member.  The result is that the Interpreter
 // structure has the same alignment as VXIinterpreterInterface.  The
 // implementation may be safely appended onto the end.
 
 extern "C" {
 typedef struct Interpreter {
   VXIinterpreterInterface interface;
   SimpleLogger * logger;
   VXIcacheInterface  * cache;
   VXIinetInterface * inet;
   VXIjsiInterface * jsi;
   VXIobjectInterface * object;
   VXIpromptInterface * prompt;
   VXIrecInterface * rec;
   VXItelInterface * tel;
   VXI * implementation;
 } Interpreter;
 }
 
 
 extern "C" VXIint32 VXIinterpreterGetVersion(void)
 {
   return VXI_CURRENT_VERSION;
 }
 
 
 extern "C" const VXIchar* VXIinterpreterGetImplementationName(void)
 {
 #ifndef COMPANY_DOMAIN
 #define COMPANY_DOMAIN L"com.yourcompany"
 #endif
   static const VXIchar IMPLEMENTATION_NAME[] = 
     COMPANY_DOMAIN L".VXI";
   return IMPLEMENTATION_NAME;
 }
 
 
 extern "C" VXIinterpreterResult VXIinterpreterRun(VXIinterpreterInterface *x,
 						  const VXIchar *name,
 						  const VXIchar *sessionArgs,
 						  VXIValue **result)
 {
   if (x == NULL) return VXIinterp_RESULT_INVALID_ARGUMENT;
   Interpreter * interpreter = reinterpret_cast<Interpreter*>(x);
 
   switch(interpreter->implementation->Run(name, sessionArgs,
                                           interpreter->logger,
                                           interpreter->inet,
                                           interpreter->cache,
                                           interpreter->jsi,
                                           interpreter->rec,
                                           interpreter->prompt,
                                           interpreter->tel,
                                           interpreter->object,
                                           result))
   {
   case -2: // Fatal error
     return VXIinterp_RESULT_FATAL_ERROR;
   case -1: // Out of memory
     return VXIinterp_RESULT_OUT_OF_MEMORY;
   case  0: // Success
     return VXIinterp_RESULT_SUCCESS;
   case  1: // Infinite loop suspected.
   case  2: // ECMAScript error
     return VXIinterp_RESULT_FAILURE;
   case  3: // Invalid startup documents
     return VXIinterp_RESULT_INVALID_DOCUMENT;
   case  4:  // Stopped
     return VXIinterp_RESULT_STOPPED;
   default:
     return VXIinterp_RESULT_FATAL_ERROR;
   }
 }
 
 
 extern "C" VXIinterpreterResult VXIinterpreterSetProperties(
                                                  VXIinterpreterInterface * x,
                                                  const VXIMap * props)
 {
   if (x == NULL) return VXIinterp_RESULT_INVALID_ARGUMENT;
   Interpreter * interpreter = reinterpret_cast<Interpreter*>(x);
 
   // Handle the degenerate case.
   if (VXIMapNumProperties(props) == 0) return VXIinterp_RESULT_SUCCESS;
 
   // Walk through each property in the map.
   const VXIchar  * key;
   const VXIValue * value;
   VXIMapIterator * i = VXIMapGetFirstProperty(props, &key, &value);
   do {
     if (key == NULL) return VXIinterp_RESULT_INVALID_PROP_NAME;
     if (value == NULL) return VXIinterp_RESULT_INVALID_PROP_VALUE;
 
     VXI * vxi = interpreter->implementation;
 
     // Only strings and integers are supported.
     if (VXIValueGetType(value) == VALUE_STRING) {
       const VXIchar * valueString
 	= VXIStringCStr(reinterpret_cast<const VXIString *>(value));
 
       vxistring keyString(key);
       
       // Translate property names.
       bool error = false;
       if (keyString == VXI_BEEP_AUDIO)
 	error |= !(vxi->SetRuntimeProperty(VXI::BeepURI, valueString));
       else if (keyString == VXI_PLATFORM_DEFAULTS)
 	error |= !(vxi->SetRuntimeProperty(VXI::PlatDefaultsURI, valueString));
       else
 	return VXIinterp_RESULT_INVALID_PROP_NAME;
 
       if (error) return VXIinterp_RESULT_INVALID_PROP_VALUE;
 
     } else if (VXIValueGetType(value) == VALUE_INTEGER) {
       vxistring keyString(key);
       
       // Translate property names.
       if (keyString == L"vxi.property.maxLoopIterations" ||
           keyString == L"vxi.property.maxDocuments" ||
           keyString == L"vxi.property.maxExeStackDepth")
       {
 #pragma message("Display log message here")
       }
       else
 	return VXIinterp_RESULT_INVALID_PROP_NAME;
     } else {
       return VXIinterp_RESULT_INVALID_PROP_NAME;
     }
 
   } while (VXIMapGetNextProperty(i, &key, &value) == VXIvalue_RESULT_SUCCESS);
 
   VXIMapIteratorDestroy(&i);
 
   return VXIinterp_RESULT_SUCCESS;
 }
 
 
 extern "C" VXIinterpreterResult VXIinterpreterValidate(
                                                  VXIinterpreterInterface *x,
                                                  const VXIchar *name)
 {
   if (x == NULL) return VXIinterp_RESULT_INVALID_ARGUMENT;
   if (name == NULL) return VXIinterp_RESULT_FETCH_ERROR;
 
   Interpreter * interpreter = reinterpret_cast<Interpreter*>(x);
 
   DocumentParser parser;
   VXIMapHolder properties;
   VXMLDocument document;
   VXIMapHolder docProperties;
 
   switch (parser.FetchDocument(name, properties, interpreter->inet,
                                interpreter->cache, *interpreter->logger,
                                document, docProperties))
   {
   case -1: // Out of memory?
     return VXIinterp_RESULT_OUT_OF_MEMORY;
   case  0: // Success
     return VXIinterp_RESULT_SUCCESS;
   case  2: // Unable to open URI
   case  3: // Unable to read from URI
     return VXIinterp_RESULT_FETCH_ERROR;
   case  4: // Unable to parse contents of URI
     return VXIinterp_RESULT_FAILURE;
   case  1: // Invalid parameter
   default:
     break;
   }
 
   return VXIinterp_RESULT_FATAL_ERROR;
 }
 
 
 extern "C" VXIinterpreterResult
 VXIinterpreterRequestStop(VXIinterpreterInterface *x, VXIbool doStop)
 {
   if (x == NULL) return VXIinterp_RESULT_INVALID_ARGUMENT;
   Interpreter * interpreter = reinterpret_cast<Interpreter*>(x);
 
   interpreter->implementation->DeclareStopRequest(doStop == TRUE);
 
   return VXIinterp_RESULT_SUCCESS;
 }
 
 
 extern "C" VXIinterpreterResult VXIinterpreterInsertEvent(
                                                     VXIinterpreterInterface *x,
                                                     const VXIchar * event,
                                                     const VXIchar * message)
 {
   if (x == NULL) return VXIinterp_RESULT_INVALID_ARGUMENT;
   if (event == NULL) return VXIinterp_RESULT_INVALID_ARGUMENT;
 
   Interpreter * interpreter = reinterpret_cast<Interpreter*>(x);
 
   switch(interpreter->implementation->DeclareExternalEvent(event, message)) {
   case  0: // Success
     return VXIinterp_RESULT_SUCCESS;
   case  1: // Invalid event name
     return VXIinterp_RESULT_INVALID_ARGUMENT;
   default:
     break;
   }
 
   return VXIinterp_RESULT_FATAL_ERROR;
 }
 
 
 extern "C" VXIinterpreterResult VXIinterpreterClearEventQueue(
                                                     VXIinterpreterInterface *x)
 {
   if (x == NULL) return VXIinterp_RESULT_INVALID_ARGUMENT;
 
   Interpreter * interpreter = reinterpret_cast<Interpreter*>(x);
 
   switch(interpreter->implementation->ClearExternalEventQueue()) {
   case  0: // Success
     return VXIinterp_RESULT_SUCCESS;
   case  1: // Invalid event name
     return VXIinterp_RESULT_INVALID_ARGUMENT;
   default:
     break;
   }
 
   return VXIinterp_RESULT_FATAL_ERROR;
 }
 
 /******************************************************************
  * Resource creation & destruction
  ******************************************************************/
 
 // VXIinterp_RESULT_OUT_OF_MEMORY
 // VXIinterp_RESULT_SUCCESS
 VXI_INTERPRETER
 VXIinterpreterResult VXIinterpreterCreateResource(VXIresources *resources,
                                                   VXIinterpreterInterface ** v)
 {
   if (resources == NULL || resources->inet == NULL || 
       resources->log == NULL || v == NULL)
     return VXIinterp_RESULT_INVALID_ARGUMENT;
 
   Interpreter * interpreter = new Interpreter;
   if (interpreter == NULL)
     return VXIinterp_RESULT_OUT_OF_MEMORY;
 
   interpreter->implementation = new VXI();
   if (interpreter->implementation == NULL) {
     delete interpreter;
     return VXIinterp_RESULT_OUT_OF_MEMORY;
   }
 
   interpreter->logger = SimpleLogger::CreateResource(resources->log);
   if (interpreter->logger == NULL) {
     delete interpreter->implementation;
     delete interpreter;
     return VXIinterp_RESULT_OUT_OF_MEMORY;
   }
 
   interpreter->interface.GetVersion = VXIinterpreterGetVersion;
   interpreter->interface.GetImplementationName = 
     VXIinterpreterGetImplementationName;
   interpreter->interface.Run = VXIinterpreterRun;
   interpreter->interface.Validate = VXIinterpreterValidate;
   interpreter->interface.SetProperties = VXIinterpreterSetProperties;
   interpreter->interface.RequestStop = VXIinterpreterRequestStop;
   interpreter->interface.InsertEvent = VXIinterpreterInsertEvent;
   interpreter->interface.ClearEventQueue = VXIinterpreterClearEventQueue;
 
   // The VXI requires a 1.1 cache interface version.
   if (resources->cache != NULL && resources->cache->GetVersion() >= 0x00010001)
     interpreter->cache = resources->cache;
   else
     interpreter->cache = NULL;
 
   interpreter->inet   = resources->inet;
   interpreter->jsi    = resources->jsi;
   interpreter->object = resources->object;
   interpreter->prompt = resources->prompt;
   interpreter->rec    = resources->rec;
   interpreter->tel    = resources->tel;
 
   *v = reinterpret_cast<VXIinterpreterInterface*>(interpreter);
   return VXIinterp_RESULT_SUCCESS;
 }
 
 
 VXI_INTERPRETER
 void VXIinterpreterDestroyResource(VXIinterpreterInterface ** v)
 {
   if (v == NULL || *v == NULL) return;
   Interpreter * interpreter = reinterpret_cast<Interpreter*>(*v);
   delete interpreter->implementation;
   SimpleLogger::DestroyResource(interpreter->logger);
   delete interpreter;
   *v = NULL;
 }
