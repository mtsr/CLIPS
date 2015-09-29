#include "net_sf_clipsrules_jni_Environment.h"
#include "clips.h"

#ifndef _Included_clipsjni_glue
#define _Included_clipsjni_glue

globle void       JNIUserFunction(void *,DATA_OBJECT_PTR);
globle int        QueryJNIRouter(void *,const char *);
globle int        ExitJNIRouter(void *,int);
globle int        PrintJNIRouter(void *,const char *,const char *);
globle int        GetcJNIRouter(void *,const char *);
globle int        UngetcJNIRouter(void *,int,const char *);
globle void       JNIPeriodicCallback(void *);
globle void       JNIParserErrorCallback(void *,const char *,const char *,const char *,long);
globle void       PrintJavaAddress(void *,const char *,void *);
globle void       NewJavaAddress(void *,DATA_OBJECT *);
globle intBool    CallJavaMethod(void *,DATA_OBJECT *,DATA_OBJECT *);
globle intBool    DiscardJavaAddress(void *,void *);
globle jlong      CreateCLIPSJNIEnvironment(JNIEnv *,jobject);

#endif