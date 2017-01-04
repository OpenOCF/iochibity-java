/**
 * @file org_iochibity_CoServiceProvider.c
 * @author Gregg Reynolds
 * @date December 2016
 *
 * @brief JNI implementation of CoServiceProvider (client) Java API:
 * `Java_org_iochibity_CoServiceProvider_coExhibit` and callback
 * `c_CoServiceProvider_coReact`
 */

#include <string.h>
/* #include <ctype.h> */
/* #include <stdlib.h> */

/* http://www.informit.com/articles/article.aspx?p=2036582&seqNum=5
OpenBSD:  strlcpy
C11:      strncpy_s
*/

#include "_threads.h"

#include "org_iochibity_CoServiceProvider.h"
#include "jni_utils.h"
#include "org_iochibity_Init.h"
#include "org_iochibity_Exceptions.h"

#include "octypes.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "ocpayload.h"
#include "ocresource.h"
#include "ocresourcehandler.h"
#include "ocstack.h"

#include "logger.h"

/* logging */
#define TAG  "CoSP"

/* externs */
// FIXME: put these in init.c?
THREAD_LOCAL response_in_t* tls_response_in	= NULL;

THREAD_LOCAL jobject tls_CoSP			= NULL;


/* _Thread_local OCDevAddr* tls_defaultCoAddress		= NULL; */

response_in_t*  g_response_map;

/* PRIVATE */

OCClientResponse* g_OCClientResponse;

/* tls struct capturing params to OCDoResource */
typedef struct RequestOut
{
    OCDoHandle           txnId;  	/* key; will be returned in response_in */
    /* val: */
    OCMethod             method;
    const char         * requestUri;
    OCDevAddr          * destination;
    OCPayload          * payload;
    OCConnectivityType   connectivityType;
    OCQualityOfService   qos;
    OCCallbackData     * cbData;
    OCHeaderOption     * options;
    uint8_t              numOptions;

    bool                 routingIsMulticast;

    struct RequestOut  * next;
} request_out_t;

THREAD_LOCAL request_out_t* tls_request_out = NULL;

typedef struct transaction
{
    OCDoHandle /* void* */ txnId;	/* key; will be returned in response_in */
    OCMethod               method;
    bool                   routingIsMulticast;
    struct transaction   * next;
} txn_t;

txn_t* g_txn_list = NULL;

THREAD_LOCAL txn_t* tls_txn;

THREAD_LOCAL bool   tls_deactivate;

void request_out_ctor()
{
  tls_request_out                   = OICCalloc(sizeof (request_out_t), 1);

  tls_request_out->txnId	     = NULL;
  tls_request_out->method	     = OC_REST_NOMETHOD;
  /* never pass NULL for destination */
  tls_request_out->destination	     = (OCDevAddr *)OICCalloc(sizeof (OCDevAddr), 1);
  tls_request_out->connectivityType = CT_DEFAULT; /* 0 */
  tls_request_out->qos		     = OC_LOW_QOS;
  tls_request_out->cbData	     = NULL;
  tls_request_out->options	     = NULL;
  tls_request_out->numOptions	     = 0;

  tls_request_out->next	     = NULL;
}

/**
 * @brief Convert OCClientResponse to an `ObservationIn` object
 *
 * Allocate an `ObservationIn` java object, then use data from incoming
 * `OCClientResponse` to initialize it.  Return the initialize object.
 * Called internally by `c_CoServiceProvider_coReact`.
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
    jobject j_DeviceAddress = (*env)->NewObject(env, K_DEVICE_ADDRESS, MID_DA_CTOR);
    if (j_DeviceAddress == NULL) {
	printf("NewObject failed for DeviceAddress\n");
	return NULL;
    }
    (*env)->SetIntField(env, j_DeviceAddress,
			FID_DA_NETWORK_PROTOCOL, c_OCClientResponse->devAddr.adapter);

    /* OCTransportFlags */
    printf("TRANSPORT FLAGS: 0x%08X\n", c_OCClientResponse->devAddr.flags);
    (*env)->SetIntField(env, j_DeviceAddress,
			FID_DA_NETWORK_POLICIES, c_OCClientResponse->devAddr.flags >> 4);
    (*env)->SetIntField(env, j_DeviceAddress,
			FID_DA_NETWORK_SCOPE, c_OCClientResponse->devAddr.flags & 0x000F);
    (*env)->SetBooleanField(env, j_DeviceAddress,
			    FID_DA_TRANSPORT_SECURITY, c_OCClientResponse->devAddr.flags & 0x0010);

    (*env)->SetIntField(env, j_DeviceAddress,
			FID_DA_PORT, c_OCClientResponse->devAddr.port);

    jstring j_addr = (*env)->NewStringUTF(env, c_OCClientResponse->devAddr.addr);
    (*env)->SetObjectField(env, j_DeviceAddress, FID_DA_ADDRESS, j_addr);

    (*env)->SetIntField(env, j_DeviceAddress, FID_DA_IFINDEX, c_OCClientResponse->devAddr.ifindex);

    /* jstring j_route = (*env)->NewStringUTF(env, c_OCClientResponse->devAddr.routeData); */
    /* (*env)->SetObjectField(env, j_DeviceAddress, FID_DA_ROUTE_DATA, j_route); */

    (*env)->SetObjectField(env, j_ObservationIn, FID_OBIN_REMOTE_DEVADDR, j_DeviceAddress);

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

/**
 *
 */
void handle_discovery_payload(OCClientResponse* c_OCClientResponse)
{
    OIC_LOG_V(DEBUG, TAG, "%s:%d ENTRY", __func__, __LINE__);

    OIC_LOG_V(DEBUG, TAG, "resource count: %ld\n",
	      OCDiscoveryPayloadGetResourceCount((OCDiscoveryPayload*)c_OCClientResponse->payload));

    OIC_LOG_DISCOVERY_RESPONSE(DEBUG, TAG, c_OCClientResponse);

    /* Incoming response record is allocated on the stack of the
       calling routine, so it will be removed after we return.  To
       keep it around we need to make a deep copy. */
    g_OCClientResponse = (OCClientResponse *)OICCalloc(sizeof (OCClientResponse), 1);
    memcpy(g_OCClientResponse, c_OCClientResponse, sizeof (OCClientResponse));

    g_OCClientResponse->addr = &(g_OCClientResponse->devAddr);
    /* copy the uripath string */
    /* g_OCClientResponse->resourceUri = */
    g_OCClientResponse->resourceUri = OICStrdup(c_OCClientResponse->resourceUri);

    /* copy payload; NB: fromDP is a linked list */
    OCDiscoveryPayload  *fromDP = (OCDiscoveryPayload*)c_OCClientResponse->payload;
    OCDiscoveryPayload **toDP   = (OCDiscoveryPayload**) &(g_OCClientResponse->payload);

    OCStringLL *fromSLL, **toSLL; // , *toSLLHead;
    while(fromDP) {
	if (fromDP->base.type == PAYLOAD_TYPE_DISCOVERY) {
	    /* 1. create new OCDiscoveryPayload */
	    (*toDP)                    = OCDiscoveryPayloadCreate();

	    /* 1a. copy fromDP sid, baseURI, name, uri strings */
	    (*toDP)->sid               = OICStrdup(fromDP->sid);
	    (*toDP)->baseURI           = OICStrdup(fromDP->baseURI);
	    (*toDP)->name              = OICStrdup(fromDP->name);
	    (*toDP)->uri               = OICStrdup(fromDP->uri);

	    /* 1b. copy fromDP types and interfaces */
	    /* types */
	    fromSLL                    = fromDP->type;
	    toSLL                      = &((*toDP)->type);
	    while(fromSLL) {
		(*toSLL)               = CloneOCStringLL(fromSLL);
		toSLL                  = &((*toSLL)->next);
		fromSLL                = fromSLL->next;
	    }
	    /* interfaces */
	    fromSLL                    = fromDP->iface;
	    toSLL                      = &((*toDP)->iface);
	    while(fromSLL) {
		(*toSLL)               = CloneOCStringLL(fromSLL);
		toSLL                  = &((*toSLL)->next);
		fromSLL                = fromSLL->next;
	    }

	    /* resources (i.e. OCResourcePayload list) */
	    OCResourcePayload*  fromRP = fromDP->resources;
	    OCResourcePayload **toRP   = &((*toDP)->resources);
	    while(fromRP) {
		(*toRP)                = (OCResourcePayload*)OICCalloc(sizeof (OCResourcePayload), 1);
		(*toRP)->uri           = OICStrdup(fromRP->uri);
		(*toRP)->bitmap        = fromRP->bitmap;
		(*toRP)->secure        = fromRP->secure;
		(*toRP)->port          = fromRP->port;
#ifdef TCP_ADAPTER
		(*toRP)->tcpPort       = fromRP->tcpPort;
#endif

		/* types */
		fromSLL                = fromRP->types;
		toSLL                  = &((*toRP)->types);
		while(fromSLL) {
		    (*toSLL)           = CloneOCStringLL(fromSLL);
		    toSLL              = &((*toSLL)->next);
		    fromSLL            = fromSLL->next;
		}
		/* interfaces */
		fromSLL                = fromRP->interfaces;
		toSLL                  = &((*toRP)->interfaces);
		while(fromSLL) {
		    (*toSLL)           = CloneOCStringLL(fromSLL);
		    toSLL              = &((*toSLL)->next);
		    fromSLL            = fromSLL->next;
		}
		fromRP                 = fromRP->next;
		toRP                   = &((*toRP)->next);
	    }
	} else {
	    /* FIXME: should not happen */
	}
	fromDP = fromDP->next;
	toDP   = &((*toDP)->next);
    }
}


/**
 * @brief Implementation of `OCClientResponseHandler` function type;
 * called by stack on receipt of incoming `OCClientResponse` from
 * server.  Fatal exceptions must thus exit the whole thing. (?)
 *
 * Stores the incoming `OCClientResponse` and `OCDoHandle` to
 * thread-local storage, then calls the `coReact` method of the
 * transaction's `CoServiceProvider` object for handling.
 *
 * The `CoServiceProvider` object is conveyed by the c_CoSP parameter
 * (in the C API, void* context), which was passed (as the "context"
 * pointer of the `OCCallbackData` param of `OCDoResource`) by the
 * `coExhibit` routine of the `CoServiceProvider`.
 *
 * IMPORTANT: this is an internal implementation routine, with no
 * header prototoype.  It serves as an intermediary between the stack
 * the the Java application's callback method.
 *
 *  @param [in] c_CoSP handle to `CoServiceProvider` (client) object
 *  containing callback method; `context` in C API
 *
 * @param [in] c_TransactionHandle token identifying originating
 * `OCDoResource` invocation (not the same as the CoAP token)
 *
 * @param [in] c_OCClientResponse response data from server
 *
 * @return result code directive to retain or delete the CoSP
 *
 * @see Java_org_iochibity_CoServiceProvider_coExhibit
 * @see Java_org_iochibity_ServiceProvider_exhibit
 */
OCStackApplicationResult c_CoServiceProvider_coReact(void* c_CoSP,
						     OCDoHandle c_TxnId,
						     OCClientResponse* c_OCClientResponse)
{
    // FIXME: switch ERROR back to DEBUG
    OIC_LOG_V(DEBUG, TAG, "%s ENTRY, %ld", __func__, (intptr_t)THREAD_ID);

    OIC_LOG_V(DEBUG, TAG, "%s txn id:", __func__);
    OIC_LOG_BUFFER(DEBUG, TAG, (const uint8_t *) c_TxnId, CA_MAX_TOKEN_LEN);

    /* To support multi-threading, we cache the incoming
       OCClientResponse record in a TLS var: */
    tls_response_in = OICCalloc(sizeof (response_in_t), 1);
    tls_response_in->txnId    = c_TxnId;
    tls_response_in->response = c_OCClientResponse;

    /* now look up the corresponding request_out record, using c_TxnId as key. */
    if (g_txn_list) {
	tls_txn = g_txn_list;
	while (tls_txn) {
	    if ( memcmp( tls_txn->txnId, c_TxnId, CA_MAX_TOKEN_LEN ) == 0 ) {
		OIC_LOG_V(ERROR, TAG, "%s: found txn key", __func__);
		break;
	    } else {
	    	if (tls_txn->next) {
	    	    tls_txn = tls_txn->next;
	    	} else {
		    OIC_LOG_V(ERROR, TAG, "%s: txn key not found!", __func__);
		    tls_txn = NULL;
		    break;
	    	}
	    }
	}
    } else {
	OIC_LOG_V(ERROR, TAG, "%s: missing transaction list!", __func__);
    }

    /* if response is OCDiscoveryPayload (containing
       OCResourcePayloads) or OCResourceCollectionPayload (containing
       OCLinksPayloads), then database the resource records (qua remote
       SP descriptors).
     */

    /* now pass control to java app code */
    /* 1. set up: attach this thread to JVM */
    /* http://stackoverflow.com/questions/12900695/how-to-obtain-jni-interface-pointer-jnienv-for-asynchronous-calls */
    /* http://adamish.com/blog/archives/327 */
    JNIEnv * env;
    // FIXMED double check it's all ok
    int getEnvStat = (*g_JVM)->GetEnv(g_JVM, (void **)&env, JNI_VERSION_1_6);
    if (getEnvStat == JNI_EDETACHED) {
	/* printf("GetEnv: not attached; attaching now\n"); */
	/* if ((*g_JVM)->AttachCurrentThreadAsDaemon(g_JVM, (void **) &env, NULL) != 0) { */
	if ((*g_JVM)->AttachCurrentThread(g_JVM, (void **) &env, NULL) != 0) {
	    printf("FATAL %s %d (%s): AttachCurrentThread failure\n", __FILE__, __LINE__,__func__);
	    exit(1);
	    /* OIC_LOG_V(FATAL, TAG, "ERROR %s %d (%s): AttachCurrentThread failure\n", */
	    /* 	      __FILE__, __LINE__,__func__); */
	    /* return OC_STACK_DELETE_TRANSACTION; */
	}
    } else if (getEnvStat == JNI_OK) {
	/* printf("GetEnv: attached\n"); */
    } else if (getEnvStat == JNI_EVERSION) {
	printf("FATAL %s %d (%s): JNI version not supported\n", __FILE__, __LINE__,__func__);
	exit(1);
	/* return OC_STACK_DELETE_TRANSACTION; */
    }
    if ((*env)->ExceptionCheck(env)) {
	(*env)->ExceptionDescribe(env);
    }

    /* if ctx param is null something went wrong */
    if (c_CoSP == NULL) {
	/* printf("ERROR %s %d (%s): ctx param is NULL for c_CoServiceProvider_coReact\n", */
	/*        __FILE__, __LINE__,__func__); */
	OIC_LOG_V(ERROR, TAG, "ERROR %s (line %d): ctx param is NULL for c_CoServiceProvider_coReact",
		  __func__,__LINE__);
	return OC_STACK_DELETE_TRANSACTION;
    }

    /* Process the observation before passing control to user-defined coReact method. */
    switch(c_OCClientResponse->payload->type) {
    case PAYLOAD_TYPE_DISCOVERY:
	OIC_LOG_V(DEBUG, TAG, "%s: PAYLOAD_TYPE_DISCOVERY", __func__);
	handle_discovery_payload(c_OCClientResponse);
	printf("VERIFICATION:\n");
	OIC_LOG_DISCOVERY_RESPONSE(DEBUG, TAG, g_OCClientResponse);
	break;
    case PAYLOAD_TYPE_DEVICE:
	OIC_LOG_V(DEBUG, TAG, "%s: PAYLOAD_TYPE_DEVICE", __func__);
	break;
    case PAYLOAD_TYPE_PLATFORM:
	OIC_LOG_V(DEBUG, TAG, "%s: PAYLOAD_TYPE_PLATFORM", __func__);
	break;
    case PAYLOAD_TYPE_REPRESENTATION:
	OIC_LOG_V(DEBUG, TAG, "%s: PAYLOAD_TYPE_REPRESENTATION", __func__);
	break;
    case PAYLOAD_TYPE_SECURITY:
	OIC_LOG_V(DEBUG, TAG, "%s: PAYLOAD_TYPE_SECURITY", __func__);
	break;
    case PAYLOAD_TYPE_PRESENCE:
	OIC_LOG_V(DEBUG, TAG, "%s: PAYLOAD_TYPE_PRESENCE", __func__);
	break;
    case PAYLOAD_TYPE_RD:
	OIC_LOG_V(DEBUG, TAG, "%s: PAYLOAD_TYPE_RD", __func__);
	break;
    case PAYLOAD_TYPE_INVALID:
	OIC_LOG_V(DEBUG, TAG, "%s: PAYLOAD_TYPE_INVALID", __func__);
	break;
    default:
	break;
    }

    // To call the Java-side handler for incoming response data, we
    // need to get a reference to it.

    /* 1. Extract ref to coReact routine from CoSP callback param. We
       get the method from the object, not the class. */

    // FIXME: replace printf with OIC_LOG_...
    jclass k_cosp = (*env)->GetObjectClass(env, (jobject)c_CoSP);
    if((*env)->ExceptionOccurred(env)) { return OC_STACK_DELETE_TRANSACTION; }
    if (k_cosp == NULL) {
	OIC_LOG_V(ERROR, TAG, "%s (line %d): GetObjectClass failed for CoServiceProvider object",
	       __func__,__LINE__);
    	return OC_STACK_DELETE_TRANSACTION;
    }

    jmethodID mid_cosp_coReact = (*env)->GetMethodID(env, k_cosp, "coReact", "()V");
    if (mid_cosp_coReact == NULL) {
	OIC_LOG_V(ERROR, TAG, "%s (line %d): GetMethodID failed for mid_cosp_coReact of CoServiceProvider",
		  __func__, __LINE__);
    	return OC_STACK_DELETE_TRANSACTION;
    }

    /* 2. call the coReact method of the CoServiceProvider */
    (*env)->CallVoidMethod(env, (jobject)c_CoSP, mid_cosp_coReact);

    if((*env)->ExceptionOccurred(env)) {
	// FIXME: print exception message
	OIC_LOG_V(ERROR, TAG, "%s:%d CAUGHT EXCEPTION thrown by coReact method",
		  __func__, __LINE__);
	return OC_STACK_DELETE_TRANSACTION;
    }

    /* FIXME: free the global cosp that we pinned in coExhibit? */
    /* (*env)->DeleteGlobalRef(env, (jobject)c_CoSP); */

    /* we're done with the JVM */
    (*g_JVM)->DetachCurrentThread(g_JVM);

    if (tls_deactivate) {
	OIC_LOG(DEBUG, TAG, "c_CoServiceProvider_react EXIT, deactivating handler");
    	return OC_STACK_DELETE_TRANSACTION;
    }
    if (tls_txn->routingIsMulticast) {
	OIC_LOG(DEBUG, TAG, "c_CoServiceProvider_react EXIT, retaining multicast handler");
    	return OC_STACK_KEEP_TRANSACTION;
    } else {
	OIC_LOG(DEBUG, TAG, "c_CoServiceProvider_react EXIT, deactivating unicast handler");
    	return OC_STACK_DELETE_TRANSACTION;
    }
}

/* EXTERNAL */

/*
 * Class:     org_iochibity_CoServiceProvider
 * Method:    ctorCoSP
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_iochibity_CoServiceProvider_ctorCoSP
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    printf("%s : %s ENTRY, %ld\n", __FILE__, __func__,  (intptr_t)THREAD_ID);
    /* tls_CoSP = this; */
    request_out_ctor();
}

/*
 * Class:     org_iochibity_CoServiceProvider
 * Method:    uriPath
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_iochibity_CoServiceProvider_uriPath__
(JNIEnv * env, jobject this)
{
  OC_UNUSED(env);
  OC_UNUSED(this);
  jstring j_uri = (*env)->NewStringUTF(env,
				       RESPONSE_IN->resourceUri);
  return  j_uri;
}

/*
 * Class:     org_iochibity_CoServiceProvider
 * Method:    uriPath
 * Signature: (Ljava/lang/String;)Lorg/iochibity/CoServiceProvider;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_CoServiceProvider_uriPath__Ljava_lang_String_2
(JNIEnv * env, jobject this, jstring j_uriPath)
{
  /* printf("Java_org_iochibity_CoServiceProvider_setUriPath ENTRY\n"); */
  tls_request_out->requestUri = (*env)->GetStringUTFChars(env, j_uriPath, 0);
  return this;
}

/*
 * Class:     org_iochibity_CoServiceProvider
 * Method:    method
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_iochibity_CoServiceProvider_method__
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    if (tls_request_out) {
	return tls_request_out->method;
    } else {
	/* FIXME: pull method for request_out associated with response_in */
	if (tls_response_in) {
	    // FIXME: use tls_response_in->handle to get associate request record
	    return tls_response_in->response;
	} else {
	    return -1;
	}
    }
}

/*
 * Class:     org_iochibity_CoServiceProvider
 * Method:    method
 * Signature: (I)Lorg/iochibity/CoServiceProvider;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_CoServiceProvider_method__I
(JNIEnv * env, jobject this, jint m)
{
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
 * Class:     org_iochibity_CoServiceProvider
 * Method:    networkAdapter
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_iochibity_CoServiceProvider_networkAdapter
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
  return -1;
}

JNIEXPORT jint JNICALL Java_org_iochibity_CoServiceProvider_networkFlags
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    return tls_request_out->connectivityType;
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
/* **************************************************************** */
/**
 * *JNI Class*:     org_iochibity_CoServiceProvider
 *
 * *JNI Method*:    transportIsSecure
 *
 * *JNI Signature*: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_iochibity_CoServiceProvider_transportIsSecure__
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    return test_network_flag(CT_FLAG_SECURE);
}

/*
 * Class:     org_iochibity_CoServiceProvider
 * Method:    transportIsSecure
 * Signature: (Z)Lorg/iochibity/CoServiceProvider;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_CoServiceProvider_transportIsSecure__Z
(JNIEnv * env, jobject this, jboolean torf)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    set_network_flag(CT_FLAG_SECURE, torf);
    return this;
}

/* **************************************************************** */
/*
 * Class:     org_iochibity_CoServiceProvider
 * Method:    transportIsUDP
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_iochibity_CoServiceProvider_transportIsUDP__
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    return test_network_flag(CT_ADAPTER_IP);
}

/*
 * Class:     org_iochibity_CoServiceProvider
 * Method:    transportIsUDP
 * Signature: (Z)Lorg/iochibity/CoServiceProvider;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_CoServiceProvider_transportIsUDP__Z
(JNIEnv * env, jobject this, jboolean torf)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    /* printf("%s: ENTRY, torf = %d\n", __func__, torf); */
    set_network_flag(CT_ADAPTER_IP, torf);
    return this;
}

/* **************************************************************** */
/*
 * Class:     org_iochibity_CoServiceProvider
 * Method:    transportIsTCP
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_iochibity_CoServiceProvider_transportIsTCP__
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    return (bool)(tls_request_out->connectivityType & CT_ADAPTER_TCP);
}


/*
 * Class:     org_iochibity_CoServiceProvider
 * Method:    transportIsTCP
 * Signature: (Z)Lorg/iochibity/CoServiceProvider;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_CoServiceProvider_transportIsTCP__Z
(JNIEnv * env, jobject this, jboolean torf)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    if (torf) {
	tls_request_out->connectivityType |= CT_ADAPTER_TCP;
    } else {
	tls_request_out->connectivityType &= ~CT_ADAPTER_TCP;
    }
    return this;
}

/* **************************************************************** */
/*
 * Class:     org_iochibity_CoServiceProvider
 * Method:    transportIsGATT
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_iochibity_CoServiceProvider_transportIsGATT__
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    return (bool)(tls_request_out->connectivityType & CT_ADAPTER_GATT_BTLE);
}

/*
 * Class:     org_iochibity_CoServiceProvider
 * Method:    transportIsGATT
 * Signature: (Z)Lorg/iochibity/CoServiceProvider;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_CoServiceProvider_transportIsGATT__Z
(JNIEnv * env, jobject this, jboolean torf)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    if (torf) {
	tls_request_out->connectivityType |= CT_ADAPTER_GATT_BTLE;
    } else {
	tls_request_out->connectivityType &= ~CT_ADAPTER_GATT_BTLE;
    }
    return this;
}

/* **************************************************************** */
/*
 * Class:     org_iochibity_CoServiceProvider
 * Method:    transportIsRFCOMM
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_iochibity_CoServiceProvider_transportIsRFCOMM__
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    return (bool)(tls_request_out->connectivityType & CT_ADAPTER_RFCOMM_BTEDR);
}

/*
 * Class:     org_iochibity_CoServiceProvider
 * Method:    transportIsRFCOMM
 * Signature: (Z)Lorg/iochibity/CoServiceProvider;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_CoServiceProvider_transportIsRFCOMM__Z
(JNIEnv * env, jobject this, jboolean torf)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    if (torf) {
	tls_request_out->connectivityType |= CT_ADAPTER_RFCOMM_BTEDR;
    } else {
	tls_request_out->connectivityType &= ~CT_ADAPTER_RFCOMM_BTEDR;
    }
    return this;
}

/* **************************************************************** */
/*
 * Class:     org_iochibity_CoServiceProvider
 * Method:    transportIsNFC
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_iochibity_CoServiceProvider_transportIsNFC__
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    return (bool)(tls_request_out->connectivityType & CT_ADAPTER_NFC);
}

/*
 * Class:     org_iochibity_CoServiceProvider
 * Method:    transportIsNFC
 * Signature: (Z)Lorg/iochibity/CoServiceProvider;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_CoServiceProvider_transportIsNFC__Z
(JNIEnv * env, jobject this, jboolean torf)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    if (torf) {
	tls_request_out->connectivityType |= CT_ADAPTER_NFC;
    } else {
	tls_request_out->connectivityType &= ~CT_ADAPTER_NFC;
    }
    return this;
}

/* **************************************************************** */
/*
 * Class:     org_iochibity_CoServiceProvider
 * Method:    networkIsIP
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_iochibity_CoServiceProvider_networkIsIP__
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    return (bool)((tls_request_out->connectivityType & CT_ADAPTER_IP)
		  ||
		  (tls_request_out->connectivityType & CT_ADAPTER_TCP));
}

/*
 * Class:     org_iochibity_CoServiceProvider
 * Method:    networkIsIP
 * Signature: (Z)Lorg/iochibity/ICoServiceProvider;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_CoServiceProvider_networkIsIP__Z
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
 * Class:     org_iochibity_CoServiceProvider
 * Method:    networkIsIPv4
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_iochibity_CoServiceProvider_networkIsIPv4__
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    return (bool)(tls_request_out->connectivityType & CT_IP_USE_V4);
}


/*
 * Class:     org_iochibity_CoServiceProvider
 * Method:    networkIsIPv4
 * Signature: (Z)Lorg/iochibity/CoServiceProvider;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_CoServiceProvider_networkIsIPv4__Z
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
 * Class:     org_iochibity_CoServiceProvider
 * Method:    networkIsIPv6
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_iochibity_CoServiceProvider_networkIsIPv6__
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    return (bool)(tls_request_out->connectivityType & CT_IP_USE_V6);
}

/*
 * Class:     org_iochibity_CoServiceProvider
 * Method:    networkIsIPv6
 * Signature: (Z)Lorg/iochibity/CoServiceProvider;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_CoServiceProvider_networkIsIPv6__Z
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
 * Class:     org_iochibity_CoServiceProvider
 * Method:    scopeIsInterface
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_iochibity_CoServiceProvider_scopeIsInterface__
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    return (bool)(tls_request_out->connectivityType & CT_SCOPE_INTERFACE);
}

/*
 * Class:     org_iochibity_CoServiceProvider
 * Method:    scopeIsInterface
 * Signature: (Z)Lorg/iochibity/CoServiceProvider;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_CoServiceProvider_scopeIsInterface__Z
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
 * Class:     org_iochibity_CoServiceProvider
 * Method:    scopeIsLink
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_iochibity_CoServiceProvider_scopeIsLink__
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    return (bool)(tls_request_out->connectivityType & CT_SCOPE_LINK);
}

/*
 * Class:     org_iochibity_CoServiceProvider
 * Method:    scopeIsLink
 * Signature: (Z)Lorg/iochibity/CoServiceProvider;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_CoServiceProvider_scopeIsLink__Z
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
 * Class:     org_iochibity_CoServiceProvider
 * Method:    scopeIsRealm
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_iochibity_CoServiceProvider_scopeIsRealm__
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    return (bool)(tls_request_out->connectivityType & CT_SCOPE_REALM);
}

/*
 * Class:     org_iochibity_CoServiceProvider
 * Method:    scopeIsRealm
 * Signature: (Z)Lorg/iochibity/CoServiceProvider;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_CoServiceProvider_scopeIsRealm__Z
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
 * Class:     org_iochibity_CoServiceProvider
 * Method:    scopeIsAdmin
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_iochibity_CoServiceProvider_scopeIsAdmin__
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    return (bool)(tls_request_out->connectivityType & CT_SCOPE_ADMIN);
}

/*
 * Class:     org_iochibity_CoServiceProvider
 * Method:    scopeIsAdmin
 * Signature: (Z)Lorg/iochibity/CoServiceProvider;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_CoServiceProvider_scopeIsAdmin__Z
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
 * Class:     org_iochibity_CoServiceProvider
 * Method:    scopeIsSite
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_iochibity_CoServiceProvider_scopeIsSite__
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    return (bool)(tls_request_out->connectivityType & CT_SCOPE_SITE);
}

/*
 * Class:     org_iochibity_CoServiceProvider
 * Method:    scopeIsSite
 * Signature: (Z)Lorg/iochibity/CoServiceProvider;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_CoServiceProvider_scopeIsSite__Z
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
 * Class:     org_iochibity_CoServiceProvider
 * Method:    scopeIsOrg
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_iochibity_CoServiceProvider_scopeIsOrg__
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    return (bool)(tls_request_out->connectivityType & CT_SCOPE_ORG);
}

/*
 * Class:     org_iochibity_CoServiceProvider
 * Method:    scopeIsOrg
 * Signature: (Z)Lorg/iochibity/CoServiceProvider;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_CoServiceProvider_scopeIsOrg__Z
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
 * Class:     org_iochibity_CoServiceProvider
 * Method:    scopeIsGlobal
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_iochibity_CoServiceProvider_scopeIsGlobal__
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    return (bool)(tls_request_out->connectivityType & CT_SCOPE_GLOBAL);
}

/*
 * Class:     org_iochibity_CoServiceProvider
 * Method:    scopeIsGlobal
 * Signature: (Z)Lorg/iochibity/CoServiceProvider;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_CoServiceProvider_scopeIsGlobal__Z
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
 * Class:     org_iochibity_CoServiceProvider
 * Method:    routingIsMulticast
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_iochibity_CoServiceProvider_routingIsMulticast__
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
 * Class:     org_iochibity_CoServiceProvider
 * Method:    routingIsMulticast
 * Signature: (Z)Lorg/iochibity/CoServiceProvider;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_CoServiceProvider_routingIsMulticast__Z
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
 * Class:     org_iochibity_CoServiceProvider
 * Method:    port
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_iochibity_CoServiceProvider_port
  (JNIEnv *, jobject);

/*
 * Class:     org_iochibity_CoServiceProvider
 * Method:    ipAddress
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_iochibity_CoServiceProvider_ipAddress
  (JNIEnv *, jobject);

/*
 * Class:     org_iochibity_CoServiceProvider
 * Method:    qualityOfService
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_iochibity_CoServiceProvider_qualityOfService__
  (JNIEnv *, jobject);

/*
 * Class:     org_iochibity_CoServiceProvider
 * Method:    qualityOfService
 * Signature: (I)Lorg/iochibity/CoServiceProvider;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_CoServiceProvider_qualityOfService__I
  (JNIEnv *, jobject, jint);

/*
 * Class:     org_iochibity_CoServiceProvider
 * Method:    coAddress
 * Signature: ()Lorg/iochibity/DeviceAddress;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_CoServiceProvider_coAddress
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
  /* printf("%s ENTRY, thread %ld\n", __func__, (intptr_t)THREAD_ID); */
  /* WARNING: we cannot set a devaddr field in our CoSP object, due to
     threading concerns but we can create on and return it on the call
     stack, which is thread safe. */
  /* The methods of the returned DeviceAddress object will pull info
     from the TLS OCClientResponse var */
  if (tls_response_in) {
    jobject j_CoAddress = NULL;
    j_CoAddress =  (*env)->NewObject(env, K_DEVICE_ADDRESS, MID_DA_CTOR);
    if (j_CoAddress == NULL) {
      THROW_JNI_EXCEPTION("NewObject failed for K_DEVICE_ADDRESS");
      return NULL;
    } else {
      return j_CoAddress;
    }
  } else {
    return NULL;
  }
}

/*
 * Class:     org_iochibity_CoServiceProvider
 * Method:    getCoSecurityId
 * Signature: ()[B
 */
JNIEXPORT jbyteArray JNICALL Java_org_iochibity_CoServiceProvider_getCoSecurityId
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
 * Class:     org_iochibity_CoServiceProvider
 * Method:    getCoResult
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_iochibity_CoServiceProvider_getCoResult
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
  /* printf("%s ENTRY\n", __func__); */
  return tls_response_in->response->result;
}

/*
 * Class:     org_iochibity_CoServiceProvider
 * Method:    getNotificationSerial
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_iochibity_CoServiceProvider_getNotificationSerial
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
 * Class:     org_iochibity_CoServiceProvider
 * Method:    coExhibit
 * Signature: ()V
 */
/**
 * @brief Wrapper for `OCDoResource`; called by called by the user to
 * send a request to a server.
 *
 * It wraps `OCDoResource`.  The parameters to `OCDoResource` are
 * reified in a hidden request_out_t struct.
 *
 * @param [in] env JNIEnv pointer
 *
 * @param [in] this_CoSP reference to the calling `CoServiceProvider`
 * instance
 *
 * @return void
 *
 * @throws org.iochibity.exceptions.JNIRuntimeException Indicates a
 * bad result from a JNI API routine (e.g. `GetIntField`)
 *
 * @see Java_org_iochibity_ServiceProvider_exhibit
 * @see c_CoServiceProvider_coReact
 */
JNIEXPORT void JNICALL Java_org_iochibity_CoServiceProvider_coExhibit
(JNIEnv * env, jobject this_CoSP)
{
    OC_UNUSED(env);
    OIC_LOG_V(DEBUG, TAG, "%s: ENTRY, tid %ld\n", __func__, (intptr_t)THREAD_ID);

    /* We will pass this_CoSP as the callbackParam to OCDoResource.
       It will be passed back on a different thread, so after we call
       OCDoResource, we will store our thread-local request_out struct
       in the global map, so it can later be retrieved for the
       matching response.  We already have our thread-local
       request_out - created by the ctor, possibly updated by app code
       - so we can use it directly to parameterize OCDoReource.  Note
       that request_out is an internal, implementation struct; the API
       only presents CoSP getters and setters, which use request_out
       as needed.
    */

    /* in order to be able to reliably call-back our react method, we
       must pin the object */
    tls_CoSP = (*env)->NewGlobalRef(env, this_CoSP);

    OCDoHandle c_TxnId = NULL;
    txn_t* txn;

    OCStackResult ret;
    OCCallbackData cbData;

    /* does this request follow receipt of a response? */
    /* FIXME: this logic is semi-broken.  tls_response_in will be on a
       react thread, we're on an coExhibit thread, which may be
       different. */

    if (tls_response_in == NULL) {
	/* NULL response does not necessarily mean we're starting from
	   scratch!  it just means we're not on a react thread.  but we
	   could be (co-)reacting to a response that was recd on another
	   thread. */

	/* FIXME: use case: we're using a predefined address */
	printf("tls_response_in is NULL\n");
	printf("tls_request_out %ld\n", (intptr_t)tls_request_out);
	printf("tls_request_out->destination %ld\n", (intptr_t)tls_request_out->destination );
	if (tls_request_out->routingIsMulticast) {
	    /* if (tls_request_out->destination->flags & OC_MULTICAST) { */
		/* if (tls_request_out->method == OC_REST_GET) { */
		tls_request_out->method = OC_REST_DISCOVER;
		/* this is broken, but it's the way the C API works: */
		tls_request_out->destination = NULL;
	    /* } */
	}
	cbData.cb = c_CoServiceProvider_coReact;
	cbData.context = (void*)(long)this_CoSP;
	cbData.cd = NULL;
	ret = OCDoResource(&c_TxnId,	/* OCDoHandle = void* */
			   tls_request_out->method,      // (OCMethod)c_method,
			   tls_request_out->requestUri,  // c_uri,
			   tls_request_out->destination, // c_destDevAddr,
			   tls_request_out->payload,     // OCPayload* payload
			   tls_request_out->connectivityType,
			   /* CT_ADAPTER_IP */
			   /* CT_FLAG_SECURE,	 /\* OCConnectivityType conn_type *\/ */
			   OC_LOW_QOS,
			   &cbData,	/* OCCallbackData* cbData */
			   NULL,	/* OCHeaderOptions* options */
			   0);	/* uint8_t numOptions */

	/* first: update the TLS request data with the newly coined txnId token */
	tls_request_out->txnId = c_TxnId;

	/* now copy tls request_out to the global list, so that we can
	 * correlate with response_in in the react routine. we cannot
	 * just refer to the TLS data since it will have gone away
	 * if/when the thread died. */
	/* why not just use the global list directly, no tls? because we
	   don't have a key (txnId) until we call OCDoResource. */
	txn                         = OICCalloc(sizeof (txn_t), 1);
	txn->txnId                  = tls_request_out->txnId;
	txn->method                 = tls_request_out->method;
	if (tls_request_out->destination) {
	    txn->routingIsMulticast = tls_request_out->destination->flags & OC_MULTICAST;
	} else {
	    txn->routingIsMulticast =tls_request_out->routingIsMulticast;
	}
	txn->next                   = NULL;

	/* FIXME: do we really need anything other than method?
	   everything else will be available in response. */

	/* size_t length; /\* this will hold the length of string copied *\/ */
	/* char buffer[MAX_URI_LENGTH]; /\* fast mem on stack *\/ */
	/* length			= strlcpy(buffer, */
	/* 				  tls_request_out->requestUri, */
	/* 				  sizeof(buffer)); */
	/* if (length < sizeof(buffer)) { */
	/*     rqst->requestUri	= buffer;	/\* it fit, use the stack buffer *\/ */
	/* } else { */
	/*     THROW_JNI_EXCEPTION("URI too long!"); */
	/* } */

	/* // FIXME: verify the ptr stuff is heap-allocated */
	/* rqst->destination = tls_request_out->destination; */
	/* rqst->payload = tls_request_out->payload; */
	/* rqst->connectivityType = tls_request_out->connectivityType; */
	/* rqst->qos = tls_request_out->qos; */
	/* rqst->cbData = tls_request_out->cbData; */
	/* rqst->options = tls_request_out->options; */
	/* rqst->numOptions = tls_request_out->numOptions; */
    } else {

	printf("tls_response_in is NOT null\n");
	/* we have an OCClientResponse - this CoSP is interacting with an SP */
	/* that probably - but not necessarily - means that we're
	   sending a request following a discovery? */

      /* But we're on an coExhibit thread; we have a tls_request_out,
       * but the args we need for OCDoResource are on a react thread,
       * in tls_response_in.  so if we have a tls_response_in, that
       * must mean that we are in fact on the react thread - maybe we
       * discovered a resource and are now trying to read it.  in that
       * case what we're doing now may be part of our reaction; but
       * since reactions are triggered by requests, not responses, we
       * should call this a co-response, since requests are not
       * transactionally paired with previous responses.
       */

	/* FIXME: a little doco and terminology will help: if a client
	   wants to react to a server's response, it can only do so by
	   starting a new interaction, which has no protocol-defined
	   relation to the previous one.  So we can call this a
	   co-reaction; the CoSP's co-reaction to a response is dual
	   to the SP's reaction to a request.  Maybe we should even
	   call the CoSP's response handler "coReact" instead of
	   "react" to highlight the difference.  Reaction will always
	   result in a response that is paired with the request,
	   whereas CoReaction never results in a response, although it
	   may involve new transactions.
	*/

      /* FIXME: pull method from the dual react thread */
      /* TEMPORARY: pull from java object: */
      /* OCMethod c_method = (OCMethod)(*env)->GetIntField(env, */
      /* 							this_CoSP, */
      /* 							FID_COSP_METHOD); */
      /* if (c_method == 0) { */
      /* 	THROW_JNI_EXCEPTION("Method OC_REST_NOMETHOD (0) not allowed"); */
      /* 	return; */
      /* } */
      /* uri */
      /* For thread safety, we must use the Uri from the response */
      /* char* c_uri tls_response_in->resourceUri, NULL); */

      /* if (c_uri == NULL) { */
      /* 	THROW_JNI_EXCEPTION("No resourceUri in tls_response_in\n"); */
      /* 	return; */
      /* } */

      /* ADDRESSING */
      /* MULTICAST: dev addr is null, connectivity type is used */
      /* UNICAST:   dev addr is used, connectivity type is ignored? */

      OCDevAddr* c_destDevAddr = NULL;
      /* jobject j_destDevAddr = NULL; */
      /* j_destDevAddr = (*env)->GetObjectField(env, tls_CoSP, FID_COSP_DESTINATION); */

      printf("UNICASTING\n");
      /* we have an OCDevAddr from a response */
      c_destDevAddr = &(tls_response_in->response->devAddr);

      cbData.cb = c_CoServiceProvider_coReact;
      cbData.context = (void*)(long)tls_CoSP;
      cbData.cd = NULL;
      /* ret = OCDoResource(&c_TxnId,	/\* OCDoHandle = void* *\/ */
      /* 			 (OCMethod)c_method, */
      /* 			 c_uri, */
      /* 			 c_destDevAddr, */
      /* 			 NULL,		 /\* OCPayload* payload *\/ */
      /* 			 CT_DEFAULT, */
      /* 			 /\* CT_ADAPTER_IP *\/ */
      /* 			 /\* CT_FLAG_SECURE,	 /\\* OCConnectivityType conn_type *\\/ *\/ */
      /* 			 OC_LOW_QOS, */
      /* 			 &cbData,	/\* OCCallbackData* cbData *\/ */
      /* 			 NULL,	/\* OCHeaderOptions* options *\/ */
      /* 			 0);	/\* uint8_t numOptions *\/ */

      /* now update the handle field */
      /* FIXME: thread safety */
      /* (*env)->SetLongField(env, tls_CoSP, FID_COSP_HANDLE, (intptr_t)c_TxnId); */
    }

    OIC_LOG_V(DEBUG, TAG, "%s txn id:", __func__);
    OIC_LOG_BUFFER(DEBUG, TAG, (const uint8_t *) c_TxnId, CA_MAX_TOKEN_LEN);

    /* FIXME: synch access to global g_txn_list */
    if (g_txn_list) {
	txn_t*  t = g_txn_list;
	while (t) {
	    if (t->next) {
		t = t->next;
	    } else {
		break;
	    }
	}
	t->next = txn;
    } else {
	g_txn_list = txn;
    }

    OIC_LOG_V(DEBUG, TAG, "%s: EXIT", __func__);
    OIC_LOG(DEBUG, TAG, "");
    return;
}

/*
 * Class:     org_iochibity_CoServiceProvider
 * Method:    deactivate
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_iochibity_CoServiceProvider_deactivate
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    tls_deactivate = true;
}
