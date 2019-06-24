/****************License************************************************
 *
 * Copyright 2000-2001.  SpeechWorks International, Inc.  
 *
 * Use of this software is subject to notices and obligations set forth
 * in the SpeechWorks Public License - Software Version 1.1 which is
 * included with this software.
 *
 * SpeechWorks is a registered trademark, and SpeechWorks Here, 
 * DialogModules and the SpeechWorks logo are trademarks of SpeechWorks 
 * International, Inc. in the United States and other countries. 
 * 
 ************************************************************************
 *
 * $Id: SBtestAssert.h,v 1.6 2002/03/06 19:25:54 dmeyer Exp $
 *
 ************************************************************************
 */

#ifndef _SBTESTASSERT_H
#define _SBTESTASSERT_H

#ifdef __cplusplus

#include <cstdlib>                    /* for abort, etc. */
#include <iostream>                   /* for cout        */

inline void _Assert(const char * text, const char * file, int line)
{
  std::cout << "Assertion failed: " << text << ", file " << file << ", line "
       << line << std::endl;
  exit(1);
}

#else

#include <stdlib.h>                  /* for abort, etc. */
#include <stdio.h>                   /* for fprintf( ) */

static void _Assert(const char * text, const char * file, int line)
{
  fprintf(stderr, "Assertion failed: %s, file %s, line %d\n",
	  text, file, line);
  exit(1);
}

#endif

/* #define Assert(exp) void ( (exp) || (_Assert(#exp, __FILE__, __LINE__), 0) )
 */
#define Assert(exp) if ( !(exp) ) _Assert(#exp, __FILE__, __LINE__)



#endif /* _SBTESTASSERT_H */
