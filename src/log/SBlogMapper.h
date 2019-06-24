
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
 

 // -----1=0-------2=0-------3=0-------4=0-------5=0-------6=0-------7=0-------8
 
 #ifndef SBLOG_MAPPER_HPP
 #define SBLOG_MAPPER_HPP
 
 #include "VXIheaderPrefix.h"
 #ifdef SBLOGMAPPER_DLL
 #ifdef SBLOGMAPPER_EXPORTS
 #define SBLOGMAPPER_API SYMBOL_EXPORT_DECL
 #else
 #define SBLOGMAPPER_API SYMBOL_IMPORT_DECL
 #endif
 #else
 #ifdef __cplusplus
 #define SBLOGMAPPER_API extern "C"
 #else
 #define SBLOGMAPPER_API extern
 #endif
 #endif
 
 #include "VXIlog.h"             // For VXIlogResult
 #include "VXIvalue.h"           // For VXIVector
 
 #ifdef __cplusplus
 class SBlogErrorMapper;
 #else
 typedef struct SBlogErrorMapper { void * dummy; } SBlogErrorMapper;
 #endif
 
 /**
  * @name SBlogMapper
  * @memo Reference implementation of XML error lookup functions
  * @doc These functions are used to load OpenSpeech Browser PIK XML
  * error mapping files at startup, using them to map module name/error
  * ID pairs to human readable error text strings and severity IDs.
  */
 
 /*@{*/
 
 /**
  * Create a new XML error mapper
  *
  * @param errorMapFiles   [IN] VXIVector of local OpenSpeech Browser PIK
  *                             XML error mapping files
  * @param mapper          [OUT] Handle to the error mapper
  *
  * @result VXIlog_RESULT_SUCCESS on success 
  */
 SBLOGMAPPER_API VXIlogResult 
 SBlogErrorMapperCreate(const VXIVector    *errorMapFiles,
 		       SBlogErrorMapper  **mapper);
 
 /**
  * Destroy an XML error mapper
  *
  * @param mapper          [IN/OUT] Handle to the error mapper, set
  *                                 to NULL on success
  *
  * @result VXIlog_RESULT_SUCCESS on success 
  */
 SBLOGMAPPER_API VXIlogResult
 SBlogErrorMapperDestroy(SBlogErrorMapper **mapper);
 
 /**
  * Map an error ID to text and a severity
  *
  * @param mapper      [IN] Handle to the error mapper
  * @param errorID     [IN] Error ID to map as passed to VXIlog::Error( )
  * @param moduleName  [IN] Module name reporting the error as passed to
  *                         VXIlog::Error( )
  * @param errorText   [OUT] Error text as defined in the error mapping
  *                          file. Owned by the error text mapper, must
  *                          not be modified or freed.
  * @param severity    [OUT] Severity identifier as defined in the error
  *                          mapping file. Owned by the error text mapper,
  *                          must not be modified or freed. Typically one
  *                          of the following:
  *                            0 -> UNKNOWN (no mapping found)
  *                            1 -> CRITICAL
  *                            2 -> SEVERE
  *                            3 -> WARNING
  *
  * @result VXIlog_RESULT_SUCCESS on success 
  */
 SBLOGMAPPER_API VXIlogResult
 SBlogErrorMapperGetErrorInfo(SBlogErrorMapper  *mapper,
 			     VXIunsigned        errorID,
 			     const VXIchar     *moduleName,
 			     const VXIchar    **errorText,
 			     VXIint            *severityLevel); 
 
 /*@}*/
 
 #include "VXIheaderSuffix.h"
 
 #endif /* include guard */
