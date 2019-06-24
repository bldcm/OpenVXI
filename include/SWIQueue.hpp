#ifndef SWIQUEUE_HPP
#define SWIQUEUE_HPP

// SWIQueue.hpp
// Copyright 1998, 1999, 2000, 2001 SpeechWorks International, Inc.
//  All Rights Reserved.
//
//
//
// Alain Caron, 23 Apr 2001
//
//
// $Id: SWIQueue.hpp,v 1.3.28.2 2003/05/14 00:28:45 gnantel Exp $
//

/**
 * Class encapsulating operations on a queue.  In order to be able to easily
 * implement copy-on-write semantic, SWIQueues created using the copy
 * constructor or using the operator= share the same data.  When SWIQueues are
 * shared, changes made on one SWIqueue object will have been made on the
 * other SWIQueue object as well.  Although NULL pointers can be put in a
 * queue, one has to be careful with the use of NULL pointers as they are used
 * as a way to determine whether a queue is empty.
 *
 * @doc <p> Copyright 1998, 1999, 2000,
 * 2001 SpeechWorks International, Inc.  All Rights Reserved.
 **/

#include "SWIList.hpp"

class SWIQueue: private SWIList
{
  /**
   * Default constructor.
   **/
 public:
  SWIQueue():
    SWIList()
  {}

  /**
   * Destructor.
   **/
 public:
  ~SWIQueue()
  {}

  /**
    * Copy constructor.
   **/
 public:
  SWIQueue(const SWIQueue& original):
    SWIList(original)
  {}

  /**
   * Assignment operator.
   **/
 public:
  SWIQueue& operator=(const SWIQueue& other)
  {
    SWIList::operator=(other);
    return *this;
  }

  /**
   * Returns the number of elements in this queue.
   *
   * @return the number of elements in this queue.
   **/
 public:
  SWIList::size;

  /**
   * Returns <tt>true</tt> if this queue contains no elements.
   *
   * @return <tt>true</tt> if this queue contains no elements
   **/
 public:
  SWIList::isEmpty;

  /**
   * Removes all of the elements from this queue.
   * This queue will be empty after this method returns.
   **/
 public:
  SWIList::clear;

  /**
   * Removes and return the first element from the queue.
   *
   * @return the next element from the queue, NULL if the queue is empty.
   **/
 public:
  void *get()
  {
    return removeFirst();
  }

  /**
   * Inserts an element at the end of the queue.
   *
   * @param item The item to insert at the end of the queue.
   * @return true if the item could be inserted, false otherwise.
   **/
 public:
  bool put(void *item)
  {
    return addLast(item);
  }
};

#endif
