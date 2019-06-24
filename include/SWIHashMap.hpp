#ifndef SWIHASHMAP_HPP
 #define SWIHASHMAP_HPP
 

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
 

 #include "SWIutilHeaderPrefix.h"
 
 #include "SWIHashable.hpp"
 
 /**
  * class to abstract hash table operations.
  * @doc <p>
  **/
 
 class SWIUTIL_API_CLASS SWIHashMap
 {
  public:
   static const int DEFAULT_CAPACITY;
   static const double DEFAULT_LOAD_FACTOR;
 
   // ................. CONSTRUCTORS, DESTRUCTOR  ............
   //
   // ------------------------------------------------------------
  public:
   SWIHashMap();
 
   SWIHashMap(int capacity);
 
   SWIHashMap(int capacity ,
              double loadFactor);
 
   /**
    * Destructor.
    **/
  public:
   virtual ~SWIHashMap();
 
   /**
    * Copy constructor.
    **/
  public:
   SWIHashMap(const SWIHashMap&);
 
   /**
    * Assignment operator.
    **/
  public:
   SWIHashMap& operator=(const SWIHashMap&);
 
  public:
   int size() const;
 
  public:
   bool isEmpty() const;
 
  public:
   const void *getValue(const SWIHashable& key) const;
 
  public:
   const void *putValue(const SWIHashable& key, const void *value);
 
  public:
   const void *remove(const SWIHashable& key);
 
  public:
   void clear();
 
  public:
   class SWIUTIL_API_CLASS Entry
   {
    private:
     Entry(const SWIHashable *key, const void *value);
     ~Entry();
 
    public:
     const SWIHashable& getKey() const;
     const void *getValue() const;
     const void *setValue(const void *value);
 
    private:
     friend class SWIHashMap;
     SWIHashable *_key;
     const void *_value;
     unsigned int _hashCode;
     Entry* _next;
     Entry* _prev;
   };
 
  public:
   class SWIUTIL_API_CLASS Iterator
   {
    public:
     Iterator(SWIHashMap& hashMap);
 
    public:
     Iterator(const Iterator& iter);
 
    public:
     Iterator& operator=(const Iterator& iter);
 
    public:
     ~Iterator();
 
    public:
     void reset() const;
 
    public:
     bool hasNext() const;
 
    public:
     const Entry *next() const;
 
    public:
     Entry *next();
 
     // Modification Operations
 
    public:
     const void *setValue(const void *value);
 
    public:
     bool remove();
 
    private:
     friend class SWIHashMap;
 
    private:
     Entry* _cursor;
     SWIHashMap* _hashMap;
     int _idx;
     Entry* _prevCursor;
     int _prevIdx;
   };
 
  private:
   int _capacity;
   double _loadFactor;
   Entry **_entries;
   int _size;
   int _threshold;
 
  private:
   void copy(const SWIHashMap& original);
   void init(int capacity , double loadFactor);
   void rehash();
   void remove(Entry* entry, int idx);
   void advance(Entry*& entry, int& idx);
   friend class Iterator;
 };
 #endif
