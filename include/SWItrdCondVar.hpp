#ifndef CONDVAR_HPP
#define CONDVAR_HPP

// CondVar.hpp
// Copyright 1998, 1999, 2000, 2001 SpeechWorks International, Inc.
//  All Rights Reserved.
//
//
//
// Alain Caron, 11 Apr 2001
//
//
// $Id: SWItrdCondVar.hpp,v 1.9.2.1 2002/03/12 18:52:10 dmeyer Exp $
//

#include "SWIutilHeaderPrefix.h"

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#else
#include <pthread.h>
#endif

/**
 * Class encapsulating condition variables mechanism among threads of a same
 * process.  Condition variables represent events that can be waited on.  The
 * condition variable can be set or reset.  Setting a condition variable
 * causes the threads waiting for the condition variable to be set to be
 * resumed.
 *
 * @doc <p>
 * Copyright 1998, 1999, 2000, 2001 SpeechWorks International, Inc.
 *  All Rights Reserved.
 **/

class SWIUTIL_API_CLASS SWItrdCondVar
{
  /**
   * Status indicating success of an operation.
   **/
 public:
  static const int SUCCESS;

  /**
   * Status indicating failure of an operation.
   **/
 public:
  static const int FAILURE;

  /**
   * Creates a new condition variable that is either set or not set depending
   * on the <code>isSet</code> parameter.
   *
   * @param isSet Initial state of the condition variable.
   **/
 public:
  SWItrdCondVar(bool isSet = false);

  /**
   * Destructor.
   **/
 public:
  ~SWItrdCondVar();

  /**
   * Sets the condition variable and resumes all threads waiting
   * on the condition variable.
   *
   * @return SUCCESS if success, <code>false</code> otherwise.
   **/
  int set();

  /**
   * Resets the condition variable.  This will cause subsequent calls to wait
   * to block.
   *
   * @return SUCCESS if success,  <code>false</code> otherwise.
   **/
  int reset();


  /**
   * Waits until the condition variable is set.
   *
   * @return SUCCESS if success,  <code>false</code> otherwise.
   **/
  int wait();

  /**
   * Waits until the condition variable is set, or until the specified amount
   * of time expires.
   *
   * @param millisecs Maximum number of milliseconds to wait for the variable
   * to be set.
   * @param expiredF if it is non-null, it is set to true to indicate that the delay expired
   * before the variable was set or to false to indicate that the variable was set first.
   * @return SUCCESS if success,  <code>false</code> otherwise.
   **/
  int wait(unsigned long millisecs, bool *expiredF = NULL);

  /**
    * Disabled copy constructor
   **/
 private:
  SWItrdCondVar(const SWItrdCondVar&);

  /**
   * Disabled assignment operator.
   **/
 private:
  SWItrdCondVar& operator=(const SWItrdCondVar&);

#ifdef _WIN32
 private:
  HANDLE _cond;
#else
  /**
   * Flag indicating whether the condition variable is set or not.
   **/
 private:
  volatile bool _isSet;

  pthread_mutex_t _mutex;
  pthread_cond_t _cond;
#endif
};

#endif

