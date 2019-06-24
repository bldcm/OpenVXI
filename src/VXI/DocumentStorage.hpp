
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
 

 #include "VXMLDocument.hpp"          // for VXMLDocument and document model
 #include "VXItrd.h"                  // for ThreadMutex
 #include <map>
 
 class DocumentStorageKey {
 public:
   DocumentStorageKey()
   { memset(raw, 0, 16 * sizeof(unsigned char)); }
 
   DocumentStorageKey(const DocumentStorageKey & x)
   { memcpy(raw, x.raw, 16 * sizeof(unsigned char)); }
 
   DocumentStorageKey & operator=(const DocumentStorageKey & x)
   { if (this != &x) { memcpy(raw, x.raw, 16 * sizeof(unsigned char)); }
     return *this; }
 
   unsigned char raw[16];
 };
 
 
 inline bool operator< (const DocumentStorageKey & x,
                        const DocumentStorageKey & y)
 { return memcmp(x.raw, y.raw, 16 * sizeof(unsigned char)) < 0; }
 
 inline bool operator==(const DocumentStorageKey & x,
                        const DocumentStorageKey & y)
 { return memcmp(x.raw, y.raw, 16 * sizeof(unsigned char)) == 0; }
 
 // ------*---------*---------*---------*---------*---------*---------*---------
 
 class DocumentStorageRecord {
 private:
   VXMLDocument  doc;
   unsigned int  size;
   unsigned long score;
 
 public:
   VXMLDocument & GetDoc(unsigned long L)    { score = L + size; return doc; }
   unsigned long GetScore() const            { return score; }
   unsigned int  GetSize()  const            { return size; }
 
   DocumentStorageRecord(VXMLDocument & d, unsigned int s, unsigned long L)
     : doc(d), size(s), score(L + s) { }
 
   DocumentStorageRecord() : doc(0), size(0), score(0) { }
 
   DocumentStorageRecord(const DocumentStorageRecord & x)
   { doc = x.doc; size = x.size; score = x.score; }
 
   DocumentStorageRecord & operator=(const DocumentStorageRecord & x)
   { if (this != &x) { doc = x.doc; size = x.size; score = x.score; }
     return *this; }
 };
 
 // ------*---------*---------*---------*---------*---------*---------*---------
 
 class DocumentStorageSingleton {
 public:
   static void Initialize(unsigned int cacheSize);
   static void Deinitialize();
 
   static DocumentStorageSingleton * Instance()
   { return DocumentStorageSingleton::ds; }
 
   bool Retrieve(VXMLDocument & doc, const VXIbyte * buffer, VXIulong bufSize);
   void Store(VXMLDocument & doc, const VXIbyte * buffer, VXIulong bufSize);
 
 private:
   DocumentStorageKey GenerateKey(const VXIbyte * buffer, VXIulong bufSize);
 
 private:
   typedef std::map<DocumentStorageKey, DocumentStorageRecord> DOC_STORAGE;
 
   VXItrdMutex * mutex;
   unsigned int bytes;
 
   DOC_STORAGE storage;
   unsigned long GreedyDualL;
 
   DocumentStorageSingleton()   { VXItrdMutexCreate(&mutex);  bytes = 0; 
                                  GreedyDualL = 0; }
   ~DocumentStorageSingleton()  { VXItrdMutexDestroy(&mutex); }
 
   static unsigned long maxBytes;
   static DocumentStorageSingleton * ds;
 };
