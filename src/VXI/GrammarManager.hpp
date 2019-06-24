
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
 
#ifndef __GRAMMAR_MANAGER_HPP__
 #define __GRAMMAR_MANAGER_HPP__
 
 #include "VXIrec.h"
 #include "VXIvalue.h"
 #include <string>
 #include <vector>
 
 typedef std::basic_string<VXIchar> vxistring;
 
 extern "C" struct VXIrecGrammar;
 extern "C" struct VXIrecInterface;
 extern "C" struct VXIrecRecordResult;
 class GrammarInfo;
 class GrammarInfoUniv;
 class PropertyList;
 class SimpleLogger;
 class VXMLElement;
 
 
 class GrammarManager {
 public:
   enum OpType {
     GRAMMAR,            
     RECOGNIZE,
     RECORD,
     TRANSFER
   };
 
   static void ThrowSpecificEventError(VXIrecResult err, OpType opType);
   
   static const VXIchar * const DTMFTerm;
   static const VXIchar * const FinalSilence;
   static const VXIchar * const MaxTime;
   static const VXIchar * const RecordingType;
 
   void LoadGrammars(const VXMLElement& doc, vxistring & id, PropertyList &,
                     bool isDefaults = false);
   // This function walks through the document, creating grammars as necessary.
   // The id of the document (passed to Enable) is returned.
   //
   // may throw: VXIException::InterpreterError & VXIException::OutOfMemory
 
   void DisableAllGrammars();
   // Deactivates all current grammars.
 
   bool EnableGrammars(const vxistring & documentID,
                       const vxistring & dialogName,
                       const vxistring & fieldName,
                       const VXIMapHolder & properties,
                       bool isModal);
   // Activates (for recognition) grammars matching the given dialog & field
   // name.  The documentID is returned by LoadGrammars.
   //
   // Returns: false - no grammars were enabled
   //          true  - at least one grammar is ready
   //
   // may throw VXIException::InterpreterEvent
 
   enum {
     Success,       // Recognition returned a hypothesis
     OutOfMemory,   // should be gone ??? // Insufficient memory available
     BadMimeType,   // should be gone ??? // The requested type is not supported
     InternalError  // VXIrecInterface::Recognize failed gave invalid results
   };
 
   int Recognize(const VXIMapHolder & properties,
                 VXIContent * & waveform,
                 VXIContent * & result);
 
   bool FindMatchedElement(const vxistring & id, VXMLElement & match) const;
 
   bool FindMatchedElement(const vxistring & id, VXMLElement & match,
                           unsigned long & ginfo) const;
 
   int CompareGrammar(unsigned long info1, unsigned long info2);
   // Compare the precedence of info1 against info2
   // Returns: 1 if info1 has higher precedence than info2
   //          0 if both has the same precedence
   //         -1 if info2 has higher precedence than info1
 
   int Record(const VXIMapHolder & properties,
              bool flushBeforeRecord,
              VXIrecRecordResult * & answer);
 
   void ReleaseGrammars();
   // Deletes all current grammars.
 
   VXIMap * GetRecProperties(const PropertyList &, int timeout = -1) const;
   VXIMap * GetRecordProperties(const PropertyList &, int timeout = -1) const;
 
   GrammarManager(VXIrecInterface * r, const SimpleLogger & l);
   // may throw: VXIException::OutOfMemory()
 
   ~GrammarManager();
 
 private:
   void SetGrammarLoadProperties(const VXMLElement & element,
                                 VXIMapHolder & props) const;
 
   void BuildGrammars(const VXMLElement& doc, const vxistring & id,
                      PropertyList &, VXIMapHolder & flattenedProperties,
                      int menuAcceptLevel = 0);
   // Invoked by LoadGrammars and called recursively to build grammars.
   //
   // may throw: VXIException::InterpreterError & VXIException::OutOfMemory
 
   void AddGrammar(VXIrecGrammar * gr, const vxistring & docID,
                   const VXMLElement & elem);
   // Invoked by LoadGrammars to commit each grammar to the collection.
   //
   // may throw: VXIException::OutOfMemory()
 
   void BuildUniversals(const VXMLElement& doc, PropertyList &);
 
   void AddUniversal(VXIrecGrammar * gr, const VXMLElement & elem,
                     const vxistring & langID, const vxistring & name);
 
   VXIrecGrammar * BuildInlineGrammar(const VXMLElement & element,
                                      const VXIMapHolder & localProps);
 
   void BuildOptionGrammars(const vxistring & docID, const VXMLElement& doc,
                            const VXIMapHolder & props);
   
   static bool GetEnclosedText(const SimpleLogger & log,
                               const VXMLElement & doc, vxistring & str);
   static VXIrecGrammar * CreateGrammarFromString(VXIrecInterface * vxirec,
                                                  const SimpleLogger & log,
                                                  const vxistring & source,
                                                  const VXIchar * type,
                                                  const VXIMapHolder & props);
   static VXIrecGrammar * CreateGrammarFromURI(VXIrecInterface * vxirec,
                                               const SimpleLogger & log,
                                               const vxistring & source,
                                               const VXIchar * type,
                                               const VXIMap * fetchProps,
                                               const VXIMapHolder & props);
 
   unsigned long GetGrammarSequence(void) { return ++grammarSequence; }
 
 private:
   typedef std::vector<GrammarInfo *> GRAMMARS;
   GRAMMARS grammars;
 
   typedef std::vector<GrammarInfoUniv *> UNIVERSALS;
   UNIVERSALS universals;
 
   const SimpleLogger & log;
   VXIrecInterface * vxirec;
   unsigned int grammarSequence;
 };
 
 #endif
