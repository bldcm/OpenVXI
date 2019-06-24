
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
 
 #ifndef _SBINETSTRINGNOSTL_HPP__
 #define _SBINETSTRINGNOSTL_HPP__
 
 // Non-STL implementation
 
 #include <stdlib.h>
 #include <wchar.h>
 
 class SBinetString
 {
  public:
   // Constructors and destructor
   SBinetString():
     data(NULL),
     allocated(0),
     dataLen(0)
   {}
 
   SBinetString(const VXIchar *str):
     data(NULL),
     allocated(0),
     dataLen(0)
   {
     operator+=(str);
   }
 
   SBinetString (const SBinetString &str):
     data(NULL),
     allocated(0),
     dataLen(0)
   {
     operator+=(str.data);
   }
 
   virtual ~SBinetString()
   {
     if ( data ) free (data);
   }
 
   // Assignment operators
   SBinetString& operator=(const SBinetString &str)
   {
     if (&str != this)
     {
       dataLen = 0;
       operator+=(str.data);
     }
     return *this;
   }
 
   SBinetString& operator=(const VXIchar *str)
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
   SBinetString& operator+=(const SBinetString& str)
   {
     return concat(str.data, str.datalen);
   }
 
   SBinetString& operator+=(const VXIchar *str)
   {
     return concat(str, wcslen(str));
   }
 
   SBinetString& operator+=(VXIchar c)
   {
     return concat(&c, 1);
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
 
   SBinetString& concat(const VXIchar *str, int len)
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
