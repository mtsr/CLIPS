#include "net_sf_clipsrules_jni_Environment.h"
#include "clips.h"

#ifndef _Included_clipsjni_utilities
#define _Included_clipsjni_utilities

globle void       ConvertPrimitiveValueToDataObject(void *,jobject,DATA_OBJECT_PTR);
globle jobject    ConvertDataObject(JNIEnv *,jobject,void *,DATA_OBJECT *);
globle jobject    ConvertSingleFieldValue(JNIEnv *,jobject,void *,int,void *);
globle void      *JLongToPointer(jlong);
globle jlong      PointerToJLong(void *);

#endif