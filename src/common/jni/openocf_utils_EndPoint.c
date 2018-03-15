/**
 * @file openocf_utils_EndPoint.c
 * @author Gregg Reynolds
 * @date December 2016
 *
 * @brief JNI implementation of EndPoint Java API (OCDevAddr)
 */

#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "_threads.h"

#include "openocf_utils_EndPoint.h"
#include "jni_utils.h"
#include "jni_init.h"
#include "org_iochibity_Exceptions.h"

#include "openocf.h"
/* #include "oic_malloc.h" */
/* #include "ocpayload.h" */
/* #include "ocresource.h" */
/* #include "ocresourcehandler.h" */
/* #include "ocstack.h" */

/* PRIVATE */

/* PUBLIC */

/*
 * Class:     openocf_utils_EndPoint
 * Method:    networkProtocol
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_openocf_utils_EndPoint_networkProtocol
(JNIEnv * env, jobject this)
{
  OC_UNUSED(env);
  OC_UNUSED(this);
  if (tls_response_in == NULL) {
    // FIXME: if our TLS var has not been initialized, we have not
    // recieved a discovery response containing the remote
    // EndPoint info.
    // FIXME: return multicast info?
    return -1;
  } else {
    return RESPONSE_IN->devAddr.adapter;
  }
}

/*
 * Class:     openocf_utils_EndPoint
 * Method:    networkFlags
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_openocf_utils_EndPoint_networkFlags
(JNIEnv * env, jobject this)
{
  OC_UNUSED(env);
  OC_UNUSED(this);
  if (tls_response_in == NULL) {
    return -1;
  } else {
    return RESPONSE_IN->devAddr.flags;
  }
}

/*
 * Class:     openocf_utils_EndPoint
 * Method:    networkScope
 * Signature: ()B
 */
JNIEXPORT jbyte JNICALL Java_openocf_utils_EndPoint_networkScope
(JNIEnv * env, jobject this)
{
  OC_UNUSED(env);
  OC_UNUSED(this);
  if (tls_response_in == NULL) {
    return -1;
  } else {
    return RESPONSE_IN->devAddr.flags & 0x0F;
  }
}

/*
 * Class:     openocf_utils_EndPoint
 * Method:    transportIsSecure
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_openocf_utils_EndPoint_transportIsSecure
(JNIEnv * env, jobject this)
{
  OC_UNUSED(env);
  OC_UNUSED(this);
  if (tls_response_in == NULL) {
    return false;
  } else {
    return  RESPONSE_IN->devAddr.flags & 0x0010;	/* (1 << 4) */
  }
}

/*
 * Class:     openocf_utils_EndPoint
 * Method:    isIPv6
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_openocf_utils_EndPoint_isIPv6
(JNIEnv * env, jobject this)
{
  OC_UNUSED(env);
  OC_UNUSED(this);
  if (tls_response_in == NULL) {
    return false;
  } else {
    return  RESPONSE_IN->devAddr.flags & 0x0020;	/* (1 << 5) */
  }
}

/*
 * Class:     openocf_utils_EndPoint
 * Method:    isIPv4
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_openocf_utils_EndPoint_isIPv4
(JNIEnv * env, jobject this)
{
  OC_UNUSED(env);
  OC_UNUSED(this);
  if (tls_response_in == NULL) {
    return false;
  } else {
    return  RESPONSE_IN->devAddr.flags & 0x0040;	/* (1 << 6) */
  }
}

/*
 * Class:     openocf_utils_EndPoint
 * Method:    isMulticast
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_openocf_utils_EndPoint_isMulticast
(JNIEnv * env, jobject this)
{
  OC_UNUSED(env);
  OC_UNUSED(this);
  if (tls_response_in == NULL) {
    return false;
  } else {
    return RESPONSE_IN->devAddr.flags & 0x0080;	/* (1 << 7) */
  }
}


/*
 * Class:     openocf_utils_EndPoint
 * Method:    port
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_openocf_utils_EndPoint_port
(JNIEnv * env, jobject this)
{
  OC_UNUSED(env);
  OC_UNUSED(this);
  if (tls_response_in == NULL) {
    return -1;
  } else {
    return RESPONSE_IN->devAddr.port;
  }
}

/*
 * Class:     openocf_utils_EndPoint
 * Method:    ipAddress
 * Signature: ()Ljava/lang/String;
 */
 jstring JNICALL Java_openocf_utils_EndPoint_ipAddress
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    /* printf("%s : %s ENTRY, %d\n", __FILE__, __func__, (intptr_t)THREAD_ID); */
    /* printf("TLS: %d\n", tls_response_in); */

    if (tls_response_in) {
	return(*env)->NewStringUTF(env, RESPONSE_IN->devAddr.addr);
    } else {
	// FIXME: if our TLS var has not been initialized, we have not
	// recieved a discovery response containing the remote
	// EndPoint info.
	printf("FIXME: implement %s\n", __func__);
	return NULL;
    }
}

/*
 * Class:     openocf_utils_EndPoint
 * Method:    ifindex
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_openocf_utils_EndPoint_ifindex
(JNIEnv * env, jobject this)
{
  OC_UNUSED(env);
  OC_UNUSED(this);
  if (tls_response_in == NULL) {
    return -1;
  } else {
    return RESPONSE_IN->devAddr.ifindex;
  }
}

/*
 * Class:     openocf_utils_EndPoint
 * Method:    routeData
 * Signature: ()Ljava/lang/String;
 */
/* JNIEXPORT jstring JNICALL Java_openocf_utils_EndPoint_routeData */
/* (JNIEnv * env, jobject this) */
/* { */
/*   OC_UNUSED(env); */
/*   OC_UNUSED(this); */
/*   return RESPONSE_IN->routeData; */
/* } */
