#ifndef SWISTACK_HPP
#define SWISTACK_HPP

// SWIStack.hpp
// Copyright 1998, 1999, 2000, 2001 SpeechWorks International, Inc.
//  All Rights Reserved.
//
//
//
// Alain Caron, 23 Apr 2001
//
//
// $Id: SWIStack.hpp,v 1.4.28.2 2003/05/14 00:28:45 gnantel Exp $
//

/**
 * Class encapsulating operations on a stack.Although NULL pointers can be put
 * in a stack, one has to be careful with the use of NULL pointers as they are
 * used as a way to determine whether a stack is empty.
 *
 * @doc <p> Copyright 1998, 1999, 2000,
 * 2001 SpeechWorks International, Inc.  All Rights Reserved.
 **/

#include "SWIList.hpp"

class SWIStack: private SWIList
{
  /**
   * Default constructor.
   **/
 public:
  SWIStack():
    SWIList()
  {}

  /**
   * Destructor.
   **/
 public:
  ~SWIStack()
  {}

  /**
    * Copy constructor.
   **/
 public:
  SWIStack(const SWIStack& original):
    SWIList(original)
  {}

  /**
   * Assignment operator.
   **/
 public:
  SWIStack& operator=(const SWIStack& other)
  {
    SWIList::operator=(other);
    return *this;
  }

  /**
   * Returns the number of elements in this stack.
   *
   * @return the number of elements in this stack.
   **/
 public:
  SWIList::size;

  /**
   * Returns <tt>true</tt> if this stack contains no elements.
   *
   * @return <tt>true</tt> if this stack contains no elements
   **/
 public:
  SWIList::isEmpty;

  /**
   * Removes all of the elements from this stack.
   * This stack will be empty after this method returns.
   **/
 public:
  SWIList::clear;

  /**
   * Returns the element on the top of the stack.
   *
   * @returns the element on top of the stack, NULL if the stack is empty.
   **/
 public:
  void *top()
  {
    return first();
  }

  /**
   * Removes and return the element on the top of the stack.
   *
   * @return the element on top of the stack, NULL if the stack is empty.
   **/
 public:
  void *pop()
  {
    return removeFirst();
  }

  /**
   * Pushes an element on top of the stack.
   *
   * @param item The item to push on top of the stack.
   * @return true if the item could be pushed, false otherwise.
   **/
 public:
  bool push(void *item)
  {
    return addFirst(item);
  }
};

#endif
