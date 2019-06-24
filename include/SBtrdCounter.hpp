/* SBtrdCounter, utility class for managing a thread safe counter */
/* C_HEADER_DOC
 *
 * Copyright 1999-2002 SpeechWorks International, Inc.  All Rights Reserved.
 *
 * SpeechWorks Source Code Files
 * Notice and Disclaimer of Liability
 *
 * This source code software is provided to the user as an example of
 * an application which utilizes OpenSpeech Browser Software to assist
 * the user to develop its own application software.  This source code
 * has not been fully tested and may contain errors.  This software is
 * not subject to any warranty or maintenance terms as may be
 * contained in the Software License Agreement between SpeechWorks and
 * User.  SPEECHWORKS PROVIDES THIS SOFTWARE ON AN 'AS IS' BASIS, AND
 * DISCLAIMS ALL WARRANTIES, WHETHER EXPRESS OR IMPLIED, INCLUDING THE
 * IIMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NON-INFRINGEMENT.
 *
 * Use of this software is subject to certain restrictions and limitations
 * set forth in a license agreement entered into between SpeechWorks
 * International Inc. and the licensee of this software.  Please refer 
 * to the license agreement for license use rights and restrictions. 
 * 
 * SpeechWorks is a registered trademark, and SpeechWorks Here, 
 * DialogModules and the SpeechWorks logo are trademarks of SpeechWorks 
 * International, Inc. in the United States and other countries. 
 * 
 * $RCSfile: SBtrdCounter.hpp,v $
 *
 * The SBtrdCounter class manages a thread safe counter, allowing
 * multiple threads to increment/test/decrement the counter using
 * atomic actions.
 *
 * Component of: SBtrdUtil
 * 
 */

/* $Id: SBtrdCounter.hpp,v 1.3.2.1 2003/01/21 21:28:43 ccao Exp $ */

// -----1=0-------2=0-------3=0-------4=0-------5=0-------6=0-------7=0-------8

#ifndef _SBTRD_COUNTER_H__
#define _SBTRD_COUNTER_H__

#include <limits.h>                   // For ULONG_MAX
#include "VXItrd.h"                   // For VXItrdMutex

class SBtrdCounter {
 public:
  // Constructor
  SBtrdCounter (unsigned long initialValue) : _counter(initialValue) { }
  virtual ~SBtrdCounter( ) { if ( _mutex ) VXItrdMutexDestroy (&_mutex); }

  // Creation
  VXItrdResult Create( ) { return VXItrdMutexCreate (&_mutex); }

  // Increment and decrement the sequence number (prefix version only)
  unsigned long operator++( ) { 
    unsigned long ret = 0;
    if ( VXItrdMutexLock (_mutex) == VXItrd_RESULT_SUCCESS ) {
      if ( _counter < ULONG_MAX )
	++_counter;
      ret = _counter;
      VXItrdMutexUnlock (_mutex);
    }
    return ret;
  }
  unsigned long operator--( ) {
    unsigned long ret = 0;
    if ( VXItrdMutexLock (_mutex) == VXItrd_RESULT_SUCCESS ) {
      if ( _counter > 0 )
	ret = --_counter;
      VXItrdMutexUnlock (_mutex);
    }
    return ret;
  }

  // Postfix decrement
  unsigned long operator--(int) { 
    unsigned long ret = 0;
    if ( VXItrdMutexLock (_mutex) == VXItrd_RESULT_SUCCESS ) {
      ret = _counter;
      if ( _counter > 0 )
	_counter--;
      VXItrdMutexUnlock (_mutex);
    }
    return ret;
  }

  // Get the value of the sequence number
  unsigned long Get( ) const {
    unsigned long ret = 0;
    if ( VXItrdMutexLock (_mutex) == VXItrd_RESULT_SUCCESS ) {
      ret = _counter;
      VXItrdMutexUnlock (_mutex);
    }
    return ret;
  }

  // Set the value, returns the previous value
  unsigned long Set (unsigned long value) {
    unsigned long ret = 0;
    if ( VXItrdMutexLock (_mutex) == VXItrd_RESULT_SUCCESS ) {
      ret = _counter;
      _counter = value;
      VXItrdMutexUnlock (_mutex);
    }
    return ret;
  }

 private:
  // Disable the copy constructor and equality operator to catch making
  // copies at compile or link time, neither is really implemented
  SBtrdCounter (const SBtrdCounter &);
  SBtrdCounter & operator= (const SBtrdCounter &);

 private:
  unsigned long   _counter;
  VXItrdMutex    *_mutex;
};

#endif  // _SBTRD_COUNTER_H__
