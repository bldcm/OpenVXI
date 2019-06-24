#ifndef SWITALLY_HPP
#define SWITALLY_HPP

// SWITally.hpp
// Copyright 1998, 1999, 2000, 2001 SpeechWorks International, Inc.
//  All Rights Reserved.
//
//
//
// Alain Caron, 30 May 2002
//
//
// $Id: SWITally.hpp,v 1.1.8.2 2003/05/14 00:32:24 gnantel Exp $
//

#include "SWIutilHeaderPrefix.h"

/**
 Class used to collect data on a given phenomenon
 Statistics collected are min, max, mean and standard deviation of the
 observations.

 Observations are recorded through the method update().

 * @doc <p>
 * Copyright 1998, 1999, 2000, 2001 SpeechWorks International, Inc.
 *  All Rights Reserved.
 **/

class SWIUTIL_API_CLASS SWITally
{
  // ................. CONSTRUCTORS, DESTRUCTOR  ............
  //
  // ------------------------------------------------------------
  /**
   * Default constructor.
   **/
 public:
  SWITally(const char *name);

  /**
   * Destructor.
   **/
 public:
  virtual ~SWITally();

  /**
   * Resets the tally to 0 observations.
   **/
 public:
  void reset();

 public:
  int getObs() const;

 public:
  void update(double v);

 public:
  double getMean() const;

 public:
  double getStdev() const;

 public:
  double getMin() const;

 public:
  double getMax() const;

 public:
  const char *getName() const;

 public:
  static void setFileName(const char *filename);

  /**
   * Disabled copy constructor.
   **/
 private:
  SWITally(const SWITally&);

  /**
   * Disabled assignment operator.
   **/
 private:
  SWITally& operator=(const SWITally&);

 private:
  int _obs;
  double _total;
  double _total2;
  double _minv;
  double _maxv;
  char *_name;
};
#endif
