
#include "net_sf_clipsrules_jni_Environment.h"

#include "clipsjni_data.h"
#include "clipsjni_utilities.h"
#include "clipsjni_glue.h"

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
   return CreateCLIPSJNIEnvironment(env,obj);
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
   void *oldContext = SetEnvironmentContext(JLongToPointer(clipsEnv),(void *) env);
   void *theEnv;
   
   theEnv = JLongToPointer(clipsEnv);
   
   EnvLoad(theEnv,(char *) cFileName);
   
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
   void *oldContext = SetEnvironmentContext(JLongToPointer(clipsEnv),(void *) env);
   void *theEnv;
   
   theEnv = JLongToPointer(clipsEnv);

   OpenStringSource(theEnv,"clipsjniloadfromstring",cLoadString,0); 
   LoadConstructsFromLogicalName(theEnv,"clipsjniloadfromstring");
   CloseStringSource(theEnv,"clipsjniloadfromstring");
   
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
   
   EnvSetEvaluationError(theCLIPSEnv,FALSE);
   EnvSetHaltExecution(theCLIPSEnv,FALSE);

   EnvEval(theCLIPSEnv,(char *) cEvalStr,&theDO);

   (*env)->ReleaseStringUTFChars(env,evalStr,cEvalStr);
   
   result = ConvertDataObject(env,obj,theCLIPSEnv,&theDO);

   SetEnvironmentContext(theCLIPSEnv,oldContext);

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

   EnvSetEvaluationError(theCLIPSEnv,FALSE);
   EnvSetHaltExecution(theCLIPSEnv,FALSE);
   
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
  
 /***********************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_incrementAddressCount: Native */
/*   function for the CLIPSJNI incrementAddressCount method.            */
/*                                                                      */
/* Class:     net_sf_clipsrules_jni_Environment                         */
/* Method:    incrementAddressCount                                     */
/* Signature: (Lnet/sf/clipsrules/jni/Environment;JJ)V                  */
/************************************************************************/
JNIEXPORT void JNICALL Java_net_sf_clipsrules_jni_Environment_incrementAddressCount(
  JNIEnv *env, 
  jclass javaClass, 
  jobject javaEnv,
  jlong clipsEnv, 
  jlong clipsExternalAddress)
  {
   void *oldContext = SetEnvironmentContext(JLongToPointer(clipsEnv),(void *) env);

   IncrementExternalAddressCount(JLongToPointer(clipsExternalAddress));
   
   SetEnvironmentContext(JLongToPointer(clipsEnv),oldContext);
  }
  
/************************************************************************/
/* Java_net_sf_clipsrules_jni_Environment_decrementAddressCount: Native */
/*   function for the CLIPSJNI decrementAddressCount method.            */
/*                                                                      */
/* Class:     net_sf_clipsrules_jni_Environment                         */
/* Method:    decrementFactCount                                        */
/* Signature: (Lnet/sf/clipsrules/jni/Environment;JJ)V                  */
/************************************************************************/
JNIEXPORT void JNICALL Java_net_sf_clipsrules_jni_Environment_decrementAddressCount(
  JNIEnv *env, 
  jclass javaClass, 
  jobject javaEnv,
  jlong clipsEnv, 
  jlong clipsExternalAddress)
  {
   void *oldContext = SetEnvironmentContext(JLongToPointer(clipsEnv),(void *) env);

   DecrementExternalAddressCount(JLongToPointer(clipsEnv),JLongToPointer(clipsExternalAddress));
   
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


