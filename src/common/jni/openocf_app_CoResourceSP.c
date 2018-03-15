/**
 * @file openocf_CoResourceSP.c
 * @author Gregg Reynolds
 * @date December 2016
 *
 * @brief JNI implementation of CoResourceSP (client) Java API:
 * `Java_openocf_app_CoResourceSP_coExhibit` and callback
 * `c_CoResourceSP_coReact`
 */

#include <string.h>
/* #include <ctype.h> */
/* #include <stdlib.h> */

/* http://www.informit.com/articles/article.aspx?p=2036582&seqNum=5
OpenBSD:  strlcpy
C11:      strncpy_s
*/

#include "_threads.h"

#include "openocf_app_CoResourceSP.h"
/* #include "_openocf_app_ClientSP.h" */
#include "_openocf_app_CoResourceSP.h"
#include "jni_utils.h"
#include "jni_init.h"
#include "org_iochibity_Exceptions.h"

#include "openocf.h"
#include "oic_malloc.h"
/* #include "oic_string.h" */
/* #include "ocpayload.h" */
/* #include "ocresource.h" */
/* #include "ocresourcehandler.h" */
/* #include "ocstack.h" */

/* #include "logger.h" */

/* logging */
#define TAG  "CoSP"

/* externs */

/*
  multithreading: each cosp calls into this to set up and send
  requests.  the rule is one thread per request, i.e. the cosp must
  be allocked on the same thread that calls in here.  that allows us
  to us TLS to store request state.  and this allows us to use one
  cosp object for various requests.
*/

/* _Thread_local OCDevAddr* tls_defaultCoAddress		= NULL; */

/* PRIVATE */

txn_t* g_txn_list = NULL;

THREAD_LOCAL request_out_t* tls_request_out = NULL;

THREAD_LOCAL response_in_t* tls_response_in	= NULL;

response_in_t*  g_response_map;	/* ???  */

THREAD_LOCAL bool   tls_deactivate;

#define FQCN_ICORESOURCE_SP "openocf/app/ICoResourceSP"
#define FQCS_ICORESOURCE_SP "Lopenocf/app/ICoResourceSP;"
jclass    K_ICORESOURCE_SP         = NULL; /* was: K_I_CO_RESOURCE_SP */
jmethodID MID_ICORSP_COREACT               = NULL;

jclass    K_CORESOURCE_SP           = NULL;
/* jfieldID  FID_CORSP_HANDLE              = NULL; */
/* jfieldID  FID_CORSP_MSG_RESPONSE_IN  = NULL; */
/* jfieldID  FID_CORSP_METHOD              = NULL; */
/* jfieldID  FID_CORSP_URI_PATH            = NULL; */
/* jfieldID  FID_CORSP_DESTINATION         = NULL; */
jmethodID MID_CORSP_EXHIBIT    = NULL;

#define TYPSIG_CORESOURCESP "openocf/app/CoResourceSP"

int init_ICoResourceSP(JNIEnv* env)
{
    jclass klass;
    klass = (*env)->FindClass(env, FQCN_ICORESOURCE_SP);
    JNI_ASSERT_NULL(klass, "FindClass failed for openocf/ICoResourceSP\n", 0);
    K_ICORESOURCE_SP = (jclass)(*env)->NewGlobalRef(env, klass);
    (*env)->DeleteLocalRef(env, klass);

    jmethodID MID_ICORSP_COREACT  = NULL;
    if (MID_ICORSP_COREACT == NULL) {
	MID_ICORSP_COREACT = (*env)->GetMethodID(env, K_ICORESOURCE_SP,
						"coReact",
						"()I");
	if (MID_ICORSP_COREACT == NULL) {
	    printf("ERROR: GetMethodID failed for 'coReact' of ICoResourceSP\n");
	    return OC_EH_INTERNAL_SERVER_ERROR;
	}
    }
    return 0;
}

int init_CoResourceSP(JNIEnv* env)
{
    jclass klass;
    klass = (*env)->FindClass(env, TYPSIG_CORESOURCESP);
    JNI_ASSERT_NULL(klass, "FindClass failed for " TYPSIG_CORESOURCESP "\n", 0);
    K_CORESOURCE_SP = (jclass)(*env)->NewGlobalRef(env, klass);
    (*env)->DeleteLocalRef(env, klass);

    //FIXME: do we need coExhibit as an SPI point? who will call it?
    /* jmethodID MID_CORSP_EXHIBIT  = NULL; */
    /* if (MID_CORSP_EXHIBIT == NULL) { */
    /* 	MID_CORSP_EXHIBIT = (*env)->GetMethodID(env, K_ICORESOURCE_SP, */
    /* 						"coExhibit", "()V"); */
    /* 	if (MID_CORSP_EXHIBIT == NULL) { */
    /* 	    printf("ERROR: GetMethodID failed for 'coExhibit' of CoResourceSP\n"); */
    /* 	    return OC_EH_INTERNAL_SERVER_ERROR; */
    /* 	} */
    /* } */

    return 0;
}

void request_out_ctor()
{
    OIC_LOG_V(DEBUG, TAG, "%s ENTRY, tid %ld", __func__, (intptr_t)THREAD_ID);
    tls_request_out                   = OICCalloc(sizeof (request_out_t), 1);

    tls_request_out->txnId	     = NULL;
    tls_request_out->method	     = OC_REST_NOMETHOD;
    /* never pass NULL for destination */
    tls_request_out->destination     = (OCDevAddr *)OICCalloc(sizeof (OCDevAddr), 1);
    tls_request_out->connectivityType = CT_DEFAULT; /* 0 */
    tls_request_out->qos	     = OC_LOW_QOS;
    tls_request_out->cbData	     = NULL;
    tls_request_out->options	     = NULL;
    tls_request_out->numOptions	     = 0;

    tls_request_out->next	     = NULL;
}

/* **************************************************************** */
jboolean test_network_flag(int flag)
{
    return (bool)(tls_request_out->connectivityType & flag);
}

void set_network_flag(int flag, jboolean torf)
{
    if (torf) {
	tls_request_out->connectivityType |= flag;
    } else {
	tls_request_out->connectivityType &= ~flag;
    }
}

/**

 * FIXME: obsolete?

 * @brief Convert OCClientResponse to an `ObservationIn` object
 *
 * Allocate an `ObservationIn` java object, then use data from incoming
 * `OCClientResponse` to initialize it.  Return the initialize object.
 * Called internally by `c_CoResourceSP_coReact`.
 *
 * @param [in] env JNIEnv pointer
 * @param [in] c_OCClientResponse response data from server
 * @return newly allocated and initialized `ObservationIn` object
 */
jobject OCClientResponse_to_ObservationIn(JNIEnv* env, OCClientResponse* c_OCClientResponse)
{
    printf("%s: OCClientResponse_to_ObservationIn ENTRY\n", __FILE__);
    jobject j_ObservationIn = (*env)->NewObject(env,
						K_OBSERVATION_IN,
						MID_OBIN_CTOR); // request_in_ctor);
    if (j_ObservationIn == NULL) {
	printf("NewObject failed for ObservationIn\n");
	return NULL;
    }

    /* set the c struct handle */
    (*env)->SetIntField(env, j_ObservationIn, FID_MSG_LOCAL_HANDLE, (jlong)c_OCClientResponse);

    /* OCDevAddr */
    jobject j_EndPoint = (*env)->NewObject(env, K_ENDPOINT, MID_EP_CTOR);
    if (j_EndPoint == NULL) {
	printf("NewObject failed for EndPoint\n");
	return NULL;
    }
    (*env)->SetIntField(env, j_EndPoint,
			FID_EP_NETWORK_PROTOCOL, c_OCClientResponse->devAddr.adapter);

    /* OCTransportFlags */
    printf("TRANSPORT FLAGS: 0x%08X\n", c_OCClientResponse->devAddr.flags);
    (*env)->SetIntField(env, j_EndPoint,
			FID_EP_NETWORK_POLICIES, c_OCClientResponse->devAddr.flags >> 4);
    (*env)->SetIntField(env, j_EndPoint,
			FID_EP_NETWORK_SCOPE, c_OCClientResponse->devAddr.flags & 0x000F);
    (*env)->SetBooleanField(env, j_EndPoint,
			    FID_EP_TRANSPORT_SECURITY, c_OCClientResponse->devAddr.flags & 0x0010);

    (*env)->SetIntField(env, j_EndPoint,
			FID_EP_PORT, c_OCClientResponse->devAddr.port);

    jstring j_addr = (*env)->NewStringUTF(env, c_OCClientResponse->devAddr.addr);
    (*env)->SetObjectField(env, j_EndPoint, FID_EP_ADDRESS, j_addr);

    (*env)->SetIntField(env, j_EndPoint, FID_EP_IFINDEX, c_OCClientResponse->devAddr.ifindex);

    /* jstring j_route = (*env)->NewStringUTF(env, c_OCClientResponse->devAddr.routeData); */
    /* (*env)->SetObjectField(env, j_EndPoint, FID_EP_ROUTE_DATA, j_route); */

    (*env)->SetObjectField(env, j_ObservationIn, FID_OBIN_REMOTE_DEVADDR, j_EndPoint);

    /* connectivity type */
    /* note: this duplicates info in the devaddr */
    printf("RESPONSE CONNECTION TYPE: 0x%08X\n",  c_OCClientResponse->connType);
    (*env)->SetIntField(env, j_ObservationIn, FID_OBIN_CONN_TYPE, c_OCClientResponse->connType);

    /* FIXME: use id_length */
    jstring j_sid = (*env)->NewStringUTF(env, (char*)c_OCClientResponse->identity.id);
    (*env)->SetObjectField(env, j_ObservationIn, FID_OBIN_REMOTE_SID, j_sid);

    (*env)->SetIntField(env, j_ObservationIn, FID_OBIN_RESULT, c_OCClientResponse->result);
    (*env)->SetIntField(env, j_ObservationIn, FID_OBIN_SERIAL, c_OCClientResponse->sequenceNumber);

    jstring j_uri = (*env)->NewStringUTF(env, c_OCClientResponse->resourceUri);
    (*env)->SetObjectField(env, j_ObservationIn, FID_OBIN_URI, j_uri);

    (*env)->SetLongField(env, j_ObservationIn,
			 FID_OBIN_OBSERVATION_HANDLE,
			 (long)(intptr_t)c_OCClientResponse->payload);

    /* set optionCount and ptr_Options in Message */
    printf("%s: OCClientResponse_to_ObservationIn EXIT\n", __FILE__);
    return j_ObservationIn;
}


/* EXTERNAL */

/*
 * Class:     openocf_CoResourceSP
 * Method:    ctorCoRSP
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_openocf_app_CoResourceSP_ctorCoRSP
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    printf("%s : %s ENTRY, %ld\n", __FILE__, __func__,  (intptr_t)THREAD_ID);
    /* tls_CoSP = this; */
    request_out_ctor();
}

/*
 * Class:     openocf_app_CoResourceSP
 * Method:    getUri
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_openocf_app_CoResourceSP_getUri(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    jstring j_uri = (*env)->NewStringUTF(env,
					 RESPONSE_IN->resourceUri);
    return  j_uri;
}

/*
 * Class:     openocf_app_CoResourceSP
 * Method:    setUri
 * Signature: (Ljava/lang/String;)Lopenocf/app/CoResourceSP;
 */
JNIEXPORT jobject JNICALL
Java_openocf_app_CoResourceSP_setUri(JNIEnv * env, jobject this, jstring j_uriPath)
{
  /* OIC_LOG_V(DEBUG, TAG, "%s: ENTRY, tid %ld\n", __func__, (intptr_t)THREAD_ID); */
  /* OIC_LOG_V(DEBUG, TAG, "%s: string: %s\n", __func__, j_uriPath); */
  if (tls_request_out) {
      tls_request_out->requestUri = (*env)->GetStringUTFChars(env, j_uriPath, 0);
  } else {
      OIC_LOG_V(DEBUG, TAG, "%s: NO TLS_REQUEST_OUT\n", __func__);
  }
  return this;
}

/* /\* */
/*  * Class:     openocf_CoResourceSP */
/*  * Method:    types */
/*  * Signature: ()Ljava/util/List; */
/*  *\/ */
/* JNIEXPORT jobject JNICALL Java_openocf_app_CoResourceSP_types */
/* (JNIEnv * env, jobject this) */
/* { */
/*     OC_UNUSED(env); */
/*     OC_UNUSED(this); */
/*     printf("%s : %s ENTRY, %ld\n", __FILE__, __func__,  (intptr_t)THREAD_ID); */
/* } */

/* /\* */
/*  * Class:     openocf_CoResourceSP */
/*  * Method:    addType */
/*  * Signature: (Ljava/lang/String;)Lorg/iochibity/CoResourceSP; */
/*  *\/ */
/* JNIEXPORT jobject JNICALL Java_openocf_app_CoResourceSP_addType */
/* (JNIEnv * env, jobject this, jstring s) */
/* { */
/*     OC_UNUSED(env); */
/*     OC_UNUSED(this); */
/*     OC_UNUSED(s); */
/*     printf("%s : %s ENTRY, %ld\n", __FILE__, __func__,  (intptr_t)THREAD_ID); */
/*     return this; */
/* } */

/* /\* */
/*  * Class:     openocf_CoResourceSP */
/*  * Method:    getInterfaces */
/*  * Signature: ()Ljava/util/List; */
/*  *\/ */
/* JNIEXPORT jobject JNICALL Java_openocf_app_CoResourceSP_getInterfaces */
/* (JNIEnv * env, jobject this) */
/* { */
/*     OC_UNUSED(env); */
/*     OC_UNUSED(this); */
/*     printf("%s : %s ENTRY, %ld\n", __FILE__, __func__,  (intptr_t)THREAD_ID); */
/* } */

/* /\* */
/*  * Class:     openocf_CoResourceSP */
/*  * Method:    addInterface */
/*  * Signature: (Ljava/lang/String;)Lorg/iochibity/CoResourceSP; */
/*  *\/ */
/* JNIEXPORT jobject JNICALL Java_openocf_app_CoResourceSP_addInterface */
/* (JNIEnv * env, jobject this, jstring s) */
/* { */
/*     OC_UNUSED(env); */
/*     OC_UNUSED(this); */
/*     OC_UNUSED(s); */
/*     printf("%s : %s ENTRY, %ld\n", __FILE__, __func__,  (intptr_t)THREAD_ID); */
/*     return this; */
/* } */

/*
 * Class:     openocf_app_CoResourceSP
 * Method:    getMethod
 * Signature: ()I
 */
JNIEXPORT jint JNICALL
Java_openocf_app_CoResourceSP_getMethod(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    if (tls_request_out) {
	return tls_request_out->method;
    } else {
	/* FIXME: pull method for request_out associated with response_in */
	if (tls_response_in) {
	    // FIXME: use tls_response_in->txnId to look up associated request record
	    return tls_response_in->response;
	} else {
	    return -1;
	}
    }
}

/*
 * Class:     openocf_app_CoResourceSP
 * Method:    setMethod
 * Signature: (I)Lopenocf/app/CoResourceSP;
 */
JNIEXPORT jobject JNICALL
Java_openocf_app_CoResourceSP_setMethod(JNIEnv * env, jobject this, jint m)
{
    OIC_LOG_V(DEBUG, TAG, "%s: ENTRY, tid %ld\n", __func__, (intptr_t)THREAD_ID);
    OC_UNUSED(env);
    OC_UNUSED(this);
    OC_UNUSED(m);
    if (tls_request_out) {
	tls_request_out->method = m;
    } else {
	/* response_in is read-only? */
    }
    return this;
}

/*
 * Class:     openocf_CoResourceSP
 * Method:    networkAdapter
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_openocf_app_CoResourceSP_networkAdapter
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
  return -1;
}

JNIEXPORT jint JNICALL Java_openocf_app_CoResourceSP_networkFlags
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    return tls_request_out->connectivityType;
}

/* **************************************************************** */
/*
 * Class:     openocf_app_CoResourceSP
 * Method:    isTransportSecure
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL
Java_openocf_app_CoResourceSP_isTransportSecure(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    return test_network_flag(CT_FLAG_SECURE);
}

/*
 * Class:     openocf_app_CoResourceSP
 * Method:    setTransportSecure
 * Signature: (Z)Lopenocf/app/CoResourceSP;
 */
JNIEXPORT jobject JNICALL
Java_openocf_app_CoResourceSP_setTransportSecure(JNIEnv * env, jobject this, jboolean torf)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    set_network_flag(CT_FLAG_SECURE, torf);
    return this;
}

/* ************************************************************ */
/*
 * Class:     openocf_app_CoResourceSP
 * Method:    isTransportUDP
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL
Java_openocf_app_CoResourceSP_isTransportUDP(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    return test_network_flag(CT_ADAPTER_IP);
}

/*
 * Class:     openocf_app_CoResourceSP
 * Method:    setTransportUDP
 * Signature: (Z)Lopenocf/app/CoResourceSP;
 */
JNIEXPORT jobject JNICALL
Java_openocf_app_CoResourceSP_setTransportUDP(JNIEnv * env, jobject this, jboolean torf)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    /* printf("%s: ENTRY, torf = %d\n", __func__, torf); */
    set_network_flag(CT_ADAPTER_IP, torf);
    return this;
}

/* ************************************************************ */
/*
 * Class:     openocf_app_CoResourceSP
 * Method:    isTransportTCP
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL
Java_openocf_app_CoResourceSP_isTransportTCP(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    return test_network_flag(CT_ADAPTER_TCP);
    /* return (bool)(tls_request_out->connectivityType & CT_ADAPTER_TCP); */
}

/*
 * Class:     openocf_app_CoResourceSP
 * Method:    setTransportTCP
 * Signature: (Z)Lopenocf/app/CoResourceSP;
 */
JNIEXPORT jobject JNICALL
Java_openocf_app_CoResourceSP_setTransportTCP(JNIEnv * env, jobject this, jboolean torf)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    set_network_flag(CT_ADAPTER_TCP, torf);
    return this;
}

/* ************************************************************ */
/*
 * Class:     openocf_app_CoResourceSP
 * Method:    isTransportGATT
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL
Java_openocf_app_CoResourceSP_isTransportGATT(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    return test_network_flag(CT_ADAPTER_GATT_BTLE);
    /* return (bool)(tls_request_out->connectivityType & CT_ADAPTER_GATT_BTLE); */
}

/*
 * Class:     openocf_app_CoResourceSP
 * Method:    setTransportGATT
 * Signature: (Z)Lopenocf/app/CoResourceSP;
 */
JNIEXPORT jobject JNICALL
Java_openocf_app_CoResourceSP_setTransportGATT(JNIEnv * env, jobject this, jboolean torf)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    set_network_flag(CT_ADAPTER_GATT_BTLE, torf);
    /* if (torf) { */
    /* 	tls_request_out->connectivityType |= CT_ADAPTER_GATT_BTLE; */
    /* } else { */
    /* 	tls_request_out->connectivityType &= ~CT_ADAPTER_GATT_BTLE; */
    /* } */
    return this;
}

/* ************************************************************ */
/*
 * Class:     openocf_app_CoResourceSP
 * Method:    isTransportRFCOMM
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_openocf_app_CoResourceSP_isTransportRFCOMM
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    return test_network_flag(CT_ADAPTER_RFCOMM_BTEDR);
    /* return (bool)(tls_request_out->connectivityType & CT_ADAPTER_RFCOMM_BTEDR); */
}

/*
 * Class:     openocf_app_CoResourceSP
 * Method:    setTransportRFCOMM
 * Signature: (Z)Lopenocf/app/CoResourceSP;
 */
JNIEXPORT jobject JNICALL
Java_openocf_app_CoResourceSP_setTransportRFCOMM(JNIEnv * env, jobject this, jboolean torf)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    set_network_flag(CT_ADAPTER_RFCOMM_BTEDR, torf);
    /* if (torf) { */
    /* 	tls_request_out->connectivityType |= CT_ADAPTER_RFCOMM_BTEDR; */
    /* } else { */
    /* 	tls_request_out->connectivityType &= ~CT_ADAPTER_RFCOMM_BTEDR; */
    /* } */
    return this;
}

/* ************************************************************ */
/*
 * Class:     openocf_app_CoResourceSP
 * Method:    isTransportNFC
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL
Java_openocf_app_CoResourceSP_isTransportNFC(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    return test_network_flag(CT_ADAPTER_NFC);
    /* return (bool)(tls_request_out->connectivityType & CT_ADAPTER_NFC); */
}

/*
 * Class:     openocf_app_CoResourceSP
 * Method:    setTransportNFC
 * Signature: (Z)Lopenocf/app/CoResourceSP;
 */
JNIEXPORT jobject JNICALL
Java_openocf_app_CoResourceSP_setTransportNFC(JNIEnv * env, jobject this, jboolean torf)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    set_network_flag(CT_ADAPTER_NFC, torf);
    /* if (torf) { */
    /* 	tls_request_out->connectivityType |= CT_ADAPTER_NFC; */
    /* } else { */
    /* 	tls_request_out->connectivityType &= ~CT_ADAPTER_NFC; */
    /* } */
    return this;
}

/* ************************************************************ */
/*
 * Class:     openocf_CoResourceSP
 * Method:    networkIsIP
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_openocf_app_CoResourceSP_networkIsIP__
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    return (bool) (test_network_flag(CT_ADAPTER_IP)
		  ||
		   test_network_flag(CT_ADAPTER_TCP));
    /* return (bool)((tls_request_out->connectivityType & CT_ADAPTER_IP) */
    /* 		  || */
    /* 		  (tls_request_out->connectivityType & CT_ADAPTER_TCP)); */
}

/*
 * Class:     openocf_CoResourceSP
 * Method:    networkIsIP
 * Signature: (Z)Lorg/iochibity/ICoResourceSP;
 */
JNIEXPORT jobject JNICALL Java_openocf_app_CoResourceSP_networkIsIP__Z
(JNIEnv * env, jobject this, jboolean torf)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    OC_UNUSED(torf);
    // FIXME: this is just a dummy, IP is implied by both
    // xx_ADAPTER_IP (i.e. UDP) and xx_ADAPTER_TCP. The implementation
    // does not currently allow selection of IP independently, only of
    // IP version.
    return this;
}


/* **************************************************************** */
/*
 * Class:     openocf_CoResourceSP
 * Method:    networkIsIPv4
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_openocf_app_CoResourceSP_networkIsIPv4__
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    return (bool)(tls_request_out->connectivityType & CT_IP_USE_V4);
}


/*
 * Class:     openocf_CoResourceSP
 * Method:    networkIsIPv4
 * Signature: (Z)Lorg/iochibity/CoResourceSP;
 */
JNIEXPORT jobject JNICALL Java_openocf_app_CoResourceSP_networkIsIPv4__Z
(JNIEnv * env, jobject this, jboolean torf)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    if (torf) {
	tls_request_out->connectivityType |= CT_IP_USE_V4;
    } else {
	tls_request_out->connectivityType &= ~CT_IP_USE_V4;
    }
    return this;
}

/* **************************************************************** */
/*
 * Class:     openocf_CoResourceSP
 * Method:    networkIsIPv6
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_openocf_app_CoResourceSP_networkIsIPv6__
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    return (bool)(tls_request_out->connectivityType & CT_IP_USE_V6);
}

/*
 * Class:     openocf_CoResourceSP
 * Method:    networkIsIPv6
 * Signature: (Z)Lorg/iochibity/CoResourceSP;
 */
JNIEXPORT jobject JNICALL Java_openocf_app_CoResourceSP_networkIsIPv6__Z
(JNIEnv * env, jobject this, jboolean torf)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    if (torf) {
	tls_request_out->connectivityType |= CT_IP_USE_V6;
    } else {
	tls_request_out->connectivityType &= ~CT_IP_USE_V6;
    }
    return this;
}

/* **************************************************************** */
/*
 * Class:     openocf_CoResourceSP
 * Method:    scopeIsInterface
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_openocf_app_CoResourceSP_scopeIsInterface__
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    return (bool)(tls_request_out->connectivityType & CT_SCOPE_INTERFACE);
}

/*
 * Class:     openocf_CoResourceSP
 * Method:    scopeIsInterface
 * Signature: (Z)Lorg/iochibity/CoResourceSP;
 */
JNIEXPORT jobject JNICALL Java_openocf_app_CoResourceSP_scopeIsInterface__Z
(JNIEnv * env, jobject this, jboolean torf)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    if (torf) {
	tls_request_out->connectivityType |= CT_SCOPE_INTERFACE;
    } else {
	tls_request_out->connectivityType &= ~CT_SCOPE_INTERFACE;
    }
    return this;
}

/* **************************************************************** */
/*
 * Class:     openocf_CoResourceSP
 * Method:    scopeIsLink
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_openocf_app_CoResourceSP_scopeIsLink__
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    return (bool)(tls_request_out->connectivityType & CT_SCOPE_LINK);
}

/*
 * Class:     openocf_CoResourceSP
 * Method:    scopeIsLink
 * Signature: (Z)Lorg/iochibity/CoResourceSP;
 */
JNIEXPORT jobject JNICALL Java_openocf_app_CoResourceSP_scopeIsLink__Z
(JNIEnv * env, jobject this, jboolean torf)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    if (torf) {
	tls_request_out->connectivityType |= CT_SCOPE_LINK;
    } else {
	tls_request_out->connectivityType &= ~CT_SCOPE_LINK;
    }
    return this;
}

/* **************************************************************** */
/*
 * Class:     openocf_CoResourceSP
 * Method:    scopeIsRealm
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_openocf_app_CoResourceSP_scopeIsRealm__
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    return (bool)(tls_request_out->connectivityType & CT_SCOPE_REALM);
}

/*
 * Class:     openocf_CoResourceSP
 * Method:    scopeIsRealm
 * Signature: (Z)Lorg/iochibity/CoResourceSP;
 */
JNIEXPORT jobject JNICALL Java_openocf_app_CoResourceSP_scopeIsRealm__Z
(JNIEnv * env, jobject this, jboolean torf)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    if (torf) {
	tls_request_out->connectivityType |= CT_SCOPE_REALM;
    } else {
	tls_request_out->connectivityType &= ~CT_SCOPE_REALM;
    }
    return this;
}

/* **************************************************************** */
/*
 * Class:     openocf_CoResourceSP
 * Method:    scopeIsAdmin
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_openocf_app_CoResourceSP_scopeIsAdmin__
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    return (bool)(tls_request_out->connectivityType & CT_SCOPE_ADMIN);
}

/*
 * Class:     openocf_CoResourceSP
 * Method:    scopeIsAdmin
 * Signature: (Z)Lorg/iochibity/CoResourceSP;
 */
JNIEXPORT jobject JNICALL Java_openocf_app_CoResourceSP_scopeIsAdmin__Z
(JNIEnv * env, jobject this, jboolean torf)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    if (torf) {
	tls_request_out->connectivityType |= CT_SCOPE_ADMIN;
    } else {
	tls_request_out->connectivityType &= ~CT_SCOPE_ADMIN;
    }
    return this;
}

/* **************************************************************** */
/*
 * Class:     openocf_CoResourceSP
 * Method:    scopeIsSite
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_openocf_app_CoResourceSP_scopeIsSite__
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    return (bool)(tls_request_out->connectivityType & CT_SCOPE_SITE);
}

/*
 * Class:     openocf_CoResourceSP
 * Method:    scopeIsSite
 * Signature: (Z)Lorg/iochibity/CoResourceSP;
 */
JNIEXPORT jobject JNICALL Java_openocf_app_CoResourceSP_scopeIsSite__Z
(JNIEnv * env, jobject this, jboolean torf)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    if (torf) {
	tls_request_out->connectivityType |= CT_SCOPE_SITE;
    } else {
	tls_request_out->connectivityType &= ~CT_SCOPE_SITE;
    }
    return this;
}

/* **************************************************************** */
/*
 * Class:     openocf_CoResourceSP
 * Method:    scopeIsOrg
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_openocf_app_CoResourceSP_scopeIsOrg__
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    return (bool)(tls_request_out->connectivityType & CT_SCOPE_ORG);
}

/*
 * Class:     openocf_CoResourceSP
 * Method:    scopeIsOrg
 * Signature: (Z)Lorg/iochibity/CoResourceSP;
 */
JNIEXPORT jobject JNICALL Java_openocf_app_CoResourceSP_scopeIsOrg__Z
(JNIEnv * env, jobject this, jboolean torf)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    if (torf) {
	tls_request_out->connectivityType |= CT_SCOPE_ORG;
    } else {
	tls_request_out->connectivityType &= ~CT_SCOPE_ORG;
    }
    return this;
}

/* **************************************************************** */
/*
 * Class:     openocf_CoResourceSP
 * Method:    scopeIsGlobal
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_openocf_app_CoResourceSP_scopeIsGlobal__
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    return (bool)(tls_request_out->connectivityType & CT_SCOPE_GLOBAL);
}

/*
 * Class:     openocf_CoResourceSP
 * Method:    scopeIsGlobal
 * Signature: (Z)Lorg/iochibity/CoResourceSP;
 */
JNIEXPORT jobject JNICALL Java_openocf_app_CoResourceSP_scopeIsGlobal__Z
(JNIEnv * env, jobject this, jboolean torf)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    if (torf) {
	tls_request_out->connectivityType |= CT_SCOPE_GLOBAL;
    } else {
	tls_request_out->connectivityType &= ~CT_SCOPE_GLOBAL;
    }
    return this;
}

/* **************************************************************** */
/*
 * Class:     openocf_CoResourceSP
 * Method:    routingIsMulticast
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_openocf_app_CoResourceSP_routingIsMulticast__
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    if (tls_request_out->destination) {
	return tls_request_out->destination->flags & OC_MULTICAST;
    } else {
	return tls_request_out->routingIsMulticast;
    }
}

/*
 * Class:     openocf_CoResourceSP
 * Method:    routingIsMulticast
 * Signature: (Z)Lorg/iochibity/CoResourceSP;
 */
JNIEXPORT jobject JNICALL Java_openocf_app_CoResourceSP_routingIsMulticast__Z
(JNIEnv * env, jobject this, jboolean torf)
{
    OC_UNUSED(env);
    /* printf("%s: ENTRY, tid %ld\n", __func__, (intptr_t)THREAD_ID); */
    if (tls_request_out->destination) {
	if (torf) {
	    tls_request_out->destination->flags
		= tls_request_out->destination->flags | OC_MULTICAST;
	} else {
	    tls_request_out->destination->flags &= ~OC_MULTICAST;
	}
    }
    tls_request_out->routingIsMulticast = torf;
    return this;
}
/*
 * Class:     openocf_CoResourceSP
 * Method:    port
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_openocf_app_CoResourceSP_port
  (JNIEnv *, jobject);

/*
 * Class:     openocf_app_CoResourceSP
 * Method:    getIPAddress
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_openocf_app_CoResourceSP_getIPAddress
  (JNIEnv *, jobject);

/*
 * Class:     openocf_CoResourceSP
 * Method:    qualityOfService
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_openocf_app_CoResourceSP_qualityOfService__
  (JNIEnv *, jobject);

/*
 * Class:     openocf_CoResourceSP
 * Method:    qualityOfService
 * Signature: (I)Lorg/iochibity/CoResourceSP;
 */
JNIEXPORT jobject JNICALL Java_openocf_app_CoResourceSP_qualityOfService__I
  (JNIEnv *, jobject, jint);

/*
 * Class:     openocf_CoResourceSP
 * Method:    coAddress
 * Signature: ()Lorg/iochibity/IPAddress;
 */
JNIEXPORT jobject JNICALL Java_openocf_app_CoResourceSP_coAddress
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
  /* printf("%s ENTRY, thread %ld\n", __func__, (intptr_t)THREAD_ID); */
  /* WARNING: we cannot set a devaddr field in our CoSP object, due to
     threading concerns but we can create one and return it on the call
     stack, which is thread safe. */
  /* The methods of the returned EndPoint object will pull info
     from the TLS OCClientResponse var */
  if (tls_response_in) {
    jobject j_CoAddress = NULL;
    j_CoAddress =  (*env)->NewObject(env, K_ENDPOINT, MID_EP_CTOR);
    if (j_CoAddress == NULL) {
      THROW_JNI_EXCEPTION("NewObject failed for K_ENDPOINT");
      return NULL;
    } else {
      return j_CoAddress;
    }
  } else {
    return NULL;
  }
}

/*
 * Class:     openocf_CoResourceSP
 * Method:    getCoSecurityId
 * Signature: ()[B
 */
JNIEXPORT jbyteArray JNICALL Java_openocf_app_CoResourceSP_getCoSecurityId
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    /* printf("%s ENTRY\n", __func__); */

    int size = RESPONSE_IN->identity.id_length;
    jbyte* buffer[size];
    memcpy(buffer, (char*) RESPONSE_IN->identity.id, size);
    jbyteArray ret = (*env)->NewByteArray(env, size);
    (*env)->SetByteArrayRegion(env, ret, 0, size, (jbyte*)buffer);
    return ret;
}

/*
 * Class:     openocf_CoResourceSP
 * Method:    getCoResult
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_openocf_app_CoResourceSP_getCoResult
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
  /* printf("%s ENTRY\n", __func__); */
  return tls_response_in->response->result;
}

/*
 * Class:     openocf_CoResourceSP
 * Method:    getNotificationSerial
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_openocf_app_CoResourceSP_getNotificationSerial
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    if (tls_response_in) {
	return RESPONSE_IN->sequenceNumber;
    } else {
	return -1;
    }
}

/*
 * Class:     openocf_CoResourceSP
 * Method:    deactivate
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_openocf_app_CoResourceSP_deactivate
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    tls_deactivate = true;
}
