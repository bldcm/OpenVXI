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
 

 static const char *rcsid = 0 ? (char *) &rcsid :
 "$Id: SBtrdTimeOfDay.cpp,v 7.1.20.2 2003/10/06 17:57:32 mpanacci Exp $";
 
 // -----1=0-------2=0-------3=0-------4=0-------5=0-------6=0-------7=0-------8
 
 #define SBTRDUTIL_EXPORTS
 #include "SBtrdTimeOfDay.hpp"           // For this class
 
 #ifdef WIN32
 #define WIN32_LEAN_AND_MEAN
 #include <windows.h>
 #endif
 
 #include <sys/types.h>
 #include <sys/timeb.h>                 // for ftime( )/_ftime( )
 
 #ifdef WIN32
 #define SBtrdFtime(x) _ftime(x)
 #define SBtrdTimeb struct _timeb
 #else
 #define SBtrdFtime(x) ftime(x)
 #define SBtrdTimeb struct timeb
 #endif
 
 // -----1=0-------2=0-------3=0-------4=0-------5=0-------6=0-------7=0-------8
 
 
 // Constructor
 SBTRDUTIL_API_CLASS SBtrdTimeOfDay::SBtrdTimeOfDay( ) :
   _time(0), _millitm(0)
 {
 }
 
 
 // Destructor
 SBTRDUTIL_API_CLASS SBtrdTimeOfDay::~SBtrdTimeOfDay( )
 {
 }
 
 
 // Set the time to the current time of day
 SBTRDUTIL_API_CLASS bool SBtrdTimeOfDay::SetCurrentTime( )
 {
   SBtrdTimeb tbuf;
   SBtrdFtime(&tbuf);
   _time = tbuf.time;
   _millitm = tbuf.millitm;
 
   return true;
 }
 
 
 // Clear the time
 SBTRDUTIL_API_CLASS void SBtrdTimeOfDay::Clear( ) 
 {
   _time = 0;
   _millitm = 0;
 }
 
 
 // Offset the time of day
 SBTRDUTIL_API_CLASS void SBtrdTimeOfDay::operator+=(VXIint32 offsetMs)
 {
   _time += static_cast<time_t>(offsetMs / 1000);
   _millitm += static_cast<unsigned short>(offsetMs % 1000);
   if ( _millitm > 1000 ) {
     _time++;
     _millitm -= 1000;
   }
 }
 
 
 // Determine if the time is set
 SBTRDUTIL_API_CLASS bool SBtrdTimeOfDay::IsSet( ) const 
 {
   return ((_time) || (_millitm) ? true : false);
 }
 
 
 // Get the milliseconds offset from the current time value
 SBTRDUTIL_API_CLASS VXIlong SBtrdTimeOfDay::GetMsecBeforeTime( ) const
 {
   SBtrdTimeOfDay current;
   current.SetCurrentTime( );
   return (_time - current._time) * 1000 + (_millitm - current._millitm);
 }
 
 
 // Get the milliseconds offset from the current time value
 SBTRDUTIL_API_CLASS VXIlong SBtrdTimeOfDay::GetMsecAfterTime( ) const 
 {
   return -GetMsecBeforeTime( );
 }
 
 
 // Get the milliseconds offset from another time value
 SBTRDUTIL_API_CLASS VXIlong 
 SBtrdTimeOfDay::GetMsecBeforeTime (const SBtrdTimeOfDay &tod) const
 {
   return (_time - tod._time) * 1000 + (_millitm - tod._millitm);
 }
 
 
 // Get the milliseconds offset from another time value
 SBTRDUTIL_API_CLASS VXIlong 
 SBtrdTimeOfDay::GetMsecAfterTime (const SBtrdTimeOfDay &tod) const 
 {
   return -GetMsecBeforeTime (tod); 
 }
 
 
 // Comparison operator
 SBTRDUTIL_API_CLASS bool 
 SBtrdTimeOfDay::operator< (const SBtrdTimeOfDay &tod) const 
 {
   return ((_time < tod._time) || 
           ((_time == tod._time) && (_millitm < tod._millitm)));
 }
 
 
 // Comparison operator
 SBTRDUTIL_API_CLASS bool 
 SBtrdTimeOfDay::operator> (const SBtrdTimeOfDay &tod) const 
 {
   return ((_time > tod._time) || 
           ((_time == tod._time) && (_millitm > tod._millitm)));
 }
 
 
 // Comparison operator
 SBTRDUTIL_API_CLASS bool 
 SBtrdTimeOfDay::operator== (const SBtrdTimeOfDay &tod) const 
 {
   return ((_time != tod._time) || (_millitm != tod._millitm));
 }
