
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
  * VXI class decl. Implementation in VXI_*.cpp
  *
  ***********************************************************************/
 
 #ifndef _VXI_H
 #define _VXI_H
 
 #include "VXIvalue.h"                     // for VXIchar, VXImap
 #include "CommonExceptions.hpp"           // for VXIException class
 #include "VXMLDocument.hpp"               // for VXMLDocument and doc model
 #include "InternalMutex.hpp"
 #include <string>
 #include <deque>
 
 typedef std::basic_string<VXIchar> vxistring;
 typedef std::deque<vxistring> STRINGDEQUE;
 
 class AnswerInformation;
 class DocumentParser;
 class ExecutionContext;
 class JumpDoc;
 class PromptManager;
 class PropertyList;
 class RecognitionAnswer;
 class SimpleLogger;
 extern "C" struct VXIcacheInterface;
 extern "C" struct VXIinetInterface;
 extern "C" struct VXIjsiInterface;
 extern "C" struct VXIpromptInterface;
 extern "C" struct VXIrecInterface;
 extern "C" struct VXItelInterface;
 extern "C" struct VXIobjectInterface;
 
 class VXI {
 public:
   VXI();  // May throw VXIException::OutOfMemory
   ~VXI();
 
   int Run(const VXIchar * initialDocument, const VXIchar * sessionScript,
           SimpleLogger * log, VXIinetInterface * inet,
           VXIcacheInterface * cache, VXIjsiInterface * jsi,
           VXIrecInterface * rec, VXIpromptInterface * prompt,
           VXItelInterface * tel, VXIobjectInterface * object,
           VXIValue ** result);
   // Returns: -2 Fatal error
   //          -1 Out of memory
   //           0 Success
   //           1 Infinite loop suspected.
   //           2 Javascript error
   //           3 Invalid startup documents
   //           4 Stopped
 
   void DeclareStopRequest(bool doStop);
   // This sets a flag which may be used to abort Run.
 
   int DeclareExternalEvent(const VXIchar * event, const VXIchar * message);
   // Returns:  0 Success
   //           1 Invalid event
 
   int ClearExternalEventQueue(void);
   // Returns:  0 Success
   //           1 Invalid event
 
   enum PropertyID {
     BeepURI,
     PlatDefaultsURI,
   };
 
   bool SetRuntimeProperty(PropertyID, const VXIchar * value);
   // Returns: true  - Property set
   //          false - Invalid parameter value
 
 private:
   void GetRuntimeProperty(PropertyID, vxistring &) const;
 
   ////////////////////////////////////////////////////////////////////////////
   // Document level functions
   ////////////////////////////////////////////////////////////////////////////
 
   int RunDocumentLoop(const vxistring & initialDocument,
                       const vxistring & sessionScript,
                       VXIValue ** result);
 
   void PerformTransition(const VXMLElement & doc, const vxistring & url,
                          VXIMap * submitData = NULL, bool isSubdialog = false, 
                          bool isSubmitElement = false);
 
   VXMLElement FindDialog(const VXMLElement & doc, const vxistring & name);
   // Finds the named dialog in the document.  If the name is empty, the first
   // item is returned.
 
   ////////////////////////////////////////////////////////////////////////////
   // Document level support functions
   ////////////////////////////////////////////////////////////////////////////
 
   bool PushExecutionContext(const vxistring & sessionScript);
   // This is responsible for adding a next execution context for the initial
   // document and for <subdialog> elements.  It also initializes the session
   // and language-neutral platform defaults.
   //
   // Returns: true - success
   //          false - failure (stack depth exceeded?)
 
   void PopExecutionContext();
   // This undoes PushExecutionContext()
 
   void InstallDocument(JumpDoc &);
 
   void ProcessRootScripts(VXMLElement & doc);
 
   void AttemptDocumentLoad(const vxistring & rawURL,
 			   const VXIMapHolder & urlProperties,
 			   VXMLDocument & doc,
 			   VXIMapHolder & docProperties,
 			   bool isDefaults = false);
 
   void PrepareDocumentTree(VXMLElement & doc);
   // Recursively walks the document tree and assigns internal names as needed.
 
 private:
   ////////////////////////////////////////////////////////////////////////////
   // Dialog level functions
   ////////////////////////////////////////////////////////////////////////////
   void RunInnerLoop();
 
   void DoInnerJump(const VXMLElement & elem, const vxistring & item);
   // Either throws an event containing the next form item to execute or
   // simply returns if none is found.
 
   ////////////////////////////////////////////////////////////////////////////
   // Dialog level support functions
   ////////////////////////////////////////////////////////////////////////////
 
   void FormInit(const VXMLElement & form, VXIMapHolder & params);
   // Perform initialization associated with property tags and form level
   // variables.  Reset the event and prompts counts.
 
   bool IsFormItemNode(const VXMLElement& doc);
   // Returns true iff this element is a 'form item'.
 
   bool IsInputItemNode(const VXMLElement& doc);
   // Returns true iff this element is an 'input item'.
 
 private:
   ////////////////////////////////////////////////////////////////////////////
   // Collect Phase and element related.
   ////////////////////////////////////////////////////////////////////////////
 
   void CollectPhase(const VXMLElement& form, const VXMLElement& item);
 
   void ProcessReturn(const VXMLElement& form, const VXMLElement& item,
                      VXIMapHolder & result);
 
   void DoEvent(const VXMLElement & item,
                const VXIException::InterpreterEvent & event);
 
   bool do_event(const VXMLElement & item,
                 const VXIException::InterpreterEvent & event);
   // Returns: true - event handled successfully.
   //          false - no handler available.
 
   void DoRecognition(const vxistring & documentID,
                      const vxistring & dialogName,
                      const vxistring & fieldName,
                      const PropertyList & properties,
                      int timeout,
                      bool isModal);
   void ProcessRecognitionResult(VXIContent * result,
                                 const PropertyList & propertyList);
 
   void HandleRemoteMatch(const VXMLElement & dialog,
                          const VXMLElement & element);
   void ProcessFilledElements(const vxistring & filled,
                              const VXMLElement & element);
   void ProcessFilledElements(STRINGDEQUE & filled,
                              const VXMLElement & element);
 
   void execute_content(const VXMLElement & doc,
                        const VXIMapHolder & vars = VXIMapHolder(NULL),
                        const VXMLElement & activeDialog = 0);
 
   void executable_element(const VXMLElement& child, const VXMLElement & activeDialog = 0);
   void executable_prompt(const VXMLElement& child);
 
   void assign_element(const VXMLElement& doc);
   void clear_element(const VXMLElement& doc);
   void disconnect_element(const VXMLElement& doc);
   void goto_element(const VXMLElement& doc);
   void exit_element(const VXMLElement& doc);
   void if_element(const VXMLElement& doc);
   void log_element(const VXMLElement& doc);
   void meta_element(const VXMLElement & doc);
   void reprompt_element(const VXMLElement& doc, const VXMLElement & activeDialog = 0);
   void return_element(const VXMLElement& doc);
   void script_element(const VXMLElement& doc);
   void submit_element(const VXMLElement& doc);
   void throw_element(const VXMLElement& doc, const VXMLElement& activeDialog = 0);
   void var_element(const VXMLElement & doc);
 
   void block_element(const VXMLElement& doc);
   void field_element(const VXMLElement& form, const VXMLElement& field);
   void menu_element(const VXMLElement& doc);
   void object_element(const VXMLElement& doc);
   void record_element(const VXMLElement& form, const VXMLElement& doc);
   void subdialog_element(const VXMLElement& doc);
   void transfer_element(const VXMLElement & form, const VXMLElement& doc);
 
   VXIMap * CollectParams(const VXMLElement & doc, bool isObject);
   
   // check line status before continue processing
   void CheckLineStatus(void);
 
 private: // All prompt related.
   void queue_prompts(const VXMLElement& doc);
 
 private:
   DocumentParser     * parser;  // owned
   SimpleLogger       * log;
   VXIcacheInterface  * cache;
   VXIinetInterface   * inet;
   VXIrecInterface    * rec;
   VXIjsiInterface    * jsi;
   VXItelInterface    * tel;
   VXIobjectInterface * object;
   PromptManager      * pm;      // owned
 
   VXIMap             * sdParams;
   VXIMap             * sdResult;
   VXIException::InterpreterEvent * sdEvent;
 
   bool updateDefaultDoc;
   VXMLDocument domDefaultDoc;
 
   // Used by Get/Set Property
   InternalMutex mutex;
   vxistring uriPlatDefaults;
   vxistring uriBeep;
 
   int stackDepth;
   ExecutionContext * exe;
 
   // Event related
   bool lineHungUp;              // Tel. status to track hang-up event
   bool stopRequested;           // Should the interpreter stop ASAP?
   bool haveExternalEvents;      // Are there unprocessed external events?
   STRINGDEQUE externalEvents;   // Unprocessed external events
   STRINGDEQUE externalMessages; // Unprocessed external events (messages)
 };
 
 #endif
