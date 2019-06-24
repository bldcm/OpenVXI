/****************License************************************************
 *
 * Copyright 2000-2001.  SpeechWorks International, Inc.  
 *
 * Use of this software is subject to notices and obligations set forth
 * in the SpeechWorks Public License - Software Version 1.1 which is
 * included with this software.
 *
 * SpeechWorks is a registered trademark, and SpeechWorks Here, 
 * DialogModules and the SpeechWorks logo are trademarks of SpeechWorks 
 * International, Inc. in the United States and other countries. 
 * 
 ***********************************************************************
 *
 * OpenSpeech configuration interface.  This interface takes a uri to
 * read the configuration from.  The URI can be a file, an HTTP, or registry:.
 *
 * Parameters are read using a LDAP like search interface.  
 *
 ************************************************************************
 */

#ifndef _SBCLIENT_INTERFACE_CONFIG_H
#define _SBCLIENT_INTERFACE_CONFIG_H


#include "VXIheaderPrefix.h"
#ifdef SBCFG_EXPORTS
#define SBCFG_API SYMBOL_EXPORT_DECL
#else
#define SBCFG_API SYMBOL_IMPORT_DECL
#endif

#include <VXIvalue.h>

#ifdef __cplusplus
extern "C" {
#endif

struct VXIlogInterface;
struct VXIinetInterface;

typedef enum SBcfgResult{
  /** Fatal error, shutdown          */
  SBcfg_RESULT_FATAL_ERROR         = -100,
  /** Failed to open the requested URI          */
  SBcfg_RESULT_URI_OPENFAIL         = -14,
  /** Unable to find the requested URI          */
  SBcfg_RESULT_URI_NOTFOUND         = -13,
  /** Asked to open an unsupported URI format   */
  SBcfg_RESULT_UNSUPPORTED_URI         = -12,
  /** The interface is not initialized          */
  SBcfg_RESULT_NOT_INITIALIZED         = -10,
  /** Attempt to shutdown while there are still active interfaces  */
  SBcfg_RESULT_ACTIVE_INTERFACES_ERROR         = -9,
  /** I/O error while reading or writing      */
  SBcfg_RESULT_IO_ERROR            =   -8,
  /** Out of memory                  */
  SBcfg_RESULT_OUT_OF_MEMORY       =   -7,
  /** System error, out of service   */
  SBcfg_RESULT_SYSTEM_ERROR        =   -6,
  /** Property name is not valid     */
  SBcfg_RESULT_INVALID_PROP_NAME   =   -3,
  /** Property value is not valid    */
  SBcfg_RESULT_INVALID_PROP_VALUE  =   -2,
  /** Invalid function argument      */
  SBcfg_RESULT_INVALID_ARGUMENT    =   -1,
  /** Success                        */
  SBcfg_RESULT_SUCCESS             =    0,
  /** Normal failure, nothing logged */
  SBcfg_RESULT_FAILURE             =    1,
  /** Non-fatal non-specific error   */
  SBcfg_RESULT_NON_FATAL_ERROR     =    2,
  /** Operation is not supported     */
  SBcfg_RESULT_UNSUPPORTED         =  100
} SBcfgResult;


#define SBconfig_FILE_WPREFIX L"file://"
#define SBconfig_FILE_PREFIX   "file://"

#define SBconfig_HTTP_WPREFIX L"http://"
#define SBconfig_HTTP_PREFIX   "http://"

/*
  @doc 
  Interface to a session specific parameter set.  When first
  constructed a copy of the global configuration is placed into the
  interface.  A session can update to the latest global version using
  UpdateConfig on the session. This will copy in the lastest global
  version overwriting the current saved version.<p>

  There is deliberatly no way to modify the global configuration
  properties except by externally modifying the file. <p>.

  Parameters in the configuration database are identified using
  the reverse domain name namespace convention.  For example a
  recognizer parameter may be named "com.sb.rec.parameter", or
  "client.cache.parameter"  <p> 

  For file based configuration, a parameter is named using this
  convention in the first column, then in the second column the type
  of the parameter is provided, and in the final column the value is
  provided.  Environment subsitution is allowed in the configuration
  files.  <p>

  The valid types are: VXIString, VXIInteger, VXIFloat. <p>

  Any parameter that is of the form $(parameter), is assumed to be
  an environment variable.  The environment is taken as the root set of parameter
  variables.<p>

  Because of the reverse domain name convention, the Find function
  returns a VXIMap.  All keys whos beginning part match the prefix
  will be returned in the VXIMap arg.  The Map will contain as keys
  all the matching keys, and as values all the resulting values.  So
  for example the prefix "client" will return all keys which start
  with the string "client."  If the Find is called with a NULL search
  parameter, then all keys are returned.<p> 
  
  Get and Set deal with singleton values only. They return VXIValue
  pointers so that they return polymorphic types of VXIInteger or
  VXIString. <p>

  Get and Find return INVALID_PROP_NAME if the key (or prefix) is not
  found in the table.<p>

  Set will overwrite the current value of a property if it is called
  with the name of a pre-existing property.<p>

  Operations are thread safe.
 */

typedef struct SBcfgInterface
{
  /**
   * @name GetVersion
   * @memo Get the VXI interface version implemented
   *
   * @return  VXIint32 for the version number. The high high word is 
   *          the major version number, the low word is the minor version 
   *          number, using the native CPU/OS byte order. The current
   *          version is VXI_CURRENT_VERSION as defined in VXItypes.h.
   */ 
  VXIint32 (*GetVersion)(void);
  
  /**
   * @name GetImplementationName
   * @memo Get the name of the implementation
   *
   * @return  Implementation defined string that must be different from
   *          all other implementations. The recommended name is one
   *          where the interface name is prefixed by the implementator's
   *          Internet address in reverse order, such as com.xyz.rec for
   *          VXIrec from xyz.com. This is similar to how VoiceXML 1.0
   *          recommends defining application specific error types.
   */
  const VXIchar* (*GetImplementationName)(void);


  /**
   @name Find
   **/ 
  SBcfgResult (*Find)(struct SBcfgInterface *pThis,
		       const VXIchar *prefix,  
		       VXIMap ** Arg,
		       time_t *lastModified);

  SBcfgResult (*Get)(struct SBcfgInterface *pThis,
		      const VXIchar *key,       
		      VXIValue ** Arg,
		      time_t  *lastModified);

  SBcfgResult (*RefGet)(struct SBcfgInterface *pThis,
			const VXIchar *key,       
			const VXIValue ** Arg,
			time_t  *lastModified);

  /**
     @doc Set the value of a key locally to this configuation
     interface. Ownership of the value is passed to the config interface so
     it should not be deleted by the caller.
  */
  SBcfgResult (*Set)(struct SBcfgInterface *pThis,
		      const VXIchar *key, 
		     VXIValue *val);
}SBcfgInterface;



/**
  @doc Initialize the configuration reading system. This specifices
  the inet component to be used for reading form the internet and the
  logging component to use for logging errors.<p> 

  Errors are all offset in number by diagLogBase.
 */

SBCFG_API SBcfgResult SBcfgInit();

  /**
     @doc
     Used to set the logging interface. This may not be callable until
     after the basic configuration has been read. The general
     bootstrep order is:
     - initialize the configuration interface.
     - read the basic configuration using a file URL (file:), HTTP URL
     (http:).
     - initialize the global log and the global inet
     - set the logging interface
     - read the remaining configuration information using these tools
  */
SBCFG_API SBcfgResult SBcfgSetLog(VXIlogInterface     *log, 
				     VXIunsigned          diagLogBase);

/**
  @doc Re-read the configuration URL and parse the resulting
  information.  The resulting configuration parameters can then be
  read through each session interface.  After calling this function
  existing session interfaces are not updated to the new value. Call SBcfgUpdateConfig
  to update all configurations to the latest global configuration.<p>

  The inet can be null if configURI is file:.  In this
  case the native methods of the platform will be used.  
*/
  
SBCFG_API SBcfgResult SBcfgParseURI(VXIinetInterface    *inet,
				       const char          *configURI);


/**
  @doc Updates the configuration of all configuration interfaces to
  the latest global value.
*/
  
SBCFG_API SBcfgResult SBcfgUpdateConfig();

/**
  @doc Globably set the configuration value for all configuration interfaces.
*/
  
SBCFG_API SBcfgResult SBcfgSet(const VXIchar *key,
				  const VXIValue *value);

/**
  @doc Saves the current global value of the configuration back out to
  a URI. Not guaranteed to rewrite any comments from the original
  file.  The URI can only be file:.
*/
  
SBCFG_API SBcfgResult SBcfgSaveURI(VXIinetInterface    *inet,
				      const char          *configURI);

/**
  @doc Shutdown a configuration component.  If shutdown is called
  while there are still active interfaces it will return an error
  ACTIVE_INTERFACES_ERROR. 
 */
SBCFG_API SBcfgResult SBcfgShutDown();

/**
  @doc Create a resource reader/writer interface. This interface can
  be used for reading parameters from the parsed  configuration data
  or for setting the parameters. Since the configuration database is a
  singleton, changes in parameters are mutexed and changes on one
  interface will be reflected in the other interfaces.
*/
SBCFG_API SBcfgResult SBcfgCreateResource(VXIlogInterface *log,
					     SBcfgInterface **);

/**
  @doc Destroy a reader/writer interface.
 */
SBCFG_API SBcfgResult SBcfgDestroyResource(SBcfgInterface **);





#ifdef __cplusplus
}
#endif

#include "VXIheaderSuffix.h"

#endif /* include guard */
