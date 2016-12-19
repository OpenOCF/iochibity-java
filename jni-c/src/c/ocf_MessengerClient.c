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

/* INTERNAL */

jobject OCClientResponse_to_MsgResponseIn(JNIEnv* env, OCClientResponse* c_OCClientResponse)
{
    printf("%s: OCClientResponse_to_MsgResponseIn ENTRY\n", __FILE__);
    jobject j_MsgResponseIn = (*env)->NewObject(env,
						K_MSG_RESPONSE_IN,
						MID_MsgRspIn_CTOR); // request_in_ctor);
    if (j_MsgResponseIn == NULL) {
	printf("NewObject failed for MsgResponseIn\n");
	return NULL;
    }

    /* set the c struct handle */
    (*env)->SetIntField(env, j_MsgResponseIn, FID_MSG_LOCAL_HANDLE, (jlong)c_OCClientResponse);

    /* OCDevAddr */
    jobject j_DeviceAddress = (*env)->NewObject(env, K_DEVICE_ADDRESS, MID_RQI_CTOR);
    (*env)->SetIntField(env, j_DeviceAddress, FID_DA_ADAPTER, c_OCClientResponse->devAddr.adapter);
    (*env)->SetIntField(env, j_DeviceAddress, FID_DA_FLAGS, c_OCClientResponse->devAddr.flags);
    printf("TRANSPORT FLAGS: 0x%08X\n", c_OCClientResponse->devAddr.flags);
    (*env)->SetIntField(env, j_DeviceAddress, FID_DA_PORT,    c_OCClientResponse->devAddr.port);

    jstring j_addr = (*env)->NewStringUTF(env, c_OCClientResponse->devAddr.addr);
    (*env)->SetObjectField(env, j_DeviceAddress, FID_DA_ADDRESS, j_addr);

    (*env)->SetIntField(env, j_DeviceAddress, FID_DA_IFINDEX, c_OCClientResponse->devAddr.ifindex);
    /* jstring j_route = (*env)->NewStringUTF(env, c_OCClientResponse->devAddr.routeData); */
    /* (*env)->SetObjectField(env, j_DeviceAddress, FID_DA_ROUTE_DATA, j_route); */
    (*env)->SetObjectField(env, j_MsgResponseIn, FID_MsgRspIn_REMOTE_DEVADDR, j_DeviceAddress);

    printf("RESPONSE CONNECTION TYPE: 0x%08X\n",  c_OCClientResponse->connType);
    (*env)->SetIntField(env, j_MsgResponseIn, FID_MsgRspIn_CONN_TYPE, c_OCClientResponse->connType);

    /* FIXME: use id_length */
    jstring j_sid = (*env)->NewStringUTF(env, (char*)c_OCClientResponse->identity.id);
    (*env)->SetObjectField(env, j_MsgResponseIn, FID_MsgRspIn_REMOTE_SID, j_sid);

    (*env)->SetIntField(env, j_MsgResponseIn, FID_MsgRspIn_RESULT, c_OCClientResponse->result);
    (*env)->SetIntField(env, j_MsgResponseIn, FID_MsgRspIn_SERIAL, c_OCClientResponse->sequenceNumber);

    jstring j_uri = (*env)->NewStringUTF(env, c_OCClientResponse->resourceUri);
    (*env)->SetObjectField(env, j_MsgResponseIn, FID_MsgRspIn_URI, j_uri);

    /* set payloadHandle in Message */
    printf("XXXXXXXXXXXXXXXX respond payload: %ld\n", (intptr_t)c_OCClientResponse->payload);
    (*env)->SetLongField(env, j_MsgResponseIn,
			 FID_MsgRspIn_PAYLOAD_HANDLE,
			 (long)(intptr_t)c_OCClientResponse->payload);

    /* set optionCount and ptr_Options in Message */
    printf("%s: OCClientResponse_to_MsgResponseIn EXIT\n", __FILE__);
    return j_MsgResponseIn;
}

OCStackApplicationResult service_response_in(void* c_MsgRequestOut, /* contains requestor cb */
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
    if (c_MsgRequestOut == NULL) {
	/* FIXME: use proper logging */
	printf("ERROR %s %d (%s): ctx param is NULL for service_response_in\n",
	       __FILE__, __LINE__,__func__);
	return OC_STACK_KEEP_TRANSACTION;
    }

    /* 2. get ResourceServiceRequestor object from MsgRequestOut */
    /* extract ref to service provider from callback param */
    jclass k_ctx = (*env)->GetObjectClass(env, c_MsgRequestOut);
    if (k_ctx == NULL) {
	printf("ERROR %s %d (%s): GetObjectClass failed for ctx\n", __FILE__, __LINE__,__func__);
	return OC_STACK_KEEP_TRANSACTION;
    }

    jobject j_ResourceServiceRequestor = (*env)->GetObjectField(env,
								(jobject)c_MsgRequestOut,
								FID_RQO_SERVICE_REQUESTOR);
    if (j_ResourceServiceRequestor == NULL) {
	printf("Failed to get IServiceRequestor object from MsgRequestOut\n");
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
    							      "(Lorg/iochibity/MsgResponseIn;)I");
    if (mid_rsr_serviceResponseIn == NULL) {
    	THROW_JNI_EXCEPTION("GetMethodID failed for serviceResponseIn of ResourceServiceRequestor\n");
    	return OC_STACK_KEEP_TRANSACTION;
    }

    /* 4. construct MsgResponseIn from incoming OCClientResponse ... */
    jobject j_MsgResponseIn = OCClientResponse_to_MsgResponseIn(env, c_OCClientResponse);
    /* ... and insert pointer to it.*/
    (*env)->SetLongField(env, j_MsgResponseIn, FID_MSG_LOCAL_HANDLE, (intptr_t)c_OCClientResponse);

    /* 5. call the serviceResponseIn method of the Requestor */
    int op_result = OC_EH_OK;
    op_result = (*env)->CallIntMethod(env,
				      j_ResourceServiceRequestor,
				      mid_rsr_serviceResponseIn,
				      j_MsgResponseIn);
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
 * Class:     org_iochibity_Messenger
 * Method:    discoverPlatforms
 * Signature: (Lorg/iochibity/MsgRequestOut;)[B
 */
JNIEXPORT jbyteArray JNICALL Java_org_iochibity_Messenger_discoverPlatforms__Lorg_iochibity_MsgRequestOut_2
(JNIEnv * env, jclass this, jobject j_MsgRequestOut)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    /* printf("Java_org_iochibity_Messenger_discoverPlatforms__Lorg_iochibity_MsgRequestOut_2 ENTRY\n"); */
    OCStackResult ret;
    OCCallbackData cbData;

    /* DO NOT forget that incoming java objects are local, to make
       them available in a callback from the c stack, we need to
       globalize them! */
    jobject x_MsgRequestOut = NULL;
    x_MsgRequestOut = (*env)->NewGlobalRef(env, j_MsgRequestOut);

    cbData.cb = service_response_in;
    cbData.context = (void*)(long)x_MsgRequestOut;
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
 * Class:     org_iochibity_Messenger
 * Method:    discoverPlatforms
 * Signature: (Lorg/iochibity/IServiceRequestor;)[B
 */
JNIEXPORT jbyteArray JNICALL Java_org_iochibity_Messenger_discoverPlatforms__Lorg_iochibity_IServiceRequestor_2
(JNIEnv * env, jclass klass, jobject j_ResourceServiceRequestor)
{
    OC_UNUSED(env);
    OC_UNUSED(klass);
    OC_UNUSED(j_ResourceServiceRequestor);
    /* printf("Java_org_iochibity_Messenger_discoverPlatforms__Lorg_iochibity_IServiceRequestor_2 ENTRY\n"); */

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
 * Class:     org_iochibity_Messenger
 * Method:    discoverPlatforms
 * Signature: (Lorg/iochibity/IServiceRequestor;[Lorg/iochibity/DeviceAddress;)[B
 */
JNIEXPORT jbyteArray JNICALL Java_org_iochibity_Messenger_discoverPlatforms__Lorg_iochibity_IServiceRequestor_2_3Lorg_iochibity_DeviceAddress_2
  (JNIEnv *, jclass, jobject, jobjectArray);

/*
 * Class:     org_iochibity_Messenger
 * Method:    discoverDevices
 * Signature: (Lorg/iochibity/MsgRequestOut;)[B
 */
JNIEXPORT jbyteArray JNICALL Java_org_iochibity_Messenger_discoverDevices__Lorg_iochibity_MsgRequestOut_2
(JNIEnv * env, jclass klass, jobject j_MsgRequestOut)
{
    OC_UNUSED(env);
    OC_UNUSED(klass);
    /* printf("Java_org_iochibity_Messenger_discoverDevices__Lorg_iochibity_MsgRequestOut_2 ENTRY\n"); */
    OCStackResult ret;
    OCCallbackData cbData;

    /* DO NOT forget that incoming java objects are local, to make
       them available in a callback from the c stack, we need to
       globalize them! */
    jobject x_MsgRequestOut = NULL;
    x_MsgRequestOut = (*env)->NewGlobalRef(env, j_MsgRequestOut);

    cbData.cb = service_response_in;
    cbData.context = (void*)(long)x_MsgRequestOut;
    cbData.cd = NULL;

    OCDoHandle c_handle;

    ret = OCDoResource(&c_handle,	/* OCDoHandle = void* */
		       OC_REST_DISCOVER, /* method */
		       OC_RSRVD_DEVICE_URI,
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
 * Class:     org_iochibity_Messenger
 * Method:    discoverResources
 * Signature: (Lorg/iochibity/MsgRequestOut;)[B
 */
JNIEXPORT jbyteArray JNICALL Java_org_iochibity_Messenger_discoverResources__Lorg_iochibity_MsgRequestOut_2
(JNIEnv * env, jclass klass, jobject j_MsgRequestOut)
{
    OC_UNUSED(env);
    OC_UNUSED(klass);
    printf("Java_org_iochibity_Messenger_discoverResources__Lorg_iochibity_MsgRequestOut_2 ENTRY\n");
    OCStackResult ret;
    OCCallbackData cbData;

    /* DO NOT forget that incoming java objects are local, to make
       them available in a callback from the c stack, we need to
       globalize them! */
    jobject x_MsgRequestOut = NULL;
    x_MsgRequestOut = (*env)->NewGlobalRef(env, j_MsgRequestOut);

    cbData.cb = service_response_in;
    cbData.context = (void*)(long)x_MsgRequestOut;
    cbData.cd = NULL;

    OCDoHandle c_handle;

    ret = OCDoResource(&c_handle,	/* OCDoHandle = void* */
		       OC_REST_DISCOVER, /* method */
		       OC_RSRVD_WELL_KNOWN_URI,
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
 * Class:     org_iochibity_Messenger
 * Method:    sendRequest
 * Signature: (ILorg/iochibity/MsgRequestOut;)V
 */
JNIEXPORT jbyteArray JNICALL Java_org_iochibity_Messenger_sendRequest__ILorg_iochibity_MsgRequestOut_2
(JNIEnv * env, jclass klass, jint j_method, jobject j_MsgRequestOut)
{
    OC_UNUSED(env);
    OC_UNUSED(klass);
    printf("Java_org_iochibity_Messenger_sendRequest__ILorg_iochibity_MsgRequestOut_2 ENTRY\n");

    OCStackResult ret;
    OCCallbackData cbData;

    /* DO NOT forget that incoming java objects are local, to make
       them available in a callback from the c stack, we need to
       globalize them! */
    jobject x_MsgRequestOut = NULL;
    x_MsgRequestOut = (*env)->NewGlobalRef(env, j_MsgRequestOut);

    /* extract destination dev address */
    jobject j_destDevAddr = (*env)->GetObjectField(env, j_MsgRequestOut, FID_RQO_DEST);
    if (j_destDevAddr == NULL) {
	THROW_JNI_EXCEPTION("GetObjectField failed for FID_RQO_DEST on j_MsgRequestOut\n");
	return NULL;
    }

    OCDevAddr* c_destDevAddr = (OCDevAddr *)OICCalloc(sizeof (OCDevAddr), 1);
    if (!c_destDevAddr) {
    	THROW_JNI_EXCEPTION("ERROR: no stack memory for OCDevAddr alloc");
    	return NULL;
    }
    c_destDevAddr->adapter = (*env)->GetIntField(env, j_destDevAddr, FID_DA_ADAPTER);
    c_destDevAddr->flags = (*env)->GetIntField(env, j_destDevAddr, FID_DA_FLAGS);
    printf("SEND TRANSPORT FLAGS: 0x%08X\n", c_destDevAddr->flags);
    c_destDevAddr->flags |= OC_SECURE;
    c_destDevAddr->port = (*env)->GetIntField(env, j_destDevAddr, FID_DA_PORT);
    jstring j_addr = (*env)->GetObjectField(env, j_destDevAddr, FID_DA_ADDRESS);
    if (j_addr == NULL) {
    	printf("GetObjectField failed for MID_DA_ADDRESS on j_destDevAddr\n");
    	return NULL;
    }
    char* c_addr = (char*) (*env)->GetStringUTFChars(env, j_addr, NULL);
    strncpy(c_destDevAddr->addr, c_addr, MAX_ADDR_STR_SIZE);
    c_destDevAddr->ifindex = (*env)->GetIntField(env, j_destDevAddr, FID_DA_IFINDEX);
    printf("c addr: %s\n", c_addr);
    printf("c port: %d\n", c_destDevAddr->port);

    /* uri */
    jobject j_uri         = (*env)->GetObjectField(env, j_MsgRequestOut, FID_RQO_URI);
    if (j_uri == NULL) {
    	printf("GetObjectField failed for FID_RQO_URI on j_destDevAddr\n");
    	return NULL;
    }
    char* c_uri = (char*)(*env)->GetStringUTFChars(env, j_uri, NULL);
    if (c_uri == NULL) {
	// ok for multicast, not unicast - throw exception
    } else {
    }

    cbData.cb = service_response_in;
    cbData.context = (void*)(long)x_MsgRequestOut;
    cbData.cd = NULL;

    OCDoHandle c_handle = NULL;

    ret = OCDoResource(&c_handle,	/* OCDoHandle = void* */
    		       (OCMethod)j_method,
		       /* OC_REST_GET, */
    		       c_uri,
    		       c_destDevAddr,    /* OCDevAddr* destination */
    		       NULL,		 /* OCPayload* payload */
		       CT_DEFAULT,
    		       /* CT_IP_USE_V4 */
		       /* | CT_ADAPTER_IP */
		       /* | CT_FLAG_SECURE,	 /\* OCConnectivityType conn_type *\/ */
    		       OC_LOW_QOS,
                       /* (qos == OC_HIGH_QOS) ? OC_HIGH_QOS : OC_LOW_QOS, /\* OCQualityOfService *\/ */
                       &cbData,	/* OCCallbackData* cbData */
    		       NULL,	/* OCHeaderOptions* options */
    		       0);	/* uint8_t numOptions */

    printf("Java_org_iochibity_Messenger_sendRequest__ILorg_iochibity_MsgRequestOut_2 EXIT\n");
    return c_handle;
}




