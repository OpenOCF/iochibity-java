#include <ctype.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "org_iochibity_OCF.h"
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
jobject OCEntityHandlerRequest_to_DocRequestIn(JNIEnv* env, OCEntityHandlerRequest* crequest_in)
{
    /* printf("OCEntityHandlerRequest_to_DocRequestIn ENTRY\n"); */

    jobject j_DocRequestIn = (*env)->NewObject(env, K_DOC_REQUEST_IN, MID_DRQI_CTOR); // request_in_ctor);

    /*FIXME: verify class of parms */
    jclass k_request_in = (*env)->GetObjectClass(env, j_DocRequestIn);
    if (k_request_in == NULL) {
	printf("ERROR:  GetObjectClass failed for DocRequestIn\n");
	fflush(NULL);
    	return NULL;
    }

    /* first, store pointer to the incoming request */
    jfieldID field = (*env)->GetFieldID(env, K_DOC_REQUEST_IN, "localHandle", "J");
    if (field == NULL) {
	printf("ERROR:  GetFieldID failed for localHandle\n");
	fflush(NULL);
    	return NULL;
    }
    (*env)->SetLongField(env, j_DocRequestIn, field, (intptr_t)crequest_in);

    field = (*env)->GetFieldID(env, K_DOC_REQUEST_IN, "remoteHandle", "J");
    if (field == NULL) {
	printf("ERROR:  GetFieldID failed for remoteHandle\n");
	fflush(NULL);
    	return NULL;
    } else {
	printf("c request remote handle: %ld\n", (long)crequest_in->requestHandle);
	(*env)->SetLongField(env, j_DocRequestIn, field, (intptr_t)crequest_in->requestHandle);
    }
    /* then do the rest */
    field = (*env)->GetFieldID(env, K_DOC_REQUEST_IN, "resourceHandle", "J");
    if (field == NULL) {
	printf("ERROR:  GetFieldID failed for resourceHandle\n");
	fflush(NULL);
    	return NULL;
    } else {
	/* printf("OCResourceHandle (in c): %ld\n", (long)crequest_in->resource); */
	(*env)->SetLongField(env, j_DocRequestIn, field, (intptr_t)crequest_in->resource);
    }

    /* method */
    field = (*env)->GetFieldID(env, K_DOC_REQUEST_IN, "method", "I");
    if (field == NULL) {
	printf("ERROR:  GetFieldID failed for 'method' fld\n");
	fflush(NULL);
    	return NULL;

    } else {
	/* printf("method in c: %d\n", crequest_in->method); */
	(*env)->SetIntField(env, j_DocRequestIn, field, crequest_in->method);
    }

    /* OCDevAddr */
    /* jfieldID dev_addr_field = (*env)->GetFieldID(env, K_DOC_REQUEST_IN, */
    /* 						 "deviceAddress", */
    /* 						 "Lorg/iochibity/DeviceAddress;"); */
    /* if (dev_addr_field == NULL) { */
    /* 	printf("ERROR:  GetFieldID failed for deviceAddress\n"); */
    /* 	fflush(NULL); */
    /* 	return NULL; */
    /* } */

    /* construct a new DeviceAddress object to insert into DocRequestIn obj */
    /* jclass k_devaddr = (*env)->FindClass(env, "org/iochibity/DeviceAddress"); */
    /* if (k_devaddr == NULL) { */
    /* 	printf("ERROR:  FindClass failed for org/iochibity/DeviceAddress\n"); */
    /* 	fflush(NULL); */
    /* 	return NULL; */
    /* } */
    /* jmethodID dev_addr_ctor = (*env)->GetMethodID(env, K_DEVICE_ADDRESS, "<init>", "()V"); */
    /* if (dev_addr_ctor == NULL) { */
    /* 	printf("ERROR:  GetFieldID failed for ctor of DeviceAddress\n"); */
    /* 	fflush(NULL); */
    /* 	return NULL; */
    /* } */

    /* jobject jdevice_address = (*env)->NewObject(env, K_DEVICE_ADDRESS, dev_addr_ctor); */
    jobject jdevice_address = (*env)->NewObject(env, K_DEVICE_ADDRESS, MID_DRQI_CTOR);

    /* OCDevAddr.adapter */
    /* field = (*env)->GetFieldID(env, K_DEVICE_ADDRESS, "adapter", "I"); */
    /* if (field == NULL) { */
    /* 	printf("ERROR:  GetFieldID failed for adapter\n"); */
    /* 	fflush(NULL); */
    /* 	return NULL; */
    /* } else { */

    /* printf("c adapter: %d\n", crequest_in->devAddr.adapter); */
    (*env)->SetIntField(env, jdevice_address, FID_DA_ADAPTER, crequest_in->devAddr.adapter);

    /* } */

    /* OCDevAddr.flags */
    /* field = (*env)->GetFieldID(env, K_DEVICE_ADDRESS, "flags", "I"); */
    /* if (field == NULL) { */
    /* 	printf("ERROR:  GetFieldID failed for 'flags' fld\n"); */
    /* 	fflush(NULL); */
    /* 	return NULL; */
    /* } else { */

	/* printf("c flags: 0x%X\n", crequest_in->devAddr.flags); */
    (*env)->SetIntField(env, jdevice_address, FID_DA_FLAGS, crequest_in->devAddr.flags);
    /* } */

    /* OCDevAddr.port */
    /* field = (*env)->GetFieldID(env, K_DEVICE_ADDRESS, "port", "I"); */
    /* if (field == NULL) { */
    /* 	printf("ERROR:  GetFieldID failed for port fld\n"); */
    /* 	fflush(NULL); */
    /* 	return NULL; */
    /* } else { */

    /* printf("c port: %d\n", crequest_in->devAddr.port); */
    (*env)->SetIntField(env, jdevice_address, field, crequest_in->devAddr.port);

    /* } */

    /* OCDevAddr.addr */
    /* field = (*env)->GetFieldID(env, K_DEVICE_ADDRESS, "address", "Ljava/lang/String;"); */
    /* if (field == NULL) { */
    /* 	printf("ERROR:  GetFieldID failed for address fld\n"); */
    /* 	fflush(NULL); */
    /* 	return NULL; */
    /* } else { */

    /* printf("c address: %s\n", crequest_in->devAddr.addr); */
    jstring js = (*env)->NewStringUTF(env, crequest_in->devAddr.addr);
    (*env)->SetObjectField(env, jdevice_address, field, js);
    /* } */

    /* OCDevAddr.ifindex */
    /* field = (*env)->GetFieldID(env, K_DEVICE_ADDRESS, "ifindex", "I"); */
    /* if (field == NULL) { */
    /* 	printf("ERROR:  GetFieldID failed for ifindex fld\n"); */
    /* 	fflush(NULL); */
    /* 	return NULL; */
    /* } else { */

    /* printf("c ifindex: %d\n", crequest_in->devAddr.ifindex); */
    (*env)->SetIntField(env, jdevice_address, field, crequest_in->devAddr.ifindex);
    /* } */

    /* now set deviceAddress field in DocRequestIn object */
    /* (*env)->SetObjectField(env, j_DocRequestIn, dev_addr_field, jdevice_address); */
    (*env)->SetObjectField(env, j_DocRequestIn, FID_DRQI_DEVICE_ADDRESS, jdevice_address);

    /* query */
    field = (*env)->GetFieldID(env, K_DOC_REQUEST_IN, "query", "Ljava/lang/String;");
    if (field == NULL) { /* make sure we got the field */
	printf("ERROR:  GetFieldID failed for query fld\n");
	fflush(NULL);
    	return NULL;
    } else {
	/* printf("query in c: '%s'\n", crequest_in->query); */
	jstring js = (*env)->NewStringUTF(env, crequest_in->query);
	(*env)->SetObjectField(env, j_DocRequestIn, field, js);
    }

    /* observation info */
    field = (*env)->GetFieldID(env, K_DOC_REQUEST_IN, "observeAction", "I");
    if (field == NULL) {
	printf("ERROR:  GetFieldID failed for observeAction fld\n");
	fflush(NULL);
    	return NULL;
    } else {
	/* printf("c observeAction: %d\n", crequest_in->obsInfo.action); */
	(*env)->SetIntField(env, j_DocRequestIn, field, crequest_in->obsInfo.action);
    }
    field = (*env)->GetFieldID(env, K_DOC_REQUEST_IN, "observeId", "I");
    if (field == NULL) {
	printf("ERROR:  GetFieldID failed for observeId fld\n");
	fflush(NULL);
    	return NULL;
    } else {
	/* printf("c observation id: %d\n", crequest_in->obsInfo.obsId); */
	(*env)->SetIntField(env, j_DocRequestIn, field, crequest_in->obsInfo.obsId);
    }

    /* vendor header options - implemented as getter method */
    field = (*env)->GetFieldID(env, K_DOC_REQUEST_IN, "vendorHeaderOptionCount", "I");
    if (field == NULL) {
	printf("ERROR:  GetFieldID failed for vendorHeaderOptionCount fld\n");
	fflush(NULL);
    	return NULL;
    } else {
	/* printf("c nbf header options: %d\n", crequest_in->numRcvdVendorSpecificHeaderOptions); */
	(*env)->SetIntField(env, j_DocRequestIn, field, crequest_in->numRcvdVendorSpecificHeaderOptions);
    }

    /* message ID */
    field = (*env)->GetFieldID(env, K_DOC_REQUEST_IN, "messageId", "I");
    if (field == NULL) { /* make sure we got the field */
	printf("ERROR:  GetFieldID failed for messageId fld\n");
	fflush(NULL);
    	return NULL;
    } else {
	/* printf("message ID in c: %d\n", crequest_in->messageID); */
	(*env)->SetIntField(env, j_DocRequestIn, field, crequest_in->messageID);
    }

    /* payload - implemented as getter, using handle */
    field = (*env)->GetFieldID(env, K_DOC_REQUEST_IN, "payloadHandle", "J");
    if (field == NULL) {
	printf("ERROR:  GetFieldID failed for payloadHandle fld\n");
	fflush(NULL);
    	return NULL;
    } else {
	/* printf("OCPayload ptr (in c): %ld\n", (long)crequest_in->payload); */
	/* printf("OCPayload type: %ld\n", (long)((OCPayload*)crequest_in->payload)->type); */
	(*env)->SetLongField(env, j_DocRequestIn, field, (intptr_t)crequest_in->payload);
    }
    return j_DocRequestIn;
    /* printf("OCEntityHandlerRequest_to_DocRequestIn EXIT\n"); */
}

/* typedef OCEntityHandlerResult (*OCEntityHandler) */
/* (OCEntityHandlerFlag flag, OCEntityHandlerRequest * entityHandlerRequest, void* callbackParam); */
OCEntityHandlerResult service_request_in(OCEntityHandlerFlag flag,
					 OCEntityHandlerRequest * c_OCEntityHandlerRequest, /* DocRequestIn */
					 void* j_IResourceServiceProvider)
{
    OC_UNUSED(flag);
    printf("\nocf_resource_manager.c/service_request_in ENTRY\n");
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

    if (j_IResourceServiceProvider == NULL) {
	/* FIXME: use proper logging */
	printf("ERROR %s %d (%s): j_IResourceServiceProvider is NULL\n", __FILE__, __LINE__,__func__);
	return OC_EH_INTERNAL_SERVER_ERROR;
    }

    /* extract ref to service provider from callback param */
    /* get from object rather than interface */
    jclass k_IResourceServiceProvider = (*env)->GetObjectClass(env, j_IResourceServiceProvider);
    if (k_IResourceServiceProvider == NULL) {
	printf("ERROR %s %d (%s): GetObjectClass failed for j_IResourceServiceProvider\n", __FILE__, __LINE__,__func__);
	return OC_EH_INTERNAL_SERVER_ERROR;
    }

    /* jfieldID fid_rsp = (*env)->GetFieldID(env, k_IResourceServiceProvider, */
    /* 					  "serviceProvider", */
    /* 					  "Lorg/iochibity/IResourceServiceProvider;"); */
    /* if (fid_rsp == NULL) { */
    /* 	printf("Failed to get serviceProvider fld id\n"); */
    /* 	return OC_EH_INTERNAL_SERVER_ERROR; */
    /* } */
    /* jobject service_provider = (*env)->GetObjectField(env, j_IResourceServiceProvider, fid_rsp); */
    /* if (service_provider == NULL) { */
    /* 	printf("Failed to get ResourceServiceProvider object\n"); */
    /* 	return OC_EH_INTERNAL_SERVER_ERROR; */
    /* } */

    /* now get the service routine, for later */
    /* jclass k_rsp = (*env)->GetObjectClass(env, service_provider); */
    /* if (k_rsp == NULL) { */
    /* 	printf("ERROR:  GetObjectClass failed for ResourceServiceProvider\n"); */
    /* 	return OC_EH_INTERNAL_SERVER_ERROR; */
    /* } */

    jmethodID mid_serviceRequestIn = (*env)->GetMethodID(env, k_IResourceServiceProvider,
							 "serviceRequestIn",
							 "(ILorg/iochibity/DocRequestIn;)I");
    if (mid_serviceRequestIn == NULL) {
    	printf("ERROR:  GetMethodID failed for serviceRequestIn of CallbackParam\n");
    	return OC_EH_INTERNAL_SERVER_ERROR;
    }

    fflush(NULL);

    /* create RequestIn object */
    /* jclass k_request_in = (*env)->FindClass(env, "org/iochibity/DocRequestIn"); */
    /* if (k_request_in == NULL) { */
    /* 	printf("ERROR:  FindClass failed for org/iochibity/DocRequestIn\n"); */
    /* 	fflush(NULL); */
    /* 	return OC_EH_INTERNAL_SERVER_ERROR; */
    /* } */

    /* jmethodID request_in_ctor = (*env)->GetMethodID(env, K_DOC_REQUEST_IN, "<init>", "()V"); */
    /* if (request_in_ctor == NULL) { */
    /* 	printf("ERROR:  GetMethodID feilad for ctor for DocRequestIn"); */
    /* 	return OC_EH_INTERNAL_SERVER_ERROR; */
    /* } */

    jobject j_DocRequestIn = NULL;
    j_DocRequestIn = OCEntityHandlerRequest_to_DocRequestIn(env, c_OCEntityHandlerRequest);
    fflush(NULL);
    if (j_DocRequestIn == NULL) {
        printf("ERROR:  OCEntityHandlerRequest_to_DocRequestIn failed\n");
    	return OC_EH_INTERNAL_SERVER_ERROR;
    }

    /* now invoke the callback on the Java side */
    int op_result = OC_EH_OK;
    op_result = (*env)->CallIntMethod(env, j_IResourceServiceProvider,
				      mid_serviceRequestIn,
				      j_DocRequestIn);
    if (op_result != OC_STACK_OK) {
        printf("ERROR:  CallIntMethod failed for ResourceServiceProvider.serviceRequestIn\n");
    	return OC_EH_INTERNAL_SERVER_ERROR;
    }

    (*g_JVM)->DetachCurrentThread(g_JVM);

    /* printf("Incoming request: %ld\n", (long)c_OCEntityHandlerRequest); */
    /* printf("Incoming requestHandle: %ld\n", (long)c_OCEntityHandlerRequest->requestHandle); */
    /* printf("Incoming request param: %ld\n", (long)j_IResourceServiceProvider); */
    /* printf("Incoming request flag: %d\n", flag); */

    printf("ocf_resource_manager.c/service_routine EXIT\n");
    return op_result;
}

/* PUBLIC */

/*
 * Class:     org_iochibity_OCF
 * Method:    sendResponse
 * Signature: (Lorg/iochibity/DocResponseOut;)V
 */
JNIEXPORT void JNICALL Java_org_iochibity_OCF_sendResponse
(JNIEnv * env, jclass klass, jobject j_response_out)
{
    OC_UNUSED(klass);
    printf("Java_org_iochibity_OCF_sendResponse ENTRY\n");

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
    jfieldID fid_rqst_in = (*env)->GetFieldID(env, k_response_out, "_requestIn", "Lorg/iochibity/DocRequestIn;");
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
    jfieldID fid_local_handle = (*env)->GetFieldID(env, k_request_in, "localHandle", "J");
    if (fid_local_handle == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for 'localHandle' on RequestIn\n");
	return;
    }
    OCEntityHandlerRequest* j_handle = (OCEntityHandlerRequest*)(intptr_t)
	(*env)->GetLongField(env, j_request_in, fid_local_handle);
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
    if (j_pll == NULL) {
	THROW_JNI_EXCEPTION("GetObjectField failed for fid_pll on j_response_out\n");
	return;
    }
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
    prep_pfrs(env); /* get all the field and method ids etc. needed for iteration */

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

    /* set OCEntityHandlerResult from this._processingResult */

    /* vendor header options */

    /* payload: iterate over the PayloadList to create OCPayload linked list */

    c_response_out->payload = (OCPayload*) c_payload;

    /* finally, send it off: */
    OCStackResult op_result = OC_STACK_OK;
    op_result = OCDoResponse(c_response_out);
    if (op_result != OC_STACK_OK) {
	THROW_STACK_EXCEPTION(op_result, "OCDoResponse failed");
	/* ehResult = OC_EH_ERROR; */
	return;
    }

    printf("Java_org_iochibity_OCF_sendResponse EXIT\n");
}
