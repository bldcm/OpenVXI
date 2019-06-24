
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
 

 // -----1=0-------2=0-------3=0-------4=0-------5=0-------6=0-------7=0-------8
 
 #ifndef _SBCACHE_COUNTER_H__
 #define _SBCACHE_COUNTER_H__
 
 #include <limits.h>           // For ULONG_MAX
 
 #include "SBcacheMisc.hpp"    // For SBcacheReaderWriterMutex
 
 class SBcacheCounter {
 public:
   // Constructor and destructor
   SBcacheCounter (VXIulong counter) : _mutex( ), _counter(counter) { }
   ~SBcacheCounter( ) { }
 
   // Creation method
   VXItrdResult Create( ) { return _mutex.Create (L"SBcacheCounter mutex"); }
 
   // Increment and test, returns an integer greater then, less then,
   // or equal to zero based on whether the resulting counter is greater
   // then, less then, or equal to the limit
   int IncrementTest (VXIulong increment, VXIulong limit) {
     int rc = -1;
     if ( _mutex.Lock( ) == VXItrd_RESULT_SUCCESS ) {
       _counter += increment;
       if ( _counter > limit )
 	rc = 1;
       else if ( _counter == limit )
 	rc = 0;
       _mutex.Unlock( );
     }
     return rc;
   }
 
   // See above
   int DecrementTest (VXIulong decrement, VXIulong limit) {
     int rc = -1;
     if ( _mutex.Lock( ) == VXItrd_RESULT_SUCCESS ) {
       _counter -= decrement;
       if ( _counter > limit )
 	rc = 1;
       else if ( _counter == limit )
 	rc = 0;
       _mutex.Unlock( );
     }
     return rc;
   }
 
   // Increment by 1 as a sequence number
   VXIulong IncrementSeqNum( ) {
     VXIulong res = 0;
     if ( _mutex.Lock( ) == VXItrd_RESULT_SUCCESS ) {
       res = _counter;
       if ( _counter == ULONG_MAX )
 	_counter = 1;
       else
 	_counter++;
       _mutex.Unlock( );
     }
     return res;
   }
 
   // Decrement
   VXIulong Decrement (VXIulong decrement) {
     VXIulong res = 0;
     if ( _mutex.Lock( ) == VXItrd_RESULT_SUCCESS ) {
       if ( _counter > decrement )
 	_counter -= decrement;
       else
 	_counter = 0;
       res = _counter;
       _mutex.Unlock( );
     }
     return res;
   }
 
   // Get the counter
   VXIulong Get( ) const {
     VXIulong counter = 0;
     if ( _mutex.Lock( ) == VXItrd_RESULT_SUCCESS ) {
       counter = _counter;
       _mutex.Unlock( );
     }
     return counter;
   }
 
   // Reset the counter
   void Reset (VXIulong counter) {
     if ( _mutex.Lock( ) == VXItrd_RESULT_SUCCESS ) {
       _counter = counter;
       _mutex.Unlock( );
     }
   }
 
 private:
   // Disabled assignment operator and copy constructor
   SBcacheCounter (const SBcacheCounter &s);
   SBcacheCounter & operator= (const SBcacheCounter &s);
 
 private:
   SBcacheMutex  _mutex;
   VXIulong      _counter;
 };
 
 #endif /* include guard */
