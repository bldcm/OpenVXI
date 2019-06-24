#ifndef _CommonExceptions_H
 #define _CommonExceptions_H

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
  * Common exceptions used by components in the VXI.
  *
  ***********************************************************************/
 
 #include "VXIvalue.h"
 #include <string>
 #include "DocumentModel.hpp"
 
 typedef std::basic_string<VXIchar> vxistring;
 
 class VXIException {
 public:
   class OutOfMemory { };
 
   class JavaScriptError { };
 
   class StopRequest { };
 
   class InterpreterEvent {
   public:
     InterpreterEvent(const vxistring & v, const vxistring & m = vxistring(), 
                      const VXMLElement & adialog = 0)
       : val(v), message(m), activeDialog(adialog)  { }
     ~InterpreterEvent()                             { }
 
     const vxistring & GetValue() const              { return val; }
     const vxistring & GetMessage() const            { return message; }
     const VXMLElement & GetActiveDialog() const     { return activeDialog; }
     
   private:
     vxistring val;
     vxistring message;
     VXMLElement activeDialog;
   };
 
   class Fatal { };
 
   class Exit {
   public:
     Exit(VXIValue * v) : exprResult(v) { }
 
     VXIValue * exprResult;
   };
 
 private: // unimplemented
   VXIException();
   ~VXIException();
 };
 
 #endif
