/* SBtrdTimeOfDay, utility class for handling a time of day */

 /****************License************************************************
  *
  * Copyright 2000-2003.  ScanSoft, Inc.    
  *
  * Use of this software is subject to notices and obligations set forth 
  * in the SpeechWorks Public License - Software Version 1.2 which is 
  * included with this software. 
  *
  * ScanSoft is a registered trademark of ScanSoft, Inc., and OpenSpeech, 
  * SpeechWorks and the SpeechWorks logo are registered trademarks or 
  * trademarks of SpeechWorks International, Inc. in the United States 
  * and other countries.
  *
  ***********************************************************************/
 

 /* $Id: SBtrdTimeOfDay.hpp,v 1.2.20.2 2003/10/06 17:59:33 mpanacci Exp $ */
 
 // -----1=0-------2=0-------3=0-------4=0-------5=0-------6=0-------7=0-------8
 
 #ifndef _SBTRD_TIMEOFDAY_H__
 #define _SBTRD_TIMEOFDAY_H__
 
 #include "VXIheaderPrefix.h"          // For SYMBOL_EXPORT_CPP_DECL
 #include <time.h>                     // For time_t
 #include "VXItypes.h"                 // For VXIint, VXIlong, etc.
 
 #ifdef SBTRDUTIL_EXPORTS
 #define SBTRDUTIL_API_CLASS SYMBOL_EXPORT_CPP_CLASS_DECL
 #else
 #define SBTRDUTIL_API_CLASS SYMBOL_IMPORT_CPP_CLASS_DECL
 #endif
 
 class SBTRDUTIL_API_CLASS SBtrdTimeOfDay {
  public:
   // Constructor and destructor
   SBtrdTimeOfDay();
   virtual ~SBtrdTimeOfDay();
 
   // Set the time to the current time of day
   bool SetCurrentTime( );
 
   // Clear the time
   void Clear( );
 
   // Offset the time of day
   void operator+= (VXIint32 offsetMs);
 
   // Determine if the time is set
   bool IsSet( ) const;
 
   // Get the milliseconds offset from the current time value
   VXIlong GetMsecBeforeTime( ) const;
   VXIlong GetMsecAfterTime( ) const;
 
   // Get the milliseconds offset from another time value
   VXIlong GetMsecBeforeTime (const SBtrdTimeOfDay &tod) const;
   VXIlong GetMsecAfterTime (const SBtrdTimeOfDay &tod) const;
 
   // Comparison operators
   bool operator< (const SBtrdTimeOfDay &tod) const;
   bool operator> (const SBtrdTimeOfDay &tod) const;
   bool operator== (const SBtrdTimeOfDay &tod) const;
 
  private:
   time_t          _time;
   unsigned short  _millitm;
 };
 
 #include "VXIheaderSuffix.h"
 
 #endif  // include guard
