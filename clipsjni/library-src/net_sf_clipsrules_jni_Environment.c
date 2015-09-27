
#include "net_sf_clipsrules_jni_Environment.h"

#include "clips.h"

#define CLIPSJNI_DATA 67

struct clipsJNIData
  { 
   int javaExternalAddressID;

   jobject tempLocalEnvironment; // TBD replace with environmentObject
   
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

   jclass voidValueClass;
   jmethodID voidValueInitMethod;

   jclass integerValueClass;
   jmethodID integerValueInitMethod;
   jclass floatValueClass;
   jmethodID floatValueInitMethod;

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

   jclass factAddressValueClass;
   jmethodID factAddressValueInitMethod;

   jclass instanceAddressValueClass;
   jmethodID instanceAddressValueInitMethod;
  };

#define CLIPSJNIData(theEnv) ((struct clipsJNIData *) GetEnvironmentData(theEnv,CLIPSJNI_DATA))

static int        QueryJNIRouter(void *,const char *);
static int        ExitJNIRouter(void *,int);
static int        PrintJNIRouter(void *,const char *,const char *);
static int        GetcJNIRouter(void *,const char *);
static int        UngetcJNIRouter(void *,int,const char *);
static void       DeallocateJNIData(void *);
static jobject    ConvertSingleFieldValue(JNIEnv *,jobject,void *,int,void *);
static jobject    ConvertDataObject(JNIEnv *,jobject,void *,DATA_OBJECT *);
static void      *JLongToPointer(jlong);
static jlong      PointerToJLong(void *);
static void       PrintJavaAddress(void *,const char *,void *);
static void       NewJavaAddress(void *,DATA_OBJECT *);
static intBool    CallJavaMethod(void *,DATA_OBJECT *,DATA_OBJECT *);
static intBool    DiscardJavaAddress(void *,void *);

/**********************************************/
/* DeallocateJNIData: Deallocates environment */
/*    data for the JNI functionality.         */
/**********************************************/
static void DeallocateJNIData(
  void *theEnv)
  {
   JNIEnv *env;
   
   env = (JNIEnv *) GetEnvironmentContext(theEnv);

   (*env)->DeleteGlobalRef(env,CLIPSJNIData(theEnv)->environmentClass);
   (*env)->DeleteWeakGlobalRef(env,CLIPSJNIData(theEnv)->environmentObject);
   (*env)->DeleteGlobalRef(env,CLIPSJNIData(theEnv)->userFunctionClass);
   (*env)->DeleteGlobalRef(env,CLIPSJNIData(theEnv)->classClass);
   (*env)->DeleteGlobalRef(env,CLIPSJNIData(theEnv)->longClass);
   (*env)->DeleteGlobalRef(env,CLIPSJNIData(theEnv)->doubleClass);
   (*env)->DeleteGlobalRef(env,CLIPSJNIData(theEnv)->arrayListClass);
   (*env)->DeleteGlobalRef(env,CLIPSJNIData(theEnv)->voidValueClass);
   (*env)->DeleteGlobalRef(env,CLIPSJNIData(theEnv)->integerValueClass);
   (*env)->DeleteGlobalRef(env,CLIPSJNIData(theEnv)->floatValueClass);
   (*env)->DeleteGlobalRef(env,CLIPSJNIData(theEnv)->symbolValueClass);
   (*env)->DeleteGlobalRef(env,CLIPSJNIData(theEnv)->stringValueClass);
   (*env)->DeleteGlobalRef(env,CLIPSJNIData(theEnv)->instanceNameValueClass);
   (*env)->DeleteGlobalRef(env,CLIPSJNIData(theEnv)->lexemeValueClass);
   (*env)->DeleteGlobalRef(env,CLIPSJNIData(theEnv)->multifieldValueClass);
   (*env)->DeleteGlobalRef(env,CLIPSJNIData(theEnv)->factAddressValueClass);
   (*env)->DeleteGlobalRef(env,CLIPSJNIData(theEnv)->instanceAddressValueClass);
  }

/********************/
/* JNIUserFunction: */
/********************/
globle void JNIUserFunction(
  void *theEnv,
  DATA_OBJECT_PTR result)
  {
   JNIEnv *env;
   jobject context;
   jobject arguments, targ, rv;
   int i, argCount;
   DATA_OBJECT theArg;
   
   result->type = RVOID;

   env = (JNIEnv *) GetEnvironmentContext(theEnv);
   
   context = GetEnvironmentFunctionContext(theEnv);

   argCount = EnvRtnArgCount(theEnv);
   arguments = (*env)->NewObject(env,
                                 CLIPSJNIData(theEnv)->arrayListClass,
                                 CLIPSJNIData(theEnv)->arrayListInitMethod,
                                 (jint) argCount);
                                 
   for (i = 1; i <= argCount; i++)
     {
      EnvRtnUnknown(theEnv,i,&theArg);
      targ = ConvertDataObject(env,CLIPSJNIData(theEnv)->environmentObject,theEnv,&theArg); 
      
      if (targ != NULL)
        { 
         (*env)->CallBooleanMethod(env,arguments,CLIPSJNIData(theEnv)->arrayListAddMethod,targ); 
         (*env)->DeleteLocalRef(env,targ);
        }
     }

   rv = (*env)->CallObjectMethod(env,context,CLIPSJNIData(theEnv)->userFunctionEvaluateMethod,arguments);  
   if (rv != NULL)
     {
      if ((*env)->IsInstanceOf(env,rv,CLIPSJNIData(theEnv)->symbolValueClass))
        {
         result->type = SYMBOL; 
         jstring theString = (*env)->CallObjectMethod(env,rv,CLIPSJNIData(theEnv)->lexemeValueGetValueMethod);
         const char *cString = (*env)->GetStringUTFChars(env,theString,NULL);
         result->value = EnvAddSymbol(theEnv,cString);
         (*env)->ReleaseStringUTFChars(env,theString,cString);
        }
      else if ((*env)->IsInstanceOf(env,rv,CLIPSJNIData(theEnv)->stringValueClass))
        { 
         result->type = STRING; 
         jstring theString = (*env)->CallObjectMethod(env,rv,CLIPSJNIData(theEnv)->lexemeValueGetValueMethod);
         const char *cString = (*env)->GetStringUTFChars(env,theString,NULL);
         result->value = EnvAddSymbol(theEnv,cString);
         (*env)->ReleaseStringUTFChars(env,theString,cString);
        }
      else if ((*env)->IsInstanceOf(env,rv,CLIPSJNIData(theEnv)->instanceNameValueClass))
        { 
         result->type = INSTANCE_NAME; 
         jstring theString = (*env)->CallObjectMethod(env,rv,CLIPSJNIData(theEnv)->lexemeValueGetValueMethod);
         const char *cString = (*env)->GetStringUTFChars(env,theString,NULL);
         result->value = EnvAddSymbol(theEnv,cString);
         (*env)->ReleaseStringUTFChars(env,theString,cString);
        }
      else if ((*env)->IsInstanceOf(env,rv,CLIPSJNIData(theEnv)->integerValueClass))
        { 
         result->type = INTEGER; 
        }
      else if ((*env)->IsInstanceOf(env,rv,CLIPSJNIData(theEnv)->floatValueClass))
        { 
         result->type = FLOAT; 
        }
     }

   (*env)->DeleteLocalRef(env,arguments);
  }

/************************/
/* JNIPeriodicCallback: */
/************************/
static void JNIPeriodicCallback(
  void *theEnv)
  {
   jobject context;
   jclass cls;
   JNIEnv *env;
   jmethodID mid;
  
   env = (JNIEnv *) GetEnvironmentContext(theEnv);

   context = GetEnvironmentCallbackContext(theEnv);
  
   cls = (*env)->GetObjectClass(env,context);

   mid = (*env)->GetMethodID(env,cls,"periodicCallback","()V");

   (*env)->DeleteLocalRef(env,cls);

   if (mid == NULL)
     { return; }

   (*env)->CallVoidMethod(env,context,mid);      
  }

/***************************/
/* JNIParserErrorCallback: */
/***************************/
static void JNIParserErrorCallback(
  void *theEnv,
  const char *fileName,
  const char *warningString,
  const char *errorString,
  long lineNumber)
  {
   JNIEnv *env;
   jmethodID mid;
   jstring str1, str2;

   if (CLIPSJNIData(theEnv)->tempLocalEnvironment == NULL)
     { return; }
     
   if (errorString == NULL) 
     { return; }
   
   env = (JNIEnv *) GetEnvironmentContext(theEnv);
   
   mid = (*env)->GetMethodID(env,CLIPSJNIData(theEnv)->environmentClass,"addError","(Ljava/lang/String;JLjava/lang/String;)V");

   if (mid == NULL)
     { return; }

   str1 = (*env)->NewStringUTF(env,fileName);
   str2 = (*env)->NewStringUTF(env,errorString);

   (*env)->CallVoidMethod(env,CLIPSJNIData(theEnv)->tempLocalEnvironment,mid,str1,lineNumber,str2);

   (*env)->DeleteLocalRef(env,str1);
   (*env)->DeleteLocalRef(env,str2);
  }

/*************************************************/
/* FindJNIRouter: Query routine for JNI routers. */
/*************************************************/
static int QueryJNIRouter(
  void *theEnv,
  const char *logicalName)
  {
   jobject context;
   jclass cls;
   JNIEnv *env;
   jmethodID mid;
   jboolean rv;
   jstring str;
  
   env = (JNIEnv *) GetEnvironmentContext(theEnv);

   context = GetEnvironmentRouterContext(theEnv);
  
   cls = (*env)->GetObjectClass(env,context);

   mid = (*env)->GetMethodID(env,cls,"query","(Ljava/lang/String;)Z");

   (*env)->DeleteLocalRef(env,cls);

   if (mid == NULL)
     { return FALSE; }

   str = (*env)->NewStringUTF(env,logicalName);

   rv = (*env)->CallBooleanMethod(env,context,mid,str);
      
   (*env)->DeleteLocalRef(env,str);
      
   return(rv);
  }

/*************************************************/
/* ExitJNIRouter:  Exit routine for JNI routers. */
/*************************************************/
#if IBM_TBC
#pragma argsused
#endif
static int ExitJNIRouter(
  void *theEnv,
  int num)
  {
#if MAC_MCW || IBM_MCW || MAC_XCD
#pragma unused(num)
#endif
#if MAC_MCW || IBM_MCW || MAC_XCD
#pragma unused(theEnv)
#endif
   /* TBD deallocate global context reference */
   return(1);
  }

/**************************************************/
/* PrintJNIRouter: Print routine for JNI routers. */
/**************************************************/
static int PrintJNIRouter(
  void *theEnv,
  const char *logicalName,
  const char *str)
  {
   jobject context;
   JNIEnv *env;
   jmethodID mid;
   jclass cls;
   jstring str1, str2;

   env = (JNIEnv *) GetEnvironmentContext(theEnv);

   context = GetEnvironmentRouterContext(theEnv);

   cls = (*env)->GetObjectClass(env,context);

   mid = (*env)->GetMethodID(env,cls,"print","(Ljava/lang/String;Ljava/lang/String;)V");

   (*env)->DeleteLocalRef(env,cls);

   if (mid == NULL)
     { return FALSE; }

   str1 = (*env)->NewStringUTF(env,logicalName);
   str2 = (*env)->NewStringUTF(env,str);

   (*env)->CallVoidMethod(env,context,mid,str1,str2);

   (*env)->DeleteLocalRef(env,str1);
   (*env)->DeleteLocalRef(env,str2);
   
   return(1);
  }

/************************************************/
/* GetcJNIRouter: Getc routine for JNI routers. */
/************************************************/
static int GetcJNIRouter(
  void *theEnv,
  const char *logicalName)
  {
   jint theChar;
   jobject context;
   JNIEnv *env;
   jmethodID mid;
   jclass cls;
   jstring str;

   env = (JNIEnv *) GetEnvironmentContext(theEnv);

   context = GetEnvironmentRouterContext(theEnv);

   cls = (*env)->GetObjectClass(env,context);

   mid = (*env)->GetMethodID(env,cls,"getchar","(Ljava/lang/String;)I");

   (*env)->DeleteLocalRef(env,cls);

   if (mid == NULL)
     { return -1; }

   str = (*env)->NewStringUTF(env,logicalName);

   theChar = (*env)->CallIntMethod(env,context,mid,str);

   (*env)->DeleteLocalRef(env,str);

   return((int) theChar);
  }

/****************************************************/
/* UngetcJNIRouter: Ungetc routine for JNI routers. */
/****************************************************/
static int UngetcJNIRouter(
  void *theEnv,
  int ch,
  const char *logicalName)
  {
   jint theChar;
   jobject context;
   JNIEnv *env;
   jmethodID mid;
   jclass cls;
   jstring str;

   env = (JNIEnv *) GetEnvironmentContext(theEnv);

   context = GetEnvironmentRouterContext(theEnv);

   cls = (*env)->GetObjectClass(env,context);

   mid = (*env)->GetMethodID(env,cls,"ungetchar","(Ljava/lang/String;I)I");

   (*env)->DeleteLocalRef(env,cls);

   if (mid == NULL)
     { return -1; }

   str = (*env)->NewStringUTF(env,logicalName);

   theChar = (*env)->CallIntMethod(env,context,mid,(jint) ch,str);

   (*env)->DeleteLocalRef(env,str);

   return((int) theChar);
  }

/******************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_getCLIPSVersion: Native */
/*   function for the CLIPSJNI getCLIPSVersion method.            */
/* Class:     CLIPSJNI_Environment                                */
/* Method:    getCLIPSVersion                                     */
/* Signature: ()Ljava/lang/String;                                */
/******************************************************************/
JNIEXPORT jstring JNICALL Java_net_sf_clipsrules_jni_Environment_getCLIPSVersion(
  JNIEnv *env, 
  jclass cls) 
  {
   return (*env)->NewStringUTF(env,VERSION_STRING);
  }

/*************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_createEnvironment: */
/*                                                           */
/*    Class:     net_sf_clipsrules_jni_Environment           */
/*    Method:    createEnvironment                           */
/*    Signature: ()J                                         */
/*************************************************************/
JNIEXPORT jlong JNICALL Java_net_sf_clipsrules_jni_Environment_createEnvironment(
  JNIEnv *env, 
  jobject obj)
  {
   void *theEnv;
   jclass theEnvironmentClass; 
   jclass theUserFunctionClass; 
   jmethodID theUserFunctionEvaluateMethod;
   jclass theClassClass; 
   jmethodID theClassGetCanonicalNameMethod;
   jclass theLongClass; 
   jmethodID theLongInitMethod;
   jclass theDoubleClass; 
   jmethodID theDoubleInitMethod;
   jclass theArrayListClass; 
   jmethodID theArrayListInitMethod, theArrayListAddMethod;
   jclass theVoidValueClass;
   jmethodID theVoidValueInitMethod;
   jclass theIntegerValueClass, theFloatValueClass;
   jmethodID theIntegerValueInitMethod, theFloatValueInitMethod;
   jclass theSymbolValueClass, theStringValueClass, theInstanceNameValueClass;
   jmethodID theSymbolValueInitMethod, theStringValueInitMethod, theInstanceNameValueInitMethod;
   jclass theLexemeValueClass;
   jmethodID theLexemeValueGetValueMethod;
   jclass theMultifieldValueClass;
   jmethodID theMultifieldValueInitMethod;
   jclass theFactAddressValueClass;
   jmethodID theFactAddressValueInitMethod;
   jclass theInstanceAddressValueClass;
   jmethodID theInstanceAddressValueInitMethod;
   struct externalAddressType javaPointer = { "java", PrintJavaAddress, PrintJavaAddress, DiscardJavaAddress, NewJavaAddress, CallJavaMethod };

   /*===========================*/
   /* Look up the Java classes. */
   /*===========================*/

   theEnvironmentClass = (*env)->FindClass(env,"net/sf/clipsrules/jni/Environment"); 
   theUserFunctionClass = (*env)->FindClass(env,"net/sf/clipsrules/jni/UserFunction"); 
   theClassClass = (*env)->FindClass(env,"java/lang/Class"); 
   theLongClass = (*env)->FindClass(env,"java/lang/Long"); 
   theDoubleClass = (*env)->FindClass(env,"java/lang/Double"); 
   theArrayListClass = (*env)->FindClass(env,"java/util/ArrayList"); 
   theVoidValueClass = (*env)->FindClass(env,"net/sf/clipsrules/jni/VoidValue");
   theIntegerValueClass = (*env)->FindClass(env,"net/sf/clipsrules/jni/IntegerValue");
   theFloatValueClass = (*env)->FindClass(env,"net/sf/clipsrules/jni/FloatValue");
   theSymbolValueClass = (*env)->FindClass(env,"net/sf/clipsrules/jni/SymbolValue");
   theStringValueClass = (*env)->FindClass(env,"net/sf/clipsrules/jni/StringValue");
   theInstanceNameValueClass = (*env)->FindClass(env,"net/sf/clipsrules/jni/InstanceNameValue");
   theLexemeValueClass = (*env)->FindClass(env,"net/sf/clipsrules/jni/LexemeValue");
   theMultifieldValueClass = (*env)->FindClass(env,"net/sf/clipsrules/jni/MultifieldValue");
   theFactAddressValueClass = (*env)->FindClass(env,"net/sf/clipsrules/jni/FactAddressValue");
   theInstanceAddressValueClass = (*env)->FindClass(env,"net/sf/clipsrules/jni/InstanceAddressValue");
                
   /*=========================================*/
   /* If the Java classes could not be found, */
   /* abort creation of the environment.      */
   /*=========================================*/
   
   if ((theEnvironmentClass == NULL) ||
       (theUserFunctionClass == NULL) ||
       (theClassClass == NULL) ||
       (theLongClass == NULL) || (theDoubleClass == NULL) ||
       (theArrayListClass == NULL) ||
       (theVoidValueClass == NULL) ||
       (theIntegerValueClass == NULL) || (theFloatValueClass == NULL) ||
       (theSymbolValueClass == NULL) || (theStringValueClass == NULL) || 
       (theInstanceNameValueClass == NULL) ||
       (theLexemeValueClass == NULL) ||
       (theMultifieldValueClass == NULL) ||
       (theFactAddressValueClass == NULL) ||
       (theInstanceAddressValueClass == NULL))
     { return((jlong) NULL); }
     
   /*================================*/
   /* Look up the Java init methods. */
   /*================================*/

   theUserFunctionEvaluateMethod = 
      (*env)->GetMethodID(env,theUserFunctionClass,"evaluate",
                          "(Ljava/util/List;)Lnet/sf/clipsrules/jni/PrimitiveValue;");
   
   theClassGetCanonicalNameMethod = (*env)->GetMethodID(env,theClassClass,"getCanonicalName","()Ljava/lang/String;");
   theLongInitMethod = (*env)->GetMethodID(env,theLongClass,"<init>","(J)V");
   theDoubleInitMethod = (*env)->GetMethodID(env,theDoubleClass,"<init>","(D)V");
   theArrayListInitMethod = (*env)->GetMethodID(env,theArrayListClass,"<init>","(I)V");
   theArrayListAddMethod = (*env)->GetMethodID(env,theArrayListClass,"add","(Ljava/lang/Object;)Z");
   theVoidValueInitMethod = (*env)->GetMethodID(env,theVoidValueClass,"<init>","()V");
   theIntegerValueInitMethod = (*env)->GetMethodID(env,theIntegerValueClass,"<init>","(Ljava/lang/Long;)V");
   theFloatValueInitMethod = (*env)->GetMethodID(env,theFloatValueClass,"<init>","(Ljava/lang/Double;)V");
   theSymbolValueInitMethod = (*env)->GetMethodID(env,theSymbolValueClass,"<init>","(Ljava/lang/String;)V");
   theStringValueInitMethod = (*env)->GetMethodID(env,theStringValueClass,"<init>","(Ljava/lang/String;)V");
   theLexemeValueGetValueMethod = (*env)->GetMethodID(env,theLexemeValueClass,"getValue","()Ljava/lang/String;");
   theInstanceNameValueInitMethod = (*env)->GetMethodID(env,theInstanceNameValueClass,"<init>","(Ljava/lang/String;)V");
   theMultifieldValueInitMethod = (*env)->GetMethodID(env,theMultifieldValueClass,"<init>","(Ljava/util/List;)V");
   theFactAddressValueInitMethod = (*env)->GetMethodID(env,theFactAddressValueClass,"<init>","(JLnet/sf/clipsrules/jni/Environment;)V");
   theInstanceAddressValueInitMethod = (*env)->GetMethodID(env,theInstanceAddressValueClass,"<init>","(JLnet/sf/clipsrules/jni/Environment;)V");

   /*==============================================*/
   /* If the Java init methods could not be found, */
   /* abort creation of the enviroment.            */
   /*==============================================*/

   if ((theUserFunctionEvaluateMethod == NULL) ||
       (theClassGetCanonicalNameMethod == NULL) ||
       (theLongInitMethod == NULL) || (theDoubleInitMethod == NULL) || 
       (theArrayListInitMethod == NULL) || (theArrayListAddMethod == NULL) ||
       (theVoidValueInitMethod == NULL) ||
       (theIntegerValueInitMethod == NULL) || (theFloatValueInitMethod == NULL) ||
       (theSymbolValueInitMethod == NULL) || (theStringValueInitMethod == NULL) ||
       (theInstanceNameValueInitMethod == NULL) ||
       (theLexemeValueGetValueMethod == NULL) ||
       (theMultifieldValueInitMethod == NULL) ||
       (theFactAddressValueInitMethod == NULL) ||
       (theInstanceAddressValueInitMethod == NULL))
     { return((jlong) NULL); }
     
   /*=========================*/
   /* Create the environment. */
   /*=========================*/

   theEnv = CreateEnvironment();
   if (theEnv == NULL) return((jlong) NULL);
   
   /*====================================*/
   /* Allocate the JNI environment data. */
   /*====================================*/

   AllocateEnvironmentData(theEnv,CLIPSJNI_DATA,sizeof(struct clipsJNIData),DeallocateJNIData);
   
   /*===================================================*/
   /* Cache the class and method references (converting */
   /* the local class references to global references   */
   /* so they won't be garbage collected.               */
   /*===================================================*/
   
   CLIPSJNIData(theEnv)->tempLocalEnvironment = NULL;
   CLIPSJNIData(theEnv)->environmentClass = (*env)->NewGlobalRef(env,theEnvironmentClass);
   CLIPSJNIData(theEnv)->environmentObject = (*env)->NewWeakGlobalRef(env,obj);
   
   CLIPSJNIData(theEnv)->userFunctionClass = (*env)->NewGlobalRef(env,theUserFunctionClass);
   CLIPSJNIData(theEnv)->userFunctionEvaluateMethod = theUserFunctionEvaluateMethod;
   
   CLIPSJNIData(theEnv)->classClass = (*env)->NewGlobalRef(env,theClassClass);
   CLIPSJNIData(theEnv)->classGetCanonicalNameMethod = theClassGetCanonicalNameMethod;

   CLIPSJNIData(theEnv)->longClass = (*env)->NewGlobalRef(env,theLongClass);
   CLIPSJNIData(theEnv)->longInitMethod = theLongInitMethod;
   CLIPSJNIData(theEnv)->doubleClass = (*env)->NewGlobalRef(env,theDoubleClass);
   CLIPSJNIData(theEnv)->doubleInitMethod = theDoubleInitMethod;
   CLIPSJNIData(theEnv)->arrayListClass = (*env)->NewGlobalRef(env,theArrayListClass);
   CLIPSJNIData(theEnv)->arrayListInitMethod = theArrayListInitMethod;
   CLIPSJNIData(theEnv)->arrayListAddMethod = theArrayListAddMethod;

   CLIPSJNIData(theEnv)->voidValueClass = (*env)->NewGlobalRef(env,theVoidValueClass);
   CLIPSJNIData(theEnv)->voidValueInitMethod = theVoidValueInitMethod;
   
   CLIPSJNIData(theEnv)->integerValueClass = (*env)->NewGlobalRef(env,theIntegerValueClass);
   CLIPSJNIData(theEnv)->integerValueInitMethod = theIntegerValueInitMethod;
   CLIPSJNIData(theEnv)->floatValueClass = (*env)->NewGlobalRef(env,theFloatValueClass);
   CLIPSJNIData(theEnv)->floatValueInitMethod = theFloatValueInitMethod;
      
   CLIPSJNIData(theEnv)->symbolValueClass = (*env)->NewGlobalRef(env,theSymbolValueClass);
   CLIPSJNIData(theEnv)->symbolValueInitMethod = theSymbolValueInitMethod;
   CLIPSJNIData(theEnv)->stringValueClass = (*env)->NewGlobalRef(env,theStringValueClass);
   CLIPSJNIData(theEnv)->stringValueInitMethod = theStringValueInitMethod;
   CLIPSJNIData(theEnv)->instanceNameValueClass = (*env)->NewGlobalRef(env,theInstanceNameValueClass);
   CLIPSJNIData(theEnv)->instanceNameValueInitMethod = theInstanceNameValueInitMethod;
   
   CLIPSJNIData(theEnv)->lexemeValueClass = (*env)->NewGlobalRef(env,theLexemeValueClass);
   CLIPSJNIData(theEnv)->lexemeValueGetValueMethod = theLexemeValueGetValueMethod;

   CLIPSJNIData(theEnv)->multifieldValueClass = (*env)->NewGlobalRef(env,theMultifieldValueClass);
   CLIPSJNIData(theEnv)->multifieldValueInitMethod = theMultifieldValueInitMethod;

   CLIPSJNIData(theEnv)->factAddressValueClass = (*env)->NewGlobalRef(env,theFactAddressValueClass);
   CLIPSJNIData(theEnv)->factAddressValueInitMethod = theFactAddressValueInitMethod;

   CLIPSJNIData(theEnv)->instanceAddressValueClass = (*env)->NewGlobalRef(env,theInstanceAddressValueClass);
   CLIPSJNIData(theEnv)->instanceAddressValueInitMethod = theInstanceAddressValueInitMethod;
   
   /*======================================*/
   /* Store the java environment for later */
   /* access by the CLIPS environment.     */
   /*======================================*/
 
   SetEnvironmentContext(theEnv,(void *) env);
     
   /*=======================================*/
   /* Deallocate the local Java references. */
   /*=======================================*/
   
   (*env)->DeleteLocalRef(env,theEnvironmentClass);
   (*env)->DeleteLocalRef(env,theUserFunctionClass);
   (*env)->DeleteLocalRef(env,theClassClass);
   (*env)->DeleteLocalRef(env,theLongClass);
   (*env)->DeleteLocalRef(env,theDoubleClass);
   (*env)->DeleteLocalRef(env,theArrayListClass);
   (*env)->DeleteLocalRef(env,theVoidValueClass);
   (*env)->DeleteLocalRef(env,theIntegerValueClass);
   (*env)->DeleteLocalRef(env,theFloatValueClass);
   (*env)->DeleteLocalRef(env,theSymbolValueClass);
   (*env)->DeleteLocalRef(env,theStringValueClass);
   (*env)->DeleteLocalRef(env,theInstanceNameValueClass);
   (*env)->DeleteLocalRef(env,theLexemeValueClass);
   (*env)->DeleteLocalRef(env,theMultifieldValueClass);
   (*env)->DeleteLocalRef(env,theFactAddressValueClass);
   (*env)->DeleteLocalRef(env,theInstanceAddressValueClass);

   /*=================================*/
   /* Set up Java External Addresses. */
   /*=================================*/
   
   CLIPSJNIData(theEnv)->javaExternalAddressID = InstallExternalAddressType(theEnv,&javaPointer);
   
   /*===================================*/
   /* Set up the parser error callback. */
   /*===================================*/
   
   EnvSetParserErrorCallback(theEnv,JNIParserErrorCallback);
   
   /*=========================*/
   /* Return the environment. */
   /*=========================*/
   
   return (PointerToJLong(theEnv));
  }

/********************************************************/
/* Java_net_sf_clipsrules_jni_Environment_clear: Native */ 
/*   function for the CLIPSJNI clear method.            */
/*                                                      */
/* Class:     net_sf_clipsrules_jni_Environment         */
/* Method:    clear                                     */
/* Signature: (J)V                                      */
/********************************************************/
JNIEXPORT void JNICALL Java_net_sf_clipsrules_jni_Environment_clear(
  JNIEnv *env, 
  jobject obj,
  jlong clipsEnv)
  {
   void *oldContext = SetEnvironmentContext(JLongToPointer(clipsEnv),(void *) env);
   
   EnvClear(JLongToPointer(clipsEnv));
   
   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext);
  }

/********************************************************/
/* Java_net_sf_clipsrules_jni_Environment_reset: Native */ 
/*   function for the CLIPSJNI reset method.            */
/*                                                      */
/* Class:     net_sf_clipsrules_jni_Environment         */
/* Method:    reset                                     */
/* Signature: (J)V                                      */
/********************************************************/
JNIEXPORT void JNICALL Java_net_sf_clipsrules_jni_Environment_reset(
  JNIEnv *env, 
  jobject obj,
  jlong clipsEnv)
  {
   void *oldContext = SetEnvironmentContext(JLongToPointer(clipsEnv),(void *) env);
   
   EnvReset(JLongToPointer(clipsEnv));
   
   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext);
  }

/*******************************************************/
/* Java_net_sf_clipsrules_jni_Environment_load: Native */ 
/*   function for the CLIPSJNI load method.            */
/*                                                     */
/* Class:     net_sf_clipsrules_jni_Environment        */
/* Method:    load                                     */
/* Signature: (JLjava/lang/String;)V                   */
/*******************************************************/
JNIEXPORT void JNICALL Java_net_sf_clipsrules_jni_Environment_load(
  JNIEnv *env, 
  jobject obj,
  jlong clipsEnv,
  jstring fileName)
  {
   const char *cFileName = (*env)->GetStringUTFChars(env,fileName,NULL);
   jobject oldLoadEnv;
   void *oldContext = SetEnvironmentContext(JLongToPointer(clipsEnv),(void *) env);
   void *theEnv;
   
   theEnv = JLongToPointer(clipsEnv);
   
   oldLoadEnv = CLIPSJNIData(theEnv)->tempLocalEnvironment;
   CLIPSJNIData(theEnv)->tempLocalEnvironment = obj;
   
   EnvLoad(theEnv,(char *) cFileName);
   
   CLIPSJNIData(theEnv)->tempLocalEnvironment = oldLoadEnv;
   
   (*env)->ReleaseStringUTFChars(env,fileName,cFileName);
   
   SetEnvironmentContext(theEnv,oldContext);
  }

/******************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_changeDirectory: Native */ 
/*   function for the CLIPSJNI loadFromString method.             */
/*                                                                */
/* Class:     net_sf_clipsrules_jni_Environment                   */
/* Method:    changeDirectory                                     */
/* Signature: (JLjava/lang/String;)I                              */
/******************************************************************/
JNIEXPORT jint JNICALL Java_net_sf_clipsrules_jni_Environment_changeDirectory(
  JNIEnv *env,
  jobject obj,
  jlong clipsEnv,
  jstring directory)
  {
   int rv;
   const char *cDirectory = (*env)->GetStringUTFChars(env,directory,NULL);
   
   rv = genchdir(cDirectory);
   
   (*env)->ReleaseStringUTFChars(env,directory,cDirectory);
   
   return rv;
  }

/*****************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_loadFromString: Native */ 
/*   function for the CLIPSJNI loadFromString method.            */
/*                                                               */
/* Class:     net_sf_clipsrules_jni_Environment                  */
/* Method:    load                                               */
/* Signature: (JLjava/lang/String;)V                             */
/*****************************************************************/
JNIEXPORT void JNICALL Java_net_sf_clipsrules_jni_Environment_loadFromString(
  JNIEnv *env, 
  jobject obj,
  jlong clipsEnv,
  jstring loadString)
  {
   const char *cLoadString = (*env)->GetStringUTFChars(env,loadString,NULL);
   jobject oldLoadEnv;
   void *oldContext = SetEnvironmentContext(JLongToPointer(clipsEnv),(void *) env);
   void *theEnv;
   
   theEnv = JLongToPointer(clipsEnv);

   oldLoadEnv = CLIPSJNIData(theEnv)->tempLocalEnvironment;
   CLIPSJNIData(theEnv)->tempLocalEnvironment = obj;
  
   OpenStringSource(theEnv,"clipsjniloadfromstring",cLoadString,0); 
   LoadConstructsFromLogicalName(theEnv,"clipsjniloadfromstring");
   CloseStringSource(theEnv,"clipsjniloadfromstring");
   
   CLIPSJNIData(theEnv)->tempLocalEnvironment = oldLoadEnv;
   
   (*env)->ReleaseStringUTFChars(env,loadString,cLoadString);
   
   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext);
  }

/*********************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_getParsingFileName: Native */
/*   function for the CLIPSJNI getParsingFileName method.            */
/*                                                                   */
/* Class:     net_sf_clipsrules_jni_Environment                      */
/* Method:    getParsingFileName                                     */
/* Signature: (J)Ljava/lang/String;                                  */
/*********************************************************************/
JNIEXPORT jstring JNICALL Java_net_sf_clipsrules_jni_Environment_getParsingFileName(
  JNIEnv *env, 
  jobject obj, 
  jlong clipsEnv)
  {
   jstring rv;
   
   char *fileName;
   
   void *oldContext = SetEnvironmentContext(JLongToPointer(clipsEnv),(void *) env);

   fileName = EnvGetParsingFileName(JLongToPointer(clipsEnv));
   
   if (fileName == NULL)
     { 
      rv = (*env)->NewStringUTF(env,""); 
      SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext);
      return rv;
     }
     
   rv = (*env)->NewStringUTF(env,fileName);
   
   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext);
   
   return rv;
  }

/*********************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_setParsingFileName: Native */
/*   function for the CLIPSJNI setParsingFileName method.            */
/*                                                                   */
/* Class:     net_sf_clipsrules_jni_Environment                      */
/* Method:    setParsingFileName                                     */
/* Signature: (JLjava/lang/String;)V                                 */
/*********************************************************************/
JNIEXPORT void JNICALL Java_net_sf_clipsrules_jni_Environment_setParsingFileName(
  JNIEnv *env, 
  jobject obj,
  jlong clipsEnv,
  jstring fileName)
  {
   const char *cFileName = (*env)->GetStringUTFChars(env,fileName,NULL);

   void *oldContext = SetEnvironmentContext(JLongToPointer(clipsEnv),(void *) env);

   EnvSetParsingFileName(JLongToPointer(clipsEnv),(char *) cFileName);
   
   (*env)->ReleaseStringUTFChars(env,fileName,cFileName);
   
   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext);
  }

/************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_loadFacts: Native */ 
/*   function for the CLIPSJNI loadFacts method.            */
/*                                                          */
/* Class:     net_sf_clipsrules_jni_Environment             */
/* Method:    loadFact                                      */
/* Signature: (JLjava/lang/String;)Z                        */
/************************************************************/
JNIEXPORT jboolean JNICALL Java_net_sf_clipsrules_jni_Environment_loadFacts(
  JNIEnv *env, 
  jobject obj,
  jlong clipsEnv,
  jstring fileName)
  {
   jboolean rv;
   const char *cFileName = (*env)->GetStringUTFChars(env,fileName,NULL);
   
   void *oldContext = SetEnvironmentContext(JLongToPointer(clipsEnv),(void *) env);

   rv = EnvLoadFacts(JLongToPointer(clipsEnv),(char *) cFileName);
   
   (*env)->ReleaseStringUTFChars(env,fileName,cFileName);
   
   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext);
   
   return rv;
  }

/********************************************************/
/* Java_net_sf_clipsrules_jni_Environment_watch: Native */ 
/*   function for the CLIPSJNI watch method.            */
/*                                                      */
/* Class:     net_sf_clipsrules_jni_Environment         */
/* Method:    watch                                     */
/* Signature: (JLjava/lang/String;)Z                    */
/********************************************************/
JNIEXPORT jboolean JNICALL Java_net_sf_clipsrules_jni_Environment_watch(
  JNIEnv *env, 
  jobject obj,
  jlong clipsEnv,
  jstring watchItem)
  {
   jboolean rv;
   const char *cWatchItem = (*env)->GetStringUTFChars(env,watchItem,NULL);
   
   void *oldContext = SetEnvironmentContext(JLongToPointer(clipsEnv),(void *) env);

   rv = EnvWatch(JLongToPointer(clipsEnv),(char *) cWatchItem);
   
   (*env)->ReleaseStringUTFChars(env,watchItem,cWatchItem);
   
   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext);
   
   return rv;
  }

/**********************************************************/
/* Java_net_sf_clipsrules_jni_Environment_unwatch: Native */ 
/*   function for the CLIPSJNI unwatch method.            */
/*                                                        */
/* Class:     net_sf_clipsrules_jni_Environment           */
/* Method:    unwatch                                     */
/* Signature: (JLjava/lang/String;)Z                      */
/**********************************************************/
JNIEXPORT jboolean JNICALL Java_net_sf_clipsrules_jni_Environment_unwatch(
  JNIEnv *env, 
  jobject obj,
  jlong clipsEnv,
  jstring watchItem)
  {
   jboolean rv;
   const char *cWatchItem = (*env)->GetStringUTFChars(env,watchItem,NULL);

   void *oldContext = SetEnvironmentContext(JLongToPointer(clipsEnv),(void *) env);

   rv = EnvUnwatch(JLongToPointer(clipsEnv),(char *) cWatchItem);
   
   (*env)->ReleaseStringUTFChars(env,watchItem,cWatchItem);

   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext);

   return rv;
  }

/*******************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_setHaltExecution: Native */ 
/*   function for the CLIPSJNI setHaltExecution method.            */
/*                                                                 */
/* Class:     net_sf_clipsrules_jni_Environment                    */
/* Method:    setHaltExecution                                     */
/* Signature: (JZ)V                                                */
/*******************************************************************/
JNIEXPORT void JNICALL Java_net_sf_clipsrules_jni_Environment_setHaltExecution(
  JNIEnv *env, 
  jobject obj, 
  jlong clipsEnv,
  jboolean value)
  {
   EnvSetHaltExecution(JLongToPointer(clipsEnv),value);
  }

/***************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_setHaltRules: Native */ 
/*   function for the CLIPSJNI setHaltRules method.            */
/*                                                             */
/* Class:     net_sf_clipsrules_jni_Environment                */
/* Method:    setHaltRules                                     */
/* Signature: (JZ)V                                            */
/***************************************************************/
JNIEXPORT void JNICALL Java_net_sf_clipsrules_jni_Environment_setHaltRules(
  JNIEnv *env, 
  jobject obj, 
  jlong clipsEnv,
  jboolean value)
  {
   EnvSetHaltRules(JLongToPointer(clipsEnv),value);
  }

/******************************************************/
/* Java_net_sf_clipsrules_jni_Environment_run: Native */ 
/*   function for the CLIPSJNI run method.            */
/*                                                    */
/* Class:     net_sf_clipsrules_jni_Environment       */
/* Method:    run                                     */
/* Signature: (JJ)J                                   */
/******************************************************/
JNIEXPORT jlong JNICALL Java_net_sf_clipsrules_jni_Environment_run(
  JNIEnv *env, 
  jobject obj, 
  jlong clipsEnv,
  jlong runLimit)
  {
   jlong rv;
   void *oldContext = SetEnvironmentContext(JLongToPointer(clipsEnv),(void *) env);

   rv = EnvRun(JLongToPointer(clipsEnv),runLimit);

   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext);

   return rv;
  }

/**************************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_eval: Native function for the   */
/*   CLIPSJNI eval method.                                                */
/*                                                                        */
/* Class:     net_sf_clipsrules_jni_Environment                           */
/* Method:    eval                                                        */
/* Signature: (JLjava/lang/String;)Lnet/sf/clipsrules/jni/PrimitiveValue; */
/*                                                                        */
/**************************************************************************/
JNIEXPORT jobject JNICALL Java_net_sf_clipsrules_jni_Environment_eval(
  JNIEnv *env,
  jobject obj,
  jlong clipsEnv,
  jstring evalStr)
  {
   DATA_OBJECT theDO;
   jobject result = NULL;
   const char *cEvalStr;
   void *theCLIPSEnv = JLongToPointer(clipsEnv);

   void *oldContext = SetEnvironmentContext(theCLIPSEnv,(void *) env);
   
   cEvalStr = (*env)->GetStringUTFChars(env,evalStr,NULL);
   
   EnvEval(theCLIPSEnv,(char *) cEvalStr,&theDO);

   (*env)->ReleaseStringUTFChars(env,evalStr,cEvalStr);
   
   result = ConvertDataObject(env,obj,theCLIPSEnv,&theDO);

   SetEnvironmentContext(theCLIPSEnv,oldContext);

   return result;  
  }

/**********************/
/* ConvertDataObject: */
/**********************/
static jobject ConvertDataObject(
  JNIEnv *env,
  jobject javaEnv,
  void *clipsEnv,
  DATA_OBJECT *theDO)
  {
   jobject result = NULL, tresult;
   jint mfLength;
   struct multifield *theList;
   long i;
   
   switch(GetpType(theDO))
     {
      case MULTIFIELD:
        mfLength = GetpDOLength(theDO);

        result = (*env)->NewObject(env,
                                   CLIPSJNIData(clipsEnv)->arrayListClass,
                                   CLIPSJNIData(clipsEnv)->arrayListInitMethod,
                                   (jint) mfLength);
                                   
        if (result == NULL)
          { return result; }
          
        theList = (struct multifield *) DOPToPointer(theDO);
        
        for (i = GetpDOBegin(theDO); i <= GetpDOEnd(theDO); i++)
         {
          tresult = ConvertSingleFieldValue(env,javaEnv,clipsEnv,GetMFType(theList,i),GetMFValue(theList,i));
          
          if (tresult != NULL)
             { (*env)->CallBooleanMethod(env,result,CLIPSJNIData(clipsEnv)->arrayListAddMethod,tresult); }

          (*env)->DeleteLocalRef(env,tresult);
         }
       
        tresult = result;
         
        result = (*env)->NewObject(env,
                                   CLIPSJNIData(clipsEnv)->multifieldValueClass,
                                   CLIPSJNIData(clipsEnv)->multifieldValueInitMethod,
                                   tresult);
        break;
        
      case RVOID:
      case SYMBOL:
      case STRING:
      case INSTANCE_NAME:
      case INTEGER:
      case FLOAT:
      case FACT_ADDRESS:
      case INSTANCE_ADDRESS:
        result = ConvertSingleFieldValue(env,javaEnv,clipsEnv,GetpType(theDO),GetpValue(theDO));
        break;

      default: 
        break;
     }

   return result;  
  }
  
/****************************/
/* ConvertSingleFieldValue: */
/****************************/
static jobject ConvertSingleFieldValue(
  JNIEnv *env,
  jobject javaEnv,
  void *clipsEnv,
  int type,
  void  *value)
  {
   jobject result = NULL, tresult;
   jstring sresult = NULL;
   
   switch(type)
     {
      case RVOID:
        result = (*env)->NewObject(env,
                                   CLIPSJNIData(clipsEnv)->voidValueClass,
                                   CLIPSJNIData(clipsEnv)->voidValueInitMethod,
                                   sresult);
        break;

      case SYMBOL:
        sresult = (*env)->NewStringUTF(env,ValueToString(value));
        result = (*env)->NewObject(env,
                                   CLIPSJNIData(clipsEnv)->symbolValueClass,
                                   CLIPSJNIData(clipsEnv)->symbolValueInitMethod,
                                   sresult);
        (*env)->DeleteLocalRef(env,sresult);
        break;
        
        
      case STRING:
        sresult = (*env)->NewStringUTF(env,ValueToString(value));
        result = (*env)->NewObject(env,
                                   CLIPSJNIData(clipsEnv)->stringValueClass,
                                   CLIPSJNIData(clipsEnv)->stringValueInitMethod,
                                   sresult);
        (*env)->DeleteLocalRef(env,sresult);
        break;
        
      case INSTANCE_NAME:
        sresult = (*env)->NewStringUTF(env,ValueToString(value));
        result = (*env)->NewObject(env,
                                   CLIPSJNIData(clipsEnv)->instanceNameValueClass,
                                   CLIPSJNIData(clipsEnv)->instanceNameValueInitMethod,
                                   sresult);
        (*env)->DeleteLocalRef(env,sresult);
        break;
        
      case INTEGER:
        tresult = (*env)->NewObject(env,
                                    CLIPSJNIData(clipsEnv)->longClass,
                                    CLIPSJNIData(clipsEnv)->longInitMethod,
                                    (jlong) ValueToLong(value));
        result = (*env)->NewObject(env,
                                   CLIPSJNIData(clipsEnv)->integerValueClass,
                                   CLIPSJNIData(clipsEnv)->integerValueInitMethod,
                                   tresult);
        (*env)->DeleteLocalRef(env,tresult);
        break;

      case FLOAT:
        tresult = (*env)->NewObject(env,
                                    CLIPSJNIData(clipsEnv)->doubleClass,
                                    CLIPSJNIData(clipsEnv)->doubleInitMethod,
                                    (jdouble) ValueToDouble(value));

        result = (*env)->NewObject(env,
                                   CLIPSJNIData(clipsEnv)->floatValueClass,
                                   CLIPSJNIData(clipsEnv)->floatValueInitMethod,
                                   tresult);
        (*env)->DeleteLocalRef(env,tresult);
        break;

      case FACT_ADDRESS:
        result = (*env)->NewObject(env,
                                   CLIPSJNIData(clipsEnv)->factAddressValueClass,
                                   CLIPSJNIData(clipsEnv)->factAddressValueInitMethod,
                                   PointerToJLong(value),javaEnv);
        break;

      case INSTANCE_ADDRESS:
        result = (*env)->NewObject(env,
                                   CLIPSJNIData(clipsEnv)->instanceAddressValueClass,
                                   CLIPSJNIData(clipsEnv)->instanceAddressValueInitMethod,
                                   PointerToJLong(value),javaEnv);
        break;

      default: 
        break;
     }

   return result;  
  }

/********************************************************/
/* Java_net_sf_clipsrules_jni_Environment_build: Native */
/*   function for the CLIPSJNI build method.            */
/*                                                      */
/* Class:     net_sf_clipsrules_jni_Environment         */
/* Method:    build                                     */
/* Signature: (JLjava/lang/String;)Z                    */
/********************************************************/
JNIEXPORT jboolean JNICALL Java_net_sf_clipsrules_jni_Environment_build(
  JNIEnv *env, 
  jobject obj, 
  jlong clipsEnv,
  jstring buildStr)
  {
   jboolean rv;
   void *theCLIPSEnv = JLongToPointer(clipsEnv);
   const char *cBuildStr = (*env)->GetStringUTFChars(env,buildStr,NULL);

   void *oldContext = SetEnvironmentContext(theCLIPSEnv,(void *) env);
   
   rv = (jboolean) EnvBuild(theCLIPSEnv,(char *) cBuildStr);

   (*env)->ReleaseStringUTFChars(env,buildStr,cBuildStr);

   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext);

   return rv;
  }

/****************************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_assertString: Native function for */
/*   the CLIPSJNI assertString method.                                      */
/*                                                                          */
/* Class:     net_sf_clipsrules_jni_Environment                             */
/* Method:    assertString                                                  */
/* Signature: (JLjava/lang/String;)Lnet/sf/clipsrules/jni/FactAddressValue; */
/****************************************************************************/
JNIEXPORT jobject JNICALL Java_net_sf_clipsrules_jni_Environment_assertString(
  JNIEnv *env,
  jobject obj, 
  jlong clipsEnv, 
  jstring factStr)
  {
   jobject rv;
   void *theFact;
   void *theCLIPSEnv = JLongToPointer(clipsEnv);
   const char *cFactStr = (*env)->GetStringUTFChars(env,factStr,NULL);
   
   void *oldContext = SetEnvironmentContext(theCLIPSEnv,(void *) env);

   theFact = EnvAssertString(theCLIPSEnv,(char *) cFactStr);

   (*env)->ReleaseStringUTFChars(env,factStr,cFactStr);
   
   if (theFact == NULL)
     { return(NULL); }
     
   rv = ConvertSingleFieldValue(env,obj,theCLIPSEnv,FACT_ADDRESS,theFact);

   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext);

   return rv;
  }

/************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_factIndex: Native */
/*   function for the CLIPSJNI factIndex method.            */
/*                                                          */
/* Class:     net_sf_clipsrules_jni_Environment             */
/* Method:    factIndex                                     */
/* Signature: (Lnet/sf/clipsrules/jni/Environment;JJ)J      */
/************************************************************/
JNIEXPORT jlong JNICALL Java_net_sf_clipsrules_jni_Environment_factIndex(
  JNIEnv *env, 
  jclass javaClass, 
  jobject javaEnv,
  jlong clipsEnv, 
  jlong clipsFact)
  {
   jlong rv;
   
   void *oldContext = SetEnvironmentContext(JLongToPointer(clipsEnv),(void *) env);

   rv = EnvFactIndex(JLongToPointer(clipsEnv),JLongToPointer(clipsFact));

   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext);

   return rv;
  }

/************************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_getFactSlot: Native function  */
/*   for the CLIPSJNI getFactSlot method.                               */
/*                                                                      */
/* Class:     net_sf_clipsrules_jni_Environment                         */
/* Method:    getFactSlot                                               */
/* Signature: (Lnet/sf/clipsrules/jni/Environment;JJLjava/lang/String;) */
/*            Lnet/sf/clipsrules/jni/PrimitiveValue;                    */
/************************************************************************/
JNIEXPORT jobject JNICALL Java_net_sf_clipsrules_jni_Environment_getFactSlot(
  JNIEnv *env,
  jclass javaClass, 
  jobject javaEnv,
  jlong clipsEnv,
  jlong clipsFact,
  jstring slotName)
  {
   jobject rv;
   DATA_OBJECT theDO;
   void *theCLIPSEnv = JLongToPointer(clipsEnv);
   const char *cSlotName = (*env)->GetStringUTFChars(env,slotName,NULL);

   void *oldContext = SetEnvironmentContext(theCLIPSEnv,(void *) env);
   
   EnvGetFactSlot(JLongToPointer(clipsEnv),JLongToPointer(clipsFact),(char *) cSlotName,&theDO);

   (*env)->ReleaseStringUTFChars(env,slotName,cSlotName);
   
   rv = ConvertDataObject(env,javaEnv,theCLIPSEnv,&theDO);
   
   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext);
   
   return rv;
  }

/*********************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_incrementFactCount: Native */
/*   function for the CLIPSJNI incrementFactCount method.            */
/*                                                                   */
/*                                                                   */
/* Class:     net_sf_clipsrules_jni_Environment                      */
/* Method:    incrementFactCount                                     */
/* Signature: (Lnet/sf/clipsrules/jni/Environment;JJ)V               */
/*********************************************************************/
JNIEXPORT void JNICALL Java_net_sf_clipsrules_jni_Environment_incrementFactCount(
  JNIEnv *env, 
  jclass javaClass, 
  jobject javaEnv,
  jlong clipsEnv, 
  jlong clipsFact)
  {
   void *oldContext = SetEnvironmentContext(JLongToPointer(clipsEnv),(void *) env);

   EnvIncrementFactCount(JLongToPointer(clipsEnv),JLongToPointer(clipsFact));
   
   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext);
  }
  
/*********************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_decrementFactCount: Native */
/*   function for the CLIPSJNI decrementFactCount method.            */
/*                                                                   */
/*                                                                   */
/* Class:     net_sf_clipsrules_jni_Environment                      */
/* Method:    decrementFactCount                                     */
/* Signature: (Lnet/sf/clipsrules/jni/Environment;JJ)V               */
/*********************************************************************/
JNIEXPORT void JNICALL Java_net_sf_clipsrules_jni_Environment_decrementFactCount(
  JNIEnv *env, 
  jclass javaClass, 
  jobject javaEnv,
  jlong clipsEnv, 
  jlong clipsFact)
  {
   void *oldContext = SetEnvironmentContext(JLongToPointer(clipsEnv),(void *) env);

   EnvDecrementFactCount(JLongToPointer(clipsEnv),JLongToPointer(clipsFact));
   
   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext);
  }
  
/********************************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_makeInstance: Native function for the */
/*   CLIPSJNI makeInstance method.                                              */
/*                                                                              */
/* Class:     net_sf_clipsrules_jni_Environment                                 */
/* Method:    makeInstance                                                      */
/* Signature: (JLjava/lang/String;)Lnet/sf/clipsrules/jni/InstanceAddressValue; */
/********************************************************************************/
JNIEXPORT jobject JNICALL Java_net_sf_clipsrules_jni_Environment_makeInstance(
  JNIEnv *env,
  jobject obj, 
  jlong clipsEnv, 
  jstring instanceStr)
  {
   jobject rv;
   void *theInstance;
   void *theCLIPSEnv = JLongToPointer(clipsEnv);
   const char *cInstanceStr = (*env)->GetStringUTFChars(env,instanceStr,NULL);
   
   void *oldContext = SetEnvironmentContext(theCLIPSEnv,(void *) env);

   theInstance = EnvMakeInstance(theCLIPSEnv,(char *) cInstanceStr);

   (*env)->ReleaseStringUTFChars(env,instanceStr,cInstanceStr);
   
   if (theInstance == NULL)
     {
      SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext);
      return(NULL); 
     }
     
   rv = ConvertSingleFieldValue(env,obj,theCLIPSEnv,INSTANCE_ADDRESS,theInstance);
      
   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext);

   return rv;
  }

/************************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_getInstanceName: Native       */
/*   function for the CLIPSJNI getInstanceName method.                  */
/*                                                                      */
/* Class:     net_sf_clipsrules_jni_Environment                         */
/* Method:    getInstanceName                                           */
/* Signature: (Lnet/sf/clipsrules/jni/Environment;JJ)Ljava/lang/String; */
/************************************************************************/
JNIEXPORT jstring JNICALL Java_net_sf_clipsrules_jni_Environment_getInstanceName(
  JNIEnv *env, 
  jclass javaClass, 
  jobject javaEnv,
  jlong clipsEnv, 
  jlong clipsInstance)
  {
   jstring rv;
   void *oldContext = SetEnvironmentContext(JLongToPointer(clipsEnv),(void *) env);

   rv = (*env)->NewStringUTF(env,EnvGetInstanceName(JLongToPointer(clipsEnv),JLongToPointer(clipsInstance)));

   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext);

   return rv;
  }

/*************************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_incrementInstanceCount: Native */
/*   function for the CLIPSJNI incrementInstanceCount method.            */
/*                                                                       */
/*                                                                       */
/* Class:     net_sf_clipsrules_jni_Environment                          */
/* Method:    incrementInstanceCount                                     */
/* Signature: (Lnet/sf/clipsrules/jni/Environment;JJ)V                   */
/*************************************************************************/
JNIEXPORT void JNICALL Java_net_sf_clipsrules_jni_Environment_incrementInstanceCount(
  JNIEnv *env, 
  jclass javaClass, 
  jobject javaEnv,
  jlong clipsEnv, 
  jlong clipsInstance)
  {
   void *oldContext = SetEnvironmentContext(JLongToPointer(clipsEnv),(void *) env);

   EnvIncrementInstanceCount(JLongToPointer(clipsEnv),JLongToPointer(clipsInstance));
   
   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext);
  }
  
/*************************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_decrementInstanceCount: Native */
/*   function for the CLIPSJNI decrementInstanceCount method.            */
/*                                                                       */
/*                                                                       */
/* Class:     net_sf_clipsrules_jni_Environment                          */
/* Method:    decrementInstanceCount                                     */
/* Signature: (Lnet/sf/clipsrules/jni/Environment;JJ)V                   */
/*************************************************************************/
JNIEXPORT void JNICALL Java_net_sf_clipsrules_jni_Environment_decrementInstanceCount(
  JNIEnv *env, 
  jclass javaClass, 
  jobject javaEnv,
  jlong clipsEnv, 
  jlong clipsInstance)
  {
   void *oldContext = SetEnvironmentContext(JLongToPointer(clipsEnv),(void *) env);

   EnvDecrementInstanceCount(JLongToPointer(clipsEnv),JLongToPointer(clipsInstance));
   
   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext);
  }

/*************************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_directGetSlot: Native function */
/*   for the CLIPSJNI directGetSlot method.                              */
/*                                                                       */
/* Class:     net_sf_clipsrules_jni_Environment                          */
/* Method:    directGetSlot                                              */
/* Signature: (Lnet/sf/clipsrules/jni/Environment;JJLjava/lang/String;)  */
/*            Lnet/sf/clipsrules/jni/PrimitiveValue;                     */
/*************************************************************************/
JNIEXPORT jobject JNICALL Java_net_sf_clipsrules_jni_Environment_directGetSlot(
  JNIEnv *env,
  jclass javaClass, 
  jobject javaEnv,
  jlong clipsEnv,
  jlong clipsInstance,
  jstring slotName)
  {
   jobject rv;
   DATA_OBJECT theDO;
   void *theCLIPSEnv = JLongToPointer(clipsEnv);
   const char *cSlotName = (*env)->GetStringUTFChars(env,slotName,NULL);

   void *oldContext = SetEnvironmentContext(theCLIPSEnv,(void *) env);
   
   EnvDirectGetSlot(JLongToPointer(clipsEnv),JLongToPointer(clipsInstance),(char *) cSlotName,&theDO);

   (*env)->ReleaseStringUTFChars(env,slotName,cSlotName);
   
   rv = ConvertDataObject(env,javaEnv,theCLIPSEnv,&theDO);
   
   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext);
   
   return rv;
  }

/*********************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_destroyEnvironment: Native */
/*   function for the CLIPSJNI destroyEnvironment method.            */
/*                                                                   */
/* Class:     net_sf_clipsrules_jni_Environment                      */
/* Method:    destroyEnvironment                                     */
/* Signature: (J)V                                                   */
/*********************************************************************/
JNIEXPORT void JNICALL Java_net_sf_clipsrules_jni_Environment_destroyEnvironment(
  JNIEnv *env,
  jobject obj,
  jlong clipsEnv)
  {
   SetEnvironmentContext(JLongToPointer(clipsEnv),(void *) env);

   DestroyEnvironment(JLongToPointer(clipsEnv));
  }

/**************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_commandLoop: Native */
/*   function for the CLIPSJNI commandLoop method.            */
/*                                                            */
/* Class:     net_sf_clipsrules_jni_Environment               */
/* Method:    commandLoop                                     */
/* Signature: (J)V                                            */
/**************************************************************/
JNIEXPORT void JNICALL Java_net_sf_clipsrules_jni_Environment_commandLoop(
  JNIEnv *env,
  jobject obj,
  jlong clipsEnv)
  {
   SetEnvironmentContext(JLongToPointer(clipsEnv),(void *) env);

   CommandLoop(JLongToPointer(clipsEnv));
  }

/***************************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_commandLoopOnceThenBatch: Native */
/*   function for the CLIPSJNI commandLoopOnceThenBatch method.            */
/*                                                                         */
/* Class:     net_sf_clipsrules_jni_Environment                            */
/* Method:    commandLoopOnceThenBatch                                     */
/* Signature: (J)V                                                         */
/***************************************************************************/
JNIEXPORT void JNICALL Java_net_sf_clipsrules_jni_Environment_commandLoopOnceThenBatch(
  JNIEnv *env,
  jobject obj,
  jlong clipsEnv)
  {
   SetEnvironmentContext(JLongToPointer(clipsEnv),(void *) env);

   CommandLoopOnceThenBatch(JLongToPointer(clipsEnv));
  }

/**************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_printBanner: Native */
/*   function for the CLIPSJNI printBanner method.            */
/*                                                            */
/* Class:     net_sf_clipsrules_jni_Environment               */
/* Method:    printBanner                                     */
/* Signature: (J)V                                            */
/**************************************************************/
JNIEXPORT void JNICALL Java_net_sf_clipsrules_jni_Environment_printBanner(
  JNIEnv *env,
  jobject obj,
  jlong clipsEnv)
  {
   void *oldContext = SetEnvironmentContext(JLongToPointer(clipsEnv),(void *) env);

   PrintBanner(JLongToPointer(clipsEnv));
   
   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext);
  }

/**************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_printPrompt: Native */
/*   function for the CLIPSJNI printPrompt method.            */
/*                                                            */
/* Class:     net_sf_clipsrules_jni_Environment               */
/* Method:    printPrompt                                     */
/* Signature: (J)V                                            */
/**************************************************************/
JNIEXPORT void JNICALL Java_net_sf_clipsrules_jni_Environment_printPrompt(
  JNIEnv *env,
  jobject obj,
  jlong clipsEnv)
  {
   void *oldContext = SetEnvironmentContext(JLongToPointer(clipsEnv),(void *) env);

   PrintPrompt(JLongToPointer(clipsEnv));
   
   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext);
  }

/*****************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_getInputBuffer: Native */
/*   function for the CLIPSJNI getInputBuffer method.            */
/*                                                               */
/* Class:     net_sf_clipsrules_jni_Environment                  */
/* Method:    getInputBuffer                                     */
/* Signature: (J)Ljava/lang/String;                              */
/*****************************************************************/
JNIEXPORT jstring JNICALL Java_net_sf_clipsrules_jni_Environment_getInputBuffer(
  JNIEnv *env, 
  jobject obj, 
  jlong clipsEnv)
  {
   jstring rv;
   
   char *command;
   
   void *oldContext = SetEnvironmentContext(JLongToPointer(clipsEnv),(void *) env);

   command = GetCommandString(JLongToPointer(clipsEnv));
   
   if (command == NULL)
     { 
      rv = (*env)->NewStringUTF(env,""); 
      SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext);
      return rv;
     }
     
   rv = (*env)->NewStringUTF(env,GetCommandString(JLongToPointer(clipsEnv)));
   
   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext);
   
   return rv;
  }

/*****************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_setInputBuffer: Native */
/*   function for the CLIPSJNI setInputBuffer method.            */
/*                                                               */
/* Class:     net_sf_clipsrules_jni_Environment                  */
/* Method:    setInputBuffer                                     */
/* Signature: (JLjava/lang/String;)V                             */
/*****************************************************************/
JNIEXPORT void JNICALL Java_net_sf_clipsrules_jni_Environment_setInputBuffer(
  JNIEnv *env, 
  jobject obj,
  jlong clipsEnv,
  jstring command)
  {
   const char *cCommand = (*env)->GetStringUTFChars(env,command,NULL);

   void *oldContext = SetEnvironmentContext(JLongToPointer(clipsEnv),(void *) env);

   SetCommandString(JLongToPointer(clipsEnv),(char *) cCommand);
   
   (*env)->ReleaseStringUTFChars(env,command,cCommand);
   
   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext);
  }

/**********************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_getInputBufferCount: Native */
/*   function for the CLIPSJNI getInputBufferCount method.            */
/*                                                                    */
/* Class:     net_sf_clipsrules_jni_Environment                       */
/* Method:    getInputBufferCount                                     */
/* Signature: (J)J                                                    */
/**********************************************************************/
JNIEXPORT jlong JNICALL Java_net_sf_clipsrules_jni_Environment_getInputBufferCount(
  JNIEnv *env,
  jobject obj,
  jlong clipsEnv)
  {
   jlong rv;
   void *oldContext = SetEnvironmentContext(JLongToPointer(clipsEnv),(void *) env);
   
   rv = RouterData(JLongToPointer(clipsEnv))->CommandBufferInputCount;

   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext);

   return rv;
  }

/**********************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_setInputBufferCount: Native */
/*   function for the CLIPSJNI setInputBufferCount method.            */
/*                                                                    */
/* Class:     net_sf_clipsrules_jni_Environment                       */
/* Method:    setInputBufferCount                                     */
/* Signature: (JJ)J                                                   */
/**********************************************************************/
JNIEXPORT jlong JNICALL Java_net_sf_clipsrules_jni_Environment_setInputBufferCount(
  JNIEnv *env,
  jobject obj,
  jlong clipsEnv,
  jlong theValue)
  {
   jlong rv;
   
   void *oldContext = SetEnvironmentContext(JLongToPointer(clipsEnv),(void *) env);

   rv = RouterData(JLongToPointer(clipsEnv))->CommandBufferInputCount;
   
   RouterData(JLongToPointer(clipsEnv))->CommandBufferInputCount = theValue;

   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext);

   return rv;
  }

/********************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_appendInputBuffer: Native */
/*   function for the CLIPSJNI appendInputBuffer method.            */
/*                                                                  */
/* Class:     net_sf_clipsrules_jni_Environment                     */
/* Method:    appendInputBuffer                                     */
/* Signature: (JLjava/lang/String;)V                                */
/********************************************************************/
JNIEXPORT void JNICALL Java_net_sf_clipsrules_jni_Environment_appendInputBuffer(
  JNIEnv *env,
  jobject obj,
  jlong clipsEnv,
  jstring commandString)
  {
   const char *cCommandString = (*env)->GetStringUTFChars(env,commandString,NULL);
   
   void *oldContext = SetEnvironmentContext(JLongToPointer(clipsEnv),(void *) env);
   
   AppendCommandString(JLongToPointer(clipsEnv),(char *) cCommandString);

   (*env)->ReleaseStringUTFChars(env,commandString,cCommandString);
   
   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext);
  }

/********************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_expandInputBuffer: Native */
/*   function for the CLIPSJNI expandInputBuffer method.            */
/*                                                                  */
/* Class:     net_sf_clipsrules_jni_Environment                     */
/* Method:    expandInputBuffer                                     */
/* Signature: (JC)V                                                 */
/********************************************************************/
JNIEXPORT void JNICALL Java_net_sf_clipsrules_jni_Environment_expandInputBuffer(
  JNIEnv *env,
  jobject obj,
  jlong clipsEnv,
  jint theChar)
  {
   void *oldContext = SetEnvironmentContext(JLongToPointer(clipsEnv),(void *) env);
   
   ExpandCommandString(JLongToPointer(clipsEnv),(int) theChar);
   
   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext);
  }

/*****************************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_inputBufferContainsCommand: Native */
/*   function for the CLIPSJNI inputBufferContainsCommand method.            */
/*                                                                           */
/* Class:     net_sf_clipsrules_jni_Environment                              */
/* Method:    inputBufferContainsCommand                                     */
/* Signature: (J)Z                                                           */
/*****************************************************************************/
JNIEXPORT jboolean JNICALL Java_net_sf_clipsrules_jni_Environment_inputBufferContainsCommand(
  JNIEnv *env,
  jobject obj, 
  jlong clipsEnv)
  {
   jboolean rv;
   void *oldContext = SetEnvironmentContext(JLongToPointer(clipsEnv),(void *) env);
  
   rv = CommandCompleteAndNotEmpty(JLongToPointer(clipsEnv));
   
   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext);
   
   return rv;
  }

/*****************************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_addPeriodicCallbackEnabled: Native */
/*   function for the CLIPSJNI setPeriodicCallbackEnabled method.            */
/*                                                                           */
/* Class:     net_sf_clipsrules_jni_Environment                              */
/* Method:    setPeriodicCallbackEnabled                                     */
/* Signature: (JZ)V                                                          */
/*****************************************************************************/
JNIEXPORT void JNICALL Java_net_sf_clipsrules_jni_Environment_setPeriodicCallbackEnabled(
  JNIEnv *env, 
  jobject obj, 
  jlong clipsEnv,
  jboolean value)
  {
   EnablePeriodicFunctions(JLongToPointer(clipsEnv),value);
  }

/******************************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_addPeriodicCallback: Native         */
/*   function for the CLIPSJNI addPeriodicCallback method.                    */
/*                                                                            */
/* Class:     net_sf_clipsrules_jni_Environment                               */
/* Method:    addPeriodicCallback                                             */
/* Signature: (JLjava/lang/String;ILnet/sf/clipsrules/jni/PeriodicCallback;)V */
/******************************************************************************/
JNIEXPORT void JNICALL Java_net_sf_clipsrules_jni_Environment_addPeriodicCallback(
  JNIEnv *env, 
  jobject obj, 
  jlong clipsEnv, 
  jstring listenerName, 
  jint priority, 
  jobject context)
  {
   jobject nobj;   
   const char *cListenerName = (*env)->GetStringUTFChars(env,listenerName,NULL);

   void *oldContext = SetEnvironmentContext(JLongToPointer(clipsEnv),(void *) env); 
      
   nobj = (*env)->NewGlobalRef(env,context);
   
   EnvAddPeriodicFunctionWithContext(JLongToPointer(clipsEnv),(char *) cListenerName,
                                     JNIPeriodicCallback,
                                     (int) priority,(void *) nobj);
   
   (*env)->ReleaseStringUTFChars(env,listenerName,cListenerName);
   
   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext); 
  }
  
/*************************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_removePeriodicCallback: Native */
/*   function for the CLIPSJNI removePeriodicCallback method.            */
/*                                                                       */
/* Class:     net_sf_clipsrules_jni_Environment                          */
/* Method:    removePeriodicCallback                                     */
/* Signature: (JLjava/lang/String;)Z                                     */
/*************************************************************************/
JNIEXPORT jboolean JNICALL Java_net_sf_clipsrules_jni_Environment_removePeriodicCallback(
  JNIEnv *env, 
  jobject obj, 
  jlong clipsEnv, 
  jstring listenerName) 
  {
   int rv;
   void *periodicContext;
   const char *cListenerName = (*env)->GetStringUTFChars(env,listenerName,NULL);

   void *oldContext = SetEnvironmentContext(JLongToPointer(clipsEnv),(void *) env); 

   periodicContext = EnvGetPeriodicFunctionContext(JLongToPointer(clipsEnv),cListenerName);
   
   rv = EnvRemovePeriodicFunction(JLongToPointer(clipsEnv),cListenerName);
   
   if (periodicContext != NULL)
     { (*env)->DeleteGlobalRef(env,periodicContext); }
       
   (*env)->ReleaseStringUTFChars(env,listenerName,cListenerName);
   
   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext); 

   return rv;
  }
  
/******************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_addUserFunction: Native */
/*   function for the CLIPSJNI addUserFunction method.            */
/*                                                                */
/* Class:     net_sf_clipsrules_jni_Environment                   */
/* Method:    addUserFunction                                     */
/* Signature: (J                                                  */
/*             Ljava/lang/String;Ljava/lang/String;               */
/*             Lnet/sf/clipsrules/jni/UserFunction;)Z             */
/******************************************************************/
JNIEXPORT jboolean JNICALL Java_net_sf_clipsrules_jni_Environment_addUserFunction(
  JNIEnv *env, 
  jobject obj, 
  jlong clipsEnv, 
  jstring functionName, 
  jstring restrictions, 
  jobject context)
  {
   int rv;
   jobject nobj;   

   const char *cFunctionName = (*env)->GetStringUTFChars(env,functionName,NULL);
   const char *cRestrictions;
   
   if (restrictions != NULL)
     { cRestrictions = (*env)->GetStringUTFChars(env,restrictions,NULL); }
   else 
     { cRestrictions = NULL; }

   void *oldContext = SetEnvironmentContext(JLongToPointer(clipsEnv),(void *) env); 

   nobj = (*env)->NewGlobalRef(env,context);
 
   rv = EnvDefineFunction2WithContext(JLongToPointer(clipsEnv),
                                     cFunctionName,'u',PTIEF JNIUserFunction,
                                     "JNIUserFunction",cRestrictions,nobj);

   (*env)->ReleaseStringUTFChars(env,functionName,cFunctionName);
   if (restrictions != NULL)
     { (*env)->ReleaseStringUTFChars(env,restrictions,cRestrictions); }
                                     
   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext); 

   return rv;
  }

/**********************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_renmoveUserFunction: Native */
/*   function for the CLIPSJNI removeUserFunction method.             */
/*                                                                    */
/* Class:     net_sf_clipsrules_jni_Environment                       */
/* Method:    removeUserFunction                                      */
/* Signature: (JLjava/lang/String;)Z                                  */
/**********************************************************************/
JNIEXPORT jboolean JNICALL Java_net_sf_clipsrules_jni_Environment_removeUserFunction(
  JNIEnv *env, 
  jobject obj, 
  jlong clipsEnv, 
  jstring functionName)
  {
   int rv;
   jobject context;
   void *theEnv;
   
   theEnv = JLongToPointer(clipsEnv);

   const char *cFunctionName = (*env)->GetStringUTFChars(env,functionName,NULL);

   void *oldContext = SetEnvironmentContext(JLongToPointer(clipsEnv),(void *) env); 

   context = GetEnvironmentFunctionContext(theEnv);
   if (context != NULL)
     {  (*env)->DeleteGlobalRef(env,context); }
     
   rv = UndefineFunction(JLongToPointer(clipsEnv),cFunctionName);

   (*env)->ReleaseStringUTFChars(env,functionName,cFunctionName);
                                     
   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext); 

   return rv;
  }

/********************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_addRouter: Native         */
/*   function for the CLIPSJNI addRouter method.                    */
/*                                                                  */
/* Class:     net_sf_clipsrules_jni_Environment                     */
/* Method:    addRouter                                             */
/* Signature: (JLjava/lang/String;ILnet/sf/clipsrules/jni/Router;)Z */
/********************************************************************/
JNIEXPORT jboolean JNICALL Java_net_sf_clipsrules_jni_Environment_addRouter(
  JNIEnv *env, 
  jobject obj, 
  jlong clipsEnv, 
  jstring routerName, 
  jint priority, 
  jobject context)
  {
   int rv;
   jobject nobj;   
   const char *cRouterName = (*env)->GetStringUTFChars(env,routerName,NULL);

   void *oldContext = SetEnvironmentContext(JLongToPointer(clipsEnv),(void *) env); 
      
   nobj = (*env)->NewGlobalRef(env,context);
   
   rv = EnvAddRouterWithContext(JLongToPointer(clipsEnv),(char *) cRouterName,(int) priority,
                                QueryJNIRouter,PrintJNIRouter,GetcJNIRouter,
                                UngetcJNIRouter,ExitJNIRouter,(void *) nobj);
   
   (*env)->ReleaseStringUTFChars(env,routerName,cRouterName);

   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext); 

   return rv;
  }

/***************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_deleteRouter: Native */
/*   function for the CLIPSJNI deleteRouter method.            */
/*                                                             */
/* Class:     net_sf_clipsrules_jni_Environment                */
/* Method:    deleteRouter                                     */
/* Signature: (JLjava/lang/String;)Z                           */
/***************************************************************/
JNIEXPORT jboolean JNICALL Java_net_sf_clipsrules_jni_Environment_deleteRouter(
  JNIEnv *env, 
  jobject obj, 
  jlong clipsEnv, 
  jstring routerName)
  {
   int rv;
   void *theCLIPSEnv = JLongToPointer(clipsEnv);
   const char *cRouterName = (*env)->GetStringUTFChars(env,routerName,NULL);
   struct router *theRouter;
   
   theRouter = EnvFindRouter(theCLIPSEnv,cRouterName);
   if (theRouter == NULL) 
     { return(0); }

   if (GetEnvironmentRouterContext(theCLIPSEnv) == theRouter->context)
     { SetEnvironmentRouterContext(theCLIPSEnv,NULL); }

   (*env)->DeleteGlobalRef(env,theRouter->context);
     
   rv = EnvDeleteRouter(theCLIPSEnv,cRouterName);
  
   (*env)->ReleaseStringUTFChars(env,routerName,cRouterName);

   return rv;
  }

/*******************************************************/
/* Java_net_sf_clipsrules_jni_Environment_printRouter  */
/* Class:     net_sf_clipsrules_jni_Environment        */
/* Method:    printRouter                              */
/* Signature: (JLjava/lang/String;Ljava/lang/String;)V */
/*******************************************************/
JNIEXPORT void JNICALL Java_net_sf_clipsrules_jni_Environment_printRouter(
  JNIEnv *env, 
  jobject obj, 
  jlong clipsEnv, 
  jstring logName, 
  jstring printString)
  {
   const char *cLogName;
   const char *cPrintString;
   void *theCLIPSEnv = JLongToPointer(clipsEnv);
   void *oldContext = SetEnvironmentContext(theCLIPSEnv,(void *) env);
   
   cLogName = (*env)->GetStringUTFChars(env,logName,NULL);
   cPrintString = (*env)->GetStringUTFChars(env,printString,NULL);

   EnvPrintRouter(theCLIPSEnv,cLogName,cPrintString);

   (*env)->ReleaseStringUTFChars(env,logName,cLogName);
   (*env)->ReleaseStringUTFChars(env,printString,cPrintString);
   
   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext);
  }

/*********************************************************/
/* Java_net_sf_clipsrules_jni_Environment_activateRouter */
/* Class:     net_sf_clipsrules_jni_Environment          */
/* Method:    activateRouter                             */
/* Signature: (JLjava/lang/String;)Z                     */
/*********************************************************/
JNIEXPORT jboolean JNICALL Java_net_sf_clipsrules_jni_Environment_activateRouter(
  JNIEnv *env, 
  jobject obj, 
  jlong clipsEnv, 
  jstring routerName)
  {
   jboolean rv;
   const char *cRouterName;
   void *theCLIPSEnv = JLongToPointer(clipsEnv);
   void *oldContext = SetEnvironmentContext(theCLIPSEnv,(void *) env);
   
   cRouterName = (*env)->GetStringUTFChars(env,routerName,NULL);

   rv = EnvActivateRouter(theCLIPSEnv,cRouterName);

   (*env)->ReleaseStringUTFChars(env,routerName,cRouterName);
   
   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext);
   
   return rv;
  }

/***********************************************************/
/* Java_net_sf_clipsrules_jni_Environment_deactivateRouter */
/* Class:     net_sf_clipsrules_jni_Environment            */
/* Method:    deactivateRouter                             */
/* Signature: (JLjava/lang/String;)Z                       */
/***********************************************************/
JNIEXPORT jboolean JNICALL Java_net_sf_clipsrules_jni_Environment_deactivateRouter(
  JNIEnv *env, 
  jobject obj, 
  jlong clipsEnv, 
  jstring routerName)
  {
   jboolean rv;
   const char *cRouterName;
   void *theCLIPSEnv = JLongToPointer(clipsEnv);
   void *oldContext = SetEnvironmentContext(theCLIPSEnv,(void *) env);
   
   cRouterName = (*env)->GetStringUTFChars(env,routerName,NULL);

   rv = EnvDeactivateRouter(theCLIPSEnv,cRouterName);

   (*env)->ReleaseStringUTFChars(env,routerName,cRouterName);
   
   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext);
   
   return rv;
  }

/******************/
/* JLongToPointer */
/******************/
static void *JLongToPointer(
  jlong value)
  {
   return (void *) value;
  }


/******************/
/* PointerToJLong */
/******************/
static jlong PointerToJLong(
  void *value)
  {
   return (jlong) value;
  }

/*********************/
/* PrintJavaAddress: */
/*********************/
static void PrintJavaAddress(
  void *theEnv,
  const char *logicalName,
  void *theValue)
  {
   jobject theObject;
   jclass cls;
   jstring str;
   char *cStr;
   JNIEnv *env;
   char buffer[20];

   EnvPrintRouter(theEnv,logicalName,"<Pointer-");
        
   theObject = (jobject) ValueToExternalAddress(theValue);
   
   if (theObject != NULL)
     {
      env = (JNIEnv *) GetEnvironmentContext(theEnv);
      
      cls = (*env)->GetObjectClass(env,theObject);
      
      str = (jstring) (*env)->CallObjectMethod(env,cls,CLIPSJNIData(theEnv)->classGetCanonicalNameMethod);

      cStr = (char *) (*env)->GetStringUTFChars(env,str,NULL);
      
      EnvPrintRouter(theEnv,logicalName,cStr);
      EnvPrintRouter(theEnv,logicalName,"-");
   
      (*env)->ReleaseStringUTFChars(env,str,cStr);
     }
   else
     { EnvPrintRouter(theEnv,logicalName,"java-"); }
   
   gensprintf(buffer,"%p",(void *) theObject);
   EnvPrintRouter(theEnv,logicalName,buffer);
   EnvPrintRouter(theEnv,logicalName,">");
  }

/********************/
/* NewJavaAddress:  */
/********************/
static void NewJavaAddress(
  void *theEnv,
  DATA_OBJECT *rv)
  {
   jclass theClass, tempClass;
   int i, numberOfArguments;
   JNIEnv *env;
   const char *className;
   char *classDescriptor;
   DATA_OBJECT theValue;
   size_t length;
   jmethodID mid;
   jobjectArray constructorList, parameterList;
   jsize theSize, c; 
   jsize paramCount, p; 
   jobject theConstructor, theObject, oldObject; 
   int found = FALSE, matches;
   DATA_OBJECT_PTR newArgs;
   jvalue *javaArgs;
   
   /*=============================================*/
   /* Retrieve the JNI environment pointer stored */
   /* in the CLIPS environment structure.         */
   /*=============================================*/
   
   env = (JNIEnv *) GetEnvironmentContext(theEnv);

   /*======================================================================*/
   /* If the Java external address type is used, additional arguments must */
   /* at least include the Java class name of the object to be created.    */
   /*======================================================================*/
   
   if ((numberOfArguments = EnvArgCountCheck(theEnv,"new (with type Java)",AT_LEAST,2)) == -1) 
     { return; }
   
   /*=======================================*/
   /* The Java class name must be a symbol. */
   /*=======================================*/
   
   if (EnvArgTypeCheck(theEnv,"new (with type Java)",2,SYMBOL,&theValue) == FALSE) 
     { return; }
   
   className = DOToString(theValue);
   
   /*=============================================*/
   /* Construct the class descriptor by replacing */
   /* any periods (.) in the class name with a    */
   /* forward slash (/).                          */
   /*=============================================*/
   
   length = strlen(className);
   classDescriptor = genalloc(theEnv,length + 1);
   for (i = 0; i < length; i++)
     {
      if (className[i] != '.')
        { classDescriptor[i] = className[i]; }
      else 
        { classDescriptor[i] = '/'; }
     }
   classDescriptor[i] = 0;
   
   /*=======================*/
   /* Search for the class. */
   /*=======================*/
   
   theClass = (*env)->FindClass(env,classDescriptor); 
   
   /*========================================*/
   /* Free the constructed class descriptor. */
   /*========================================*/
   
   genfree(theEnv,classDescriptor,length + 1);

   /*============================================*/
   /* Signal an error if the class wasn't found. */
   /*============================================*/
   
   if (theClass == NULL)
     {
      if ((*env)->ExceptionOccurred(env))
        { (*env)->ExceptionClear(env); }
      EnvSetEvaluationError(theEnv,TRUE);
      ExpectedTypeError1(theEnv,"new (with type Java)",2,"Java class name");
      return;
     }
      
   /*===========================================================================*/
   /* Evaluate the CLIPS arguments that will be passed to the java constructor. */
   /*===========================================================================*/
   
   if (numberOfArguments - 2 == 0)
     { newArgs = NULL; }
   else
     {
      newArgs = (DATA_OBJECT_PTR) genalloc(theEnv,sizeof(DATA_OBJECT) * (numberOfArguments - 2));
      for (i = 0; i < numberOfArguments - 2; i++)
        {
         EnvRtnUnknown(theEnv,i+3,&newArgs[i]);
         if (EnvGetEvaluationError(theEnv))
           {   
            (*env)->DeleteLocalRef(env,theClass);
            return;
           }
        }
     }

   /*========================================================================*/
   /* Construct an array in which to store the corresponding java arguments. */
   /*========================================================================*/
   
   if (numberOfArguments - 2 == 0)
     { javaArgs = NULL; }
   else
     { javaArgs = (jvalue *) genalloc(theEnv,sizeof(jvalue) * (numberOfArguments - 2)); }
   
   /*=============================================*/
   /* Get the method index of the getConstructors */
   /* method from the java.lang.Class class.      */
   /*=============================================*/

   tempClass = (*env)->FindClass(env,"java/lang/Class");
   mid = (*env)->GetMethodID(env,tempClass,"getConstructors","()[Ljava/lang/reflect/Constructor;"); 
   (*env)->DeleteLocalRef(env,tempClass);
   
   /*=======================================================*/
   /* Get the list of constructors for the specified class. */
   /*=======================================================*/
     
   constructorList = (jobjectArray) (*env)->CallObjectMethod(env,theClass,mid);

   /*======================================================*/
   /* Get the method index of the getParameterTypes method */
   /* from the java.lang.reflect.Constructor class.        */
   /*======================================================*/

   tempClass = (*env)->FindClass(env,"java/lang/reflect/Constructor"); 
   mid = (*env)->GetMethodID(env,tempClass,"getParameterTypes","()[Ljava/lang/Class;"); 
   (*env)->DeleteLocalRef(env,tempClass);

   /*===============================================*/
   /* Search the constructor list for a constructor */
   /* with matching arguments.                      */
   /*===============================================*/
      
   theSize = (*env)->GetArrayLength(env,constructorList); 
   for (c = 0; c < theSize; c++) 
     { 
      theConstructor = (*env)->GetObjectArrayElement(env,constructorList,c); 
      
      parameterList = (jobjectArray) (*env)->CallObjectMethod(env,theConstructor,mid);
      
      paramCount = (*env)->GetArrayLength(env,parameterList); 
      
      if (paramCount != (numberOfArguments - 2))
        { continue; }
        
      matches = TRUE;
      
      for (p = 0; (p < paramCount) && matches; p++)
        {
         jstring str;
         char *cStr;
   
         tempClass = (jclass) (*env)->GetObjectArrayElement(env,parameterList,p);
         
         str = (jstring) (*env)->CallObjectMethod(env,tempClass,CLIPSJNIData(theEnv)->classGetCanonicalNameMethod);

         cStr = (char *) (*env)->GetStringUTFChars(env,str,NULL);
                  
         if (GetType(newArgs[p]) == INTEGER)
           {
            if (strcmp(cStr,"long") == 0)
              { 
               printf("p[%d] = %s\n",(int) p,cStr);
               javaArgs[p].j = DOToLong(newArgs[p]);
              }
            else if (strcmp(cStr,"int") == 0)  
              { 
               printf("p[%d] = %s\n",(int) p,cStr);
               javaArgs[p].i = DOToLong(newArgs[p]);
              }
            else
              { matches = FALSE; }
           }
         else
           { matches = FALSE; }
         
         (*env)->ReleaseStringUTFChars(env,str,cStr);
      
         (*env)->DeleteLocalRef(env,tempClass);
        }
      
      if (matches)
        { 
         found = TRUE;
         break; 
        }
     } 

   /*==========================================*/
   /* If an appropriate constructor was found, */
   /* invoke it to create a new java object.   */
   /*==========================================*/
   
   theObject = NULL;
   if (found)
     {
      if (paramCount == 0)
        {
         mid = (*env)->FromReflectedMethod(env,theConstructor);
         theObject = (*env)->NewObject(env,theClass,mid);
        }
      else
        {
         mid = (*env)->FromReflectedMethod(env,theConstructor);
         theObject = (*env)->NewObjectA(env,theClass,mid,javaArgs);
        }
     }
 
   /*========================================================*/
   /* Delete the local reference to the class of the object. */
   /*========================================================*/
   
   (*env)->DeleteLocalRef(env,theClass);

   /*=========================================================*/
   /* If the object was created, add a global reference to it */
   /* and delete the local reference. This will prevent the   */
   /* object from being garbage collected until CLIPS deletes */
   /* the global reference.                                   */
   /*=========================================================*/
   
   if (theObject != NULL)
     {
      oldObject = theObject;
      theObject = (*env)->NewGlobalRef(env,theObject);
      (*env)->DeleteLocalRef(env,oldObject);
     }

   /*=============================================*/
   /* Return the array containing the DATA_OBJECT */
   /* arguments to the new function.              */
   /*=============================================*/
   
   if (newArgs != NULL)
     { genfree(theEnv,newArgs,sizeof(DATA_OBJECT) * (numberOfArguments - 2)); }
     
   if (javaArgs != NULL)
     { genfree(theEnv,javaArgs,sizeof(jvalue) * (numberOfArguments - 2)); }
   
   /*=============================================*/
   /* If a java exception occurred, set the CLIPS */
   /* error flag and clear the java exception.    */
   /*=============================================*/
   
   if ((*env)->ExceptionOccurred(env))
     { 
      EnvSetEvaluationError(theEnv,TRUE);
      (*env)->ExceptionClear(env); 
     }

   /*==========================================*/
   /* Return the newly created java object if  */
   /* it was successfully created, otherwise   */
   /* leave the default return value of FALSE. */
   /*==========================================*/
   
   if (theObject != NULL)
     {
      SetpType(rv,EXTERNAL_ADDRESS);
      SetpValue(rv,EnvAddExternalAddress(theEnv,theObject,CLIPSJNIData(theEnv)->javaExternalAddressID));
     }
  }

/*******************/
/* CallJavaMethod: */
/*******************/
static intBool CallJavaMethod(
  void *theEnv,
  DATA_OBJECT *target,
  DATA_OBJECT *rv)
  {
   int numberOfArguments;
   jobject theObject, theMethod;
   jclass objectClass, tempClass;
   jmethodID mid, getNameID;
   JNIEnv *env;
   jobjectArray methodList, parameterList;
   jsize theSize, c; 
   jsize paramCount, p; 
   DATA_OBJECT theValue;
   const char *methodName;
   jstring str;
   char *cStr;
   int matches;
   DATA_OBJECT_PTR newArgs;
   jvalue *javaArgs;
   int i;
   
   /*=============================================*/
   /* Retrieve the JNI environment pointer stored */
   /* in the CLIPS environment structure.         */
   /*=============================================*/
   
   env = (JNIEnv *) GetEnvironmentContext(theEnv);

   /*=================================================================*/
   /* If the Java external address type is used, additional arguments */
   /* must at least include the name of the method being called.      */
   /*=================================================================*/
   
   if ((numberOfArguments = EnvArgCountCheck(theEnv,"call (with type Java)",AT_LEAST,2)) == -1) 
     { return FALSE; }

   /*========================================*/
   /* The Java method name must be a symbol. */
   /*========================================*/
   
   if (EnvArgTypeCheck(theEnv,"call (with type Java)",2,SYMBOL,&theValue) == FALSE) 
     { return FALSE; }
   
   methodName = DOToString(theValue);

   /*===========================================================================*/
   /* Evaluate the CLIPS arguments that will be passed to the java constructor. */
   /*===========================================================================*/
   
   if (numberOfArguments - 2 == 0)
     { newArgs = NULL; }
   else
     {
      newArgs = (DATA_OBJECT_PTR) genalloc(theEnv,sizeof(DATA_OBJECT) * (numberOfArguments - 2));
      for (i = 0; i < numberOfArguments - 2; i++)
        {
         EnvRtnUnknown(theEnv,i+3,&newArgs[i]);
         if (EnvGetEvaluationError(theEnv))
           { return FALSE; }
        }
     }

   /*========================================================================*/
   /* Construct an array in which to store the corresponding java arguments. */
   /*========================================================================*/
   
   if (numberOfArguments - 2 == 0)
     { javaArgs = NULL; }
   else
     { javaArgs = (jvalue *) genalloc(theEnv,sizeof(jvalue) * (numberOfArguments - 2)); }

   /*===============================================*/
   /* If the target is an external address, then we */
   /* should be invoking a method of an instance.   */
   /*===============================================*/

   if (GetpType(target) == EXTERNAL_ADDRESS)
     {
      theObject = DOPToExternalAddress(target);

      /*=========================================*/
      /* Determine the class of the java object. */
      /*=========================================*/
      
      objectClass = (*env)->GetObjectClass(env,theObject);

      /*=============================================*/
      /* Get the method index of the getConstructors */
      /* method from the java.lang.Class class.      */
      /*=============================================*/

      tempClass = (*env)->FindClass(env,"java/lang/Class"); /* TBD Cache this Value */
      mid = (*env)->GetMethodID(env,tempClass,"getMethods","()[Ljava/lang/reflect/Method;"); 
      (*env)->DeleteLocalRef(env,tempClass);

      /*==================================================*/
      /* Get the list of methods for the specified class. */
      /*==================================================*/
     
      methodList = (jobjectArray) (*env)->CallObjectMethod(env,objectClass,mid);
      (*env)->DeleteLocalRef(env,objectClass);

      /*======================================================*/
      /* Get the method index of the getParameterTypes method */
      /* from the java.lang.reflect.Method class.             */
      /*======================================================*/

      tempClass = (*env)->FindClass(env,"java/lang/reflect/Method"); /* TBD Cache this Value */
      mid = (*env)->GetMethodID(env,tempClass,"getParameterTypes","()[Ljava/lang/Class;"); 
      getNameID = (*env)->GetMethodID(env,tempClass,"getName","()Ljava/lang/String;"); 
      (*env)->DeleteLocalRef(env,tempClass);

      /*=====================================*/
      /* Search the method list for a method */
      /* with matching arguments.            */
      /*=====================================*/

      theSize = (*env)->GetArrayLength(env,methodList); 
      for (c = 0; c < theSize; c++) 
        { 
         theMethod = (*env)->GetObjectArrayElement(env,methodList,c); 
         str = (jstring) (*env)->CallObjectMethod(env,theMethod,getNameID);
         cStr = (char *) (*env)->GetStringUTFChars(env,str,NULL);
         
         /*===================================*/
         /* If the method name doesn't match, */
         /* move on to the next method.       */
         /*===================================*/
         
         if (strcmp(methodName,cStr) != 0)
           {
            (*env)->ReleaseStringUTFChars(env,str,cStr);
            continue;
           }
         (*env)->ReleaseStringUTFChars(env,str,cStr);
         
         /*==========================================*/
         /* Get the parameter list of the method and */
         /* determine the number of parameters.      */
         /*==========================================*/
         
         parameterList = (jobjectArray) (*env)->CallObjectMethod(env,theMethod,mid);
      
         paramCount = (*env)->GetArrayLength(env,parameterList); 
      
         if (paramCount != (numberOfArguments - 2))
           { 
            (*env)->ReleaseStringUTFChars(env,str,cStr);
            continue; 
           }

         matches = TRUE;
      
         for (p = 0; (p < paramCount) && matches; p++)
           {
            tempClass = (jclass) (*env)->GetObjectArrayElement(env,parameterList,p);
         
            str = (jstring) (*env)->CallObjectMethod(env,tempClass,CLIPSJNIData(theEnv)->classGetCanonicalNameMethod);

            cStr = (char *) (*env)->GetStringUTFChars(env,str,NULL);
             
            printf("p[%d] = %s\n",(int) p,cStr);
            
            if (GetType(newArgs[p]) == INTEGER)
              {
               if (strcmp(cStr,"long") == 0)
                 { 
                  /* printf("p[%d] = %s\n",(int) p,cStr); */
                  javaArgs[p].j = DOToLong(newArgs[p]);
                 }
               else if (strcmp(cStr,"int") == 0)  
                 { 
                  /* printf("p[%d] = %s\n",(int) p,cStr); */
                  javaArgs[p].i = DOToLong(newArgs[p]);
                 }
               else
                 { matches = FALSE; }
              }
            else
              { matches = FALSE; }

            (*env)->ReleaseStringUTFChars(env,str,cStr);
         
            (*env)->DeleteLocalRef(env,tempClass);
           }
      
         if (matches) break;
        }
     }

   if (newArgs != NULL)
     { genfree(theEnv,newArgs,sizeof(DATA_OBJECT) * (numberOfArguments - 2)); }

   if (javaArgs != NULL)
     { genfree(theEnv,javaArgs,sizeof(jvalue) * (numberOfArguments - 2)); }
     
   return TRUE;
  }
  
/***********************/
/* DiscardJavaAddress: */
/***********************/
static intBool DiscardJavaAddress(
  void *theEnv,
  void *theValue)
  {
   JNIEnv *env;

   printf("Discarding Java Address %p\n",theValue);
   
   if (theValue != NULL)
     {
      env = (JNIEnv *) GetEnvironmentContext(theEnv);
      (*env)->DeleteGlobalRef(env,theValue);
     }
   
   return TRUE;
  }
