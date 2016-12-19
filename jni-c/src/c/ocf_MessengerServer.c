#include <ctype.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "org_iochibity_Messenger.h"
#include "ocf_init.h"
#include "ocf_exceptions.h"
#include "jni_utils.h"

#include "octypes.h"
#include "ocresource.h"
#include "ocstack.h"
#include "oic_malloc.h"
#include "oic_string.h"

/* PRIVATE */

/* FIXME: this is called from the runtime stack, not java - deal with exceptions */
/* THROW_JNI_EXCEPTION won't work since we're not called from Java */
jobject OCEntityHandlerRequest_to_MsgRequestIn(JNIEnv* env, OCEntityHandlerRequest* crequest_in)
{
    printf("OCEntityHandlerRequest_to_MsgRequestIn ENTRY\n");

    jobject j_MsgRequestIn = (*env)->NewObject(env, K_MSG_REQUEST_IN, MID_RQI_CTOR); // request_in_ctor);

    /* set fields in MsgForServiceProvider ancestor */
    (*env)->SetLongField(env, j_MsgRequestIn,
			 FID_MSG_LOCAL_HANDLE, (intptr_t)crequest_in);
    (*env)->SetLongField(env, j_MsgRequestIn,
			 FID_MFSP_REMOTE_RQST_HANDLE, (intptr_t)crequest_in->requestHandle);
    (*env)->SetLongField(env, j_MsgRequestIn,
			 FID_MFSP_RESOURCE_HANDLE, (intptr_t)crequest_in->resource);

    /* set fields in Message ancestor */
    /* OCDevAddr */
    jobject jdevice_address = (*env)->NewObject(env, K_DEVICE_ADDRESS, MID_DA_CTOR);
    (*env)->SetIntField(env, jdevice_address, FID_DA_ADAPTER, crequest_in->devAddr.adapter);
    (*env)->SetIntField(env, jdevice_address, FID_DA_FLAGS, crequest_in->devAddr.flags);
    (*env)->SetIntField(env, jdevice_address, FID_DA_PORT, crequest_in->devAddr.port);

    printf("Request-In Device Address address: %s\n", crequest_in->devAddr.addr);
    jstring js = (*env)->NewStringUTF(env, crequest_in->devAddr.addr);
    (*env)->SetObjectField(env, jdevice_address, FID_DA_ADDRESS, js);
    (*env)->SetIntField(env, jdevice_address, FID_DA_IFINDEX, crequest_in->devAddr.ifindex);

    (*env)->SetObjectField(env, j_MsgRequestIn, FID_MSG_REMOTE_DEVADDR, jdevice_address);

    /* method */
    printf("XXXXXXXXXXXXXXXX RQI METHOD: %d\n", crequest_in->method);
    (*env)->SetIntField(env, j_MsgRequestIn, FID_MSG_METHOD, crequest_in->method);

    /* set fields in MsgRequestIn */
    js = (*env)->NewStringUTF(env, crequest_in->query);
    (*env)->SetObjectField(env, j_MsgRequestIn, FID_RQI_QUERY, js);

    /* FIXME: deal with OCEntityHandlerRequest.obsInfo */
    /* we encode this info as methods WATCH, UNWATCH */

    /* FIXME: deal with header options: */
    /* numRcvdVendorSpecificHeaderOptions */
    /* rcvdVendorSpecificHeaderOptions; */

    /* message ID */
    (*env)->SetIntField(env, j_MsgRequestIn, FID_RQI_MSG_ID, crequest_in->messageID);

    (*env)->SetLongField(env, j_MsgRequestIn, FID_MSG_PAYLOAD_HANDLE, (intptr_t)crequest_in->payload);

    printf("OCEntityHandlerRequest_to_MsgRequestIn EXIT\n");
    return j_MsgRequestIn;
}

/* typedef OCEntityHandlerResult (*OCEntityHandler) */
/* (OCEntityHandlerFlag flag, OCEntityHandlerRequest * entityHandlerRequest, void* callbackParam); */
OCEntityHandlerResult service_request_in(OCEntityHandlerFlag flag,
					 OCEntityHandlerRequest * c_OCEntityHandlerRequest, /* MsgRequestIn */
					 void* j_IServiceProvider)
{
    OC_UNUSED(flag);
    printf("\nocf_api_server.c/service_request_in ENTRY\n");
    /* printf("REQUEST URI: %s\n", ((OCResource*)(c_OCEntityHandlerRequest->resource))->uri); */

    /* printf("request resource properties: 0x%X\n", */
    /* 	   ((OCResource*)(c_OCEntityHandlerRequest->resource))->resourceProperties); */

    /* 1. set up jvm, env */
    /* http://stackoverflow.com/questions/12900695/how-to-obtain-jni-interface-pointer-jnienv-for-asynchronous-calls */
    /* http://adamish.com/blog/archives/327 */
    JNIEnv * env;
    // double check it's all ok
    int getEnvStat = (*g_JVM)->GetEnv(g_JVM, (void **)&env, JNI_VERSION_1_6);
    if (getEnvStat == JNI_EDETACHED) {
	/* printf("GetEnv: not attached; attaching now\n"); */
	if ((*g_JVM)->AttachCurrentThread(g_JVM, (void **) &env, NULL) != 0) {
	    printf("ERROR %s %d (%s): AttachCurrentThread failure\n", __FILE__, __LINE__,__func__);
	    return OC_EH_INTERNAL_SERVER_ERROR;
	}
    } else if (getEnvStat == JNI_OK) {
	/* printf("GetEnv: attached\n"); */
    } else if (getEnvStat == JNI_EVERSION) {
	printf("ERROR %s %d (%s): JNI version not supported\n", __FILE__, __LINE__,__func__);
	return OC_EH_INTERNAL_SERVER_ERROR;
    }
    if ((*env)->ExceptionCheck(env)) {
	(*env)->ExceptionDescribe(env);
    }

    if (j_IServiceProvider == NULL) {
	/* FIXME: use proper logging */
	printf("ERROR %s %d (%s): j_IServiceProvider is NULL\n", __FILE__, __LINE__,__func__);
	return OC_EH_INTERNAL_SERVER_ERROR;
    }

    jobject j_MsgRequestIn = NULL;
    j_MsgRequestIn = OCEntityHandlerRequest_to_MsgRequestIn(env, c_OCEntityHandlerRequest);
    fflush(NULL);
    if (j_MsgRequestIn == NULL) {
        printf("ERROR:  OCEntityHandlerRequest_to_MsgRequestIn failed\n");
    	return OC_EH_INTERNAL_SERVER_ERROR;
    }

    /* now invoke the callback on the Java side */
    int op_result = OC_EH_OK;
    op_result = (*env)->CallIntMethod(env, j_IServiceProvider,
				      MID_ISP_SERVICE_REQUEST_IN,
				      j_MsgRequestIn);
    if (op_result != OC_STACK_OK) {
        printf("ERROR:  CallIntMethod failed for ResourceServiceProvider.serviceRequestIn\n");
    	return OC_EH_INTERNAL_SERVER_ERROR;
    }

    (*g_JVM)->DetachCurrentThread(g_JVM);

    /* printf("Incoming request: %ld\n", (long)c_OCEntityHandlerRequest); */
    /* printf("Incoming requestHandle: %ld\n", (long)c_OCEntityHandlerRequest->requestHandle); */
    /* printf("Incoming request param: %ld\n", (long)j_IServiceProvider); */
    /* printf("Incoming request flag: %d\n", flag); */

    printf("ocf_resource_manager.c/service_routine EXIT\n");
    return op_result;
}

/* PUBLIC */

/*
 * Class:     org_iochibity_Messenger
 * Method:    sendResponse
 * Signature: (Lorg/iochibity/MsgResponseOut;)V
 */
JNIEXPORT void JNICALL Java_org_iochibity_Messenger_sendResponse
(JNIEnv * env, jclass klass, jobject j_response_out)
{
    OC_UNUSED(klass);
    printf("Java_org_iochibity_Messenger_sendResponse ENTRY\n");

    /* prep_java(env); */

    /* 1. alloc the OCEntityHandlerResponse struct */
    OCEntityHandlerResponse * c_response_out =
                        (OCEntityHandlerResponse *) OICCalloc(1, sizeof(*c_response_out));
    if (!c_response_out) {
	THROW_EH_EXCEPTION(OC_EH_ERROR, "OCEntityHandlerResponse calloc failure\n");
	return;
    }

    /* 2. get handles from j_response_out._requestIn */
    jclass k_response_out = (*env)->GetObjectClass(env, j_response_out);
    if (k_response_out == NULL) {
	THROW_JNI_EXCEPTION("GetObjectClass failed for ResponseOut object\n");
	return;
    }
    /* get the RequestIn object */
    jfieldID fid_rqst_in = (*env)->GetFieldID(env, k_response_out, "_requestIn", "Lorg/iochibity/MsgRequestIn;");
    if (fid_rqst_in == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for '_requestIn' on ResponseOut\n");
	return;
    }
    jobject j_request_in = (*env)->GetObjectField(env, j_response_out, fid_rqst_in);
    if (j_request_in == NULL) {
	THROW_JNI_EXCEPTION("GetObjectField failed for fid_rqst_in on j_response_out\n");
	return;
    }
    /* now we have the original RequestIn object; let's pull the handles from it */

    /* first we need the RequestIn class so we can get the c struct handle */
    jclass k_request_in = (*env)->GetObjectClass(env, j_request_in);
    if (k_request_in == NULL) {
	THROW_JNI_EXCEPTION("GetObjectClass failed for RequestIn object\n");
	return;
    }

    /* get the localHandle */
    /* jfieldID fid_local_handle = (*env)->GetFieldID(env, k_request_in, "localHandle", "J"); */
    /* if (fid_local_handle == NULL) { */
    /* 	THROW_JNI_EXCEPTION("GetFieldID failed for 'localHandle' on RequestIn\n"); */
    /* 	return; */
    /* } */
    OCEntityHandlerRequest* j_handle = (OCEntityHandlerRequest*)(intptr_t)
	(*env)->GetLongField(env, j_request_in, FID_MSG_LOCAL_HANDLE);
    if (j_handle == NULL) {
	THROW_JNI_EXCEPTION("GetObjectField failed for fid_remote_handle on j_request_in\n");
	return;
    }
    printf("RESPONSE localHandle: %ld\n", (long)j_handle);
    printf("RESPONSE remoteHandle: %ld\n", (long)j_handle->requestHandle);
    c_response_out->requestHandle = j_handle->requestHandle;

    /* ditto for resourceHandle */
    printf("RESPONSE resourceHandle: %ld\n", (long)j_handle->resource);
    c_response_out->resourceHandle = j_handle->resource;

    /* defaults */
    c_response_out->numSendVendorSpecificHeaderOptions = 0;

    memset(c_response_out->sendVendorSpecificHeaderOptions, 0,
	   sizeof c_response_out->sendVendorSpecificHeaderOptions);
    memset(c_response_out->resourceUri, 0, sizeof(c_response_out->resourceUri));

    c_response_out->persistentBufferFlag = 0;

    /* PayloadList */
    jfieldID fid_pll = (*env)->GetFieldID(env, k_response_out,
					  "_payloadList", "Lorg/iochibity/PayloadList;");
    if (fid_pll == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for '_payloadList' on ResponseOut\n");
	return;
    }
    jobject j_pll = (*env)->GetObjectField(env, j_response_out, fid_pll);
    if (j_pll != NULL) {
	jclass k_pll = (*env)->GetObjectClass(env, j_pll);
	if (k_pll == NULL) {
	    THROW_JNI_EXCEPTION("GetObjectClass failed for PayloadList object\n");
	    return;
	}
	/* we're going to iterate using jni, dunno about efficiency */
	jmethodID mid_pll_size = (*env)->GetMethodID(env, k_pll, "size", "()I");
	if (mid_pll_size == NULL) {
	    THROW_JNI_EXCEPTION("GetMethodID failed for 'size' of PayloadList\n");
	    return;
	}
	int sz = 0;
	sz = (*env)->CallIntMethod(env, j_pll, mid_pll_size);
	printf("PayloadList SIZE: %d\n", sz);

	jmethodID mid_pll_get = (*env)->GetMethodID(env, k_pll, "get", "(I)Ljava/lang/Object;");
	if (mid_pll_get == NULL) {
	    THROW_JNI_EXCEPTION("GetMethodID failed for 'get' of PayloadList\n");
	    return;
	}

	/* since PayloadList contains Payload objects */
	K_PAYLOAD = (*env)->FindClass(env, "Lorg/iochibity/Payload;");
	if (K_PAYLOAD == NULL) {
	    THROW_JNI_EXCEPTION("FindClass failed for org/iochibity/Payload\n");
	}
	FID_PAYLOAD_HANDLE = (*env)->GetFieldID(env, K_PAYLOAD, "_handle", "J");
	if (FID_PAYLOAD_HANDLE == NULL) {
	    THROW_JNI_EXCEPTION("GetFieldID failed for '_handle' of Payload\n");
	    return;
	}
	FID_PAYLOAD_TYPE = (*env)->GetFieldID(env, K_PAYLOAD, "type", "I");
	if (FID_PAYLOAD_TYPE == NULL) {
	    THROW_JNI_EXCEPTION("GetFieldID failed for 'type' on Payload\n");
	    return;
	}

	/* pfrs = PayloadForResourceState */
	/* prep_pfrs(env); /\* get all the field and method ids etc. needed for iteration *\/ */

	jobject j_payload = NULL;
	int c_type;
	OCPayload* c_payload = NULL;
	for (int i=0; i<sz; i++) {
	    j_payload = (*env)->CallObjectMethod(env, j_pll, mid_pll_get, i);
	    c_type = (*env)->GetIntField(env, j_payload, FID_PAYLOAD_TYPE);
	    printf("PAYLOAD TYPE: %d\n", c_type);

	    /* the OCPayload structs are already there in the _handle fields */
	    /* but we need to synch them with the Java objects */
	    switch(c_type) {
	    case PAYLOAD_TYPE_INVALID:
		printf("INVALID PAYLOAD TYPE NOT SUPPORTED\n");
		break;
	    case PAYLOAD_TYPE_DISCOVERY:
		printf("DISCOVERY PAYLOAD NOT YET SUPPORTED\n");
		break;
	    case PAYLOAD_TYPE_DEVICE:
		printf("DEVICE PAYLOAD NOT YET SUPPORTED\n");
		break;
	    case PAYLOAD_TYPE_PLATFORM:
		printf("PLATFORM PAYLOAD NOT YET SUPPORTED\n");
		break;
	    case PAYLOAD_TYPE_REPRESENTATION:
		c_payload = (OCPayload*)pfrs_to_OCRepPayload(env, j_payload);
		break;
	    case PAYLOAD_TYPE_SECURITY:
		printf("SECURITY PAYLOAD NOT YET SUPPORTED\n");
		break;
	    case PAYLOAD_TYPE_PRESENCE:
		printf("PRESENCE PAYLOAD NOT YET SUPPORTED\n");
		break;
		/* obsolete as of 1.2.1: */
		/* case PAYLOAD_TYPE_RD: */
		/*     printf("RD PAYLOAD NOT YET SUPPORTED\n"); */
		/*     break; */
	    default:
		printf("UNKNOWN PAYLOAD - NOT SUPPORTED\n");
		break;
	    }
	}
	c_response_out->payload = (OCPayload*) c_payload;
    } else {
	/* OK: use left the payload null */
	/* THROW_JNI_EXCEPTION("GetObjectField failed for fid_pll on j_response_out\n"); */
	/* return; */
    }

    /* set OCEntityHandlerResult from this._processingResult */

    /* vendor header options */

    /* payload: iterate over the PayloadList to create OCPayload linked list */


    /* finally, send it off: */
    OCStackResult op_result = OC_STACK_OK;
    op_result = OCDoResponse(c_response_out);
    if (op_result != OC_STACK_OK) {
	THROW_STACK_EXCEPTION(op_result, "OCDoResponse failed");
	/* ehResult = OC_EH_ERROR; */
	return;
    }

    printf("Java_org_iochibity_Messenger_sendResponse EXIT\n");
}
