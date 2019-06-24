#ifndef _VXML_Document_HPP
 #define _VXML_Document_HPP

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
  * Definition of a VXML document and its serialization interface.
  *
  ***********************************************************************/
 
 #include "DocumentModel.hpp"
 
 class SerializerOutput {
 public:
   virtual void Write(const VXIbyte * data, VXIulong size) = 0;
 
   SerializerOutput()  { }
   virtual ~SerializerOutput()  { }
 };
 
 
 class SerializerInput {
 public:
   virtual VXIulong Read(VXIbyte * buffer, VXIulong bufSize) = 0;
 
   SerializerInput()  { }
   virtual ~SerializerInput()  { }
 };
 
 
 class VXMLDocument {
 public:
   VXMLDocument(VXMLDocumentRep * = NULL);
   VXMLDocument(const VXMLDocument &);
   VXMLDocument & operator=(const VXMLDocument &);
   ~VXMLDocument();
 
   VXMLElement GetRoot() const;
 
   void WriteDocument(SerializerOutput &) const;
   void ReadDocument(SerializerInput &);
   
   class SerializationError { };
   // This exception may be generated during serialization attempts.
 
 private:
   VXMLDocumentRep * internals;
 };
 
 #endif // #define _VXML_Document_HPP
