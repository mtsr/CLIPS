#include "clips.h"

#define CLIPSJNI_DATA 67

struct clipsJNIData
  { 
   int javaExternalAddressID;
      
   jclass environmentClass;
   jobject environmentObject;

   jclass userFunctionClass;
   jmethodID userFunctionEvaluateMethod;
  
   jclass classClass;
   jmethodID classGetCanonicalNameMethod;
   
   jclass longClass;
   jmethodID longInitMethod;

   jclass doubleClass;
   jmethodID doubleInitMethod;
   
   jclass arrayListClass;
   jmethodID arrayListInitMethod;
   jmethodID arrayListAddMethod;

   jclass primitiveValueClass;
   jmethodID getCLIPSTypeValueMethod;

   jclass voidValueClass;
   jmethodID voidValueInitMethod;

   jclass integerValueClass;
   jmethodID integerValueInitMethod;
   jmethodID integerValueLongValueMethod;
   jclass floatValueClass;
   jmethodID floatValueInitMethod;
   jmethodID floatValueDoubleValueMethod;

   jclass symbolValueClass;
   jmethodID symbolValueInitMethod;
   jclass stringValueClass;
   jmethodID stringValueInitMethod;
   jclass instanceNameValueClass;
   jmethodID instanceNameValueInitMethod;
   
   jclass lexemeValueClass;
   jmethodID lexemeValueGetValueMethod;

   jclass multifieldValueClass;
   jmethodID multifieldValueInitMethod;
   jmethodID multifieldValueGetMethod;
   jmethodID multifieldValueSizeMethod;
   
   jclass factAddressValueClass;
   jmethodID factAddressValueInitMethod;
   jmethodID factAddressValueGetFactAddressMethod;

   jclass instanceAddressValueClass;
   jmethodID instanceAddressValueInitMethod;
   jmethodID instanceAddressValueGetInstanceAddressMethod;
  };

#define CLIPSJNIData(theEnv) ((struct clipsJNIData *) GetEnvironmentData(theEnv,CLIPSJNI_DATA))
