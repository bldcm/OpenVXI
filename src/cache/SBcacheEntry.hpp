
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
 
 #ifndef _SBCACHE_ENTRY_H__
 #define _SBCACHE_ENTRY_H__
 
 #include <time.h>             // for time_t
 
 #include "VXIcache.h"         // For VXIcacheResult, CACHE_[...]
 #include "SBcacheMisc.hpp"    // For SBcacheString, SBcacheKey, SBcachePath
 
 // Forward declarations
 extern "C" struct VXIlogInterface;
 class SBcacheEntryDetails;
 
 // SBcacheEntry, cache entry
 class SBcacheEntry {
 public:
   // Constructor and destructor
   SBcacheEntry( ) : _details(NULL) { }
   SBcacheEntry (SBcacheEntryDetails *details);
   virtual ~SBcacheEntry( );
   
   // Create the entry
   VXIcacheResult Create(VXIlogInterface *log, 
 			VXIunsigned diagTagBase,
 			SBcacheMutex *refCountMutex);
 
   // Open the entry
   VXIcacheResult Open(VXIlogInterface       *log,
 		      const SBcacheString   &moduleName,
 		      const SBcacheKey      &key,
 		      const SBcachePath     &path,
 		      VXIcacheOpenMode       mode,
 		      VXIint32               flags,
 		      VXIulong               maxSizeBytes,
 		      const VXIMap          *properties,
 		      VXIMap                *streamInfo,
 		      VXIcacheStream       **stream);
 
   // Unlock the entry
   VXIcacheResult Unlock(VXIlogInterface     *log);
 
   // Accessors
   bool IsLocked( ) const;
   bool IsExpired (time_t cutoffTime, time_t *lastAccessed) const;
   const SBcacheKey & GetKey( ) const;
   const SBcachePath & GetPath( ) const;
   VXIulong GetSizeBytes (bool haveEntryOpen = false) const;
 
   // Error logging
   VXIlogResult LogIOError (VXIunsigned errorID) const;
 
   // Copy constructor and assignment operator
   SBcacheEntry(const SBcacheEntry &entry);
   SBcacheEntry &operator=(const SBcacheEntry &entry);
 
   // Comparison operators, smaller is defined as a preference for
   // deleting this entry first, equality is having equal preference
   bool operator< (const SBcacheEntry &entry) const;
   bool operator> (const SBcacheEntry &entry) const;
   bool operator== (const SBcacheEntry &entry) const;
   bool operator!= (const SBcacheEntry &entry) const;
 
   bool Equivalent(const SBcacheEntry &other) const { return _details == other._details; }
 
 public:
   // Only for SBcacheStream use
 
   // Close
   VXIcacheResult Close (VXIlogInterface  *log,
 			VXIcacheOpenMode  mode, 
 			VXIunsigned       sizeBytes,
 			bool              invalidate);
 
 private:
   SBcacheEntryDetails  *_details;
 };
 
 #endif /* include guard */
