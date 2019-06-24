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

#ifndef _SBCLIENT_CONFIG_CPP_H
#define _SBCLIENT_CONFIG_CPP_H


#include <SBconfig.h>

/** 
    @doc
    Utility C++ interface for working with SBconfig
*/

class SBcfg
{
 public:
  SBcfg(SBcfgInterface *cfg):cfg_(cfg){};
  ~SBcfg(){}

  SBcfgResult Get(const VXIchar *key, VXIint *v, time_t *t){
    SBcfgResult res;
    VXIValue *val;
    res = (*cfg_->Get)(cfg_, key,&val,t); // now been copied out
    if(res == 0){
      if(VXIValueGetType(val) == VALUE_INTEGER){
	*v = VXIIntegerValue((const VXIInteger *)val);
      }else{
	return SBcfg_RESULT_INVALID_ARGUMENT;
      }
      VXIValueDestroy(&val);
      return SBcfg_RESULT_SUCCESS;
    }
    else{
      return SBcfg_RESULT_INVALID_ARGUMENT;
    }
  }

  SBcfgResult Get(const VXIchar *key, VXIbool *v, time_t *t){
    SBcfgResult res;
    VXIValue *val;
    res = (*cfg_->Get)(cfg_, key,&val,t); // now been copied out
    if(res == 0){
      if(VXIValueGetType(val) == VALUE_INTEGER){
	*v = (VXIbool) VXIIntegerValue((const VXIInteger *)val);
      }else{
	return SBcfg_RESULT_INVALID_ARGUMENT;
      }
      VXIValueDestroy(&val);
      return SBcfg_RESULT_SUCCESS;
    }
    else{
      return SBcfg_RESULT_INVALID_ARGUMENT;
    }
  }

  SBcfgResult Get(const VXIchar *key, VXIflt32 *v, time_t *t){
    SBcfgResult res;
    VXIValue *val;
    res = (*cfg_->Get)(cfg_, key,&val,t); // now been copied out
    if(res == 0){
      if(VXIValueGetType(val) == VALUE_FLOAT){
	*v = VXIFloatValue((const VXIFloat *)val);
      }else{
	return SBcfg_RESULT_INVALID_ARGUMENT;
      }
      VXIValueDestroy(&val);
      return SBcfg_RESULT_SUCCESS;
    }
    else{
      return SBcfg_RESULT_INVALID_ARGUMENT;
    }
  }

  SBcfgResult Get(const VXIchar *key, VXIchar **v, time_t *t){
    SBcfgResult res;
    VXIValue *val;
    res = (*cfg_->Get)(cfg_, key,&val,t); // now been copied out
    if(res == 0){
      if(VXIValueGetType(val) == VALUE_STRING){
		  VXIchar *v_ptr = NULL;
		  const VXIchar *v_str = VXIStringCStr((const VXIString *)val);
		  int len = wcslen(v_str);
		  v_ptr = new VXIchar[len+1];
		  wcscpy(v_ptr, v_str);

	*v = v_ptr;
      }else{
	return SBcfg_RESULT_INVALID_ARGUMENT;
      }
      VXIValueDestroy(&val);
      return SBcfg_RESULT_SUCCESS;
    }
    else{
      return SBcfg_RESULT_INVALID_ARGUMENT;
    }
  }

  SBcfgResult Get(const VXIchar *key, const VXIchar **v, time_t *t){
    SBcfgResult res;
    const VXIValue *val;
    res = (*cfg_->RefGet)(cfg_, key,&val,t); // now been copied out
    if(res == 0){
      if(VXIValueGetType(val) == VALUE_STRING){
	*v = VXIStringCStr((const VXIString *)val);
      }else{
	return SBcfg_RESULT_INVALID_ARGUMENT;
      }
      return SBcfg_RESULT_SUCCESS;
    }
    else{
      return SBcfg_RESULT_INVALID_ARGUMENT;
    }
  }

 protected:
  SBcfgInterface *cfg_;
};

#endif /* include guard */
