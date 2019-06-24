
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
 

 #ifndef __ANSWER_PARSER_HPP__
 #define __ANSWER_PARSER_HPP__
 
 #include "VXItypes.h"
 #include <string>
 #include <parsers/XercesDOMParser.hpp>
 
 extern "C" struct VXIContent;
 extern "C" struct VXIValue;
 class SimpleLogger;
 class AnswerHolder;
 
 typedef std::basic_string<VXIchar> vxistring;
 
 class AnswerTranslator {
 public:
   virtual void EvaluateExpression(const vxistring & expression) = 0;
   virtual void SetString(const vxistring & var, const vxistring & val) = 0;
 
   AnswerTranslator() { }
   virtual ~AnswerTranslator() { }
 };
 
 
 class AnswerParser {
 public:
   int Parse(SimpleLogger & log, AnswerTranslator &, GrammarManager &, int, VXIContent *, vxistring &);
   // Returns: -3 Parse error
   //          -2 Unsupported content type
   //          -1 Invalid parameter - content was null or invalid
   //           0 Success - string contains the grammar id of the best match
   //           1 Success - noinput event
   //           2 Success - nomatch event
 
   AnswerParser();
   ~AnswerParser();
 
 private:
   int ProcessNLSML(SimpleLogger & log, AnswerTranslator &, GrammarManager &, int, vxistring &grammar);
   // Returns: -1 Parse error
   //           0 Success - application.lastresult$ written
   //           1 Success - noinput event
   //           2 Success - nomatch event
 
   bool IsAllWhiteSpace(const XMLCh* str);
    
   xercesc::XercesDOMParser * nlsmlParser;
 };
 
 #endif
