#ifndef SWIHASHABLE_HPP
 #define SWIHASHABLE_HPP
 

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
 

 /**
  * class representing objects that can be used as keys in SWIHashMap.  It really is only an interface as all its methods
  * are pure virtual.
  * @doc <p>
  **/
 
 #include "SWIutilHeaderPrefix.h"
 
 class SWIUTIL_API_CLASS SWIHashable
 {
  public:
   SWIHashable()
   {}
 
   /**
    * Destructor.
    **/
  public:
   virtual ~SWIHashable()
   {}
 
  public:
   virtual unsigned int hashCode() const = 0;
 
  public:
   virtual SWIHashable* clone() const = 0;
 
  public:
   virtual bool equals(const SWIHashable *rhs) const = 0;
 
 };
 #endif
