/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class org_iochibity_ServiceManager */

#ifndef _Included_org_iochibity_ServiceManager
#define _Included_org_iochibity_ServiceManager
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     org_iochibity_ServiceManager
 * Method:    resourceCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_iochibity_ServiceManager_resourceCount
  (JNIEnv *, jclass);

/*
 * Class:     org_iochibity_ServiceManager
 * Method:    registerPlatformProvider
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_iochibity_ServiceManager_registerPlatformProvider
  (JNIEnv *, jclass, jstring, jstring, jstring, jstring, jstring, jstring, jstring, jstring, jstring, jstring, jstring);

/*
 * Class:     org_iochibity_ServiceManager
 * Method:    registerDeviceProvider
 * Signature: (Ljava/lang/String;[Ljava/lang/String;Ljava/lang/String;[Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_iochibity_ServiceManager_registerDeviceProvider
  (JNIEnv *, jclass, jstring, jobjectArray, jstring, jobjectArray);

/*
 * Class:     org_iochibity_ServiceManager
 * Method:    registerDefaultServiceRoutine
 * Signature: (Ljava/lang/Object;Ljava/lang/Object;)V
 */
JNIEXPORT void JNICALL Java_org_iochibity_ServiceManager_registerDefaultServiceRoutine
  (JNIEnv *, jclass, jobject, jobject);

/*
 * Class:     org_iochibity_ServiceManager
 * Method:    registerServiceProvider
 * Signature: (Lorg/iochibity/ServiceProvider;)Lorg/iochibity/ServiceProvider;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_ServiceManager_registerServiceProvider
  (JNIEnv *, jclass, jobject);

#ifdef __cplusplus
}
#endif
#endif