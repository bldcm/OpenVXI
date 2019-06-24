
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
 

 #include "DocumentStorage.hpp"
 #include "md5.h"
 
 DocumentStorageSingleton * DocumentStorageSingleton::ds = NULL;
 unsigned long DocumentStorageSingleton::maxBytes = 1024*1024;
 
 // ------*---------*---------*---------*---------*---------*---------*---------
 // About GreedyDual caching
 //
 // This is a simple functions with the following desirable characteristics.
 //  * Recently accessed items remain for a time
 //  * Frequently accessed items tend to remain in cache
 //  * Very few parameters are required and the computational cost is small
 // 
 // Initialize the limit, L, to 0.
 // For every document, d:
 //
 //   1. If d is in memory, set H(p) = L + c(p)
 //   2. If d is not in memory
 //      a. While there is not room, set L to the lowest score and delete
 //         documents having that score.
 //      b. Store d in memory and set H(d) = L + c(p)
 //
 // ------*---------*---------*---------*---------*---------*---------*---------
 
 
 void DocumentStorageSingleton::Initialize(unsigned int cacheSize)
 {
   DocumentStorageSingleton::ds = new DocumentStorageSingleton(); 
   DocumentStorageSingleton::maxBytes = cacheSize * 1024;
 }
 
 void DocumentStorageSingleton::Deinitialize()
 { delete DocumentStorageSingleton::ds; }
 
 void DocumentStorageSingleton::Store(VXMLDocument & doc,
                                      const VXIbyte * buffer,
                                      VXIulong bufSize)
 {
   // Handle the case where the memory cache is disabled.
   if (bufSize > maxBytes) return;
 
   // Generate key.
   DocumentStorageKey key = GenerateKey(buffer, bufSize);
 
   VXItrdMutexLock(mutex);
   while (bytes + bufSize > maxBytes) {
     // Buffer is full.
 
     DOC_STORAGE::iterator minIterator = storage.end();
     unsigned long minScore = 0;
 
     for (DOC_STORAGE::iterator i = storage.begin(); i != storage.end(); ++i) {
       if ((*i).second.GetScore() > minScore) continue;
       minScore = (*i).second.GetScore();
       minIterator = i;
     }
 
     bufSize -= (*minIterator).second.GetSize();
     storage.erase(minIterator);
 
     // Note: In a two level cache, this document might be written to disk before
     // being flushed from memory.
   }
 
   DOC_STORAGE::iterator i = storage.find(key);
   if (i == storage.end()) {
     storage[key] = DocumentStorageRecord(doc, bufSize, GreedyDualL);
     bytes += bufSize;
   }
   VXItrdMutexUnlock(mutex);
 }
 
 
 bool DocumentStorageSingleton::Retrieve(VXMLDocument & doc,
                                         const VXIbyte * buffer,
                                         VXIulong bufSize)
 {
   // Handle the case where the memory cache is disabled.
   if (bufSize > maxBytes) return false;
 
   bool result = false;
   VXItrdMutexLock(mutex);
   DOC_STORAGE::iterator i = storage.find(GenerateKey(buffer, bufSize));
   if (i != storage.end()) {
     doc = (*i).second.GetDoc(GreedyDualL);
     result = true;
   }
   if (result == false) {
     // Note: In a two level cache scheme, this is where the data would be read
     //       back from disk.  In this one level implementation, no action is
     //       performed.
   }
   VXItrdMutexUnlock(mutex);
 
   return result;
 }
 
 
 DocumentStorageKey DocumentStorageSingleton::GenerateKey(const VXIbyte * buf,
                                                          VXIulong bufSize)
 {
   // Create a MD5 digest for the key
   MD5_CTX md5;
   MD5Init (&md5);
   MD5Update (&md5, buf, bufSize);
 
   DocumentStorageKey temp;
   MD5Final (temp.raw, &md5);
   return temp;
 }
