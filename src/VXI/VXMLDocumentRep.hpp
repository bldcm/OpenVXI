
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
 

 #include "DocumentModel.hpp"
 
 class VXMLDocumentRep {
 public:
   const VXMLNodeRef * GetRoot() const;
 
   void StartElement(VXMLElementType n);
   void PruneWhitespace();
   void EndElement();
 
   void AddContent(const VXIchar * c, unsigned int len);
   void AddAttribute(VXMLAttributeType name, const vxistring & attr);
 
   bool GetAttribute(VXMLAttributeType key, vxistring & attr) const;
 
   VXMLDocumentRep();
 
   static void AddRef(VXMLDocumentRep * t);
   static void Release(VXMLDocumentRep * & t);
 
   VXMLElementType GetParentType() const;
 
 private:
   void AddChild(VXMLNodeRef *);
 
 private:
   ~VXMLDocumentRep();                                   // only used by Release
   VXMLDocumentRep(const VXMLDocumentRep &);             // not implemented
   VXMLDocumentRep & operator=(const VXMLDocumentRep &); // not implemented
 
   const VXMLNodeRef * root;
   VXMLNodeRef * pos;
   VXMLNode::VXMLNodeType posType;
   int count;
 };
