
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
 

 #include "VXItypes.h"
 #include <map>
 #include <string>
 
 typedef std::basic_string<VXIchar> vxistring;
 class VXMLElement;
 
 class EventCounter {
 public:
   void Clear()
   { counts.clear(); }
 
   void Clear(const vxistring & name)
   { COUNTS::iterator i = counts.find(name);
     if (i != counts.end()) counts.erase(i); }
 
   void Increment(const vxistring & fieldItem, const vxistring & eventName);
   // Increments the count associated with all events.  For instance, given
   // 'error.semantic.bad_name', this will increment 'error', 'error.semantic',
   // and 'error.semantic.bad_name'.
 
   int GetCount(const vxistring & fieldItem, const vxistring & eventName, 
                const vxistring & catchName) const;
   // Returns the count associated with this catch _if_ it matches the event,
   // otherwise zero.
 
 private:
   // A map that holds event counter for each event
   typedef std::map<vxistring, int> EVENTCOUNTS;
   // A map of field item to event counter
   typedef std::map<vxistring, EVENTCOUNTS> COUNTS;
   COUNTS counts;
 };
 
 
 class PromptTracker {
 public:
   void Clear()
   { counts.clear(); }
 
   void Clear(const vxistring & name)
   { COUNTS::iterator i = counts.find(name);
     if (i != counts.end()) counts.erase(i); }
 
   int Increment(const vxistring & name)
   { COUNTS::iterator i = counts.find(name);
     if (i != counts.end()) return ++(*i).second;
     counts[name] = 1;
     return 1; }
 
 private:
   typedef std::map<vxistring, int> COUNTS;
   COUNTS counts;
 };
