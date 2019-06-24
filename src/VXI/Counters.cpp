
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
 

 #include "Counters.hpp"
 #include "DocumentModel.hpp"
 
 static const VXIchar* FORMLEVEL = L"$_FRM_LEVEL_$";
 
 // ------*---------*---------*---------*---------*---------*---------*---------
 #include <iostream>
 
 void EventCounter::Increment(const vxistring & fieldItem, const vxistring & eventName)
 { 
   // (1) Search for the event name from the given field item
   //     If the field item is blank, assume the event names are in form level
   const VXIchar* fieldName = fieldItem.empty() ? FORMLEVEL : fieldItem.c_str();
   vxistring::size_type start = 0;
 
   COUNTS::iterator fieldMap = counts.find(fieldName);
 
   // (2) If the field name does not exist, add to the map
   if( fieldMap == counts.end() ) {
     EVENTCOUNTS  eventCount;
     do {    
       vxistring::size_type pos = eventName.find('.', start);
       if (pos == vxistring::npos) pos = eventName.length();
       if (pos != start) { // ignore isolated '.' or '..'
         eventCount[eventName.substr(0, pos)] = 1;
       }
       start = pos + 1;
     } while (start < eventName.length());
     // add to the map
     counts[fieldName] = eventCount;
   }
   else {
     // (3) If the field name exists, increment its event counter and 
     //     add one if neccessary
     EVENTCOUNTS & eventCount = (*fieldMap).second;
     do {
       vxistring::size_type pos = eventName.find('.', start);
       if (pos == vxistring::npos) pos = eventName.length();
       if (pos != start) { // ignore isolated '.' or '..'
         EVENTCOUNTS::iterator i = eventCount.find(eventName.substr(0, pos));
         if (i != eventCount.end()) ++(*i).second;
         else eventCount[eventName.substr(0, pos)] = 1;
       }
       start = pos + 1;
     } while (start < eventName.length());
   }
 }
 
 
 int EventCounter::GetCount(const vxistring & fieldItem,
                            const vxistring & eventName,
                            const vxistring & rawCatchName) const
 {
   if (eventName.empty()) return 0;
   if (rawCatchName.length() > eventName.length()) return 0;
 
   // (1) Search for the event name from the given field item
   //     If the field item is blank, assume the event names are in form level
   const VXIchar* fieldName = fieldItem.empty() ? FORMLEVEL : fieldItem.c_str();
 
   COUNTS::const_iterator fieldMap = counts.find(fieldName);
   
   // (2) Can't find the event counter for this field name
   if( fieldMap == counts.end() ) return 0;
   
   // This is necessary to support the 'catch all' semantic.  We make the catch
   // as unspecific as possible.
 
   vxistring catchName(rawCatchName);
 
   if (catchName.empty()) {
     vxistring::size_type pos = eventName.find('.');
     if (pos == vxistring::npos) catchName = eventName;
     else catchName = eventName.substr(0, pos);
   }
 
   // Now look up the count for the event.
 
   if (eventName.find(catchName) != 0) return 0;
   if (eventName.length() != catchName.length() &&
       eventName[catchName.length()] != '.') return 0;
 
   // Get the event counter in this field item and return
   // the appropriate counter
   const EVENTCOUNTS & eventCount = (*fieldMap).second;
   EVENTCOUNTS::const_iterator i = eventCount.find(catchName);
   if (i != eventCount.end()) return (*i).second;
   
   return 0; // This shouldn't happen if Increment was called first.
 }
 
 // ------*---------*---------*---------*---------*---------*---------*---------
