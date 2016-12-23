/**
 * @file ocf_ServiceProvider.c
 * @author Gregg Reynolds
 * @date December 2016
 *
 * @brief JNI implementation of ServiceProvider Java API
 */

#include <ctype.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "org_iochibity_ServiceProvider.h"
#include "ocf_Init.h"
#include "ocf_exceptions.h"
#include "jni_utils.h"

#include "octypes.h"
#include "ocresource.h"
#include "ocstack.h"
#include "oic_malloc.h"
#include "oic_string.h"

/* PRIVATE */

/**
 * @brief Convert incoming `OCEntityHandlerRequest` to `StimulusIn`
 * object.  Called by `c_ServiceProvider_observe_stimulus`.
 *
 */
/* FIXME: this is called indirectly from the runtime stack, not java -
   deal with exceptions */
/* THROW_JNI_EXCEPTION won't work since we're not called from Java */
jobject OCEntityHandlerRequest_to_StimulusIn(JNIEnv* env,
					     OCEntityHandlerRequest* c_EHRequest,
					     OCEntityHandlerFlag c_watch_flag)
{
    printf("%s: OCEntityHandlerRequest_to_StimulusIn ENTRY\n", __FILE__);

    jobject j_StimulusIn = (*env)->NewObject(env, K_MSG_REQUEST_IN, MID_RQI_CTOR); // request_in_ctor);

    /* set fields in MsgForServiceProvider ancestor */
    (*env)->SetLongField(env, j_StimulusIn,
			 FID_MSG_LOCAL_HANDLE, (intptr_t)c_EHRequest);
    (*env)->SetLongField(env, j_StimulusIn,
			 FID_MFSP_REMOTE_RQST_HANDLE, (intptr_t)c_EHRequest->requestHandle);
    (*env)->SetLongField(env, j_StimulusIn,
			 FID_MFSP_RESOURCE_HANDLE, (intptr_t)c_EHRequest->resource);

    /* set fields in Message ancestor */
    /* OCDevAddr */
    jobject jdevice_address = (*env)->NewObject(env, K_DEVICE_ADDRESS, MID_DA_CTOR);
    (*env)->SetIntField(env, jdevice_address, FID_DA_NETWORK_PROTOCOL, c_EHRequest->devAddr.adapter);
    (*env)->SetIntField(env, jdevice_address, FID_DA_NETWORK_POLICIES, c_EHRequest->devAddr.flags);
    (*env)->SetIntField(env, jdevice_address, FID_DA_PORT, c_EHRequest->devAddr.port);

    printf("Request-In Device Address address: %s\n", c_EHRequest->devAddr.addr);
    jstring js = (*env)->NewStringUTF(env, c_EHRequest->devAddr.addr);
    (*env)->SetObjectField(env, jdevice_address, FID_DA_ADDRESS, js);
    (*env)->SetIntField(env, jdevice_address, FID_DA_IFINDEX, c_EHRequest->devAddr.ifindex);

    (*env)->SetObjectField(env, j_StimulusIn, FID_MSG_REMOTE_DEVADDR, jdevice_address);

    /* set fields in StimulusIn */
    js = (*env)->NewStringUTF(env, c_EHRequest->query);
    (*env)->SetObjectField(env, j_StimulusIn, FID_RQI_QUERY, js);

    /* method, watch_flag and obsInfo */
    /* watch_flag =  OBSERVE | REQUEST */
    /* if watch_flag is OBSERVE, then : */
    /*     obsInfo.action = REGISTER = 0, DEREGISTER=1, NO_OPTION=2, MQ_SUBSCRIBER=3, MQ_UNSUBSCRIBER=4 */
    /*     obsInfo.Id is for ?? */
    /* NB: an OBSERVE is always also a REQUEST! e.g. flag = (OC_REQUEST_FLAG | OC_OBSERVE_FLAG) */
    /* NB: on client side, method OBSERVE is converted to GET plus Observe indicator (header option) */
    /*     on server side, method OBSERVE must be inferred from watch_flag */
    /*  so we do the obvious thing: reconstruct the OBSERVE method, but we call it WATCH */
    printf("WATCH FLAG:   %d\n", c_watch_flag);
    printf("OBSRV ACTION: %d\n", c_EHRequest->obsInfo.action);
    printf("OBSRV ID:     %d\n", c_EHRequest->obsInfo.obsId);
    if (c_watch_flag & OC_OBSERVE_FLAG) {
	if (c_EHRequest->method == OC_REST_GET) {
	    (*env)->SetIntField(env, j_StimulusIn, FID_MSG_METHOD, OC_REST_OBSERVE);
	    /* c_EHRequest->obsInfo.action */
	    (*env)->SetIntField(env, j_StimulusIn, FID_RQI_WATCH_ACTION, c_EHRequest->obsInfo.action);
	    /* c_EHRequest->obsInfo.Id */
	    (*env)->SetIntField(env, j_StimulusIn, FID_RQI_WATCH_ID, c_EHRequest->obsInfo.obsId);
	} else {
	    /* FIXME:  this should not happen? */
	}
    } else {
	(*env)->SetIntField(env, j_StimulusIn, FID_MSG_METHOD, c_EHRequest->method);
    }



    /* FIXME: deal with header options: */
    /* numRcvdVendorSpecificHeaderOptions */
    /* rcvdVendorSpecificHeaderOptions; */
    printf("Nbr header options: %d\n", c_EHRequest->numRcvdVendorSpecificHeaderOptions);

    /* message ID */
    (*env)->SetIntField(env, j_StimulusIn, FID_RQI_MSG_ID, c_EHRequest->messageID);

    (*env)->SetLongField(env, j_StimulusIn, FID_MSG_OBSERVATION_HANDLE, (intptr_t)c_EHRequest->payload);

    printf("OCEntityHandlerRequest_to_StimulusIn EXIT\n");
    return j_StimulusIn;
}

/**
 * @brief `OCEntityHandler` callback
 *
 * @see Java_org_iochibity_ServiceProvider_exhibitBehavior
 */
/* typedef OCEntityHandlerResult (*OCEntityHandler) */
/* (OCEntityHandlerFlag flag, OCEntityHandlerRequest * entityHandlerRequest, void* callbackParam); */
OCEntityHandlerResult c_ServiceProvider_observe_stimulus(OCEntityHandlerFlag watch_flag,
					 OCEntityHandlerRequest * c_OCEntityHandlerRequest, /* StimulusIn */
					 void* j_IServiceProvider)
{
    printf("\n%s | %s ENTRY on thread %d\n",
	   __FILE__, __func__, (int)pthread_self());

    /* printf("OCEntityHandlerFlag: %d (rqst: %d, obs: %d)\n", watch_flag, OC_REQUEST_FLAG, OC_OBSERVE_FLAG); */
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

    jobject j_StimulusIn = NULL;
    j_StimulusIn = OCEntityHandlerRequest_to_StimulusIn(env, c_OCEntityHandlerRequest, watch_flag);
    fflush(NULL);
    if (j_StimulusIn == NULL) {
        printf("ERROR:  OCEntityHandlerRequest_to_StimulusIn failed\n");
    	return OC_EH_INTERNAL_SERVER_ERROR;
    }

    /* now invoke the callback on the Java side */
    int op_result = OC_EH_OK;
    op_result = (*env)->CallIntMethod(env, j_IServiceProvider,
				      MID_ISP_OBSERVE_STIMULUS,
				      j_StimulusIn);
    if (op_result != OC_STACK_OK) {
        printf("ERROR:  CallIntMethod failed for ResourceServiceProvider.ObserveStimulus\n");
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

/**
 * @brief  Wrapper on `OCDoResponse`
 *
 *
 * Example usage:
 * @code{.java}
 * ...
 * this.exhibitBehavior();
 * ...
 * @endcode
 *
 * @see Java_org_iochibity_CoServiceProvider_exhibitStimulus
 * @see c_ServiceProvider_observe_stimulus
 */
JNIEXPORT void JNICALL Java_org_iochibity_ServiceProvider_exhibitBehavior
(JNIEnv * env, jobject this_SP)
/* JNIEXPORT void JNICALL Java_org_iochibity_ServiceProvider_exhibitBehavior */
/* (JNIEnv * env, jclass klass, jobject j_ObservationOut) */
{
    printf("%s | %s: ENTRY on thread %d\n", __FILE__, __func__, (int)pthread_self());

    /* /\* prep_java(env); *\/ */

    /* /\* 1. alloc the OCEntityHandlerResponse struct *\/ */
    /* OCEntityHandlerResponse * c_response_out = */
    /*                     (OCEntityHandlerResponse *) OICCalloc(1, sizeof(*c_response_out)); */
    /* if (!c_response_out) { */
    /* 	THROW_EH_EXCEPTION(OC_EH_ERROR, "OCEntityHandlerResponse calloc failure\n"); */
    /* 	return; */
    /* } */
    /* /\* get the RequestIn object *\/ */
    /* jobject j_StimulusIn = (*env)->GetObjectField(env, j_ObservationOut, FID_MsgRspOut_RQST_IN); */
    /* if (j_StimulusIn == NULL) { */
    /* 	printf("ERROR: GetObjectField failed for FID_MsgRspOut_RQST_IN on j_ObservationOut\n"); */
    /* 	return; */
    /* } */
    /* /\* now we have the original RequestIn object; let's pull the handles from it *\/ */

    /* printf("BBBBBBBBBBBBBBBB %ld\n", (intptr_t)j_StimulusIn); */

    /* /\* first we need the RequestIn class so we can get the c struct handle *\/ */
    /* jclass k_request_in = (*env)->GetObjectClass(env, j_StimulusIn); */
    /* if (k_request_in == NULL) { */
    /* 	printf("ERROR: GetObjectClass failed for RequestIn object\n"); */
    /* 	return; */
    /* } */

    /* /\* get the localHandle *\/ */
    /* /\* jfieldID fid_local_handle = (*env)->GetFieldID(env, k_request_in, "localHandle", "J"); *\/ */
    /* /\* if (fid_local_handle == NULL) { *\/ */
    /* /\* 	printf("ERROR: GetFieldID failed for 'localHandle' on RequestIn\n"); *\/ */
    /* /\* 	return; *\/ */
    /* /\* } *\/ */

    /* printf("AAAAAAAAAAAAAAAA\n"); */
    /* OCEntityHandlerRequest* j_handle = (OCEntityHandlerRequest*)(intptr_t) */
    /* 	(*env)->GetLongField(env, j_StimulusIn, FID_RQI_LOCAL_HANDLE); */
    /* if (j_handle == NULL) { */
    /* 	printf("ERROR: GetObjectField failed for FID_LOCAL_HANDLE on j_StimulusIn\n"); */
    /* 	return; */
    /* } */
    /* printf("RESPONSE localHandle: %ld\n", (long)j_handle); */
    /* printf("RESPONSE remoteHandle: %ld\n", (long)j_handle->requestHandle); */
    /* c_response_out->requestHandle = j_handle->requestHandle; */

    /* /\* ditto for resourceHandle *\/ */
    /* printf("RESPONSE resourceHandle: %ld\n", (long)j_handle->resource); */
    /* c_response_out->resourceHandle = j_handle->resource; */

    /* /\* defaults *\/ */
    /* c_response_out->numSendVendorSpecificHeaderOptions = 0; */

    /* memset(c_response_out->sendVendorSpecificHeaderOptions, 0, */
    /* 	   sizeof c_response_out->sendVendorSpecificHeaderOptions); */
    /* memset(c_response_out->resourceUri, 0, sizeof(c_response_out->resourceUri)); */

    /* c_response_out->persistentBufferFlag = 0; */

    /* /\* ObservationList *\/ */
    /* jfieldID fid_pll = (*env)->GetFieldID(env, K_MSG_RESPONSE_OUT, */
    /* 					  "_observationList", "Lorg/iochibity/ObservationList;"); */
    /* if (fid_pll == NULL) { */
    /* 	printf("ERROR: GetFieldID failed for '_observationList' on ResponseOut\n"); */
    /* 	return; */
    /* } */
    /* jobject j_pll = (*env)->GetObjectField(env, j_ObservationOut, fid_pll); */
    /* if (j_pll != NULL) { */
    /* 	jclass k_pll = (*env)->GetObjectClass(env, j_pll); */
    /* 	if (k_pll == NULL) { */
    /* 	    printf("ERROR: GetObjectClass failed for ObservationList object\n"); */
    /* 	    return; */
    /* 	} */
    /* 	/\* we're going to iterate using jni, dunno about efficiency *\/ */
    /* 	jmethodID mid_pll_size = (*env)->GetMethodID(env, k_pll, "size", "()I"); */
    /* 	if (mid_pll_size == NULL) { */
    /* 	    printf("ERROR: GetMethodID failed for 'size' of ObservationList\n"); */
    /* 	    return; */
    /* 	} */
    /* 	int sz = 0; */
    /* 	sz = (*env)->CallIntMethod(env, j_pll, mid_pll_size); */
    /* 	printf("ObservationList SIZE: %d\n", sz); */

    /* 	jmethodID mid_pll_get = (*env)->GetMethodID(env, k_pll, "get", "(I)Ljava/lang/Object;"); */
    /* 	if (mid_pll_get == NULL) { */
    /* 	    printf("ERROR: GetMethodID failed for 'get' of ObservationList\n"); */
    /* 	    return; */
    /* 	} */

    /* 	/\* pfrs = PayloadForResourceState *\/ */
    /* 	/\* prep_pfrs(env); /\\* get all the field and method ids etc. needed for iteration *\\/ *\/ */

    /* 	jobject j_observation = NULL; */
    /* 	int c_type; */
    /* 	OCPayload* c_payload = NULL; */
    /* 	for (int i=0; i<sz; i++) { */
    /* 	    j_observation = (*env)->CallObjectMethod(env, j_pll, mid_pll_get, i); */
    /* 	    c_type = (*env)->GetIntField(env, j_observation, FID_OBSERVATION_TYPE); */
    /* 	    printf("OBSERVATION TYPE: %d\n", c_type); */

    /* 	    /\* the OCPayload structs are already there in the _handle fields *\/ */
    /* 	    /\* but we need to synch them with the Java objects *\/ */
    /* 	    switch(c_type) { */
    /* 	    case PAYLOAD_TYPE_INVALID: */
    /* 		printf("INVALID PAYLOAD TYPE NOT SUPPORTED\n"); */
    /* 		break; */
    /* 	    case PAYLOAD_TYPE_DISCOVERY: */
    /* 		printf("DISCOVERY PAYLOAD NOT YET SUPPORTED\n"); */
    /* 		break; */
    /* 	    case PAYLOAD_TYPE_DEVICE: */
    /* 		printf("DEVICE PAYLOAD NOT YET SUPPORTED\n"); */
    /* 		break; */
    /* 	    case PAYLOAD_TYPE_PLATFORM: */
    /* 		printf("PLATFORM PAYLOAD NOT YET SUPPORTED\n"); */
    /* 		break; */
    /* 	    case PAYLOAD_TYPE_REPRESENTATION: */
    /* 		c_payload = (OCPayload*)pfrs_to_OCRepPayload(env, j_observation); */
    /* 		break; */
    /* 	    case PAYLOAD_TYPE_SECURITY: */
    /* 		printf("SECURITY PAYLOAD NOT YET SUPPORTED\n"); */
    /* 		break; */
    /* 	    case PAYLOAD_TYPE_PRESENCE: */
    /* 		printf("PRESENCE PAYLOAD NOT YET SUPPORTED\n"); */
    /* 		break; */
    /* 		/\* obsolete as of 1.2.1: *\/ */
    /* 		/\* case PAYLOAD_TYPE_RD: *\/ */
    /* 		/\*     printf("RD PAYLOAD NOT YET SUPPORTED\n"); *\/ */
    /* 		/\*     break; *\/ */
    /* 	    default: */
    /* 		printf("UNKNOWN PAYLOAD - NOT SUPPORTED\n"); */
    /* 		break; */
    /* 	    } */
    /* 	} */
    /* 	c_response_out->payload = (OCPayload*) c_payload; */
    /* } else { */
    /* 	/\* OK: use left the payload null *\/ */
    /* 	/\* printf("ERROR: GetObjectField failed for fid_pll on j_ObservationOut\n"); *\/ */
    /* 	/\* return; *\/ */
    /* } */

    /* /\* set OCEntityHandlerResult from this._processingResult *\/ */

    /* /\* vendor header options *\/ */

    /* /\* payload: iterate over the PayloadList to create OCPayload linked list *\/ */


    /* /\* finally, send it off: *\/ */
    /* OCStackResult op_result = OC_STACK_OK; */
    /* op_result = OCDoResponse(c_response_out); */
    /* if (op_result != OC_STACK_OK) { */
    /* 	THROW_STACK_EXCEPTION(op_result, "OCDoResponse failed"); */
    /* 	/\* ehResult = OC_EH_ERROR; *\/ */
    /* 	return; */
    /* } */
    printf("%s | %s: EXIT\n", __FILE__, __func__);
}
