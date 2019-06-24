
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
 
#ifndef SBINETUTILS_HPP
 #define SBINETUTILS_HPP
 
 #include "VXIvalue.h"
 
 class SBinetValidator;
 
 class SBinetUtils
 {
   // Simple helper functions to retrieve information from a VXIMap.
  public:
   static bool getInteger(const VXIMap *theMap, const VXIchar *prop, VXIint32& value);
 
   static bool getFloat(const VXIMap *theMap, const VXIchar *prop, VXIflt32& value);
 
   static const VXIchar *getString(const VXIMap *theMap, const VXIchar *prop);
 
   static bool getContent(const VXIMap *theMap, const VXIchar *prop,
                          const char* &content, VXIulong& contentLength);
 
   static VXIContent *createContent(const char *str);
 
   static char *getTimeStampStr(time_t  timestamp,
 			       char   *timestampStr);
 
   static char *getTimeStampMsecStr(char  *timestampStr);
 
   static bool setValidatorInfo(VXIMap *streamInfo, const SBinetValidator *validator);
 
  private:
   SBinetUtils();
 };
 
 
 #endif
