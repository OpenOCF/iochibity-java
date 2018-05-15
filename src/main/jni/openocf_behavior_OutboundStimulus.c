#include "jni_init.h"

#include "org_iochibity_Exceptions.h"

#include "openocf_behavior_OutboundStimulus.h"
#include "openocf_utils_Endpoint.ids.h"

int init_OutboundStimulus(JNIEnv* env)
{
    jclass klass;
    klass = (*env)->FindClass(env, "openocf/OutboundStimulus");
    JNI_ASSERT_NULL(klass, "FindClass failed for openocf/OutboundStimulus\n", 0);
    K_OUTBOUND_STIMULUS = (jclass)(*env)->NewGlobalRef(env, klass);
    (*env)->DeleteLocalRef(env, klass);

    if (MID_RQO_CTOR == NULL) {
	MID_RQO_CTOR = (*env)->GetMethodID(env, K_OUTBOUND_STIMULUS,
					   "<init>", "(Lopenocf/app/ICoResourceSP;)V");
	if (MID_RQO_CTOR == NULL) {
	    printf("ERROR:  GetMethodID failed for ctor for OutboundStimulus\n");
	    return OC_EH_INTERNAL_SERVER_ERROR;
	}
    }
    if (FID_RQO_LOCAL_HANDLE == NULL) {
	FID_RQO_LOCAL_HANDLE = (*env)->GetFieldID(env, K_OUTBOUND_STIMULUS, "_localHandle", "J");
	if (FID_RQO_LOCAL_HANDLE == NULL) {
	    printf("ERROR: GetFieldID failed for '_localHandle' of OutboundStimulus\n");
	    return OC_EH_INTERNAL_SERVER_ERROR;
	}
    }
    /* if (FID_RQO_CORESOURCE_SP == NULL) { */
    /* 	FID_RQO_CORESOURCE_SP = (*env)->GetFieldID(env, K_OUTBOUND_STIMULUS, */
    /* 						       "coResourceSP", */
    /* 						       "Lopenocf/app/ICoResourceSP;"); */
    /* 	if (FID_RQO_CORESOURCE_SP == NULL) { */
    /* 	    printf("ERROR: GetFieldID failed for 'coResourceSP' of OutboundStimulus\n"); */
    /* 	    return OC_EH_INTERNAL_SERVER_ERROR; */
    /* 	} */
    /* } */
    if (FID_RQO_METHOD == NULL) {
	FID_RQO_METHOD = (*env)->GetFieldID(env, K_OUTBOUND_STIMULUS, "_method", "I");
	if (FID_RQO_METHOD == NULL) {
	    printf("ERROR: GetFieldID failed for '_method' of OutboundStimulus\n");
	    return OC_EH_INTERNAL_SERVER_ERROR;
	}
    }
    /* if (FID_RQO_URI_PATH == NULL) { */
    /* 	FID_RQO_URI_PATH = (*env)->GetFieldID(env, K_OUTBOUND_STIMULUS, */
    /* 					      "_uriPath", */
    /* 					      "Ljava/lang/String;"); */
    /* 	if (FID_RQO_URI_PATH == NULL) { */
    /* 	    printf("ERROR: GetFieldID failed for '_uriPath' of OutboundStimulus\n"); */
    /* 	    return OC_EH_INTERNAL_SERVER_ERROR; */
    /* 	} */
    /* } */
    if (FID_RQO_DEST == NULL) {
	FID_RQO_DEST = (*env)->GetFieldID(env, K_OUTBOUND_STIMULUS,
					  "_ep",
					  FQCS_ENDPOINT);
	if (FID_RQO_DEST == NULL) {
	    printf("ERROR: GetFieldID failed for '_ep' of OutboundStimulus\n");
	    return OC_EH_INTERNAL_SERVER_ERROR;
	}
    }
    return 0;
}
