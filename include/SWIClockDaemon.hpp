#ifndef SWICLOCKDAEMON_HPP
#define SWICLOCKDAEMON_HPP

// SWIClockDaemon.hpp
// Copyright 1998, 1999, 2000, 2001 SpeechWorks International, Inc.
//  All Rights Reserved.
//
//
//
// Alain Caron, 13 Jul 2001
//
//
// $Id: SWIClockDaemon.hpp,v 1.10.2.2 2002/04/12 16:00:44 dmeyer Exp $
//

/**
 * class to
 * @doc <p>
 * Copyright 1998, 1999, 2000, 2001 SpeechWorks International, Inc.
 *  All Rights Reserved.
 **/

#include "SWIutilHeaderPrefix.h"

#include "SWIHeap.hpp"
#include "SWItrdMonitor.hpp"
#include <VXItrd.h>
#include "SWITimeStamp.hpp"

class SWIUTIL_API_CLASS SWIClockDaemon
{
  // ................. CONSTRUCTORS, DESTRUCTOR  ............
  //
  // ------------------------------------------------------------
  /**
   * Default constructor.
   **/
 public:
  SWIClockDaemon();

  /**
   * Destructor.
   **/
 public:
  virtual ~SWIClockDaemon();

 public:
  class SWIUTIL_API_CLASS Task
  {
   public:
    Task();

   public:
    virtual ~Task();

    /**
     * Executes the task.
     **/
   public:
    virtual void execute() = 0;

    /**
     * Cancels the task.  This does not remove the task from the list of
     * tasks to perform.  Instead, it only sets the cancelled flag of the task
     * so that it won't be executed when its time comes.
     **/
   public:
    bool cancel();

    /**
     * Releases ownership of the task.  There is an implicit contract with
     * release that it won't be accesed anymore.  The daemon is allowed to
     * delete the task after its execution if it has not been rescheduled.
     * After a Task is released, any access to it is invalid.  The task is
     * deleted only at its scheduled time.
     *
     * @param deleteNowIfCancelled if the task is cancelled and this flag is
     * set to true, the task will be immediately deleted instead of waiting
     * until its execution time occurs.  This parameter has no effect if the
     * task is not cancelled.
     **/
   public:
    void release(bool deleteNowIfCancelled = false);

    /**
     * Reschedules the task at the specified delay.  A released task should
     * not be rescheduled.  Rescheduling a cancelled task, clears its
     * cancellation flag.
     **/
   public:
    bool reschedule(unsigned long delay);

   private:
    enum State
    {
      WAITING, /* the task is in the heap waiting for execution. */
      EXECUTE, /* the task has been removed from the heap because its
                  execution time expired. */
      CANCELLED /* the task is in the heap but is cancelled. */
    };

   private:
    long expiration();
    void setTimeStamp(unsigned long delay);

   private:
    static int compare(Task *a, Task *b);

   private:
    friend class SWIClockDaemon;
    volatile State _state;
    SWIClockDaemon *_daemon;
    SWITimeStamp _timestamp;
    int _refCount;
  };

 public:
  int stop();

 public:
  int start();

 public:
  bool schedule(unsigned long delay, Task* task);

 private:
  friend class Task;

  /**
    * Disabled copy constructor.
   **/
 private:
  SWIClockDaemon(const SWIClockDaemon&);

  /**
    * Disabled assignment operator.
   **/
 private:
  SWIClockDaemon& operator=(const SWIClockDaemon&);

 private:
  static VXITRD_DEFINE_THREAD_FUNC(threadMain, userData);

 private:
  bool reschedule(Task *task);

 private:
  SWItrdMonitor _monitor;

 private:
  VXItrdThread *_thread;

 private:
  SWIHeap _heap;

 private:
  enum State
  {
    IDLE,
    RUNNING,
    STOPPING,
    KILLED
  };

 private:
  volatile State _state;
};
#endif
