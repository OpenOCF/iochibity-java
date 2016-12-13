#include <ctype.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "org_iochibity_document.h"
#include "ocf_init.h"
#include "ocf_exceptions.h"
#include "jni_utils.h"

#include "octypes.h"
#include "ocresource.h"
#include "ocstack.h"
#include "oic_malloc.h"
#include "oic_string.h"

/* EXTERNAL */

/*
 * Class:     org_iochibity_Document
 * Method:    getPDUPayload
 * Signature: ()Lorg/iochibity/PayloadList;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_Document_getPDUPayload
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    printf("Java_org_iochibity_Document_getPDUPayload ENTRY\n");
    /* if (MID_DRSPI_GET_PAYLOAD == NULL) { */
    /* 	MID_DRSPI_GET_PAYLOAD = (*env)->GetMethodID(env, K_DOC_RESPONSE_IN, */
    /* 						    "getPDUPayload", */
    /* 						    "()Lorg/iochibity/PayloadList;"); */
    /* 	if (MID_DRSPI_GET_PAYLOAD == NULL) { */
    /* 	    THROW_JNI_EXCEPTION("GetFieldID failed for 'getPDUPayload' of DocResponseIn\n"); */
    /* 	    return -1; */
    /* 	} */
    /* } */
    return NULL;
}

/*
 * Class:     org_iochibity_Document
 * Method:    getPayloadType
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_iochibity_Document_getPayloadType
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    printf("Java_org_iochibity_Document_getPayloadType ENTRY\n");
    /* 1. get payloadHandle from this */
    OCPayload* j_payload = (OCPayload*)(intptr_t)
	(*env)->GetLongField(env, this, FID_DOC_PAYLOAD_HANDLE);
    if (j_payload == NULL) {
	THROW_JNI_EXCEPTION("GetObjectField failed for FID_DOC_PAYLOAD_HANDLE on Document");
	return -1;
    }
    return j_payload->type;
}

/*
 * Class:     org_iochibity_Document
 * Method:    getOptions
 * Signature: ()Ljava/util/List;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_Document_getOptions
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    printf("Java_org_iochibity_Document_getOptions\n");
    /* if (FID_DRSPI_PTR_OPTIONS == NULL) { */
    /* 	FID_DRSPI_PTR_OPTIONS = (*env)->GetFieldID(env, K_DOC_RESPONSE_IN, "ptr_Options", "J"); */
    /* 	if (FID_DRSPI_PTR_OPTIONS == NULL) { */
    /* 	    THROW_JNI_EXCEPTION("GetFieldID failed for 'ptr_Options' of DocResponseIn\n"); */
    /* 	    return -1; */
    /* 	} */
    /* } */
    /* if (FID_DRSPI_OPTION_COUNT == NULL) { */
    /* 	FID_DRSPI_OPTION_COUNT = (*env)->GetFieldID(env, K_DOC_RESPONSE_IN, "optionCount", "I"); */
    /* 	if (FID_DRSPI_OPTION_COUNT == NULL) { */
    /* 	    THROW_JNI_EXCEPTION("GetFieldID failed for 'optionCount' of DocResponseIn\n"); */
    /* 	    return -1; */
    /* 	} */
    /* } */
    /* if (MID_DRSPI_GET_OPTIONS == NULL) { */
    /* 	MID_DRSPI_GET_OPTIONS = (*env)->GetMethodID(env, K_DOC_RESPONSE_IN, */
    /* 						    "getOptions", "()Ljava/util/List;"); */
    /* 	if (MID_DRSPI_GET_OPTIONS == NULL) { */
    /* 	    THROW_JNI_EXCEPTION("GetFieldID failed for 'getOptions' of DocResponseIn\n"); */
    /* 	    return -1; */
    /* 	} */
    /* } */
    return NULL;
}

