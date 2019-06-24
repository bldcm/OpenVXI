
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
 
/***********************************************************************
  *
  * Routines to handle caching of documents and resources.
  *
  ***********************************************************************/
 
 #include "VXICacheStream.hpp"
 
 
 VXICacheStream::VXICacheStream(VXIcacheInterface * cache,
                                const VXIbyte * buffer,
                                VXIulong bufferSize)
   : writable(false), bad(false), stream(NULL), cache(NULL)
 {
   if (cache == NULL || buffer == NULL || bufferSize == 0) {
     bad = true;
     return;
   }
 
   VXIcacheStream * stream = NULL;
   VXIcacheResult rc = cache->OpenEx(cache, L"VXI", buffer, bufferSize,
                                     CACHE_MODE_READ_CREATE, CACHE_FLAG_NULL,
                                     NULL, NULL, &stream);
 
   switch (rc) {
   case VXIcache_RESULT_ENTRY_CREATED: // We have write permission.
     writable = true;
     break;
   case VXIcache_RESULT_SUCCESS: // Already in cache, just read it in.
     writable = false;
     break;
   default:
     bad = true;
     return;
   }
 
   this->cache = cache;
   this->stream = stream;
   return;
 }
 
 
 void VXICacheStream::CommitEntry()
 {
   if (cache != NULL && stream != NULL && writable) {
     cache->CloseEx(cache, true, &stream);
     stream = NULL; // just in case
   }
 }
 
 
 void VXICacheStream::DiscardEntry()
 {
   if (cache != NULL && stream != NULL && writable) {
     cache->CloseEx(cache, false, &stream);
     stream = NULL; // just in case
   }
 }
 
 
 VXICacheStream::~VXICacheStream()
 {
   if (cache == NULL || stream == NULL) return;
   cache->CloseEx(cache, !writable, &stream);
 }
