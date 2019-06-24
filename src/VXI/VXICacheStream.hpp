
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
 
 #include "VXMLDocument.hpp"         // For SerializerInput, SerializerOutput
 #include "VXIcache.h"
 
 /*
 // VXICacheStream: Manages cache stream creation, access, and destruction.
 //
 // The typical usage will look something like this.  The stream is created from
 // a VXIcacheInterface and a VXIbyte array of specified length.
 //
 //     VXICacheStream stream(cacheInterface, buffer, bufferSize);
 //
 //     if (stream.IsValid()) {
 //       if (stream.IsWritable()) {
 //         Process_And_Write_New_Entry();
 //         stream.CommitEntry();
 //       }
 //       else Read_Existing_Entry();
 //     }
 //     else Handle_Error();
 //
 // There are two utility functions to determine if creation succeeded
 // (IsInvalid) and if the stream is writable (IsWritable).  If the entry is
 // successfully written, it may be committed.  Finally, the stream is closed
 // automatically by the destructor.
 */
 
 class VXICacheStream : public SerializerOutput, public SerializerInput {
 public:
   // SerializerOutput & public SerializerInput functions.
   virtual void Write(const VXIbyte * data, VXIulong size);
   virtual VXIulong Read(VXIbyte * buffer, VXIulong bufSize);
 
   void CommitEntry();
   // The entry was successfully written.  This will close the stream.
 
   void DiscardEntry();
   // A failure occured while writing the entry; discard it.  This will close
   // the stream.
 
   bool IsValid() const     { return !bad; }
   bool IsWritable() const  { return writable; }
   // Status checks after stream creation.
 
   VXICacheStream(VXIcacheInterface *, const VXIbyte * buffer, VXIulong size);
   ~VXICacheStream();
 
 private:
   VXICacheStream(const VXICacheStream &);                // not defined
   VXICacheStream & operator=(const VXICacheStream &);    // not defined
 
   bool writable;
   bool bad;
   VXIcacheStream * stream;
   VXIcacheInterface * cache;
 };
 
 
 inline void VXICacheStream::Write(const VXIbyte * data, VXIulong size)
 {
   VXIulong temp;
   if (cache->Write(cache, data, size, &temp, stream) != VXIcache_RESULT_SUCCESS
       || temp != size)
     throw VXMLDocument::SerializationError();
 }
 
 
 inline VXIulong VXICacheStream::Read(VXIbyte * buffer, VXIulong bufSize)
 {
   VXIulong temp;
   switch( cache->Read(cache, buffer, bufSize, &temp, stream) )
   {
     case VXIcache_RESULT_SUCCESS:
     case VXIcache_RESULT_END_OF_STREAM:
       break;
     default:
       throw VXMLDocument::SerializationError();
   }
   return temp;
 }
