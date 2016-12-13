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

/* INTERNAL */

jobject OCClientResponse_to_DocResponseIn(JNIEnv* env, OCClientResponse* c_OCClientResponse)
{
    printf("OCClientResponse_to_DocResponseIn ENTRY\n");
    jobject j_DocResponseIn = (*env)->NewObject(env,
						K_DOC_RESPONSE_IN,
						MID_DRSPI_CTOR); // request_in_ctor);
    if (j_DocResponseIn == NULL) {
	printf("NewObject failed for DocResponseIn\n");
	return NULL;
    }

    /* set the c struct handle */
    (*env)->SetIntField(env, j_DocResponseIn, FID_DRSPI_PTR_RESPONSE, (jlong)c_OCClientResponse);

    /* OCDevAddr */
    jobject j_DeviceAddress = (*env)->NewObject(env, K_DEVICE_ADDRESS, MID_DRQI_CTOR);
    (*env)->SetIntField(env, j_DeviceAddress, FID_DA_ADAPTER, c_OCClientResponse->devAddr.adapter);
    (*env)->SetIntField(env, j_DeviceAddress, FID_DA_FLAGS,   c_OCClientResponse->devAddr.flags);
    (*env)->SetIntField(env, j_DeviceAddress, FID_DA_PORT,    c_OCClientResponse->devAddr.port);
    jstring j_addr = (*env)->NewStringUTF(env, c_OCClientResponse->devAddr.addr);
    (*env)->SetObjectField(env, j_DeviceAddress, FID_DA_ADDRESS, j_addr);
    (*env)->SetIntField(env, j_DeviceAddress, FID_DA_IFINDEX, c_OCClientResponse->devAddr.ifindex);
    /* jstring j_route = (*env)->NewStringUTF(env, c_OCClientResponse->devAddr.routeData); */
    /* (*env)->SetObjectField(env, j_DeviceAddress, FID_DA_ROUTE_DATA, j_route); */
    (*env)->SetObjectField(env, j_DocResponseIn, FID_DRSPI_DEVICE_ADDRESS, j_DeviceAddress);

    (*env)->SetIntField(env, j_DocResponseIn, FID_DRSPI_CONN_TYPE, c_OCClientResponse->connType);

    /* FIXME: use id_length */
    printf("SID length: %d\n", c_OCClientResponse->identity.id_length);
    jstring j_sid = (*env)->NewStringUTF(env, (char*)c_OCClientResponse->identity.id);
    (*env)->SetObjectField(env, j_DocResponseIn, FID_DRSPI_REMOTE_SID, j_sid);

    (*env)->SetIntField(env, j_DocResponseIn, FID_DRSPI_RESULT, c_OCClientResponse->result);
    (*env)->SetIntField(env, j_DocResponseIn, FID_DRSPI_SERIAL, c_OCClientResponse->sequenceNumber);

    jstring j_uri = (*env)->NewStringUTF(env, c_OCClientResponse->resourceUri);
    (*env)->SetObjectField(env, j_DocResponseIn, FID_DRSPI_URI, j_uri);

    /* set payloadHandle in Document */
    printf("c PAYLOAD TYPE: %d\n", c_OCClientResponse->payload->type);
    (*env)->SetLongField(env, j_DocResponseIn,
			 FID_DOC_PAYLOAD_HANDLE,
			 (jlong)c_OCClientResponse->payload);

    /* set optionCount and ptr_Options in Document */

    return j_DocResponseIn;
}

OCStackApplicationResult service_response_in(void* c_DocRequestOut, /* contains requestor cb */
					     OCDoHandle handle,
					     OCClientResponse * c_OCClientResponse)
{
    OC_UNUSED(handle);
    printf("service_response_in ENTRY\n");

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
	    return OC_STACK_KEEP_TRANSACTION;
	}
    } else if (getEnvStat == JNI_OK) {
	/* printf("GetEnv: attached\n"); */
    } else if (getEnvStat == JNI_EVERSION) {
	printf("ERROR %s %d (%s): JNI version not supported\n", __FILE__, __LINE__,__func__);
	return OC_STACK_KEEP_TRANSACTION;
    }
    if ((*env)->ExceptionCheck(env)) {
	(*env)->ExceptionDescribe(env);
    }

    /* if ctx param is null something went wrong */
    if (c_DocRequestOut == NULL) {
	/* FIXME: use proper logging */
	printf("ERROR %s %d (%s): ctx param is NULL for service_response_in\n",
	       __FILE__, __LINE__,__func__);
	return OC_STACK_KEEP_TRANSACTION;
    }

    /* 2. get ResourceServiceRequestor object from DocRequestOut */
    /* extract ref to service provider from callback param */
    jclass k_ctx = (*env)->GetObjectClass(env, c_DocRequestOut);
    if (k_ctx == NULL) {
	printf("ERROR %s %d (%s): GetObjectClass failed for ctx\n", __FILE__, __LINE__,__func__);
	return OC_STACK_KEEP_TRANSACTION;
    }

    jobject j_ResourceServiceRequestor = (*env)->GetObjectField(env,
								(jobject)c_DocRequestOut,
								FID_DRQO_SERVICE_REQUESTOR);
    if (j_ResourceServiceRequestor == NULL) {
	printf("Failed to get IResourceServiceRequestor object from DocRequestOut\n");
	return OC_STACK_KEEP_TRANSACTION;
    }

    /* we need to get the method from the object, not the class (since
       it's an interface) */
    jclass k_rsr = (*env)->GetObjectClass(env, j_ResourceServiceRequestor);
    if (k_rsr == NULL) {
    	THROW_JNI_EXCEPTION("GetObjectClass failed for ResourceServiceRequestor object\n");
    	return OC_STACK_KEEP_TRANSACTION;
    }
    jmethodID mid_rsr_serviceResponseIn = (*env)->GetMethodID(env, k_rsr,
    							      "serviceResponseIn",
    							      "(Lorg/iochibity/DocResponseIn;)I");
    if (mid_rsr_serviceResponseIn == NULL) {
    	THROW_JNI_EXCEPTION("GetMethodID failed for serviceResponseIn of ResourceServiceRequestor\n");
    	return OC_STACK_KEEP_TRANSACTION;
    }

    /* 4. construct DocResponseIn from incoming OCClientResponse */
    jobject j_DocResponseIn = OCClientResponse_to_DocResponseIn(env, c_OCClientResponse);

    /* 5. call the serviceResponseIn method of the Requestor */
    int op_result = OC_EH_OK;
    op_result = (*env)->CallIntMethod(env,
				      j_ResourceServiceRequestor,
				      mid_rsr_serviceResponseIn,
				      j_DocResponseIn);
    if (op_result != OC_STACK_OK) {
        printf("ERROR:  CallIntMethod failed for ResourceServiceRequestor.serviceRequestIn\n");
	return OC_STACK_KEEP_TRANSACTION;
    }

    (*g_JVM)->DetachCurrentThread(g_JVM);

    printf("service_response_in EXIT\n");
    /* the stack handles this automatically so we can return anything */
    return OC_STACK_KEEP_TRANSACTION;
}

/* EXTERNAL */
/*
 * Class:     org_iochibity_OCF
 * Method:    discoverPlatform
 * Signature: (Lorg/iochibity/DocRequestOut;)[B
 */
JNIEXPORT jbyteArray JNICALL Java_org_iochibity_OCF_discoverPlatform__Lorg_iochibity_DocRequestOut_2
(JNIEnv * env, jclass this, jobject j_DocRequestOut)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    printf("Java_org_iochibity_OCF_discoverPlatform__Lorg_iochibity_DocRequestOut_2 ENTRY\n");
    OCStackResult ret;
    OCCallbackData cbData;

    /* DO NOT forget that incoming java objects are local, to make
       them available in a callback from the c stack, we need to
       globalize them! */
    jobject x_DocRequestOut = NULL;
    x_DocRequestOut = (*env)->NewGlobalRef(env, j_DocRequestOut);

    cbData.cb = service_response_in;
    cbData.context = (void*)(long)x_DocRequestOut;
    cbData.cd = NULL;

    OCDoHandle c_handle;

    ret = OCDoResource(&c_handle,	/* OCDoHandle = void* */
		       OC_REST_DISCOVER, /* method */
		       OC_RSRVD_PLATFORM_URI,
		       NULL,		 /* OCDevAddr* destination */
		       0,		 /* OCPayload* payload */
		       CT_DEFAULT,	 /* OCConnectivityType conn_type */
		       OC_LOW_QOS,
                       /* (qos == OC_HIGH_QOS) ? OC_HIGH_QOS : OC_LOW_QOS, /\* OCQualityOfService *\/ */
                       &cbData,	/* OCCallbackData* cbData */
		       NULL,	/* OCHeaderOptions* options */
		       0);	/* uint8_t numOptions */

    return c_handle;
}


/*
 * Class:     org_iochibity_OCF
 * Method:    discoverPlatform
 * Signature: (Lorg/iochibity/IResourceServiceRequestor;)[B
 */
JNIEXPORT jbyteArray JNICALL Java_org_iochibity_OCF_discoverPlatform__Lorg_iochibity_IResourceServiceRequestor_2
(JNIEnv * env, jclass klass, jobject j_ResourceServiceRequestor)
{
    OC_UNUSED(env);
    OC_UNUSED(klass);
    OC_UNUSED(j_ResourceServiceRequestor);
    printf("Java_org_iochibity_OCF_discoverPlatform__Lorg_iochibity_IResourceServiceRequestor_2 ENTRY\n");

    /* get qos from j_ResourceServiceRequestor */

    /* jbyteArray j_handle =  (*env)->NewByteArray(env, 8); */

    OCStackResult ret;
    OCCallbackData cbData;

    cbData.cb = service_response_in;
    cbData.context = (void*)j_ResourceServiceRequestor;
    cbData.cd = NULL;

    OCDoHandle c_handle;

    ret = OCDoResource(&c_handle,	/* OCDoHandle = void* */
		       OC_REST_DISCOVER, /* method */
		       OC_RSRVD_PLATFORM_URI,
		       NULL,		 /* OCDevAddr* destination */
		       0,		 /* OCPayload* payload */
		       CT_DEFAULT,	 /* OCConnectivityType conn_type */
		       OC_LOW_QOS,
                       /* (qos == OC_HIGH_QOS) ? OC_HIGH_QOS : OC_LOW_QOS, /\* OCQualityOfService *\/ */
                       &cbData,	/* OCCallbackData* cbData */
		       NULL,	/* OCHeaderOptions* options */
		       0);	/* uint8_t numOptions */

    return c_handle;
}

/*
 * Class:     org_iochibity_OCF
 * Method:    discoverPlatform
 * Signature: (Lorg/iochibity/IResourceServiceRequestor;[Lorg/iochibity/DeviceAddress;)[B
 */
JNIEXPORT jbyteArray JNICALL Java_org_iochibity_OCF_discoverPlatform__Lorg_iochibity_IResourceServiceRequestor_2_3Lorg_iochibity_DeviceAddress_2
  (JNIEnv *, jclass, jobject, jobjectArray);
