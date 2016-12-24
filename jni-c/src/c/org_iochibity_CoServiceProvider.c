/**
 * @file org_iochibity_CoServiceProvider.c
 * @author Gregg Reynolds
 * @date December 2016
 *
 * @brief JNI implementation of CoServiceProvider (client) Java API:
 * `Java_org_iochibity_CoServiceProvider_exhibitStimulus` and `c_CoServiceProvider_observe_behavior`
 */

#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "org_iochibity_CoServiceProvider.h"
#include "jni_utils.h"
#include "org_iochibity_Init.h"
#include "org_iochibity_Exceptions.h"

#include "octypes.h"
#include "oic_malloc.h"
#include "ocpayload.h"
#include "ocresource.h"
#include "ocresourcehandler.h"
#include "ocstack.h"

/* PRIVATE */

/* FIXME: we need a list of these, one for every CoSP use */
jobject g_CoSP = NULL;

/**
 * @brief Convert OCClientResponse to an `ObservationIn` object
 *
 * Allocate an `ObservationIn` java object, then use data from incoming
 * `OCClientResponse` to initialize it.  Return the initialize object.
 * Called internally by `c_CoServiceProvider_observe_behavior`.
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
 * @brief Wrapper for `OCClientResponseHandler`; called by stack on
 * receipt of incoming `OCClientResponse` from server.
 *
 * Convert the incoming response data to an `ObservationIn` object,
 * and then pass that to the `observeBehavior` method of the transaction's
 * `CoServiceProvider` object.  That object is conveyed by the c_CoSP
 * parameter (in the C API, void* context), which was passed (as the
 * "context" pointer of the OCCallbackData param of OCDoResource) by
 * the _exhibitStimulus_ routine of the CoServiceProvider.
 *
 *  @param [in] c_CoSP handle to `CoServiceProvider` (client) object
 *  containing callback method; `context` in C API
 *
 * @param [in] c_TransactionHandle token identifying originating
 * `OCDoResource` invocation (not the same as the CoAP token)
 *
 * @param [in] c_OCClientResponse response data from server
 *
 * @return result code of type `OCStackApplicationResult`
 *
 * @see Java_org_iochibity_CoServiceProvider_exhibitStimulus
 * @see observe_stimulus
 * @see Java_org_iochibity_ServiceProvider_exhibitBehavior
 */
OCStackApplicationResult c_org_iochibity_CoServiceProvider_observe_behavior(void* c_CoSP,
					  OCDoHandle c_TransactionHandle,
					  OCClientResponse* c_OCClientResponse)
{
    OC_UNUSED(c_TransactionHandle);
    printf("ocf_CoServiceProvider_c_CoServiceProvider_observe_behavior ENTRY\n");

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
	    return OC_STACK_DELETE_TRANSACTION;
	}
    } else if (getEnvStat == JNI_OK) {
	/* printf("GetEnv: attached\n"); */
    } else if (getEnvStat == JNI_EVERSION) {
	printf("ERROR %s %d (%s): JNI version not supported\n", __FILE__, __LINE__,__func__);
	return OC_STACK_DELETE_TRANSACTION;
    }
    if ((*env)->ExceptionCheck(env)) {
	(*env)->ExceptionDescribe(env);
    }

    /* print_class_name(env, c_CoSP); */

    /* if ctx param is null something went wrong */
    if (c_CoSP == NULL) {
	/* FIXME: use proper logging */
	printf("ERROR %s %d (%s): ctx param is NULL for c_CoServiceProvider_observe_behavior\n",
	       __FILE__, __LINE__,__func__);
	return OC_STACK_DELETE_TRANSACTION;
    }

    /* 2. construct ObservationIn from incoming OCClientResponse ... */
    jobject j_ObservationIn = OCClientResponse_to_ObservationIn(env, c_OCClientResponse);

    /* ... and insert pointer to it.*/
    (*env)->SetLongField(env, j_ObservationIn, FID_MSG_LOCAL_HANDLE, (intptr_t)c_OCClientResponse);

    /* 2. extract ref to observeBehavior routine from CoSP callback param */
    /* /\* we need to get the method from the object, not the class? */
    jclass k_cosp = (*env)->GetObjectClass(env, (jobject)c_CoSP);
    if (k_cosp == NULL) {
    	THROW_JNI_EXCEPTION("GetObjectClass failed for CoServiceProvider object\n");
    	return OC_STACK_DELETE_TRANSACTION;
    }

    jmethodID mid_cosp_c_CoServiceProvider_observe_behavior = (*env)->GetMethodID(env, k_cosp,
    								 "observeBehavior",
    								 "(Lorg/iochibity/ObservationIn;)I");
    if (mid_cosp_c_CoServiceProvider_observe_behavior == NULL) {
    	THROW_JNI_EXCEPTION("GetMethodID failed for mid_cosp_c_CoServiceProvider_observe_behavior of CoServiceProvider\n");
    	return OC_STACK_DELETE_TRANSACTION;
    }

    /* 5. call the observeBehavior method of the CoServiceProvider */
    int op_result = OC_EH_OK;
    printf("BBBBBBBBBBBBBBBB\n");
    op_result = (*env)->CallIntMethod(env,
				      (jobject)c_CoSP,
				      mid_cosp_c_CoServiceProvider_observe_behavior,
				      /* MID_ICOSP_C_SERVICEPROVIDER_OBSERVE_BEHAVIOR, */
				      j_ObservationIn);
    printf("CCCCCCCCCCCCCCCC\n");

    if (op_result != OC_STACK_OK) {
        printf("ERROR:  CallIntMethod failed for CoServiceProvider.ObserveStimulus\n");
	return OC_STACK_DELETE_TRANSACTION;
    }

    /* free the global cosp that we pinned in exhibitStimulus */
    /* (*env)->DeleteGlobalRef(env, (jobject)c_CoSP); */

    (*g_JVM)->DetachCurrentThread(g_JVM);

    printf("c_CoServiceProvider_observe_behavior EXIT\n");
    /* the stack handles this automatically so we can return anything */
    return OC_STACK_KEEP_TRANSACTION;
}

/* EXTERNAL */

/**
 * @brief Wrapper for `OCDoResource`; called by called by the user to
 * send a request to a server.
 *
 * It wraps `OCDoResource`.  The parameters to `OCDoResource` are
 * extracted from _this_CoSP_.
 *
 * @param [in] env JNIEnv pointer
 * @param [in] this_CoSP _this_ handle, reference to a `CoServiceProvider` instance
 * @return void
 *
 * @throws org.iochibity.exceptions.JNIRuntimeException Indicates a
 * bad result from a JNI API routine (e.g. `GetIntField`)
 *
 * @see observe_stimulus
 * @see Java_org_iochibity_ServiceProvider_exhibitBehavior
 * @see c_CoServiceProvider_observe_behavior
 */
JNIEXPORT void JNICALL Java_org_iochibity_CoServiceProvider_exhibitStimulus
(JNIEnv * env, jobject this_CoSP)
{
    OC_UNUSED(env);
    printf("Java_org_iochibity_CoServiceProvider_exhibitStimulus ENTRY\n");

    /* DO NOT forget that incoming java objects are local, to make
       them available in a callback from the c stack, we need to
       globalize them! */
    g_CoSP = (*env)->NewGlobalRef(env, this_CoSP);

    /* now extract the OCDoResource params */

    /* method */
    OCMethod c_method = (OCMethod)(*env)->GetIntField(env, g_CoSP, FID_COSP_METHOD);
    if (c_method == 0) {
	THROW_JNI_EXCEPTION("Method OC_REST_NOMETHOD (0) not allowed");
    	return;
    }

    /* uri */
    jobject j_uri         = (*env)->GetObjectField(env, g_CoSP, FID_COSP_URI_PATH);
    if (j_uri == NULL) {
	THROW_JNI_EXCEPTION("GetObjectField failed for FID_COSP_URI_PATH on j_StimulusOut\n");
    	return;
    }
    char* c_uri = (char*)(*env)->GetStringUTFChars(env, j_uri, NULL);
    if (c_uri == NULL) {
	THROW_JNI_EXCEPTION("GetObjectField failed for FID_RQO_URI on j_StimulusOut\n");
	return;
    }

    /* destination dev address */
    OCDevAddr* c_destDevAddr = NULL;
    jobject j_destDevAddr = NULL;
    j_destDevAddr = (*env)->GetObjectField(env, g_CoSP, FID_COSP_DESTINATION);
    if (j_destDevAddr == NULL || (*env)->IsSameObject(env, j_destDevAddr, NULL) ) {
	printf("MULTICASTING - NULL dest address\n");
	/* no devaddr with GET means DISCOVERY multicast */
	if (c_method == OC_REST_GET) {
	    c_method = OC_REST_DISCOVER;
	}
    } else {
	printf("UNICASTING\n");
	c_destDevAddr = (OCDevAddr *)OICCalloc(sizeof (OCDevAddr), 1);
	if (!c_destDevAddr) {
	    THROW_JNI_EXCEPTION("ERROR: no stack memory for OCDevAddr alloc");
	    return;
	}
	c_destDevAddr->adapter = (*env)->GetIntField(env, j_destDevAddr, FID_DA_NETWORK_PROTOCOL);
	c_destDevAddr->flags = (*env)->GetIntField(env, j_destDevAddr, FID_DA_NETWORK_POLICIES);
	printf("SEND TRANSPORT FLAGS: 0x%08X\n", c_destDevAddr->flags);
	c_destDevAddr->flags |= OC_SECURE;
	c_destDevAddr->port = (*env)->GetIntField(env, j_destDevAddr, FID_DA_PORT);
	jstring j_addr = (*env)->GetObjectField(env, j_destDevAddr, FID_DA_ADDRESS);
	if (j_addr == NULL) {
	    printf("GetObjectField failed for MID_DA_ADDRESS on j_destDevAddr\n");
	    return;
	}
	char* c_addr = (char*) (*env)->GetStringUTFChars(env, j_addr, NULL);
	strncpy(c_destDevAddr->addr, c_addr, MAX_ADDR_STR_SIZE);
	c_destDevAddr->ifindex = (*env)->GetIntField(env, j_destDevAddr, FID_DA_IFINDEX);
	printf("Destination addr: %s\n", c_addr);
	printf("Destination port: %d\n", c_destDevAddr->port);
    }

    OCStackResult ret;
    OCCallbackData cbData;

    cbData.cb = c_org_iochibity_CoServiceProvider_observe_behavior;
    cbData.context = (void*)(long)g_CoSP;
    cbData.cd = NULL;

    OCDoHandle c_handle = NULL;

    ret = OCDoResource(&c_handle,	/* OCDoHandle = void* */
    		       (OCMethod)c_method,
    		       c_uri,
    		       NULL,  /* c_destDevAddr, */
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

    /* now update the handle field */
    (*env)->SetLongField(env, g_CoSP, FID_COSP_HANDLE, (intptr_t)c_handle);

    printf("Java_org_iochibity_CoServiceProvider_exhibitStimulus EXIT\n");
    return;
}

/* /\* */
/*  * Class:     org_iochibity_CoServiceProvider */
/*  * Method:    getStimulusOut */
/*  * Signature: ()Lorg/iochibity/StimulusOut; */
/*  *\/ */
/* JNIEXPORT jobject JNICALL Java_org_iochibity_CoServiceProvider_getStimulusOut */
/* (JNIEnv * env, jobject this_CoSP) */
/* { */
/*     printf("Java_org_iochibity_CoServiceProvider_getStimulusOut ENTRY\n"); */

/*     jobject j_StimulusOut = (*env)->NewObject(env, K_MSG_REQUEST_OUT, */
/* 					      MID_RQO_CTOR, this_CoSP); */

/*     /\* set uri *\/ */
/*     jstring j_uriPath  = (*env)->GetObjectField(env, this_CoSP, FID_COSP_URI_PATH); */
/*     if (j_uriPath == NULL) { */
/* 	THROW_JNI_EXCEPTION("GetObjectField failed for FID_COSP_URI_PATH on j_StimulusOut\n"); */
/* 	return NULL; */
/*     } */
/*     (*env)->SetObjectField(env, j_StimulusOut, FID_RQO_URI_PATH, j_uriPath); */

/*     /\* set method *\/ */
/*     int c_Method = (int)(*env)->GetIntField(env, this_CoSP, FID_COSP_METHOD); */
/*     if (c_Method == 0) { */
/* 	THROW_JNI_EXCEPTION("OC_REST_NOMETHOD DISALLOWED"); */
/* 	return NULL; */
/*     } */
/*     /\* (*env)->SetObjectField(env, j_StimulusOut, FID_RQO_METHOD, c_Method); *\/ */
/*     (*env)->SetIntField(env, j_StimulusOut,  FID_RQO_METHOD, c_Method); */

/*     /\* set dest device address *\/ */
/*     jobject j_Destination = (*env)->GetObjectField(env, this_CoSP, FID_COSP_DESTINATION); */
/*     if (j_Destination == NULL) { */
/*     	printf("NULL DEST: MULTICAST\n"); */
/*     } else { */
/*     	printf("Setting Unicast destination\n"); */
/* 	(*env)->SetObjectField(env, j_StimulusOut, FID_RQO_DEST, j_Destination); */
/*     } */

/*     /\* set payload *\/ */

/*     /\* set callback (CoServiceProvider, i.e. this_CoSP) *\/ */
/*     (*env)->SetObjectField(env, j_StimulusOut, FID_RQO_CO_SERVICE_PROVIDER, this_CoSP); */


/*     /\* set context *\/ */

/*     /\* set context deleter *\/ */

/*     /\* set header options *\/ */

/*     return j_StimulusOut; */
/* } */
