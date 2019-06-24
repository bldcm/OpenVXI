
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
 

 // Xerces related
 #ifndef HAVE_XERCES
 #error Need Apache Xerces to build the VoiceXML interpreter
 #endif
 #include <sax2/ContentHandler.hpp>
 using namespace xercesc;
 #include "VXML.h"
 #include <string>
 
 typedef std::basic_string<VXIchar> vxistring;
 
 class VXMLDocumentRep;
 
 class DocumentConverter : public ContentHandler {
 public:
   static bool Initialize();
   // One time initialization.
   //
   // Returns: True - initialization succeeded.
   //          False - initialization failed.
 
   static void Deinitialize();
   // One time cleanup of DocumentParser interface.
 
 public:
   DocumentConverter();
   virtual ~DocumentConverter();
 
   virtual void startDocument();
   virtual void endDocument();
   virtual void setDocumentLocator(const Locator* const locator);
 
   virtual void startElement(const XMLCh* const uri,
                             const XMLCh* const localname,
                             const XMLCh* const qname,
                             const Attributes&  attrs);
   virtual void endElement(const XMLCh* const uri,
                           const XMLCh* const localname,
                           const XMLCh* const qname);
 
   virtual void characters(const XMLCh* const chars,
                           const unsigned int length);
   virtual void ignorableWhitespace(const XMLCh* const chars,
                                    const unsigned int length);
 
   virtual void processingInstruction(const XMLCh* const target,
                                      const XMLCh* const data);
 
   virtual void startPrefixMapping(const XMLCh* const prefix,
                                   const XMLCh* const uri)
   { }
 
   virtual void endPrefixMapping(const XMLCh* const prefix)
   { }
 
   virtual void skippedEntity(const XMLCh* const name)
   { }
 
   VXMLDocumentRep * GetDocument();
   // This function releases the document representation to the caller.
   
   float GetXMLVersion(void) { return version; }
   
   void ResetDocument();
 
   void SetBaseUri(const VXIchar* const base)
   { if( base != NULL ) baseUri = base; else baseUri=L""; }
   
   const vxistring & GetBaseUri(void) { return baseUri; }
 
   void SetGrammarType(const VXIchar* const grammartype)
   { if( grammartype != NULL ) grammarType = grammartype; else grammarType=L""; }
   
   const vxistring & GetGrammarType(void) { return grammarType; }
 
 private:
   void ParseException(const VXIchar * message) const;
 
   void ProcessNodeAttribute(VXMLElementType elemType, int attrType,
                             const VXIchar* const value);
 
   void ProcessNodeFinal(VXMLElementType elemType);
 
   bool IsIgnorable(int elemType);
 
   void CopyElementStart(const XMLCh* const localname, const Attributes &attrs);
   void CopyElementClose(const XMLCh* const localname);
   void StartImplicitPrompt();
   void EndImplicitPrompt();
 
   const Locator * locator;
   VXMLDocumentRep * doc;
   float version;
   bool strict;
   vxistring baseUri;
   vxistring grammarType;
   vxistring defaultLang;
   vxistring documentLang;
   
   // Ignoring for <metadata>
   int ignoreDepth;
 
   // Used to detect attribute vs. content conflicts.
   bool contentForbidden;    // Is content forbidden within this element?
   bool hasContent;          // Did content appear.
 
   // Menu <choice> numbering
   int choiceNumber;         // This is used to number choices in a menu.
 
   // Copying of <prompt> and <grammar> contents
   bool implicitPrompt;      // Is this an implicit prompt (see hasValidPCDATA)?
   bool explicitPrompt;      // Are we inside a <prompt>?
   bool inGrammar;           // Are we inside a <grammar>?
   int copyDepth;            // How deeply are we copying elements?
   bool pcdataImpliesPrompt; // False indicates PCDATA is valid and not SSML
 };
