/* SBtrdRefCount, base class for managing reference counts */

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
 

 /* $Id: SBtrdRefCount.hpp,v 7.1.32.2 2003/10/06 17:59:33 mpanacci Exp $ */
 
 // -----1=0-------2=0-------3=0-------4=0-------5=0-------6=0-------7=0-------8
 
 #ifndef _SBTRD_REFCOUNT_H__
 #define _SBTRD_REFCOUNT_H__
 
 #include "VXIheaderPrefix.h"          // For SYMBOL_EXPORT_CPP_DECL
 #include "VXItrd.h"                   // For error codes
 
 #ifdef SBTRDUTIL_EXPORTS
 #define SBTRDUTIL_API_CLASS SYMBOL_EXPORT_CPP_CLASS_DECL
 #else
 #define SBTRDUTIL_API_CLASS SYMBOL_IMPORT_CPP_CLASS_DECL
 #endif
 
 class SBtrdMutex;
 
 class SBTRDUTIL_API_CLASS SBtrdRefCount {
  public:
   // Constructor and destructor, pass NULL for the mutex if mutex
   // protection is not required. Note that this class assumes the
   // passed mutex will be shared across many instances to avoid having
   // huge numbers of mutexes in the system, see above.
   SBtrdRefCount (SBtrdMutex *mutex) : _refCount(1), _mutex(mutex) { }
   virtual ~SBtrdRefCount( ) { }
 
   // Add a reference and release a reference. Note: Release sets the
   // pointer to NULL.
   static VXItrdResult AddRef  (SBtrdRefCount *obj);
   static VXItrdResult Release (SBtrdRefCount **obj);
 
   // Get a unique copy if this is being shared in preparation for a
   // write operation, don't want to change this under someone else's
   // feet. The passed pointer will be changed if this is being shared.
   // The passed mutex is used for future use by the new copy if a new
   // copy is made, if NULL is passed the mutex used for the original
   // copy will be shared.
   static VXItrdResult GetUniqueCopy (SBtrdRefCount **obj,
 				     SBtrdMutex *mutex);
   
   // Copy constructor for deep copies of derrived class during
   // GetUniqueCopy( )
   SBtrdRefCount (const SBtrdRefCount &r) : _refCount(1), _mutex(r._mutex) { }
 
  protected:
   // Ability to specify the mutex after the constructor
   void SetRefCountMutex (SBtrdMutex *mutex) { _mutex = mutex; }
 
   // Used to allocate a copy of the real derrived class when we don't
   // know/care what the real derrived class is, must be implemented by
   // all derrived classes. Default implementation here is to not
   // support this, meaning GetUniqueCopy( ) is not supported.
   virtual SBtrdRefCount *AllocateCopy( ) const { return NULL; }
 
  private:
   // Disable the assignment operator, just doesn't make sense to do
   // this on the actual derrived class (need to have a wrapper class
   // that does so by copying the pointer to the derrived class and
   // adding a reference)
   SBtrdRefCount & operator= (const SBtrdRefCount &r);
 
  private:
   unsigned long  _refCount;
   SBtrdMutex    *_mutex;
 };
 
 #include "VXIheaderSuffix.h"
 
 #endif  // _SBTRD_REFCOUNT_H__
