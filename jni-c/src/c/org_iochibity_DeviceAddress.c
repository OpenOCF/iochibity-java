/**
 * @file org_iochibity_DeviceAddress.c
 * @author Gregg Reynolds
 * @date December 2016
 *
 * @brief JNI implementation of DeviceAddress Java API
 */

#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "_threads.h"

#include "org_iochibity_DeviceAddress.h"
#include "jni_utils.h"
#include "jni_init.h"
#include "org_iochibity_Exceptions.h"

#include "octypes.h"
#include "oic_malloc.h"
#include "ocpayload.h"
#include "ocresource.h"
#include "ocresourcehandler.h"
#include "ocstack.h"

/* PRIVATE */

/* PUBLIC */

/*
 * Class:     org_iochibity_DeviceAddress
 * Method:    networkProtocol
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_iochibity_DeviceAddress_networkProtocol
(JNIEnv * env, jobject this)
{
  OC_UNUSED(env);
  OC_UNUSED(this);
  if (tls_response_in == NULL) {
    // FIXME: if our TLS var has not been initialized, we have not
    // recieved a discovery response containing the remote
    // DeviceAddress info.
    // FIXME: return multicast info?
    return -1;
  } else {
    return RESPONSE_IN->devAddr.adapter;
  }
}

/*
 * Class:     org_iochibity_DeviceAddress
 * Method:    networkFlags
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_iochibity_DeviceAddress_networkFlags
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
 * Class:     org_iochibity_DeviceAddress
 * Method:    networkScope
 * Signature: ()B
 */
JNIEXPORT jbyte JNICALL Java_org_iochibity_DeviceAddress_networkScope
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
 * Class:     org_iochibity_DeviceAddress
 * Method:    transportIsSecure
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_iochibity_DeviceAddress_transportIsSecure
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
 * Class:     org_iochibity_DeviceAddress
 * Method:    isIPv6
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_iochibity_DeviceAddress_isIPv6
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
 * Class:     org_iochibity_DeviceAddress
 * Method:    isIPv4
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_iochibity_DeviceAddress_isIPv4
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
 * Class:     org_iochibity_DeviceAddress
 * Method:    isMulticast
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_iochibity_DeviceAddress_isMulticast
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
 * Class:     org_iochibity_DeviceAddress
 * Method:    port
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_iochibity_DeviceAddress_port
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
 * Class:     org_iochibity_DeviceAddress
 * Method:    ipAddress
 * Signature: ()Ljava/lang/String;
 */
 jstring JNICALL Java_org_iochibity_DeviceAddress_ipAddress
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
	// DeviceAddress info.
	printf("FIXME: implement %s\n", __func__);
	return NULL;
    }
}

/*
 * Class:     org_iochibity_DeviceAddress
 * Method:    ifindex
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_iochibity_DeviceAddress_ifindex
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
 * Class:     org_iochibity_DeviceAddress
 * Method:    routeData
 * Signature: ()Ljava/lang/String;
 */
/* JNIEXPORT jstring JNICALL Java_org_iochibity_DeviceAddress_routeData */
/* (JNIEnv * env, jobject this) */
/* { */
/*   OC_UNUSED(env); */
/*   OC_UNUSED(this); */
/*   return RESPONSE_IN->routeData; */
/* } */

