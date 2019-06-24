
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
 
 #include "SBcacheMisc.hpp"   // For these classes
 
 // Define static class members
 const char SBcachePath::PATH_SEPARATOR = '/';
 
 // -----1=0-------2=0-------3=0-------4=0-------5=0-------6=0-------7=0-------8
 
 
 // Create the directory tree for the path
 VXIcacheResult SBcachePath::CreateDirectories( ) const
 {
   VXIcacheResult rc = VXIcache_RESULT_SUCCESS;
 
   // Go through the directories for the relative portion and create
   // them
   SBcacheNString::size_type componentLen = _baseLen;
   while ( componentLen != SBcacheNString::npos ) {
     SBcacheNString tempPath (_path, 0, componentLen);
     SBcacheMkdir (tempPath);
     componentLen = _path.find_first_of ("/\\", componentLen + 1);
   }
 
   return rc;
 }
