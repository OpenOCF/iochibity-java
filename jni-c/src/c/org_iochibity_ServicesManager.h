/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class org_iochibity_ServicesManager */

#ifndef _Included_org_iochibity_ServicesManager
#define _Included_org_iochibity_ServicesManager
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     org_iochibity_ServicesManager
 * Method:    resourceCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_iochibity_ServicesManager_resourceCount
  (JNIEnv *, jclass);

/*
 * Class:     org_iochibity_ServicesManager
 * Method:    registerPlatformProvider
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_iochibity_ServicesManager_registerPlatformProvider
  (JNIEnv *, jclass, jstring, jstring, jstring, jstring, jstring, jstring, jstring, jstring, jstring, jstring, jstring);

/*
 * Class:     org_iochibity_ServicesManager
 * Method:    registerDeviceProvider
 * Signature: (Ljava/lang/String;[Ljava/lang/String;Ljava/lang/String;[Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_iochibity_ServicesManager_registerDeviceProvider
  (JNIEnv *, jclass, jstring, jobjectArray, jstring, jobjectArray);

/*
 * Class:     org_iochibity_ServicesManager
 * Method:    registerDefaultServiceRoutine
 * Signature: (Ljava/lang/Object;Ljava/lang/Object;)V
 */
JNIEXPORT void JNICALL Java_org_iochibity_ServicesManager_registerDefaultServiceRoutine
  (JNIEnv *, jclass, jobject, jobject);

/*
 * Class:     org_iochibity_ServicesManager
 * Method:    registerServiceProvider
 * Signature: (Lorg/iochibity/ServiceProvider;)Lorg/iochibity/ServiceProvider;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_ServicesManager_registerServiceProvider
  (JNIEnv *, jclass, jobject);

#ifdef __cplusplus
}
#endif
#endif
