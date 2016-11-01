/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class CLIPSJNI_Environment */

#ifndef _Included_CLIPSJNI_Environment
#define _Included_CLIPSJNI_Environment
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     CLIPSJNI_Environment
 * Method:    getCLIPSVersion
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_CLIPSJNI_Environment_getCLIPSVersion
  (JNIEnv *, jclass);

/*
 * Class:     CLIPSJNI_Environment
 * Method:    createEnvironment
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_CLIPSJNI_Environment_createEnvironment
  (JNIEnv *, jobject);

/*
 * Class:     CLIPSJNI_Environment
 * Method:    clear
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_CLIPSJNI_Environment_clear
  (JNIEnv *, jobject, jlong);

/*
 * Class:     CLIPSJNI_Environment
 * Method:    reset
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_CLIPSJNI_Environment_reset
  (JNIEnv *, jobject, jlong);

/*
 * Class:     CLIPSJNI_Environment
 * Method:    load
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_CLIPSJNI_Environment_load
  (JNIEnv *, jobject, jlong, jstring);

/*
 * Class:     CLIPSJNI_Environment
 * Method:    loadFacts
 * Signature: (JLjava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_CLIPSJNI_Environment_loadFacts
  (JNIEnv *, jobject, jlong, jstring);

/*
 * Class:     CLIPSJNI_Environment
 * Method:    watch
 * Signature: (JLjava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_CLIPSJNI_Environment_watch
  (JNIEnv *, jobject, jlong, jstring);

/*
 * Class:     CLIPSJNI_Environment
 * Method:    unwatch
 * Signature: (JLjava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_CLIPSJNI_Environment_unwatch
  (JNIEnv *, jobject, jlong, jstring);

/*
 * Class:     CLIPSJNI_Environment
 * Method:    run
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_CLIPSJNI_Environment_run
  (JNIEnv *, jobject, jlong, jlong);

/*
 * Class:     CLIPSJNI_Environment
 * Method:    eval
 * Signature: (JLjava/lang/String;)LCLIPSJNI/PrimitiveValue;
 */
JNIEXPORT jobject JNICALL Java_CLIPSJNI_Environment_eval
  (JNIEnv *, jobject, jlong, jstring);

/*
 * Class:     CLIPSJNI_Environment
 * Method:    build
 * Signature: (JLjava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_CLIPSJNI_Environment_build
  (JNIEnv *, jobject, jlong, jstring);

/*
 * Class:     CLIPSJNI_Environment
 * Method:    assertString
 * Signature: (JLjava/lang/String;)LCLIPSJNI/FactAddressValue;
 */
JNIEXPORT jobject JNICALL Java_CLIPSJNI_Environment_assertString
  (JNIEnv *, jobject, jlong, jstring);

/*
 * Class:     CLIPSJNI_Environment
 * Method:    factIndex
 * Signature: (LCLIPSJNI/Environment;JJ)J
 */
JNIEXPORT jlong JNICALL Java_CLIPSJNI_Environment_factIndex
  (JNIEnv *, jclass, jobject, jlong, jlong);

/*
 * Class:     CLIPSJNI_Environment
 * Method:    getFactSlot
 * Signature: (LCLIPSJNI/Environment;JJLjava/lang/String;)LCLIPSJNI/PrimitiveValue;
 */
JNIEXPORT jobject JNICALL Java_CLIPSJNI_Environment_getFactSlot
  (JNIEnv *, jclass, jobject, jlong, jlong, jstring);

/*
 * Class:     CLIPSJNI_Environment
 * Method:    makeInstance
 * Signature: (JLjava/lang/String;)LCLIPSJNI/InstanceAddressValue;
 */
JNIEXPORT jobject JNICALL Java_CLIPSJNI_Environment_makeInstance
  (JNIEnv *, jobject, jlong, jstring);

/*
 * Class:     CLIPSJNI_Environment
 * Method:    getInstanceName
 * Signature: (LCLIPSJNI/Environment;JJ)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_CLIPSJNI_Environment_getInstanceName
  (JNIEnv *, jclass, jobject, jlong, jlong);

/*
 * Class:     CLIPSJNI_Environment
 * Method:    directGetSlot
 * Signature: (LCLIPSJNI/Environment;JJLjava/lang/String;)LCLIPSJNI/PrimitiveValue;
 */
JNIEXPORT jobject JNICALL Java_CLIPSJNI_Environment_directGetSlot
  (JNIEnv *, jclass, jobject, jlong, jlong, jstring);

/*
 * Class:     CLIPSJNI_Environment
 * Method:    destroyEnvironment
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_CLIPSJNI_Environment_destroyEnvironment
  (JNIEnv *, jobject, jlong);

/*
 * Class:     CLIPSJNI_Environment
 * Method:    commandLoop
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_CLIPSJNI_Environment_commandLoop
  (JNIEnv *, jobject, jlong);

/*
 * Class:     CLIPSJNI_Environment
 * Method:    getInputBuffer
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_CLIPSJNI_Environment_getInputBuffer
  (JNIEnv *, jobject, jlong);

/*
 * Class:     CLIPSJNI_Environment
 * Method:    setInputBuffer
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_CLIPSJNI_Environment_setInputBuffer
  (JNIEnv *, jobject, jlong, jstring);

/*
 * Class:     CLIPSJNI_Environment
 * Method:    getInputBufferCount
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_CLIPSJNI_Environment_getInputBufferCount
  (JNIEnv *, jobject, jlong);

/*
 * Class:     CLIPSJNI_Environment
 * Method:    setInputBufferCount
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_CLIPSJNI_Environment_setInputBufferCount
  (JNIEnv *, jobject, jlong, jlong);

/*
 * Class:     CLIPSJNI_Environment
 * Method:    expandInputBuffer
 * Signature: (JC)V
 */
JNIEXPORT void JNICALL Java_CLIPSJNI_Environment_expandInputBuffer
  (JNIEnv *, jobject, jlong, jchar);

/*
 * Class:     CLIPSJNI_Environment
 * Method:    appendInputBuffer
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_CLIPSJNI_Environment_appendInputBuffer
  (JNIEnv *, jobject, jlong, jstring);

/*
 * Class:     CLIPSJNI_Environment
 * Method:    inputBufferContainsCommand
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_CLIPSJNI_Environment_inputBufferContainsCommand
  (JNIEnv *, jobject, jlong);

/*
 * Class:     CLIPSJNI_Environment
 * Method:    commandLoopOnceThenBatch
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_CLIPSJNI_Environment_commandLoopOnceThenBatch
  (JNIEnv *, jobject, jlong);

/*
 * Class:     CLIPSJNI_Environment
 * Method:    printBanner
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_CLIPSJNI_Environment_printBanner
  (JNIEnv *, jobject, jlong);

/*
 * Class:     CLIPSJNI_Environment
 * Method:    printPrompt
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_CLIPSJNI_Environment_printPrompt
  (JNIEnv *, jobject, jlong);

/*
 * Class:     CLIPSJNI_Environment
 * Method:    addRouter
 * Signature: (JLjava/lang/String;ILCLIPSJNI/Router;)Z
 */
JNIEXPORT jboolean JNICALL Java_CLIPSJNI_Environment_addRouter
  (JNIEnv *, jobject, jlong, jstring, jint, jobject);

/*
 * Class:     CLIPSJNI_Environment
 * Method:    incrementFactCount
 * Signature: (LCLIPSJNI/Environment;JJ)V
 */
JNIEXPORT void JNICALL Java_CLIPSJNI_Environment_incrementFactCount
  (JNIEnv *, jobject, jobject, jlong, jlong);

/*
 * Class:     CLIPSJNI_Environment
 * Method:    decrementFactCount
 * Signature: (LCLIPSJNI/Environment;JJ)V
 */
JNIEXPORT void JNICALL Java_CLIPSJNI_Environment_decrementFactCount
  (JNIEnv *, jobject, jobject, jlong, jlong);

/*
 * Class:     CLIPSJNI_Environment
 * Method:    incrementInstanceCount
 * Signature: (LCLIPSJNI/Environment;JJ)V
 */
JNIEXPORT void JNICALL Java_CLIPSJNI_Environment_incrementInstanceCount
  (JNIEnv *, jobject, jobject, jlong, jlong);

/*
 * Class:     CLIPSJNI_Environment
 * Method:    decrementInstanceCount
 * Signature: (LCLIPSJNI/Environment;JJ)V
 */
JNIEXPORT void JNICALL Java_CLIPSJNI_Environment_decrementInstanceCount
  (JNIEnv *, jobject, jobject, jlong, jlong);

#ifdef __cplusplus
}
#endif
#endif
