#ifndef SWILOGGER_HPP
#define SWILOGGER_HPP

// Copyright 1998, 1999, 2000, 2001, 2002 SpeechWorks International, Inc.
// All Rights Reserved.
//
//  $Id: SWILogBlock.hpp,v 1.1.4.1 2002/04/12 16:00:44 dmeyer Exp $
//

#include <VXIlog.h>
#include <cwchar>

class SWILogBlock
{
  /**
   * Constructor printing a message that we are entering a block.  The return
   * code of the block is initialized to the value of <code>rc</code> which
   * defaults to <code>0</code>.
   **/
 public:
  inline SWILogBlock(VXIlogInterface *log,
		  unsigned int tag,
		  const wchar_t* subtag,
                  int rc = 0):
    _logHandle(log),
    _tag(tag),
    _subtag(subtag),
    _rc(rc)
  {
    if (isEnabled())
    {
      logDiagnostic(L"entered.");
    }
  }

  /**
   * Destructor printing a message that we are exiting a block.
   **/
 public:
  inline ~SWILogBlock()
  {
    if (isEnabled())
      logDiagnostic(L"return: rc = %d", _rc);
  }

 public:
  /**
   * Sets the return code for this block.
   *
   * @param _rc The new value of the return code for the block.
   * @return <code>this</code>.
   **/
  SWILogBlock& operator=(int rc)
  {
    _rc = rc;
    return *this;
  }

 public:
  /**
   * Converts a SWILogBlock into an int.
   * @return the current return code for this SWILogBlock.
   **/
  operator int()
  {
    return _rc;
  }

 public:
  bool isEnabled()
  {
    return isEnabled(_tag);
  }

 public:
  bool isEnabled(unsigned int tag)
  {
    return (_logHandle != NULL &&
            _logHandle->DiagnosticIsEnabled != NULL &&
            _logHandle->DiagnosticIsEnabled(_logHandle, tag));
  }

 public:
  void logDiagnostic(unsigned int tag, const wchar_t *format, ...)
  {
    va_list vargs;
    va_start(vargs, format);
    if (_logHandle != NULL && _logHandle->VDiagnostic != NULL)
    {
      (_logHandle->VDiagnostic)(_logHandle, tag, _subtag, format, vargs);
    }
    va_end(vargs);
  }

 public:
  void logDiagnostic(const wchar_t *format, ...)
  {
    va_list vargs;
    va_start(vargs, format);
    if (_logHandle != NULL && _logHandle->VDiagnostic != NULL)
    {
      (_logHandle->VDiagnostic)(_logHandle, _tag, _subtag, format, vargs);
    }
    va_end(vargs);
  }

 public:
  void logError(const wchar_t* moduleName,
                VXIunsigned errorID,
                const wchar_t *format, ...)
  {
    va_list vargs;
    va_start(vargs, format);
    if (_logHandle != NULL && _logHandle->VError != NULL)
    {
      (_logHandle->VError)(_logHandle, moduleName, errorID, format, vargs);
    }
    va_end(vargs);
  }

 public:
  const wchar_t *getSubtag() const
  {
    return _subtag;
  }

 private:
  unsigned int getTag() const
  {
    return _tag;
  }

  /* Just to ensure that SWILogBlock cannot be copied. */
 private:
  SWILogBlock(const SWILogBlock&);
  SWILogBlock& operator=(const SWILogBlock&);

  // members

 private:
  VXIlogInterface *_logHandle;

 private:
  /**
   * The tag to use for logging
   **/
  unsigned int _tag;

 private:
  /**
   * The subtag of the logging block
   **/
  const wchar_t* _subtag;

 private:
  /**
   * The return code of the function block
   **/
  int _rc;

};
#endif
