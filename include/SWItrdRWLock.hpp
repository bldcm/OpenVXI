#ifndef SWITRDRWLOCK_HPP
#define SWITRDRWLOCK_HPP

// SWItrdRWLock.hpp
// Copyright 1998, 1999, 2000, 2001 SpeechWorks International, Inc.
//  All Rights Reserved.
//
//
//
// Alain Caron, 13 Dec 2001
//
//
// $Id: SWItrdRWLock.hpp,v 1.4.2.2 2002/03/20 20:07:52 dmeyer Exp $
//

/**
 * class to encapsulate read-write locks.  Strong preference is given to
 * writers.  A reader won't be granted a lock when a writer is waiting.
 * Readers can upgrade their lock to write lock if they are the only reader.
 * Locks are re-entrant: When a thread calls lockR or lockW, it has to call
 * unlockR or unlockW the same number of times.
 *
 * @doc <p>
 * Copyright 1998, 1999, 2000, 2001 SpeechWorks International, Inc.
 *  All Rights Reserved.
 **/

#include "SWIutilHeaderPrefix.h"

#include "SWItrdMonitor.hpp"
#include "SWIList.hpp"

class SWIUTIL_API_CLASS SWItrdRWLock: private SWItrdMonitor
{
  /**
   * Status indicating success of an operation.
   **/
 public:
  SWItrdMonitor::SUCCESS;

  /**
   * Status indicating failure of an operation because of internal error.
   **/
 public:
  SWItrdMonitor::FAILURE;

  /**
   * Status indicating failure of an unlock operation because the calling
   * thread does not have the corresponding lock.
   **/
 public:
  SWItrdMonitor::NOT_OWNER;

 public:
  SWItrdRWLock();

  /**
   * Destructor.
   **/
 public:
  virtual ~SWItrdRWLock();

 public:
  /**
   * Acquires the write-lock: block until all readers and the current writer
   * release their lock.
   **/
  int lockW();

  /**
   * Unlocks the write-lock.
   **/
  int unlockW();

  /**
   * Acquires the read-lock: block until the current writer releases its lock
   * and all pending writers are done.
   **/
  int lockR() const;

  /**
   * Releases the read-lock.
  **/
  int unlockR() const;

  /**
    * Disabled copy constructor.
   **/
 private:
  SWItrdRWLock(const SWItrdRWLock&);

  /**
    * Disabled assignment operator.
   **/
 private:
  SWItrdRWLock& operator=(const SWItrdRWLock&);

 private:
  /**
   * Number of writers waiting for the lock.
   **/
  volatile int _pendingWriterCount;
  /**
   * Number of writers having the lock: this is basicly how many times the
   * thread owning the write-lock have called lockW.
   **/
  volatile int _writerCount;

  /**
   * Number of current readers.
   **/
  volatile int _readerCount;

  /**
   * The id of the thread owning the write-lock.
   **/
  volatile VXIthreadID _writingThread;

  /**
   * Contains information about each reader: threadId and readCount (how many
   * times was the lockR() method called).
   **/
  SWIList _readerList;
};

#endif
