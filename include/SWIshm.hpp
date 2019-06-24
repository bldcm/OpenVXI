//-----------------------------------------------------------------------
// SWIshm.hpp
// Copyright 1998, 1999, 2000, 2001 SpeechWorks International, Inc.
//  All Rights Reserved.
//
// Created: 23 Feb 01
//
// $Id: SWIshm.hpp,v 1.2.2.2.6.2 2003/05/14 00:28:45 gnantel Exp $
//
#ifndef _SWIshm_HPP
#define _SWIshm_HPP


/**
 * Class to encapsulate the operation of a shared memory segment.
 * Once a shared memory segment is obtained
 * from the OS, it is casted into a SWIshm.  Therefore, the fist
 * <code>sizeof(SWIshm)</code> of the segment are used for
 * book-keeping and the remaining of the segment is the actual buffer of data.
 *
 *
 * @doc <p>
 * Copyright 1998, 1999, 2000, 2001 SpeechWorks International, Inc.
 *  All Rights Reserved.
 **/


#include "SWIutilHeaderPrefix.h"

typedef enum SWIshmResult {
  SWIshm_FATAL_ERROR = -100,
  SWIshm_OUT_OF_MEMORY_ERROR = -80,
  SWIshm_MEMORY_ERROR = -80,
  SWIshm_GENERIC_ERROR = -10,
  SWIshm_SUCCESS = 0,
  SWIshm_CREATE_FAILED = 1,
  SWIshm_ATTACH_FAILED = 2,
  SWIshm_PERMISSION_DENIED = 10,
  SWIshm_SEGMENT_NOT_FOUND = 11,
  SWIshm_OP_INTERRUPTED =12,
  SWIshm_CREATE_DENIED = 13,
  SWIshm_SEGMENT_ALREADY_EXISTS = 14,
  SWIshm_CREATE_MUTEX_FAILED = 15,
  SWIshm_NO_SEGMENT =16,
  SWIshm_DESTROY_FAILED = 17
} SWIshmResult;

class SWIshm_;

class SWIUTIL_API_CLASS SWIshm
{
private:
  SWIshm(){};

  friend class SWIshm_;

public:
  /**
   * Creates a named shared memory segment capable of containing a queue of
   * <code>size</code> bytes.
   *
   * @param shmid  [in] The identifier of the shared segment. On
   *                    Windows this becomes a string prepended with
   *                    SWIshm. So the identifer is
   *                    "SWIshm%ld"
   *
   * @param size   [in] The number of bytes that the segment should
   *                    contain for the user. Additional space may be
   *                    allocated for control signals.
   *
   * @param shm    [out] The created shared memory segment. NULL on error.
   * @return Result of the create
   **/
  static SWIshmResult create(int shmid,  unsigned int size, SWIshm** shm);

  /**
   * Gets a header corresponding to the shared memory id.
   *
   * @param shmid [in]   The shared memory id.
   * @param shm   [out]  The newly attached shared memory segement
   *                     corresponding to the segment id.  NULL on error
   **/
  static SWIshmResult attach(int shmid,  SWIshm** header, unsigned int size = 0);


  /**
     If the this shared memory segment is the owner of the segment
     then this does a destroy of the segment.  If the segment was
     gotten via an attach then it simply does a detach.
  */
  virtual ~SWIshm(){};


  /**
   * Retrieves the size of the shared memory segment.
   *
   **/
  virtual unsigned int size()=0;


  /**
   * @doc
   * Reads some number of bytes from shared memory segment and stores them
   * into the buffer <code>data</code>. The number of bytes actually
   * read is returned as an integer.  This method is non-blocking.
   *
   * <p> If <code>data</code> is <code>null</code>, a
   * <code>SWI_STREAM_INVALID_ARGUMENT</code> is returned.  If
   * <code>dataSize</code> is zero, then no bytes are read and
   * <code>0</code> is returned; otherwise, there is an attempt to read at
   * least one byte. if <code>cancelRead</code> is called before
   *
   * @param offset   [in]  the starting point in the shared memory to start the read from
   * @param data     [out] the buffer into which the data is read.
   * @param dataSize [in]  the number of bytes to be read.
   * @param nread    [out] the number of bytes read.
   * @return Returns the result of the read.
   **/
  virtual SWIshmResult read(unsigned int offset,
			    unsigned char *data, unsigned int dataSize,
			    int* nread) = 0;

  /**
   * Writes <code>bufferSize</code> bytes to the shared memory segment.
   *
   * <p>
   * If <code>buffer</code> is <code>null</code>,
   * <code>SWI_STREAM_INVALID_ARGUMENT</code> is returned.
   * <p>
   *
   * @param offset   [in]  the starting point in the shared memory to start the write from
   * @param      buffer       the data to be written.
   * @param      bufferSize   the number of bytes to write.
   * @param      nwritten     the number of bytes written.
   * @return     result of the write.
   **/
  virtual SWIshmResult write(unsigned int offset,
			     const unsigned char* buffer, unsigned int bufferSize,
			     int *nwritten)=0;

  /**
   * Set a section of the the memory to a fixed value.
   * @param offset   [in]  the starting point in the shared memory to start the write from
   * @param value    [in]  the value to write into memory
   * @param nwrite   [in]  number of times to write the value
   * @param nwritten     the number of bytes written.
   **/
  virtual SWIshmResult set(unsigned int offset, unsigned char value,
			   unsigned int nwrite, int *nwritten)=0;

};

#endif
