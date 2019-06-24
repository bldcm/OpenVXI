
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
 
 #ifndef _SBCACHE_MISC_H__
 #define _SBCACHE_MISC_H__
 
 #include <string>                // For STL string and wstring classes
 #include <sys/types.h> 
 #include <sys/stat.h>            // For stat( )
 
 #include "VXIcache.h"            // For VXIcacheResult
 #include "SBinetLogger.hpp"      // For SBinetLogger
 #include "SBcacheLog.h"          // For logging defines
 #include "SBtrdMutex.hpp"        // For SBtrd[...]Mutex
 #include "SBtrdRefCount.hpp"     // For SBtrdRefCount
 
 // String types
 typedef std::string SBcacheNString;
 typedef std::basic_string<VXIchar> SBcacheString;
 
 // Mutex for the cache
 typedef SBtrdMutex SBcacheMutex;
 typedef SBtrdReaderWriterMutex SBcacheReaderWriterMutex;
 typedef SBtrdMutexPool SBcacheMutexPool;
 typedef SBtrdRefCount SBcacheRefCount;
 
 // Key for the cache
 typedef SBcacheString SBcacheKey;
 
 // Path for a cache entry
 class SBcachePath {
 public:
   static const char PATH_SEPARATOR;
 
 public:
   // Constructors and destructor
   SBcachePath( ) : _path(), _baseLen(0) { }
   SBcachePath (const SBcacheNString &base, const SBcacheNString &relative) :
     _path(base + PATH_SEPARATOR + relative), _baseLen(base.length( )) { }
   virtual ~SBcachePath( ) { }
   bool operator == (const SBcachePath& p) const { return _path == p._path; }
   bool operator != (const SBcachePath& p) const { return _path != p._path; }
 
   // Create the directory tree for the path
   VXIcacheResult CreateDirectories( ) const;
 
   // Clear the path
   void Clear( ) { _baseLen = 0; _path = ""; }
 
   // Accessors
   SBcacheNString::size_type length( ) const { return _path.length( ); }
   const SBcacheNString & str( ) const { return _path; }
   const char * const c_str( ) const { return _path.c_str( ); }
 
 private:
   SBcacheNString::size_type  _baseLen;
   SBcacheNString             _path;
 };
 
 
 // VXIcacheStream base class definition, read/write access to an entry
 struct VXIcacheStream : public SBinetLogger {
 public:
   // Constructor and destructor
   VXIcacheStream (VXIlogInterface      *log, 
 		  VXIunsigned           diagTagBase,
 		  const SBcacheString  &moduleName,
 		  const SBcacheKey     &key) : 
     SBinetLogger(MODULE_SBCACHE, log, diagTagBase), _moduleName(moduleName),
     _key(key) { }
   virtual ~VXIcacheStream( ) { }
 
   // Read and write
   virtual VXIcacheResult Read (VXIbyte          *buffer,
 			       VXIulong          buflen,
 			       VXIulong         *nread) = 0;
 
   virtual VXIcacheResult Write (const VXIbyte   *buffer,
 				VXIulong         buflen,
 				VXIulong        *nwritten) = 0;
 
   // Close
   virtual VXIcacheResult Close(bool invalidate) = 0;
 
   // Get the key name
   const SBcacheKey &GetKey( ) const { return _key; }
 
   // Get the module name
   const SBcacheString &GetModuleName( ) const { return _moduleName; }
 
 private:
   SBcacheString _moduleName;
   SBcacheKey    _key;
 };
 
 
 #ifdef WIN32
 #define WIN32_LEAN_AND_MEAN
 #include <windows.h>             // for CreateDirectory( )
 
 typedef struct _stat SBcacheStatInfo;
 inline bool SBcacheStat(const SBcacheNString &path, 
 			SBcacheStatInfo *statInfo) {
   return (_stat (path.c_str( ), statInfo) == 0 ? true : false); }
 
 inline bool SBcacheIsDir(const SBcacheStatInfo &statInfo) {
 #ifdef _S_ISDIR
   return (_S_ISDIR(statInfo->st_mode) ? true : false);
 #else
   return ((statInfo.st_mode & _S_IFDIR) ? true : false);
 #endif
 }
 
 inline bool SBcacheMkdir(const SBcacheNString &path) {
   return (CreateDirectory (path.c_str( ), NULL) ? true : false); }
 
 #else /* ! WIN32 */
 
 typedef struct stat SBcacheStatInfo;
 inline bool SBcacheStat(const SBcacheNString &path, 
 			SBcacheStatInfo *statInfo) {
   return (stat (path.c_str( ), statInfo) == 0 ? true : false); }
 
 inline bool SBcacheIsDir(const SBcacheStatInfo &statInfo) {
 #ifdef S_ISDIR
   return (S_ISDIR(statInfo.st_mode) ? true : false);
 #else
   return ((statInfo.st_mode & S_IFDIR) ? true : false);
 #endif
 }
 
 #ifdef S_IRWXU
 #define SBCACHE_DIR_MASK (S_IRWXU|S_IRWXG|S_IRWXO)
 #else
 #edfine SBCACHE_DIR_MASK 777
 #endif
 
 inline bool SBcacheMkdir(const SBcacheNString &path) {
   return (mkdir (path.c_str( ), SBCACHE_DIR_MASK) == 0 ? true : false); }
 
 #endif /* WIN32 */
 
 #endif /* include guard */
