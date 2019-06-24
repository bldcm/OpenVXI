#ifndef SWIHEAP_HPP
#define SWIHEAP_HPP

// SWIHeap.hpp
// Copyright 1998, 1999, 2000, 2001 SpeechWorks International, Inc.
//  All Rights Reserved.
//
//
//
// Alain Caron, 12 Jul 2001
//
//
// $Id: SWIHeap.hpp,v 1.7.2.1.6.2 2003/05/14 00:28:44 gnantel Exp $
//

/**
 * class to abstract heap operations
 * @doc <p>
 * Copyright 1998, 1999, 2000, 2001 SpeechWorks International, Inc.
 *  All Rights Reserved.
 **/


#include "SWIutilHeaderPrefix.h"

class SWIUTIL_API_CLASS SWIHeap
{
  /**
   * Comparison function.  Should return negative value (< 0)if a < b,
   * 0 if a is equal to b and positive value if a > b.
   **/
 public:
  typedef int (*CompareFunction)(void *a, void *b);

  // ................. CONSTRUCTORS, DESTRUCTOR  ............
  //
  // ------------------------------------------------------------

  /**
   * Constructs a heap with the specified comparison function.
   **/
 public:
  SWIHeap(CompareFunction f);

  /**
   * Constructs a heap with the specified comparison function and
   * initial capacity.
   **/
 public:
  SWIHeap(CompareFunction f, int capacity);

  /**
   * Destructor.
   **/
 public:
  virtual ~SWIHeap();

  /**
   * Removes and returns the root element of the heap.  Returns NULL if the
   * heap is empty.
   **/
 public:
  void *pop();

  /**
   * Inserts an item into the heap.  The comparison function is used to
   * determine its proper location.
   **/
 public:
  bool push(void *item);

  /**
   * Returns (but does not remove) the root element of the heap.  Returns NULL
   * if the heap is empty.
   **/
 public:
  void *top() const;

  /**
   * Returns true if the heap is empty, false otherwise.
   **/
 public:
  bool isEmpty() const;

  /**
   * Returns the number of elements of the heap.
   **/
 public:
  int size() const;

  /**
   * Returns the position in the heap of the specified item.  This method
   * uses the == operator.
   **/
 public:
  bool contains(void *item) const;

  /**
   * Removes the item from the heap while maintaining heap property.
   * This takes linear time as it involves a search with the == operator
   * in order to find the item in the heap.
   *
   * @return true if the item was found and removed.
   **/
 public:
  bool remove(void *item);

  /**
   * Replaces item1 with item2 and re-heapify the heap.
   * This takes linear time as it involves a search with the == operator
   * in order to find item1 in the heap.
   *
   * @return item1 if it was found, NULL otherwise.
   **/
 public:
  void *replace(void *item1, void *item2);

  /**
   * Rebuilds the heap.  This method is useful if the items in the heap are
   * modified in such a way that the order relation is not maintained.  All
   * public methods of the SWIHeap class maintain the heap property.
   *
   **/
 public:
  void heapify();

  /**
   * Copy constructor.
   **/
 public:
  SWIHeap(const SWIHeap&);

  /**
   * Assignment operator.
   **/
 public:
  SWIHeap& operator=(const SWIHeap&);

 private:
  bool moveUp(int child);
  bool moveDown(int parent);

 private:
  int _size;
  int _capacity;
  void **_elements;
  CompareFunction _compare;
};

#endif
