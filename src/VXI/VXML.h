#ifndef _VXML_H
 #define _VXML_H

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
 

 #include "VXItypes.h"
 
 enum VXMLElementType {
   NODE_ASSIGN,
   NODE_AUDIO,
   NODE_BLOCK,
   NODE_CANCEL,
   NODE_CATCH,
   NODE_CHOICE,
   NODE_CLEAR,
   NODE_DISCONNECT,
   NODE_ELSE,
   NODE_ELSEIF,
   NODE_ENUMERATE,
   NODE_EXIT,
   NODE_FIELD,
   NODE_FILLED,
   NODE_FORM,
   NODE_GOTO,
   NODE_GRAMMAR,
   NODE_IF,
   NODE_INITIAL,
   NODE_LINK,
   NODE_LOG,
   NODE_MENU,
   NODE_META,
   NODE_OBJECT,
   NODE_OPTION,
   NODE_PARAM,
   NODE_PROMPT,
   NODE_PROPERTY,
   NODE_RECORD,
   NODE_RETURN,
   NODE_REPROMPT,
   NODE_SCRIPT,
   NODE_SUBDIALOG,
   NODE_SUBMIT,
   NODE_THROW,
   NODE_TRANSFER,
   NODE_VALUE,
   NODE_VAR,
   NODE_VXML,
 
   DEFAULTS_ROOT,
   DEFAULTS_LANGUAGE
 };
 
 
 enum VXMLAttributeType {
   ATTRIBUTE__ITEMNAME,
   ATTRIBUTE_AAI,
   ATTRIBUTE_AAIEXPR,
   ATTRIBUTE_ACCEPT,
   ATTRIBUTE_APPLICATION,
   ATTRIBUTE_ARCHIVE,
   ATTRIBUTE_AUDIOBASE,
   ATTRIBUTE_BARGEIN,
   ATTRIBUTE_BARGEINTYPE,
   ATTRIBUTE_BASE,
   ATTRIBUTE_BEEP,
   ATTRIBUTE_BRIDGE,
   ATTRIBUTE_CHARSET,
   ATTRIBUTE_CLASSID,
   ATTRIBUTE_CODEBASE,
   ATTRIBUTE_CODETYPE,
   ATTRIBUTE_COND,
   ATTRIBUTE_CONNECTTIME,
   ATTRIBUTE_CONTENT,
   ATTRIBUTE_COUNT,
   ATTRIBUTE_DATA,
   ATTRIBUTE_DEST,
   ATTRIBUTE_DESTEXPR,
   ATTRIBUTE_DTMF,
   ATTRIBUTE_DTMFTERM,
   ATTRIBUTE_ENCTYPE,
   ATTRIBUTE_EVENT,
   ATTRIBUTE_EVENTEXPR,
   ATTRIBUTE_EXPR,
   ATTRIBUTE_EXPRITEM,
   ATTRIBUTE_FETCHAUDIO,
   ATTRIBUTE_FETCHHINT,
   ATTRIBUTE_FETCHTIMEOUT,
   ATTRIBUTE_FINALSILENCE,
   ATTRIBUTE_HTTP_EQUIV,
   ATTRIBUTE_ID,
   ATTRIBUTE_LABEL,
   ATTRIBUTE_MAXAGE,
   ATTRIBUTE_MAXSTALE,
   ATTRIBUTE_MAXTIME,
   ATTRIBUTE_MESSAGE,
   ATTRIBUTE_MESSAGEEXPR,
   ATTRIBUTE_METHOD,
   ATTRIBUTE_MODAL,
   ATTRIBUTE_MODE,
   ATTRIBUTE_NAME,
   ATTRIBUTE_NAMELIST,
   ATTRIBUTE_NEXT,
   ATTRIBUTE_NEXTITEM,
   ATTRIBUTE_ROOT,
   ATTRIBUTE_SCOPE,
   ATTRIBUTE_SLOT,
   ATTRIBUTE_SRC,
   ATTRIBUTE_SRCEXPR,
   ATTRIBUTE_TAGFORMAT,
   ATTRIBUTE_TIMEOUT,
   ATTRIBUTE_TRANSFERAUDIO,
   ATTRIBUTE_TYPE,
   ATTRIBUTE_VALUE,
   ATTRIBUTE_VALUETYPE,
   ATTRIBUTE_VERSION,
   ATTRIBUTE_WEIGHT,
   ATTRIBUTE_XMLLANG
 };
 
 
 typedef const VXIchar * const VXML_SYMBOL;
 
 // Standard Event definitions
 
 static VXML_SYMBOL EV_CANCEL             = L"cancel";
 static VXML_SYMBOL EV_TELEPHONE_HANGUP   = L"connection.disconnect.hangup";
 static VXML_SYMBOL EV_TELEPHONE_TRANSFER = L"connection.disconnect.transfer";
 static VXML_SYMBOL EV_EXIT               = L"exit";
 static VXML_SYMBOL EV_HELP               = L"help";
 static VXML_SYMBOL EV_NOINPUT            = L"noinput";
 static VXML_SYMBOL EV_NOMATCH            = L"nomatch";
 static VXML_SYMBOL EV_MAXSPEECH          = L"maxspeechtimeout";
 
 static VXML_SYMBOL EV_ERROR_BADFETCH     = L"error.badfetch";
 static VXML_SYMBOL EV_ERROR_BADURI       = L"error.badfetch.baduri";
 static VXML_SYMBOL EV_ERROR_BADDIALOG    = L"error.badfetch.baddialog";
 static VXML_SYMBOL EV_ERROR_APP_BADURI   = L"error.badfetch.applicationuri";
 
 static VXML_SYMBOL EV_ERROR_SEMANTIC     = L"error.semantic";
 static VXML_SYMBOL EV_ERROR_ECMASCRIPT   = L"error.semantic.ecmascript";
 static VXML_SYMBOL EV_ERROR_RECORD_PARAM = L"error.semantic.recordparameter";
 static VXML_SYMBOL EV_ERROR_BAD_THROW    = L"error.semantic.no_event_in_throw";
 static VXML_SYMBOL EV_ERROR_NO_GRAMMARS  = L"error.semantic.nogrammars";
 
 static VXML_SYMBOL EV_ERROR_NOAUTHORIZ   = L"error.noauthorization";
 static VXML_SYMBOL EV_ERROR_NORESOURCE   = L"error.noresource";
 
 static VXML_SYMBOL EV_UNSUPPORT_FORMAT   = L"error.unsupported.format";
 static VXML_SYMBOL EV_UNSUPPORT_OBJECT   = L"error.unsupported.objectname";
 static VXML_SYMBOL EV_UNSUPPORT_LANGUAGE = L"error.unsupported.language";
 static VXML_SYMBOL EV_UNSUPPORT_TRANSFER = L"error.unsupported.transfer";
 static VXML_SYMBOL EV_UNSUPPORT_RECORD_M = L"error.unsupported.record.modal";
 static VXML_SYMBOL EV_UNSUPPORT_BUILTIN  = L"error.unsupported.builtin";
 
 static VXML_SYMBOL EV_TELEPHONE_NOAUTHORIZ = L"error.connection.noauthorization";
 static VXML_SYMBOL EV_TELEPHONE_BAD_DEST = L"error.connection.baddestination";
 static VXML_SYMBOL EV_TELEPHONE_NOROUTE  = L"error.connection.noroute";
 static VXML_SYMBOL EV_TELEPHONE_NORESOURCE = L"error.connection.noresource";
 static VXML_SYMBOL EV_TELEPHONE_UNSUPPORT_BLIND = L"error.unsupported.transfer.blind";
 static VXML_SYMBOL EV_TELEPHONE_UNSUPPORT_BRIDGE = L"error.unsupported.transfer.bridge";
 static VXML_SYMBOL EV_TELEPHONE_UNSUPPORT_URI = L"error.unsupported.uri";
  
 // Outside VXML specification
 
 static VXML_SYMBOL EV_ERROR_BAD_GRAMMAR  = L"error.grammar";
 static VXML_SYMBOL EV_ERROR_BAD_INLINE   = L"error.grammar.inlined";
 static VXML_SYMBOL EV_ERROR_BAD_CHOICE   = L"error.grammar.choice";
 static VXML_SYMBOL EV_ERROR_BAD_OPTION   = L"error.grammar.option";
 static VXML_SYMBOL EV_ERROR_RECOGNITION  = L"error.recognition";
 static VXML_SYMBOL EV_ERROR_RECORD       = L"error.record";
 static VXML_SYMBOL EV_ERROR_TRANSFER     = L"error.transfer";
 static VXML_SYMBOL EV_ERROR_STACK_OVERFLOW = L"error.stack_overflow";
 static VXML_SYMBOL EV_ERROR_LOOP_COUNT   = L"error.max_loop_count_exceeded";
 static VXML_SYMBOL EV_ERROR_OBJECT       = L"error.object";
 
 // Property names
 
 static const VXIchar * const PROP_BARGEIN        = L"bargein";
 static const VXIchar * const PROP_BARGEINTYPE    = L"bargeintype";
 static const VXIchar * const PROP_COMPLETETIME   = L"completetimeout";
 static const VXIchar * const PROP_CONFIDENCE     = L"confidencelevel";
 static const VXIchar * const PROP_INCOMPLETETIME = L"incompletetimeout";
 static const VXIchar * const PROP_INPUTMODES     = L"inputmodes";
 static const VXIchar * const PROP_INTERDIGITTIME = L"interdigittimeout";
 static const VXIchar * const PROP_MAXNBEST       = L"maxnbest";
 static const VXIchar * const PROP_SENSITIVITY    = L"sensitivity";
 static const VXIchar * const PROP_SPEEDVSACC     = L"speedvsaccuracy";
 static const VXIchar * const PROP_TERMCHAR       = L"termchar";
 static const VXIchar * const PROP_TERMTIME       = L"termtimeout";
 static const VXIchar * const PROP_TIMEOUT        = L"timeout";
 static const VXIchar * const PROP_UNIVERSALS     = L"universals";
 
 #endif
