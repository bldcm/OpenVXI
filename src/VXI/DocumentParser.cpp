
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
 

 #include "DocumentParser.hpp"
 #include "DocumentStorage.hpp"
 #include "CommonExceptions.hpp"
 #include "SimpleLogger.hpp"
 #include "VXML.h"                    // for attribute names
 #include "DocumentConverter.hpp"     // for DocumentConverter
 #include "PropertyList.hpp"
 #include "XMLChConverter.hpp"
 #include "ValueLogging.hpp"     // for VXIValue dumping
 #include "VXIinet.h"
 
 // Internal documents
 #include "Schema.hpp"
 
 // Xerces related
 #ifndef HAVE_XERCES
 #error Need Apache Xerces to build the VoiceXML interpreter
 #endif
 
 #include <util/PlatformUtils.hpp>
 #include <util/TransService.hpp>
 #include <sax2/XMLReaderFactory.hpp>
 #include <framework/MemBufInputSource.hpp>
 #include <sax/SAXParseException.hpp> // by DOMTreeErrorReporter
 #include <sax/EntityResolver.hpp>    // by DTDResolver
 #include <sax/ErrorHandler.hpp>      // by DOMTreeErrorReporter
 #include <validators/common/Grammar.hpp>
 using namespace xercesc;
 
 //#############################################################################
 // Utilities - these are specific to Xerces
 //#############################################################################
 
 class DOMTreeErrorReporter : public ErrorHandler {
 public:
   DOMTreeErrorReporter()  { }
   ~DOMTreeErrorReporter() { }
   
   void warning(const SAXParseException& toCatch)     { /* Ignore */ }
   void fatalError(const SAXParseException& toCatch)  { error(toCatch); }
   void resetErrors() { }
 
   void error(const SAXParseException & toCatch)
   { throw SAXParseException(toCatch); }
 
 private:
   DOMTreeErrorReporter(const DOMTreeErrorReporter &);
   void operator=(const DOMTreeErrorReporter &);
 };
 
 
 class DTDResolver : public EntityResolver {
 public:
   virtual ~DTDResolver() { }
   DTDResolver() { }
 
   virtual InputSource * resolveEntity(const XMLCh * const publicId,
                                       const XMLCh * const systemId)
   {
 
     if (Compare(publicId, L"SB_Defaults")) {
       VXIcharToXMLCh name(L"VXML Defaults DTD (for SB 1.0)");
       return new MemBufInputSource(VALIDATOR_DATA + VALIDATOR_DEFAULTS_DTD,
                                    VALIDATOR_DEFAULTS_DTD_SIZE,
                                    name.c_str(), false);
     }
 
     if (Compare(systemId, L"http://www.w3.org/TR/voicexml20/vxml.xsd")) {
       VXIcharToXMLCh name(L"http://www.w3.org/TR/voicexml20/vxml.xsd (SB)");
       return new MemBufInputSource(VALIDATOR_DATA + VALIDATOR_VXML,
                                    VALIDATOR_VXML_SIZE,
                                    name.c_str(), false);
     }
 
     if (Compare(systemId, L"vxml-datatypes.xsd")) {
       VXIcharToXMLCh name(L"vxml-datatypes.xsd (SB)");
       return new MemBufInputSource(VALIDATOR_DATA + VALIDATOR_VXML_DATA,
                                    VALIDATOR_VXML_DATA_SIZE,
                                    name.c_str(), false);
     }
 
     if (Compare(systemId, L"vxml-attribs.xsd")) {
       VXIcharToXMLCh name(L"vxml-attribs.xsd (SB)");
       return new MemBufInputSource(VALIDATOR_DATA + VALIDATOR_VXML_ATTR,
                                    VALIDATOR_VXML_ATTR_SIZE,
                                    name.c_str(), false);
     }
 
     if (Compare(systemId, L"vxml-grammar-extension.xsd")) {
       VXIcharToXMLCh name(L"vxml-grammar-extension.xsd (SB)");
       return new MemBufInputSource(VALIDATOR_DATA + VALIDATOR_SRGF_EXTN,
                                    VALIDATOR_SRGF_EXTN_SIZE,
                                    name.c_str(), false);
     }
 
     if (Compare(systemId, L"vxml-grammar-restriction.xsd")) {
       VXIcharToXMLCh name(L"vxml-grammar-restriction.xsd (SB)");
       return new MemBufInputSource(VALIDATOR_DATA + VALIDATOR_SRGF_RSTR,
                                    VALIDATOR_SRGF_RSTR_SIZE,
                                    name.c_str(), false);
     }
 
     if (Compare(systemId, L"vxml-synthesis-extension.xsd")) {
       VXIcharToXMLCh name(L"vxml-synthesis-extension.xsd (SB)");
       return new MemBufInputSource(VALIDATOR_DATA + VALIDATOR_SSML_EXTN,
                                    VALIDATOR_SSML_EXTN_SIZE,
                                    name.c_str(), false);
     }
 
     if (Compare(systemId, L"vxml-synthesis-restriction.xsd")) {
       VXIcharToXMLCh name(L"vxml-synthesis-restriction.xsd (SB)");
       return new MemBufInputSource(VALIDATOR_DATA + VALIDATOR_SSML_RSTR,
                                    VALIDATOR_SSML_RSTR_SIZE,
                                    name.c_str(), false);
     }
 
     if (Compare(systemId, L"synthesis-core.xsd")) {
       VXIcharToXMLCh name(L"synthesis-core.xsd (SB)");
       return new MemBufInputSource(VALIDATOR_DATA + VALIDATOR_SSML_CORE,
                                    VALIDATOR_SSML_CORE_SIZE,
                                    name.c_str(), false);
     }
 
     if (Compare(systemId, L"grammar-core.xsd")) {
       VXIcharToXMLCh name(L"grammar-core.xsd (SB)");
       return new MemBufInputSource(VALIDATOR_DATA + VALIDATOR_SRGF_CORE,
                                    VALIDATOR_SRGF_CORE_SIZE,
                                    name.c_str(), false);
     }
 
     if (Compare(systemId, L"http://www.w3.org/2001/xml.xsd")) {
       VXIcharToXMLCh name(L"http://www.w3.org/2001/xml.xsd (SB)");
       return new MemBufInputSource(VALIDATOR_DATA + VALIDATOR_XML,
                                    VALIDATOR_XML_SIZE,
                                    name.c_str(), false);
     }
 
     if (Compare(systemId, L"XMLSchema.dtd")) {
       VXIcharToXMLCh name(L"XMLSchema.dtd (SB)");
       return new MemBufInputSource(VALIDATOR_DATA + VALIDATOR_SCHEMA_DTD,
                                    VALIDATOR_SCHEMA_DTD_SIZE,
                                    name.c_str(), false);
     }
     
     if (Compare(systemId, L"datatypes.dtd")) {
       VXIcharToXMLCh name(L"datatypes.dtd (SB)");
       return new MemBufInputSource(VALIDATOR_DATA + VALIDATOR_DATATYPE_DTD,
                                    VALIDATOR_DATATYPE_DTD_SIZE,
                                    name.c_str(), false);
     }
 
     if (Compare(publicId, L"-//W3C//DTD VOICEXML 2.0//EN") ||
         Compare(systemId, L"http://www.w3.org/TR/voicexml20/vxml.dtd"))
     {
       VXIcharToXMLCh name(L"VXML DTD (SB)");
       return new MemBufInputSource(VALIDATOR_DATA + VALIDATOR_VXML_DTD,
                                    VALIDATOR_VXML_DTD_SIZE,
                                    name.c_str(), false);
     }
 
     /*
     VXIcharToXMLCh name(L"VXML DTD (for SB 1.0)");
     return new MemBufInputSource(VALIDATOR_DATA + VALIDATOR_VXML_DTD,
                                  VALIDATOR_VXML_DTD_SIZE,
                                  name.c_str(), false);
     */
 
     return NULL;
   }
 };
 
 //#############################################################################
 // Document Parser
 //#############################################################################
 
 // xerces crashes when on multi-thread app. that simultaneously load schema
 // grammar therefore use a global mutext to restrict access to only thread
 // at a time
 static VXItrdMutex* gblXMLGrammarMutex = NULL;
 
 bool DocumentParser::Initialize(unsigned int cacheSize)
 {
   try {
     XMLPlatformUtils::Initialize();
     if (!VXMLDocumentModel::Initialize()) return false;
     DocumentConverter::Initialize();
     VXItrdMutexCreate(&gblXMLGrammarMutex);
   }
 
   catch (const XMLException &) {
     return false;
   }
 
   DocumentStorageSingleton::Initialize(cacheSize);
 
   return true;
 }
 
 
 void DocumentParser::Deinitialize()
 {
   DocumentStorageSingleton::Deinitialize();
 
   try {
     VXItrdMutexDestroy(&gblXMLGrammarMutex);
     DocumentConverter::Deinitialize();
     VXMLDocumentModel::Deinitialize();
     XMLPlatformUtils::Terminate();
   }
   catch (const XMLException &) {
     // do nothing
   }
 }
 
 static void LockLoadGrammar(void)
 {
   if( gblXMLGrammarMutex ) VXItrdMutexLock(gblXMLGrammarMutex);
 }
 
 static void UnlockLoadGrammar(void)
 {
   if( gblXMLGrammarMutex ) VXItrdMutexUnlock(gblXMLGrammarMutex);
 }  
 
 DocumentParser::DocumentParser()
   : parser(NULL), converter(NULL), loadedVXML20(false)
 {
   converter = new DocumentConverter();
   if (converter == NULL) throw VXIException::OutOfMemory();
 
   parser = XMLReaderFactory::createXMLReader();
   if (parser == NULL) {
     delete converter;
     throw VXIException::OutOfMemory();
   }
 
   DTDResolver * dtd = new DTDResolver();
   if (dtd == NULL) {
     delete converter;
     delete parser;
     throw VXIException::OutOfMemory();
   }
   parser->setEntityResolver(dtd);
 
   // These settings below should not change the Xerces defaults.  Their
   // presence makes the defaults explicit.
 
   parser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);
   parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
   parser->setFeature(XMLUni::fgXercesDynamic, false);
   parser->setFeature(XMLUni::fgXercesSchema, true);
   parser->setFeature(XMLUni::fgXercesSchemaFullChecking, true);
   parser->setFeature(XMLUni::fgSAX2CoreNameSpacePrefixes, false);
 
   ErrorHandler *errReporter = new DOMTreeErrorReporter();
   parser->setErrorHandler(errReporter);
 
   parser->setContentHandler(converter);
 }
 
 
 DocumentParser::~DocumentParser()
 {
   if (parser != NULL) {
     const ErrorHandler * reporter = parser->getErrorHandler();
     delete reporter;
 
     const EntityResolver * resolver = parser->getEntityResolver();
     delete resolver;
 
     delete parser;
     delete converter;
     parser = NULL;
   }
 }
 
 
 //****************************************************************************
 // FetchBuffer
 //****************************************************************************
 
 // 1: Invalid parameter
 // 2: Unable to open URL
 // 3: Unable to read from URL
 int DocumentParser::FetchBuffer(const VXIchar * url,
                                 const VXIMapHolder & properties,
                                 VXIMapHolder & streamInfo,
                                 VXIinetInterface * inet,
                                 SimpleLogger & log,
                                 const VXIbyte * & result,
                                 VXIulong & read,
                                 vxistring & docURL)
 {
   if (log.IsLogging(2)) {
     log.StartDiagnostic(2) << L"DocumentParser::FetchBuffer(" << url
                            << L", " << properties.GetValue() << L")";
     log.EndDiagnostic();
   }
 
   if (inet == NULL || url == NULL || wcslen(url) == 0) return 1;
   
   // (1) Open URL
   VXIinetStream * stream;
 
   // VXIMapHolder streamInfo;
   if (streamInfo.GetValue() == NULL) 
   {
     return -1;
   }
   
   if (inet->Open(inet, L"vxi", url, INET_MODE_READ, 0, properties.GetValue(),
                  streamInfo.GetValue(), &stream) != 0)
   {
     if (log.IsLogging(0)) {
       log.StartDiagnostic(0) << L"DocumentParser::FetchBuffer - could not "
         L"open URL: " << url;
       log.EndDiagnostic();
     }
     return 2;
   }
   
   // (2) Determine document size & read into local memory buffer.
   const VXIValue * tempURL = NULL;
   tempURL = VXIMapGetProperty(streamInfo.GetValue(), INET_INFO_ABSOLUTE_NAME);
   if (tempURL == NULL || VXIValueGetType(tempURL) != VALUE_STRING) {
     inet->Close(inet, &stream);
     if (log.IsLogging(0)) {
       log.StartDiagnostic(0) << L"DocumentParser::FetchBuffer - could not "
         L"retrieve absolute path of document at URL: " << url;
       log.EndDiagnostic();
     }
     return 2;
   }
   docURL = VXIStringCStr(reinterpret_cast<const VXIString *>(tempURL));
 
   const VXIValue * tempSize = NULL;
   tempSize = VXIMapGetProperty(streamInfo.GetValue(), INET_INFO_SIZE_BYTES);
   if (tempSize == NULL || VXIValueGetType(tempSize) != VALUE_INTEGER) {
     inet->Close(inet, &stream);
     if (log.IsLogging(0)) {
       log.StartDiagnostic(0) << L"DocumentParser::FetchBuffer - could not "
         L"retrieve size of document at URL: " << url;
       log.EndDiagnostic();
     }
     return 2;
   }
 
   VXIint32 bufSize
     = VXIIntegerValue(reinterpret_cast<const VXIInteger *>(tempSize));
 
   if (bufSize < 2047)
     bufSize = 2047;
 
   ++bufSize;
   VXIbyte * buffer = new VXIbyte[bufSize];
   if(buffer == NULL) {
     log.LogError(202);
     return -1;
   }
 
   bool reachedEnd = false; 
   read = 0;
 
   while (!reachedEnd) {
     VXIulong bytesRead = 0;
     switch (inet->Read(inet, buffer+read, bufSize-read, &bytesRead, stream)) {
     case VXIinet_RESULT_SUCCESS:
       read += bytesRead;
       break;
     case VXIinet_RESULT_END_OF_STREAM:
       read += bytesRead;
       reachedEnd = true;  // exit while
       break;
     case VXIinet_RESULT_WOULD_BLOCK:
       VXItrdThreadYield();
       break;
     default:
       inet->Close(inet, &stream);
       delete[] buffer;
 
       log.LogDiagnostic(0, L"DocumentParser::FetchBuffer - "
                         L"could not read from URL.");
       return 3;
     }
 
     if (read == static_cast<VXIulong>(bufSize)) {
       // The number of bytes read exceeds the number expected.  Double the
       // size and keep reading.
       VXIbyte * temp = new VXIbyte[2*bufSize];
       if(temp == NULL) {
         log.LogError(202);
         delete [] buffer;
         return -1;
       }
       memcpy(static_cast<void *>(temp), static_cast<void *>(buffer),
              bufSize * sizeof(VXIbyte));
       delete[] buffer;
       buffer = temp;
       bufSize *= 2;
     }
   }
 
   inet->Close(inet, &stream);
   result = buffer;
 
   log.LogDiagnostic(2, L"DocumentParser::FetchBuffer - success");
 
   return 0;
 }
 
 
 void DocumentParser::ReleaseBuffer(const VXIbyte * & buffer)
 {
   if (buffer != VALIDATOR_DATA + VXML_DEFAULTS)
     delete[] const_cast<VXIbyte *>(buffer);
 
   buffer = NULL;
 }
 
 
 //#############################################################################
 // FetchDocument
 //#############################################################################
 
 // -2: Internal error
 int DocumentParser::FetchDocument(const VXIchar * url,
                                   const VXIMapHolder & properties,
                                   VXIinetInterface * inet,
                                   VXIcacheInterface * cache,
                                   SimpleLogger & log,
                                   VXMLDocument & document,
                                   VXIMapHolder & docProperties,
                                   bool isDefaults,
                                   VXIbyte **content,
                                   VXIulong *size)
 {
   int result;
 
   if (log.IsLogging(2)) {
     log.StartDiagnostic(2) << L"DocumentParser::FetchDocument(" << url << L")";
     log.EndDiagnostic();
   }
 
   // (1) Load the VXML DTD for validation.  This will override an externally
   // specified DTD if the user provides a link.
 
   try {
     if (isDefaults) {
       parser->parse(MemBufInputSource(VALIDATOR_DATA + DUMMY_VXML_DEFAULTS_DOC,
                                       DUMMY_VXML_DEFAULTS_DOC_SIZE,
                                       "vxml 1.0 defaults"));
       converter->ResetDocument(); // Throw this document away.
     }
 
     if (!isDefaults && !loadedVXML20) {
       // Preload the VXML 2.0 schema.
       VXIcharToXMLCh name(L"http://www.w3.org/TR/voicexml20/vxml.xsd");
       LockLoadGrammar();
       parser->loadGrammar(name.c_str(), Grammar::SchemaGrammarType, true);
 
       // Reuse cached grammars if available.
       parser->setFeature(XMLUni::fgXercesUseCachedGrammarInParse, true);
       
       loadedVXML20 = true;
       UnlockLoadGrammar();
     }
   }
   catch (const XMLException & exception) {
     log.StartDiagnostic(0) << L"DocumentParser::FetchDocument - XML parsing "
       L"error from DOM: " << XMLChToVXIchar(exception.getMessage());
     log.EndDiagnostic();
     log.LogError(999, SimpleLogger::MESSAGE, L"unable to load VXML DTD");
     return 4;
   }
   catch (const SAXParseException & exception) {
     log.StartDiagnostic(0) << L"DocumentParser::FetchDocument - Parse error "
                            << L"in file \"" 
 			   << XMLChToVXIchar(exception.getSystemId()) 
                            << L"\", line " << exception.getLineNumber()
                            << L", column " << exception.getColumnNumber()
                            << L" - " << XMLChToVXIchar(exception.getMessage());
     log.EndDiagnostic();
     log.LogError(999, SimpleLogger::MESSAGE, L"unable to load VXML DTD");
     return 4;
   }
 
   // (2) Load the url into memory.
 
   const VXIbyte * buffer;
   VXIulong bufSize;
   vxistring docURL;
   bool isDefaultDoc = false;
   
 
   if (isDefaults && wcslen(url) == 0) {
     buffer  = VALIDATOR_DATA + VXML_DEFAULTS;
     bufSize = VXML_DEFAULTS_SIZE;
     docURL = L"builtin defaults";
     result = 0;
     isDefaultDoc = true;
   }
   else {
     result = DocumentParser::FetchBuffer(url, properties, docProperties, inet, log,
                                          buffer, bufSize, docURL);
   }
 
   if (result != 0) {
     if (log.IsLogging(0)) {
       log.StartDiagnostic(0) << L"DocumentParser::FetchDocument - exiting "
         L"with error result " << result;
       log.EndDiagnostic();
     }
     return result; // may return { -1, 1, 2, 3 }
   }
 
   // store buffer for reference
   if (content) {
     VXIbyte *tempbuf = new VXIbyte[bufSize];
     if(tempbuf == NULL) {
       log.LogError(202);
       DocumentParser::ReleaseBuffer(buffer);
       return -1;
     }
     memcpy(tempbuf, buffer, bufSize);
     if (size != NULL) *size = bufSize;
     *content = tempbuf;
   }
   
   // (3) Pull the document from cache.
   
   // (3.1) Set the base uri for this document, but
   // ignore if it is the default doc
   if(!isDefaultDoc) 
     converter->SetBaseUri(docURL.c_str());
 
   VXMLDocument doc;
   if (!DocumentStorageSingleton::Instance()->Retrieve(doc, buffer, bufSize)) {
 
     // (4) Not in cache; parse buffer into our VXML document representation
     try {
       VXIcharToXMLCh membufURL(docURL.c_str());
       parser->parse(MemBufInputSource(buffer, bufSize, membufURL.c_str(), false));
     }
     catch (const XMLException & exception) {
       DocumentParser::ReleaseBuffer(buffer);
       if (log.IsLogging(0)) {
         log.StartDiagnostic(0) << L"DocumentParser::FetchDocument - XML "
           L"parsing error from DOM: " <<XMLChToVXIchar(exception.getMessage());
         log.EndDiagnostic();
       }
       return 4;
     }
     catch (const SAXParseException & exception) {
       DocumentParser::ReleaseBuffer(buffer);
       if (log.IsLogging(0)) {
         log.StartDiagnostic(0) << L"DocumentParser::FetchDocument - Parse "
                                << L"error in file \"" 
                                << XMLChToVXIchar(exception.getSystemId())
                                << L"\", line " << exception.getLineNumber()
                                << L", column " << exception.getColumnNumber()
                                << L" - " 
                                << XMLChToVXIchar(exception.getMessage());
         log.EndDiagnostic();
       }
 
       return 4;
     }
 
     // (5) Write the compiled form out to the cache.
 
     doc = converter->GetDocument();
     DocumentStorageSingleton::Instance()->Store(doc, buffer, bufSize);
   }
 
   DocumentParser::ReleaseBuffer(buffer);
 
   // (6) Parse was successful, process document.  We want only the top level
   // <vxml> node.
 
   const VXMLElement root = doc.GetRoot();
   VXMLElementType nodeName = root.GetName();
 
   // If we're looking for the defaults, we can exit early.
   if (isDefaults && nodeName == DEFAULTS_ROOT) {
     log.LogDiagnostic(2, L"DocumentParser::FetchDocument - success");
     document = doc;
     return 0;
   }
   else if (nodeName != NODE_VXML) {
     document = VXMLDocument();
     if (log.IsLogging(0)) {
       log.StartDiagnostic(0) << L"DocumentParser::FetchDocument - unable to "
         L"find " << NODE_VXML << L" in document.";
       log.EndDiagnostic();
     }
     return 4;
   }
 
   vxistring temp;
 
   // If the properties map is NULL, don't bother with the remaining settings
   if (docProperties.GetValue() != NULL) {
     // Retrieve the properties and put them into the map.
 
     VXIString * str = NULL;
     temp = docURL;
     str = VXIStringCreate(temp.c_str());
     if (str == NULL) 
       throw VXIException::OutOfMemory();
 
     VXIMapSetProperty(docProperties.GetValue(), PropertyList::AbsoluteURI,
                       reinterpret_cast<VXIValue *>(str));
 
     str = VXIStringCreate(converter->GetBaseUri().empty() ? L"" : 
                           converter->GetBaseUri().c_str());
     
     if (str == NULL)
       throw VXIException::OutOfMemory();
 
     VXIMapSetProperty(docProperties.GetValue(), PropertyList::BaseURI,
                       reinterpret_cast<VXIValue *>(str));
 
     if (root.GetAttribute(ATTRIBUTE_XMLLANG, temp)) {
       str = VXIStringCreate(temp.c_str());
       if (str == NULL)
         throw VXIException::OutOfMemory();
 
       VXIMapSetProperty(docProperties.GetValue(), PropertyList::Language,
                         reinterpret_cast<VXIValue *>(str));
     }
   }
 
   log.LogDiagnostic(2, L"DocumentParser::FetchDocument - success");
 
   document = doc;
   return 0;
 }
 
 
 int DocumentParser::FetchContent(const VXIchar * uri,
                                  const VXIMapHolder & properties,
                                  VXIMapHolder & fetchInfo,
                                  VXIinetInterface * inet,
                                  SimpleLogger & log,
                                  const vxistring & encoding,
                                  vxistring & content)
 {
   const VXIbyte * buffer;
   VXIulong bufSize;
   vxistring docURL;
 
   // (1) Retrieve the URI.
   switch (DocumentParser::FetchBuffer(uri, properties, fetchInfo, inet, log,
                                       buffer, bufSize, docURL)) {
   case -1: // Out of memory?
     return -1;
   case  0: // Success
     break;
   case  2: // Unable to open URI
     return 2;
   case  3: // Unable to read from URI
     return 3;
   case  1: // Invalid parameter
   default:
     return 1;
   }
 
   // (2) Create a transcoder for the requested type.
 
   VXIcharToXMLCh encName(encoding.c_str());
   XMLTransService::Codes failReason;
   XMLTranscoder* transcoder = 
       XMLPlatformUtils::fgTransService->makeNewTranscoderFor(encName.c_str(),
                                                              failReason,
                                                              8*1064
 #if _XERCES_VERSION <= 20200                                                             
                                                               );
 #else
                                                              ,XMLPlatformUtils::fgMemoryManager);
 #endif
 
   if (transcoder == NULL) return 4;
 
   // (3) Allocate memory for the conversion.
 
   XMLCh * convertedString = new XMLCh[bufSize+1];
   unsigned char* charSizes = new unsigned char[bufSize];
 
   if (convertedString == NULL || charSizes == NULL) {
     delete[] convertedString;
     delete[] charSizes;
     return -1;
   }
 
   // (4) Transcode the values into our string.
 
   unsigned int bytesEaten;
   unsigned int charsDone = transcoder->transcodeFrom(buffer, bufSize,
                                                      convertedString, bufSize,
                                                      bytesEaten, charSizes);
 
   // (5) Finally convert from XMLCh to VXIchar.
   convertedString[charsDone] = '\0';  // Add terminator. 
   XMLChToVXIchar result(convertedString);
 
   // (6) Done.  Release memory.
 
   content = result.c_str();
   delete[] convertedString;
   delete[] charSizes;
   ReleaseBuffer(buffer);
 
   return 0;
 }
