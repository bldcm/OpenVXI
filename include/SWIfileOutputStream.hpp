//-----------------------------------------------------------------------
// SWIfileOutputStream.hpp
// Copyright 1998, 1999, 2000, 2001 SpeechWorks International, Inc.
//  All Rights Reserved.
//
// Created: 26 Feb 01
//
// $Id: SWIfileOutputStream.hpp,v 1.1.6.2 2003/05/14 00:32:25 gnantel Exp $
//
#ifndef SWIfileOutputStream_HPP
#define SWIfileOutputStream_HPP

#include "SWIoutputStream.hpp"
#include <stdio.h>

/**
 * This class represents an output stream that writes to a file.
 *
 * @doc <p> Copyright 1998, 1999, 2000, 2001 SpeechWorks International, Inc.
 * All Rights Reserved.
 **/
class SWIUTIL_API_CLASS SWIfileOutputStream: public SWIoutputStream
{
  // ................. CONSTRUCTORS, DESTRUCTOR  ............
  //
 public:
  SWIfileOutputStream(const char *filename);
  SWIfileOutputStream();

  int open(const char *filename);

  // ------------------------------------------------------------
  /// destructor
 public:
  virtual ~SWIfileOutputStream()
  {
    close();
  }

  //
  // ......... METHODS ..........
  //
  /**
   * Writes <code>bufferSize</code> bytes from the specified buffer.
   *
   * <p> If <code>dataSize</code> is less than <code>0</code>, or
   * <code>data</code> is <code>null</code> and <code>dataSize</code> is not
   * <code>0</code>, then <code>SWI_STREAM_INVALID_ARGUMENT</code> is
   * returned.
   *
   * @param    data   [in] the data to be written.
   * @param    dataSize [in]  the number of bytes to write.
   * @return   the number of bytes written, or a negative number indicating failure.
   **/
 public:
  virtual int writeBytes(const void* data, int dataSize);

  /**
   * Flushes this output stream and forces any buffered output bytes
   * to be written out.
   *
   * @return <code>SWI_STREAM_SUCCESS</code> if succes, negative value if failure.
   **/
 public:
  virtual Result flush();

 public:
  virtual bool isBuffered() const;

 public:
  virtual Result printString(const char *s);

 public:
  virtual Result printChar(char c);

 public:
  virtual Result printInt(int i);

  /**
   * Closes this output stream and releases any system resources
   * associated with this stream.
   *
   * @return SWI_STREAM_SUCCESS if success, negative value if failure.
   **/
 public:
  virtual Result close();

  // members.

  /**
   * The underlying stream to which the data is written.
   **/
 private:
  FILE *_fhandle;
};
#endif
