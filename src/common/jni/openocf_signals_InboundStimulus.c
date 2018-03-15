#include "jni_init.h"

#include "org_iochibity_Exceptions.h"

#include "openocf_signals_InboundStimulus.h"

int init_InboundStimulus(JNIEnv* env)
{
    jclass klass;
    klass = (*env)->FindClass(env, "openocf/InboundStimulus");
    JNI_ASSERT_NULL(klass, "FindClass failed for openocf/InboundStimulus\n", 0);
    K_INBOUND_STIMULUS = (jclass)(*env)->NewGlobalRef(env, klass);
    (*env)->DeleteLocalRef(env, klass);

    if (MID_INBOUND_STIMULUS_CTOR == NULL) {
	MID_INBOUND_STIMULUS_CTOR = (*env)->GetMethodID(env, K_INBOUND_STIMULUS, "<init>", "()V");
	if (MID_INBOUND_STIMULUS_CTOR == NULL) {
	    printf("ERROR:  GetMethodID failed for ctor for InboundStimulus");
	    return OC_EH_INTERNAL_SERVER_ERROR;
	}
    }
    if (FID_INBOUND_STIMULUS_LOCAL_HANDLE == NULL) {
    	FID_INBOUND_STIMULUS_LOCAL_HANDLE = (*env)->GetFieldID(env, K_INBOUND_STIMULUS, "_localHandle", "J");
    	if (FID_INBOUND_STIMULUS_LOCAL_HANDLE == NULL) {
    	    printf("ERROR: GetFieldID failed for '_localHandle' of InboundStimulus\n");
    	    return OC_EH_INTERNAL_SERVER_ERROR;
    	}
    }
    if (FID_INBOUND_STIMULUS_WATCH_ACTION == NULL) {
    	FID_INBOUND_STIMULUS_WATCH_ACTION = (*env)->GetFieldID(env, K_INBOUND_STIMULUS, "watchAction", "I");
    	if (FID_INBOUND_STIMULUS_WATCH_ACTION == NULL) {
    	    printf("ERROR: GetFieldID failed for 'watchAction' of InboundStimulus\n");
    	    return OC_EH_INTERNAL_SERVER_ERROR;
    	}
    }
    if (FID_INBOUND_STIMULUS_WATCH_ID == NULL) {
    	FID_INBOUND_STIMULUS_WATCH_ID = (*env)->GetFieldID(env, K_INBOUND_STIMULUS, "watchId", "I");
    	if (FID_INBOUND_STIMULUS_WATCH_ID == NULL) {
    	    printf("ERROR: GetFieldID failed for 'watchId' of InboundStimulus\n");
    	    return OC_EH_INTERNAL_SERVER_ERROR;
    	}
    }

    /* method */
    if (FID_INBOUND_STIMULUS_METHOD == NULL) {
	FID_INBOUND_STIMULUS_METHOD = (*env)->GetFieldID(env, K_INBOUND_STIMULUS, "_method", "I");
	if (FID_INBOUND_STIMULUS_METHOD == NULL) {
	    printf("ERROR:  GetFieldID failed for '_method' of InboundStimulus\n");
    	return OC_EH_INTERNAL_SERVER_ERROR;
	}
    }
    /* if (MID_INBOUND_STIMULUS_GET_METHOD == NULL) { */
    /* 	MID_INBOUND_STIMULUS_GET_METHOD = (*env)->GetMethodID(env, K_INBOUND_STIMULUS, "getMethod", "I"); */
    /* 	if (MID_INBOUND_STIMULUS_GET_METHOD == NULL) { */
    /* 	    printf("ERROR:  GetMethodID failed for 'getMethod' for InboundStimulus\n"); */
    /* 	    return -1; */
    /* 	} */
    /* } */

    /* query */
    if (FID_INBOUND_STIMULUS_QUERY == NULL) {
	FID_INBOUND_STIMULUS_QUERY = (*env)->GetFieldID(env, K_INBOUND_STIMULUS, "_query", "Ljava/lang/String;");
	if (FID_INBOUND_STIMULUS_QUERY == NULL) {
	    printf("ERROR:  GetFieldID failed for '_query' of InboundStimulus\n");
	    return OC_EH_INTERNAL_SERVER_ERROR;
	}
    }
    if (MID_INBOUND_STIMULUS_GET_QUERY == NULL) {
	MID_INBOUND_STIMULUS_GET_QUERY = (*env)->GetMethodID(env, K_INBOUND_STIMULUS,
						"getQueryString", "()Ljava/lang/String;");
	if (MID_INBOUND_STIMULUS_GET_QUERY == NULL) {
	    printf("ERROR:  GetMethodID failed for 'getQueryString' for InboundStimulus\n");
	    return -1;
	}
    }


    /* remote device address - handled by Message*/


    /* msg id */
    if (FID_INBOUND_STIMULUS_MSG_ID == NULL) {
	FID_INBOUND_STIMULUS_MSG_ID = (*env)->GetFieldID(env, K_INBOUND_STIMULUS, "_messageId", "I");
	if (FID_INBOUND_STIMULUS_MSG_ID == NULL) {
	    printf("ERROR:  GetFieldID failed for '_messageId' of InboundStimulus\n");
	    return OC_EH_INTERNAL_SERVER_ERROR;
	}
    }
    /* if (MID_INBOUND_STIMULUS_GET_MSG_ID == NULL) { */
    /* 	MID_INBOUND_STIMULUS_GET_MSG_ID = (*env)->GetMethodID(env, K_INBOUND_STIMULUS, */
    /* 							 "getRemoteDeviceAddress", */
    /* 							 "()Lopenocf/EndPoint;"); */
    /* 	if (MID_INBOUND_STIMULUS_GET_MSG_ID == NULL) { */
    /* 	    printf("ERROR:  GetMethodID failed for getRemoteDeviceAddress for InboundStimulus\n"); */
    /* 	    return -1; */
    /* 	} */
    /* } */

    return 0;
}
