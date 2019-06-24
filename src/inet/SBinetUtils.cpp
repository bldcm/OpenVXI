
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
 

 #include "SBinetInternal.h"
 
 #include "SBinetUtils.hpp"
 #include "SBinetValidator.h"
 
 #include <stdio.h>
 #include <string.h>
 #include <time.h>
 #include <sys/timeb.h>                  // for ftime( )/_ftime( )
 
 bool SBinetUtils::getInteger(const VXIMap *theMap, const VXIchar *prop,
 			     VXIint32& val)
 {
   const VXIValue *value = VXIMapGetProperty(theMap, prop);
   if (value != NULL && VXIValueGetType(value) == VALUE_INTEGER)
   {
     val = VXIIntegerValue((const VXIInteger *) value);
     return true;
   }
   return false;
 }
 
 bool SBinetUtils::getFloat(const VXIMap *theMap, const VXIchar *prop,
                            VXIflt32& val)
 {
   const VXIValue *value = VXIMapGetProperty(theMap, prop);
   if (value != NULL && VXIValueGetType(value) == VALUE_FLOAT)
   {
     val = VXIFloatValue((const VXIFloat *) value);
     return true;
   }
   return false;
 }
 
 const VXIchar *SBinetUtils::getString(const VXIMap *theMap,
                                       const VXIchar *prop)
 {
   const VXIValue *value = VXIMapGetProperty(theMap, prop);
   if (value != NULL && VXIValueGetType(value) == VALUE_STRING)
   {
     return VXIStringCStr((const VXIString *) value);
   }
   return NULL;
 }
 
 bool SBinetUtils::getContent(const VXIMap *theMap,
                              const VXIchar *prop,
                              const char *&content,
                              VXIulong &contentLength)
 {
   const VXIValue *value = VXIMapGetProperty(theMap, prop);
   if (value != NULL && VXIValueGetType(value) == VALUE_CONTENT)
   {
     const VXIchar *mimeType;
 
     VXIvalueResult rc = VXIContentValue((const VXIContent *) value, &mimeType,
                                         (const VXIbyte **) &content,
 					&contentLength);
 
     if (rc == VXIvalue_RESULT_SUCCESS && content != NULL)
     {
       // FIXME: should check for text/plain mimetype!
       return true;
     }
   }
 
   return false;
 }
 
 static void DestroyContent(VXIbyte **content, void *userdata)
 {
   if (content)
   {
     delete [] (char *)(*content);
     *content = NULL;
   }
 }
 
 VXIContent *SBinetUtils::createContent(const char *str)
 {
   int len = ::strlen(str);
   return VXIContentCreate(L"text/plain",
                           (VXIbyte *) ::strcpy(new char[len +1], str), len,
                           DestroyContent, NULL);
 }
 
 char *SBinetUtils::getTimeStampStr(time_t  timestamp,
 				   char   *timestampStr)
 {
 #ifdef WIN32
   char *timeStr = ctime(&timestamp);
   if (timeStr)
     ::strcpy(timestampStr, timeStr);
   else
     ::strcpy(timestampStr, "<bad timestamp>");
 #else
   char *timeStr = ctime_r(&timestamp, timestampStr);
   if (! timeStr)
     ::strcpy(timestampStr, "<bad timestamp>");
 #endif
 
   return timestampStr;
 }
 
 char *SBinetUtils::getTimeStampMsecStr(char  *timestampStr)
 {
 #ifdef WIN32
   struct _timeb tbuf;
   _ftime(&tbuf);
   char *timeStr = ctime(&tbuf.time);
 #else
   struct timeb tbuf;
   ftime(&tbuf);
   char timeStr_r[64] = "";
   char *timeStr = ctime_r(&tbuf.time, timeStr_r);
 #endif
 
   if (timeStr) {
     // Strip the weekday name from the front, year from the end,
     // append hundredths of a second (the thousandths position is
     // inaccurate, remains constant across entire runs of the process)
     ::strncpy(timestampStr, &timeStr[4], 15);
     ::sprintf(&timestampStr[15], ".%02u", tbuf.millitm / 10);
   } else {
     ::strcpy(timestampStr, "<bad timestamp>");
   }
 
   return timestampStr;
 }
 
 bool SBinetUtils::setValidatorInfo(VXIMap *streamInfo, const SBinetValidator *validator)
 {
   // Build caching information required to put in the streamInfo map.  This
   // consist of three properties:
   //
   // 1) INET_INFO_EXPIRES_HINT: indicates when a new request should be made to
   // the remote server.
   //
   // 2) INET_INFO_VALIDATOR_STRONG: indicates that the ETag provided by the
   // remote server is a strong one.
   //
   // 3) INET_INFO_VALIDATOR: the actual validator information as returned by
   // the remote server.
 
   if (validator->isStrong())
   {
     // We have a strong validator.
     if (VXIMapSetProperty(streamInfo, INET_INFO_VALIDATOR_STRONG, (VXIValue *) VXIIntegerCreate(TRUE)) !=
         VXIvalue_RESULT_SUCCESS)
       return false;
   }
   else
   {
     VXIMapDeleteProperty(streamInfo, INET_INFO_VALIDATOR_STRONG);
   }
 
   time_t expiresTimeStamp = validator->getExpiresTime();
   if (expiresTimeStamp < (time_t) 0) expiresTimeStamp = (time_t) 0;
   if (VXIMapSetProperty(streamInfo, INET_INFO_EXPIRES_HINT, (VXIValue *) VXIIntegerCreate(expiresTimeStamp)) !=
       VXIvalue_RESULT_SUCCESS)
     return false;
 
 
   return VXIMapSetProperty(streamInfo, INET_INFO_VALIDATOR, (VXIValue *) validator->serialize()) ==
     VXIvalue_RESULT_SUCCESS;
 }
