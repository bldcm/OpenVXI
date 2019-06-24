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
 * Error message logging to files.
 *
 ***********************************************************************/

#include "SimpleLogger.hpp"
#include "VXIlog.h"
#include <sstream>

static unsigned int MESSAGE_BASE;
static const VXIchar * const MODULE_NAME  = COMPANY_DOMAIN L".vxi";

// ------*---------*---------*---------*---------*---------*---------*---------

static const VXIchar * const TAG_ENCODING  = L"encoding";
static const VXIchar * const TAG_EXCEPTION = L"exception";
static const VXIchar * const TAG_MESSAGE   = L"message";
static const VXIchar * const TAG_RECORDING = L"recording";

static const VXIchar * const TAG_LABEL     = L"label";
static const VXIchar * const TAG_EXPR      = L"expr";
static const VXIchar * const TAG_CONTENT   = L"content";

inline const VXIchar * const GetInfoTagText(SimpleLogger::InfoTag t)
{
  switch (t) {
  case SimpleLogger::ENCODING:    return TAG_ENCODING;
  case SimpleLogger::EXCEPTION:   return TAG_EXCEPTION;
  case SimpleLogger::MESSAGE:     return TAG_MESSAGE;
  case SimpleLogger::RECORDING:   return TAG_RECORDING;
  default:                        return TAG_MESSAGE;
  }
}

// ------*---------*---------*---------*---------*---------*---------*---------

class SimpleLoggerImpl : public SimpleLogger {
public:
  SimpleLoggerImpl(VXIlogInterface * l) : log(l)
  {
    isVectorSupported = LOG_EVENT_VECTOR_SUPPORTED(l);
    isContentSupported = LOG_CONTENT_METHODS_SUPPORTED(l);
  }

  ~SimpleLoggerImpl() { }

  // Diagnostic...
  typedef unsigned int TAGID;

  virtual bool IsLogging(TAGID tagID) const
  { return (log->DiagnosticIsEnabled(log, MESSAGE_BASE + tagID) == TRUE); }

  virtual std::basic_ostream<wchar_t> & StartDiagnostic(TAGID tagID) const
  { 
    id = tagID;
    buffer.str(L"");
    return buffer; }

  virtual void EndDiagnostic() const
  { if (buffer.str().empty()) return;
    log->Diagnostic(log, MESSAGE_BASE + id, L"", buffer.str().c_str()); }

  virtual void LogDiagnostic(TAGID tagID, const wchar_t * text) const
  { log->Diagnostic(log, MESSAGE_BASE + tagID, L"" , text); }
    
  virtual void LogDiagnostic(TAGID tagID, const wchar_t* subtag,
                             const wchar_t * format, ...)
  {
    va_list args;
    va_start(args, format);
    log->VDiagnostic(log, MESSAGE_BASE + tagID, subtag, format, args);
    va_end(args);  
  }

  // Error

  virtual void LogError(int errorNum,
                        const wchar_t * key, const wchar_t * txt) const
  { log->Error(log, MODULE_NAME, errorNum, L"%s%s", key, txt); }

  virtual void LogError(int errorNum,
                        SimpleLogger::InfoTag i1, const wchar_t * txt1,
                        SimpleLogger::InfoTag i2, const wchar_t * txt2) const
  { log->Error(log, MODULE_NAME, errorNum, L"%s%s%s%s", 
               GetInfoTagText(i1), txt1, GetInfoTagText(i2), txt2); }

  virtual void LogError(int errorNum,
                        SimpleLogger::InfoTag i, const wchar_t * txt) const
  { log->Error(log, MODULE_NAME, errorNum, L"%s%s", GetInfoTagText(i), txt); }

  virtual void LogError(int errorNum) const
  { log->Error(log, MODULE_NAME, errorNum, NULL); }

  // Event
  virtual void LogEvent(int eventNum, const VXIVector * keys,
                        const VXIVector * values) const
  { if (isVectorSupported)
      log->EventVector(log, eventNum, keys, values); }

  // Content logging.
  virtual bool LogContent(const VXIchar *mimetype, 
			  const VXIbyte *content, 
			  VXIulong size, 
			  VXIString **key, 
			  VXIString **value) const;

private:
  VXIlogInterface * log;
  mutable TAGID id;
  mutable std::basic_ostringstream<wchar_t> buffer;
  
  bool isVectorSupported;
  bool isContentSupported;
};


bool SimpleLoggerImpl::LogContent(const VXIchar *mimetype, 
				  const VXIbyte *content, 
				  VXIulong size, 
				  VXIString **key, 
				  VXIString **value) const 
{
  VXIlogResult ret = VXIlog_RESULT_SUCCESS;
  *key = NULL;
  *value = NULL;
  
  // Only dump the page if content logging is supported by the log
  // implementation
  if (isContentSupported) {
    VXIlogStream *stream = NULL;
    ret = log->ContentOpen(log, MODULE_NAME, mimetype, key, value, &stream);
    if (ret == VXIlog_RESULT_SUCCESS) {
      VXIulong nwrite = 0;
      ret = log->ContentWrite(log, content, size, &nwrite, stream);
      log->ContentClose(log, &stream);
    }
    
    if (ret != VXIlog_RESULT_SUCCESS)
      LogError(990);
  } else {
    ret = VXIlog_RESULT_FAILURE;
  }
  
  return (ret == VXIlog_RESULT_SUCCESS ? true : false);
}


SimpleLogger * SimpleLogger::CreateResource(VXIlogInterface * l)
{
  return new SimpleLoggerImpl(l);
}


void SimpleLogger::DestroyResource(SimpleLogger * & log)
{
  if (log == NULL) return;
  delete log;
  log = NULL;
}


void SimpleLogger::SetMessageBase(unsigned int base)
{
  MESSAGE_BASE = base;
}
