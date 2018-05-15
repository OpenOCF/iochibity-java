#include "jni_init.h"

#include "org_iochibity_Exceptions.h"

#include "openocf_behavior_InboundResponse.h"
#include "openocf_utils_Endpoint.ids.h"

#define FQCN_INBOUND_RESPONSE "openocf/behavior/InboundResponse"
jclass    K_INBOUND_RESPONSE                = NULL;
jmethodID MID_INRESP_CTOR               = NULL;
jfieldID  FID_INRESP_HANDLE = NULL;
jfieldID  FID_INRESP_OBSERVATION_HANDLE = NULL;
/* jfieldID  FID_INRESP_REMOTE_DEVADDR     = NULL; */

jmethodID MID_INRESP_GET_URI = NULL;
char     *MID_INRESP_GET_URI_NM = "getUri";
char     *MID_INRESP_GET_URI_T = "()Ljava/lang/String;";

jmethodID MID_INRESP_GET_EP = NULL;
char     *MID_INRESP_GET_EP_NM = "getEndpoint";
char     *MID_INRESP_GET_EP_T = "()Lopenocf/utils/Endpoint;";

jfieldID  FID_INRESP_IS_RETAIN          = NULL;

/* jfieldID  FID_INRESP_CONN_TYPE          = NULL; */
/* jfieldID  FID_INRESP_REMOTE_SID         = NULL; */
/* jfieldID  FID_INRESP_RESULT             = NULL; */
/* jfieldID  FID_INRESP_SERIAL             = NULL; */
/* jfieldID  FID_INRESP_URI                = NULL; */
/* jfieldID  FID_INRESP_OPTIONS            = NULL; */
/* jfieldID  FID_INRESP_PTR_OPTIONS        = NULL; */
jmethodID MID_INRESP_GET_OBSERVATION    = NULL;
jmethodID MID_INRESP_GET_OPTIONS        = NULL;

int init_InboundResponse(JNIEnv* env)
{
    jclass klass;
    klass = (*env)->FindClass(env, FQCN_INBOUND_RESPONSE);
    JNI_ASSERT_NULL(klass, ERR_MSG(ERR_CLASS, FQCN_INBOUND_RESPONSE), -1);
    K_INBOUND_RESPONSE = (jclass)(*env)->NewGlobalRef(env, klass);
    (*env)->DeleteLocalRef(env, klass);

    if (MID_INRESP_CTOR == NULL) {
	MID_INRESP_CTOR = (*env)->GetMethodID(env, K_INBOUND_RESPONSE, "<init>", "()V");
	JNI_ASSERT_NULL(MID_INRESP_CTOR, ERR_MSG(ERR_METHOD, "<init> for InboundResponse"), -1);
    }

    if (FID_INRESP_IS_RETAIN == NULL) {
    	FID_INRESP_IS_RETAIN = (*env)->GetFieldID(env, K_INBOUND_RESPONSE,
						  "isRetain", "Z");
	JNI_ASSERT_NULL(FID_INRESP_IS_RETAIN,
			ERR_MSG(ERR_FLD, "InboundResponse.isRetain"), -1);
    }

    if (MID_INRESP_GET_EP == NULL) {
	MID_INRESP_GET_EP = (*env)->GetMethodID(env, K_INBOUND_RESPONSE,
						MID_INRESP_GET_EP_NM,
						MID_INRESP_GET_EP_T);
	JNI_ASSERT_NULL(MID_INRESP_GET_EP,
			ERR_MSG(ERR_METHOD, "InboundResponse.getEndpoint"), -1);
    }

    if (MID_INRESP_GET_URI == NULL) {
	MID_INRESP_GET_URI = (*env)->GetMethodID(env, K_INBOUND_RESPONSE,
						 MID_INRESP_GET_URI_NM,
						 MID_INRESP_GET_URI_T);
	JNI_ASSERT_NULL(MID_INRESP_GET_EP,
			ERR_MSG(ERR_METHOD, "InboundResponse.getURI"), -1);
    }

    /* if (MID_INRESP_GET_REMOTE_DEVADDR == NULL) { */
    /* 	MID_INRESP_GET_REMOTE_DEVADDR = (*env)->GetMethodID(env, K_INBOUND_RESPONSE, */
    /* 							      "getRemoteDeviceAddress", */
    /* 							      "()Lopenocf/Endpoint;"); */
    /* 	if (MID_INRESP_GET_REMOTE_DEVADDR == NULL) { */
    /* 	    printf("ERROR:  GetMethodID failed for getRemoteDeviceAddress for InboundResponse\n"); */
    /* 	    return -1; */
    /* 	} */
    /* } */
    /* if (FID_INRESP_CONN_TYPE == NULL) { */
    /* 	FID_INRESP_CONN_TYPE = (*env)->GetFieldID(env, K_INBOUND_RESPONSE, "connType", "I"); */
    /* 	if (FID_INRESP_CONN_TYPE == NULL) { */
    /* 	    printf("ERROR: GetFieldID failed for 'connType' of InboundResponse\n"); */
    /* 	    return -1; */
    /* 	} */
    /* } */

    /* if (FID_INRESP_REMOTE_SID == NULL) { */
    /* 	FID_INRESP_REMOTE_SID = (*env)->GetFieldID(env, K_INBOUND_RESPONSE, */
    /* 						   "_secID", "Ljava/lang/String;"); */
    /* 	JNI_ASSERT_NULL(FID_INRESP_REMOTE_SID, */
    /* 			ERR_MSG(ERR_FLD, "InboundResponse._secID"), -1); */
    /* 	/\* if (FID_INRESP_REMOTE_SID == NULL) { *\/ */
    /* 	/\*     printf("ERROR: GetFieldID failed for 'secID' of InboundResponse\n"); *\/ */
    /* 	/\*     return -1; *\/ */
    /* 	/\* } *\/ */
    /* } */
    /* if (FID_INRESP_RESULT == NULL) { */
    /* 	FID_INRESP_RESULT = (*env)->GetFieldID(env, K_INBOUND_RESPONSE, "resourceSPResult", "I"); */
    /* 	JNI_ASSERT_NULL(FID_INRESP_RESULT, */
    /* 			ERR_MSG(ERR_FLD, "InboundResponse.resourceSPResult"), -1); */
    /* } */

    /* if (FID_INRESP_SERIAL == NULL) { */
    /* 	FID_INRESP_SERIAL = (*env)->GetFieldID(env, K_INBOUND_RESPONSE, */
    /* 					       "notificationSerial", "I"); */
    /* 	JNI_ASSERT_NULL(FID_INRESP_SERIAL, */
    /* 			ERR_MSG(ERR_FLD, "InboundResponse.notificationSerial"), -1); */
    /* } */

    /* if (FID_INRESP_URI == NULL) { */
    /* 	FID_INRESP_URI = (*env)->GetFieldID(env, K_INBOUND_RESPONSE, "_uriPath", "Ljava/lang/String;"); */
    /* 	if (FID_INRESP_URI == NULL) { */
    /* 	    printf("ERROR: GetFieldID failed for 'uri' of InboundResponse\n"); */
    /* 	    return -1; */
    /* 	} */
    /* } */
    /* if (MID_INRESP_GET_OBSERVATION == NULL) { */
    /* 	MID_INRESP_GET_OBSERVATION = (*env)->GetMethodID(env, K_INBOUND_RESPONSE, */
    /* 						    "getPDUObservation", */
    /* 						    "()Lopenocf/ObservationList;"); */
    /* 	if (MID_INRESP_GET_OBSERVATION == NULL) { */
    /* 	    printf("ERROR: GetFieldID failed for 'getPDUObservation' of InboundResponse\n"); */
    /* 	    return -1; */
    /* 	} */
    /* } */
    /* if (FID_INRESP_PTR_OPTIONS == NULL) { */
    /* 	FID_INRESP_PTR_OPTIONS = (*env)->GetFieldID(env, K_INBOUND_RESPONSE, "ptr_Options", "J"); */
    /* 	if (FID_INRESP_PTR_OPTIONS == NULL) { */
    /* 	    printf("ERROR: GetFieldID failed for 'ptr_Options' of InboundResponse\n"); */
    /* 	    return -1; */
    /* 	} */
    /* } */
    /* if (FID_INRESP_OPTION_COUNT == NULL) { */
    /* 	FID_INRESP_OPTION_COUNT = (*env)->GetFieldID(env, K_INBOUND_RESPONSE, "optionCount", "I"); */
    /* 	if (FID_INRESP_OPTION_COUNT == NULL) { */
    /* 	    printf("ERROR: GetFieldID failed for 'optionCount' of InboundResponse\n"); */
    /* 	    return -1; */
    /* 	} */
    /* } */
    /* if (MID_INRESP_GET_OPTIONS == NULL) { */
    /* 	MID_INRESP_GET_OPTIONS = (*env)->GetMethodID(env, K_INBOUND_RESPONSE, */
    /* 						    "getOptions", "()Ljava/util/List;"); */
    /* 	if (MID_INRESP_GET_OPTIONS == NULL) { */
    /* 	    printf("ERROR: GetFieldID failed for 'getOptions' of InboundResponse\n"); */
    /* 	    return -1; */
    /* 	} */
    /* } */
    return 0;
}

/* /\* */
/*  * Class:     openocf_app_CoResourceSP */
/*  * Method:    getUri */
/*  * Signature: ()Ljava/lang/String; */
/*  *\/ */
/* JNIEXPORT jstring JNICALL */
/* Java_openocf_app_CoResourceSP_getUri(JNIEnv * env, jobject this) */

/*
 * Class:     openocf_behavior_InboundResponse
 * Method:    getUri
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_openocf_behavior_InboundResponse_getUri(JNIEnv *env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    jstring j_uri = (*env)->NewStringUTF(env, tls_response_in->response->resourceUri);
    return  j_uri;
}

/*
 * Class:     openocf_behavior_InboundResponse
 * Method:    getMethod
 * Signature: ()I
 *
 * InboundResponse does not contain method.  The method will be
 * implicit in the coReact callback, _if_ we use a different callback
 * for each method.  But that is not required; we could in principle
 * use the same callback for different operations _if_ we could pull
 * the method from the inbound response. So our only option is to find
 * the request associated with this response.
 */
JNIEXPORT jint JNICALL
Java_openocf_behavior_InboundResponse_getMethod(JNIEnv *env, jobject this)
{
    OCEntityHandlerRequest *handle = (OCEntityHandlerRequest*)
	(*env)->GetLongField(env, this, FID_INBOUND_STIMULUS_HANDLE);
    return (jint) handle->method;
}


/*
 * Class:     openocf_behavior_InboundResponse
 * Method:    getEndpoint
 * Signature: ()Lopenocf/utils/Endpoint;
 */
JNIEXPORT jobject JNICALL
Java_openocf_behavior_InboundResponse_getEndpoint(JNIEnv *env , jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    printf("InboundReponse.getEndpoint ENTRY\n");

    OCDevAddr *ep_handle = &(tls_response_in->response->devAddr);

    jobject j_Endpoint = (*env)->NewObject(env, K_ENDPOINT, MID_EP_CTOR);
    if (j_Endpoint == NULL) {
	THROW_JNI_EXCEPTION("Endpoint() (ctor)");;
    }

    (*env)->SetLongField(env, j_Endpoint,
			 FID_EP_HANDLE,
			 (long)(intptr_t)ep_handle);
    if ((*env)->ExceptionCheck(env)) {
	(*env)->ExceptionDescribe(env);
    }
    return j_Endpoint;
}

/*
 * Class:     openocf_behavior_InboundResponse
 * Method:    getOptions
 * Signature: ()Ljava/util/List;
 */
JNIEXPORT jobject JNICALL
Java_openocf_behavior_InboundResponse_getOptions(JNIEnv *env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    return NULL;
}


