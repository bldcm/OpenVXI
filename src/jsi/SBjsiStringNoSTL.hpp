/*****************************************************************************
  *****************************************************************************
  *
  * $Id: SBjsiStringNoSTL.hpp,v 1.1.2.3 2003/10/06 19:19:17 mpanacci Exp $
  *
  * SBjsiString, string class that is a subset of STL wstring
  *
  * The SBjsiString class stores a string in a grow-only buffer, a
  * functional subset of the STL wstring class. This header merely
  * exists to make it easy to eliminate the use of STL wstring which
  * does not exist on some Linux versions.
  *
  *****************************************************************************
  ****************************************************************************/
 

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
 
 #ifndef _SBJSISTRINGNOSTL_HPP__
 #define _SBJSISTRINGNOSTL_HPP__
 
 // Non-STL implementation
 
 #include <stdlib.h>
 #include <wchar.h>
 
 class SBjsiString
 {
  public:
   // Constructors and destructor
   SBjsiString():
     data(NULL),
     allocated(0),
     dataLen(0)
   {}
 
   SBjsiString(const VXIchar *str):
     data(NULL),
     allocated(0),
     dataLen(0)
   {
     operator+=(str);
   }
 
   SBjsiString (const SBjsiString &str):
     data(NULL),
     allocated(0),
     dataLen(0)
   {
     operator+=(str.data);
   }
 
   virtual ~SBjsiString()
   {
     if ( data ) free (data);
   }
 
   // Assignment operators
   SBjsiString& operator=(const SBjsiString &str)
   {
     if (&str != this)
     {
       dataLen = 0;
       operator+=(str.data);
     }
     return *this;
   }
 
   SBjsiString& operator=(const VXIchar *str)
   {
     dataLen = 0;
     return operator+=(str);
     return *this;
   }
 
   // Clear operator
   void clear()
   {
     dataLen = 0;
   }
 
   // Operators for appending data to the string
   SBjsiString& operator+=(const SBjsiString& str)
   {
     return concat(str.data, str.datalen);
   }
 
   SBjsiString& operator+=(const VXIchar *str)
   {
     return concat(str, wcslen(str));
   }
 
   SBjsiString& operator+=(VXIchar c)
   {
     return concat(&c, 1);
   }
 
 	bool operator==(const VXIchar *str)
 	{
 		return wcscmp( str, this->c_str() ) == 0;
 	}
 	
 	bool operator==(const SBjsiString& str)
 	{
 		return wcscmp( str.c_str(), this->c_str() ) == 0;
 	}	
 		
   // Operators to access the string data
   unsigned int length( ) const { return dataLen; }
   const VXIchar *c_str( ) const { return data; }
 
   void replace(int p, int n, VXIchar *s)
   {
     if((s == NULL) return;
     if(p + n > dataLen) n = dataLen - p;
     ::wcsncpy(&data[p], s, n);
   }
 
  private:
 
   SBjsiString& concat(const VXIchar *str, int len)
   {
     if (datalen + len + 1 <= allocated || Grow(len + 1))
     {
       wcsncpy(data + datalen, str, len);
       datalen += len;
       data[datalen] = L'\0';
     }
     return *this;
   }
 
   // Grow the buffer
   bool Grow (unsigned int size)
   {
     if ( size < 128 ) size = 128;
     VXIchar *newData =
       (VXIchar *) realloc (data, (dataLen + size) * sizeof (VXIchar));
     if (!newData) return false;
     data = newData;
     allocated = dataLen + size;
     return true;
   }
 
  private:
   unsigned int  allocated;
   unsigned int  dataLen;
   VXIchar      *data;
 };
 
 #endif /* include guard */
