#include <iostream>
#include "VXIvalue.h"

static std::basic_ostream<VXIchar>& operator<<(std::basic_ostream<VXIchar>& os,
                                               const VXIValue * val)
{
  if (val == NULL)
    return os << L"NULL";

  switch (VXIValueGetType(val)) {
  case VALUE_BOOLEAN:
    if(VXIBooleanValue(reinterpret_cast<const VXIBoolean*>(val)) == TRUE)
      return os << L"true";
    else
      return os << L"false";
  case VALUE_INTEGER:
    return os << VXIIntegerValue(reinterpret_cast<const VXIInteger*>(val));
  case VALUE_FLOAT:
    return os << VXIFloatValue(reinterpret_cast<const VXIFloat*>(val));
  case VALUE_STRING:
    return os << VXIStringCStr(reinterpret_cast<const VXIString *>(val));
  case VALUE_VECTOR:
    {
      const VXIVector * v = reinterpret_cast<const VXIVector *>(val);
      const VXIunsigned len = VXIVectorLength(v);

      os << L"{ ";
      for (VXIunsigned i = 0; i < len; ++i) {
        if (i != 0) os << L", ";
        os << VXIVectorGetElement(v, i);
      }
      return os << L" }";
    }
  case VALUE_MAP:
    {
      const VXIMap * m = reinterpret_cast<const VXIMap *>(val);
      const VXIchar * key;
      const VXIValue * value;

      os << L"{ ";
      if (VXIMapNumProperties(m) != 0) {
        VXIMapIterator * i = VXIMapGetFirstProperty(m, &key, &value);
        os << L"(" << key << L", " << value << L")";
        
        while (VXIMapGetNextProperty(i, &key, &value)
               == VXIvalue_RESULT_SUCCESS)
          os << L", (" << key << L", " << value << L")";

        VXIMapIteratorDestroy(&i);
      }
      return os << L" }";
    }
  default:
    break;
  }

  return os << L"(unprintable result)";
}


static std::basic_ostream<VXIchar>& operator<<(std::basic_ostream<VXIchar>& os,
                                               const VXIMap * val)
{
  return os << reinterpret_cast<const VXIValue *>(val);
}
