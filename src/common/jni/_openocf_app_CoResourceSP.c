/* This is part of the Service Programming Interface (SPI), which
   provides services to the lower-level OpenOCF stack. The service
   points in the SPI are only accessed from below; they are never
   accessed by application code. */

#include "_openocf_app_CoResourceSP.h"

#include <jni.h>
#include "_threads.h"

THREAD_LOCAL txn_t* tls_txn;

/**
 * @brief Implementation of `OCClientResponseHandler` function type (occlientcb.c);
 *typedef OCStackApplicationResult (* OCClientResponseHandler)(void *context,
 *							      OCDoHandle handle, // GAR: misnamed, s/b txn_id
 * 						              OCClientResponse * clientResponse);
 *
 * called by stack on receipt of incoming `OCClientResponse` from
 * server.  Fatal exceptions must thus exit the whole thing. (?)
 *
 * Stores the incoming `OCClientResponse` and `OCDoHandle` to
 * thread-local storage, then calls the `coReact` method of the
 * transaction's `CoResourceSP` object for handling.
 *
 * The `CoResourceSP` object is conveyed by the c_CoRSP parameter
 * (in the C API, void* context), which was passed (as the "context"
 * pointer of the `OCCallbackData` param of `OCDoResource`) by the
 * `coExhibit` routine of the `OCFClientSP`.
 *
 * IMPORTANT: this is an internal implementation routine, with no
 * header prototoype.  It serves as an intermediary between the stack
 * the the Java application's callback method.
 *
 *  @param [in] jni global c_CoRSP handle to `CoResourceSP` (client) object
 *  containing callback method; `context` in C API. must be freed with DeleteGlobalRef
 *
 * @param [in] c_TransactionHandle token identifying originating
 * `OCDoResource` invocation (not the same as the CoAP token)
 *
 * @param [in] c_OCClientResponse response data from server
 *
 * @return result code directive to retain or delete the CoRSP
 *
 * @see Java_openocf_app_CoResourceSP_coExhibit
 * @see Java_openocf_ServiceProvider_exhibit
 */
// was: c_CoResourceSP_coReact
OCStackApplicationResult _openocf_app_CoResourceSP_coReact(void* c_CoRSP,
							   OCDoHandle c_TxnId,
							   OCClientResponse* c_OCClientResponse)
{
    // FIXME: switch ERROR back to DEBUG
    OIC_LOG_V(DEBUG, __FILE__, "%s ENTRY, %ld", __func__, (intptr_t)THREAD_ID);
    OIC_LOG_V(DEBUG, __FILE__, "%s txn id:", __func__);
    OIC_LOG_BUFFER(DEBUG, __FILE__, (const uint8_t *) c_TxnId, CA_MAX_TOKEN_LEN);

    OIC_LOG_V(DEBUG, __FILE__, "%s c_CoRSP: %p", __func__, c_CoRSP);

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
		OIC_LOG_V(DEBUG, __FILE__, "%s: found txn key", __func__);
		break;
	    } else {
	    	if (tls_txn->next) {
	    	    tls_txn = tls_txn->next;
	    	} else {
		    OIC_LOG_V(ERROR, __FILE__, "%s: txn key not found!", __func__);
		    tls_txn = NULL;
		    break;
	    	}
	    }
	}
    } else {
	OIC_LOG_V(ERROR, __FILE__, "%s: missing transaction list!", __func__);
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
	printf("GetEnv: not attached; attaching now\n");
	/* if ((*g_JVM)->AttachCurrentThreadAsDaemon(g_JVM, (void **) &env, NULL) != 0) { */
	if ((*g_JVM)->AttachCurrentThread(g_JVM, (void **) &env, NULL) != 0) {
	    printf("FATAL %s %d (%s): AttachCurrentThread failure\n", __FILE__, __LINE__,__func__);
	    exit(1);
	    /* OIC_LOG_V(FATAL, __FILE__, "ERROR %s %d (%s): AttachCurrentThread failure\n", */
	    /* 	      __FILE__, __LINE__,__func__); */
	    /* return OC_STACK_DELETE_TRANSACTION; */
	}
    } else if (getEnvStat == JNI_OK) {
	printf("GetEnv: attached\n");
    } else if (getEnvStat == JNI_EVERSION) {
	printf("FATAL %s %d (%s): JNI version not supported\n", __FILE__, __LINE__,__func__);
	exit(1);
	/* return OC_STACK_DELETE_TRANSACTION; */
    }
    if ((*env)->ExceptionCheck(env)) {
	(*env)->ExceptionDescribe(env);
    }

    /* if ctx param is null something went wrong */
    if (c_CoRSP == NULL) {
	/* printf("ERROR %s %d (%s): ctx param is NULL for c_CoResourceSP_coReact\n", */
	/*        __FILE__, __LINE__,__func__); */
	OIC_LOG_V(ERROR, __FILE__, "ERROR %s (line %d): ctx param is NULL for c_CoResourceSP_coReact",
		  __func__,__LINE__);
	return OC_STACK_DELETE_TRANSACTION;
    }

    /* Process the observation before passing control to user-defined coReact method. */

    /* FIXME: client is responsible for saving msg */

    switch(c_OCClientResponse->payload->type) {
    case PAYLOAD_TYPE_DISCOVERY:
	OIC_LOG_V(DEBUG, __FILE__, "[%d] %s: PAYLOAD_TYPE_DISCOVERY", __LINE__, __func__);

	/* register_resources(c_OCClientResponse); */

	/* printf("VERIFICATION:\n"); */
	/* OIC_LOG_DISCOVERY_RESPONSE(DEBUG, __FILE__, g_OCClientResponse); */
	break;
    case PAYLOAD_TYPE_DEVICE:
	OIC_LOG_V(DEBUG, __FILE__, "%s: PAYLOAD_TYPE_DEVICE", __func__);
	/* register_device(c_OCClientResponse); */
	break;
    case PAYLOAD_TYPE_PLATFORM:
	OIC_LOG_V(DEBUG, __FILE__, "%s: PAYLOAD_TYPE_PLATFORM", __func__);
	/* register_platform(c_OCClientResponse); */
	break;
    case PAYLOAD_TYPE_REPRESENTATION:
	OIC_LOG_V(DEBUG, __FILE__, "%s: PAYLOAD_TYPE_REPRESENTATION", __func__);
	break;
    case PAYLOAD_TYPE_SECURITY:
	OIC_LOG_V(DEBUG, __FILE__, "%s: PAYLOAD_TYPE_SECURITY", __func__);
	break;
    case PAYLOAD_TYPE_PRESENCE:
	OIC_LOG_V(DEBUG, __FILE__, "%s: PAYLOAD_TYPE_PRESENCE", __func__);
	break;
    /* case PAYLOAD_TYPE_RD: */
    /* 	OIC_LOG_V(DEBUG, __FILE__, "%s: PAYLOAD_TYPE_RD", __func__); */
    /* 	break; */
	/*GAR: v 1.3 types: */
    case PAYLOAD_TYPE_DIAGNOSTIC:
	OIC_LOG_V(DEBUG, __FILE__, "%s: PAYLOAD_TYPE_DIAGNOSTIC", __func__);
	break;
    case PAYLOAD_TYPE_INTROSPECTION:
	OIC_LOG_V(DEBUG, __FILE__, "%s: PAYLOAD_TYPE_INTROSPECTION", __func__);
	break;
    case PAYLOAD_TYPE_INVALID:
	OIC_LOG_V(DEBUG, __FILE__, "%s: PAYLOAD_TYPE_INVALID", __func__);
	break;
    default:
	break;
    }

    // To call the Java-side handler for incoming response data, we
    // need to get a reference to it.

    /* 1. Extract ref to coSP class from the CoRSP callback param (object) */
    jclass k_cosp = (*env)->GetObjectClass(env, (jobject)c_CoRSP);
    if((*env)->ExceptionOccurred(env)) { return OC_STACK_DELETE_TRANSACTION; }
    if (k_cosp == NULL) {
	OIC_LOG_V(ERROR, __FILE__, "[%d] %s: GetObjectClass failed for CoResourceSP object",
	       __LINE__,__func__);
    	return OC_STACK_DELETE_TRANSACTION;
    }

    /* 2. Pull the coReact method ID from the coSP class */
    jmethodID mid_cosp_coReact = (*env)->GetMethodID(env, k_cosp, "coReact", "()I");
    if (mid_cosp_coReact == NULL) {
	OIC_LOG_V(ERROR, __FILE__, "%s (line %d): GetMethodID failed for mid_cosp_coReact of CoResourceSP",
		  __func__, __LINE__);
    	return OC_STACK_DELETE_TRANSACTION;
    }

    /* 3. call user's coReact method of the CoResourceSP */
    OIC_LOG_V(INFO, __FILE__, "[%d] %s: Calling user coReact method", __LINE__, __func__);
    int coReact_result;
    coReact_result = (*env)->CallIntMethod(env, (jobject)c_CoRSP, mid_cosp_coReact);

    if((*env)->ExceptionOccurred(env)) {
	OIC_LOG_V(ERROR, __FILE__, "[%d] %s: CAUGHT EXCEPTION thrown by user coReact method",
		  __LINE__, __func__);
	return OC_STACK_DELETE_TRANSACTION;
    }

    /* we're done with the JVM */
    (*g_JVM)->DetachCurrentThread(g_JVM);

    /* if (tls_deactivate) { */
    /* 	/\* free the global cosp that we pinned in coExhibit *\/ */
    /* 	(*env)->DeleteGlobalRef(env, (jobject)c_CoRSP); */
    /* 	OIC_LOG(DEBUG, __FILE__, "c_CoResourceSP_react EXIT, deactivating handler"); */
    /* 	return OC_STACK_DELETE_TRANSACTION; */
    /* } else { */
	if (tls_txn->routingIsMulticast) {
	    OIC_LOG_V(DEBUG, __FILE__, "[%d] %s EXITing multicast handler", __LINE__, __func__);
	    return coReact_result;
	    /* return (OC_STACK_KEEP_TRANSACTION); */
	    /* return (OC_STACK_KEEP_TRANSACTION | OC_STACK_KEEP_PAYLOAD); */
	    /* return (OC_STACK_DELETE_TRANSACTION | OC_STACK_KEEP_PAYLOAD); */
	} else {
	    /* free the global cosp that we pinned in coExhibit */
	    (*env)->DeleteGlobalRef(env, (jobject)c_CoRSP);
	    OIC_LOG_V(DEBUG, __FILE__, "[%d] %s EXITing unicast handler",
		    __LINE__, __func__);
	    /* always remove unicast CB */
	    return (coReact_result & OC_STACK_DELETE_TRANSACTION);
	}
    /* } */
}
