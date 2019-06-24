//-----------------------------------------------------------------------
// SWIfileInputStream.hpp
// Copyright 1998, 1999, 2000, 2001 SpeechWorks International, Inc.
//  All Rights Reserved.
//
// Created: 23 Feb 01
//
// $Id: SWIfileInputStream.hpp,v 1.1.6.2 2003/05/14 00:32:24 gnantel Exp $
//
#ifndef SWIfileInputStream_HPP
#define SWIfileInputStream_HPP

#include <SWIinputStream.hpp>
#include <stdio.h>

/**
 * Class read from a file as a stream.
 *
 *
 * @doc <p>
 * Copyright 1998, 1999, 2000, 2001 SpeechWorks International, Inc.
 *  All Rights Reserved.
 **/
class SWIUTIL_API_CLASS SWIfileInputStream: public SWIinputStream
{
  // ................. CONSTRUCTORS, DESTRUCTOR  ............
  //
  // ------------------------------------------------------------
  /**
   * Creates a inputstream on the file name.
   *
   * @param filename [in] The file name of the file to be read.
   **/
 public:
  SWIfileInputStream();
  SWIfileInputStream(const char *filename);

  // ------------------------------------------------------------
  /// destructor
 public:
  virtual ~SWIfileInputStream()
  {
    close();
  }

  int open(const char *filename);

  //
  // ......... METHODS ..........
  //
  /**
   * Reads some number of bytes from the input stream and stores them into the
   * buffer array <code>buffer</code>.  The number of bytes actually read
   * is returned as an integer.
   *
   * <p> If <code>data</code> is <code>null</code>, a
   * <code>SWI_STREAM_INVALID_ARGUMENT</code> is returned.  If
   * <code>dataSize</code> of is zero, then no bytes are read and
   * <code>0</code> is returned; otherwise, there is an attempt to read at
   * least one byte. If no byte is available because the stream is at end of
   * file, the value <code>SWI_STREAM_EOF</code> is returned; otherwise, at
   * least one byte is read and stored into <code>buffer</code>.  If the
   * stream is not opened, <code>SWI_STREAM_ILLEGAL_STATE</code> is returned.
   *
   * @param      data   [out] the buffer into which the data is read.
   * @param      dataSize [in] the number of bits to be read.
   * @return the total number of bytes read into the buffer, or a negative
   * number indicating there is no more data because the end of the stream has
   * been reached, or to indicate an error.
   **/
 public:
  virtual int readBytes(void* data, int dataSize);

  virtual int read();

  virtual int peek(int offset = 0) const;

  virtual int getLookAhead() const;

  /**
   * Returns true to indicate that it is a buffered stream.
   **/
 public:
  bool isBuffered() const;

  /**
   * Closes this input stream and releases any system resources associated
   * with the stream.
   *
   **/
 public:
  virtual Result close();

 private:
  FILE* _fhandle;
};
#endif
