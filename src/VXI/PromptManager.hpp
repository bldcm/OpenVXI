
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
 

 #include "VXIvalue.h"
 #include "VXIprompt.h"
 #include <string>
 
 typedef std::basic_string<VXIchar> vxistring;
 
 extern "C" struct VXIpromptInterface;
 class PromptTranslator;
 class PropertyList;
 class SimpleLogger;
 class VXMLElement;
 class VXMLNode;
 
 
 class PromptTranslator {
 public:
   virtual VXIValue * EvaluateExpression(const vxistring & expression) = 0;
   // Evaluates an expression which arises during prompt construction.  The
   // returned value will be freed by the PromptManager.
 
   virtual void SetVariable(const vxistring & name, const vxistring & value) =0;
   // Sets a local variable in the current scope.
 
   PromptTranslator() { }
   virtual ~PromptTranslator() { }
 };
 
 
 class PromptManager {
 public:
   enum BargeIn {
     UNSPECIFIED,
     ENABLED,
     DISABLED
   };
 
   enum SegmentType {
     SEGMENT_AUDIO,
     SEGMENT_SSML
   };
 
   void PreloadPrompts(const VXMLElement& doc, PropertyList &, PromptTranslator &);
   // This function walks through the document, preloading prompts as necessary.
   //
   // may throw: VXIException::InterpreterError & VXIException::OutOfMemory
 
   void PlayFiller(const vxistring & src, const VXIMapHolder & properties);
   // Start playing 'filler' material (for fetchaudio, percolation, etc.)
 
   void Play(bool *playedBarginDisabledPrompt);
   // Waits until the prompts currenly marked for playing are done.  Then starts
   // playing everything remaining in the queue.  This is generally followed by
   // a recognition / record attempt.
 
   void PlayAll();
   // Play everything currently in the queue.  The user is unable to barge in.
 
   void Queue(const VXMLElement & child, const VXMLElement & reference,
              const PropertyList &, PromptTranslator &);
   // This resolves <enumerate> and <value>.
 
   void Queue(const vxistring & uri);
   // Queues a segment from a known URI.
 
   void WaitAndCheckError(void);
 
   int GetMillisecondTimeout() const;
   // Returns: the recognition timeout, in milliseconds, or -1 if none was
   // specified.
 
 public:
   bool ConnectResources(SimpleLogger *, VXIpromptInterface *);
   PromptManager() : log(NULL), prompt(NULL) { }
   ~PromptManager() { }
 
 private:
   void DoPreloadPrompts(const VXMLElement& doc,
                         PropertyList & properties,
                         VXIMapHolder & levelProperties,
                         PromptTranslator & translator);
 
   bool ProcessPrefetchPrompts(const VXMLNode & node, const VXMLElement& item, 
                       const PropertyList & propertyList,
                       PromptTranslator & translator, 
                       vxistring & sofar);
 
   void ProcessSegments(const VXMLNode & node,
                        const VXMLElement & item,
                        const PropertyList & propertyList,
                        PromptTranslator & translator,
                        BargeIn,
                        VXIMapHolder & props,
                        vxistring & sofar);
 
   bool AddSegment(SegmentType, const vxistring & data,
                   const VXIMapHolder & properties, BargeIn,
                   bool throwExceptions = true);
   // Returns: true - segment successfully queued
   //          false - segment addition failed
 
   void AddContent(VXIMapHolder & props,
                   vxistring & sofar,
                   VXIValue * value);
   // NOTE: This takes ownership of the VXIValue.
 
   void ThrowEventIfError(VXIpromptResult rc);
   
 private:
   SimpleLogger       * log;
   VXIpromptInterface * prompt;
   bool enabledSegmentInQueue;
   bool playedBargeinDisabledPrompt;
   int timeout;
 };
 
