#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "org_iochibity_DocForServiceProvider.h"
#include "jni_utils.h"
#include "ocf_exceptions.h"

#include "octypes.h"
#include "ocresource.h"
#include "ocstack.h"

/*
 * Class:     org_iochibity_DocForServiceProvider
 * Method:    getResource
 * Signature: ()Lorg/iochibity/ResourceLocal;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_DocForServiceProvider_getResource
(JNIEnv * env, jobject this)
{
    /* printf("org_iochibity_MessageService/getResource ENTRY\n"); */

    jclass request_in_clazz = (*env)->GetObjectClass(env, this);
    if (request_in_clazz == NULL) {
	printf("Failed to find MessageService class\n");
    } else {
	/* printf("Found MessageService class\n"); */
    }

    /* get handle to OCResource from MessageService */
    jlong j_resource_handle = 0;
    jfieldID resource_field = (*env)->GetFieldID(env, request_in_clazz, "resourceHandle", "J");
    if (resource_field == NULL) {
	printf("Failed to get handle fld id from request\n");
    } else {
	j_resource_handle = (*env)->GetLongField(env, this, resource_field);
	/* printf("Got resource handle %ld from request\n", (long)j_resource_handle); */
    }

    OCResourceType* resource_types = ((OCResource*)(intptr_t)j_resource_handle)->rsrcType;
    while(resource_types) {
	/* printf("c resource type: %s\n", resource_types->resourcetypename); */
	resource_types = resource_types->next;
    }


    /* now prepare a Resource object from the OCResource struct */
    jclass resource_clazz = (*env)->FindClass(env, "Lorg/iochibity/ResourceLocal;");
    if (resource_clazz == NULL) {
	printf("Failed to find Resource class\n");
    } else {
	/* printf("Found Resource class\n"); */
    }

    jmethodID resource_ctor = (*env)->GetMethodID(env, resource_clazz, "<init>", "()V");
    if (resource_ctor == NULL) {
	printf("Failed to find ctor method\n");
    } else {
	/* printf("Found ctor\n"); */
    }

    /* create and populate Resource java object */
    jobject jresource = (*env)->NewObject(env, resource_clazz, resource_ctor);
    jfieldID j_handle_field = (*env)->GetFieldID(env, resource_clazz, "handle", "J");
    if (j_handle_field != NULL) {
	(*env)->SetLongField(env, jresource, j_handle_field, j_resource_handle);
    } else {
	printf("Failed to get handle fld for new Resource\n");
    }
    /* printf("c resource uri: %s\n", ((OCResource*)j_resource_handle)->uri); */
    jfieldID jurifield = (*env)->GetFieldID(env, resource_clazz, "uri", "Ljava/lang/String;");
    if (jurifield != NULL) {
	jstring  juri  = (*env)->NewStringUTF(env, ((OCResource*)(intptr_t)j_resource_handle)->uri);
	if (juri != NULL) {
	    (*env)->SetObjectField(env, jresource, jurifield, juri);
	} else {
	    printf("Failed to get UTF string from c uri\n");
	}
    } else {
	printf("Failed to get resource uri fld id\n");
    }

    jfieldID fid_policies = (*env)->GetFieldID(env, resource_clazz, "policies", "I");
    if (fid_policies != NULL) {
	(*env)->SetIntField(env, jresource, fid_policies,
			    (jint)((OCResource*)(intptr_t)j_resource_handle)->resourceProperties);
    } else {
	printf("Failed to find policies field id\n");
    }


    /* printf("org_iochibity_MessageService/getResource EXIT\n"); */
    return jresource;
}

/*
 * Class:     org_iochibity_DocForServiceProvider
 * Method:    getPDUPayload
 * Signature: ()Lorg/iochibity/PayloadList;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_DocForServiceProvider_getPDUPayload
(JNIEnv * env, jobject this)
{
    /* printf("Java_org_iochibity_MessageService_getPayload ENTRY\n"); */



    /* 1. create a linked list */
    jclass ll_clazz = (*env)->FindClass(env, "org/iochibity/PayloadList");
    if (ll_clazz == 0) {
	printf("Find class method Failed.\n");
    }
    jmethodID ll_ctor = (*env)->GetMethodID(env, ll_clazz, "<init>", "()V");
    if (ll_ctor == 0) {
	printf("Find ctor method for ll Failed.\n");
    }
    jobject ll  = (*env)->NewObject(env, ll_clazz, ll_ctor);
    if (ll == NULL) {
	printf("NewObject failed for LinkedList\n");
	/* exit */
    }
    jmethodID j_add = (*env)->GetMethodID(env, ll_clazz, "add", "(Ljava/lang/Object;)Z");
    if (j_add == NULL) {
	printf("Find add method Failed.\n");
    }else {
	/* printf("Found add method succeeded.\n"); */
    }

    /* 3.  get reeady to create java Payload objects */
    jclass request_in_clazz = (*env)->GetObjectClass(env, this);
    if (request_in_clazz == NULL) {
	printf("Failed to find MessageService class\n");
    } else {
	/* printf("Found MessageService class\n"); */
    }

    /* get handle to OCPayload* from MessageService */
    OCPayload* c_payload = NULL;
    jfieldID field = (*env)->GetFieldID(env, request_in_clazz, "payloadHandle", "J");
    if (field == NULL) {
	printf("Failed to get payload handle fld id from MessageService object\n");
    } else {
	c_payload = (OCPayload*)(intptr_t) (*env)->GetLongField(env, this, field);
	/* printf("Found payload handle %ld from MessageService\n", (long)c_payload); */
    }

    jobject jpayload = 0;
    /* prepare for Payload objects */
    /* for this to work we must test the type of each Payload obj
       (OCPayload*) and create the appropriate java object */
    switch(c_payload->type) {
	// OCPayloadType enum
    case PAYLOAD_TYPE_INVALID:
	break;
    case PAYLOAD_TYPE_DISCOVERY:
	break;
    case PAYLOAD_TYPE_DEVICE:
	break;
    case PAYLOAD_TYPE_PLATFORM:
	break;
    case PAYLOAD_TYPE_REPRESENTATION:
	break;
    case PAYLOAD_TYPE_SECURITY:
	break;
    case PAYLOAD_TYPE_PRESENCE:
	break;
    /* case PAYLOAD_TYPE_RD: */
    default:
    	break;
    }


    jclass payload_clazz = (*env)->FindClass(env, "Lorg/iochibity/Payload;");
    if (payload_clazz == NULL) {
	printf("Failed to find Payload class\n");
    } else {
	/* printf("Found Payload class\n"); */
    }

    jmethodID payload_ctor = (*env)->GetMethodID(env, payload_clazz, "<init>", "()V");
    if (payload_ctor == NULL) {
	printf("Failed to find ctor method for Payload class\n");
    } else {
	/* printf("Found Payload ctor\n"); */
    }

    /* now create and populate one Payload java object for each OCPayload */
    while(c_payload) {
	
	jobject j_payload = (*env)->NewObject(env, payload_clazz, payload_ctor);
	if (j_payload == NULL) {
	    THROW_JNI_EXCEPTION("NewObject failed for Payload\n");
	}
	/* populate it */
    }
    /* printf("Java_org_iochibity_MessageService_getPayload EXIT\n"); */
    return jpayload;
}

/*
 * Class:     org_iochibity_DocForServiceProvider
 * Method:    getVendorHeaderOptions
 * Signature: ()Ljava/util/ArrayList;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_DocForServiceProvider_getVendorHeaderOptions
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    /* printf("Java_org_iochibity_MessageService_getVendorHeaderOptions ENTRY\n"); */

    /* first create an ArrayList */
    jclass al_clazz = (*env)->FindClass(env, "java/util/ArrayList");
    if (al_clazz == 0) {
	printf("Find ArrayList class method Failed.\n");
    }else {
	/* printf("Found class method succeeded.\n"); */
    }

    jmethodID ctor = (*env)->GetMethodID(env, al_clazz, "<init>", "()V");
    if (ctor == 0) {
	printf("Find ctor method Failed.\n");
    }else {
	/* printf("Found ctor method succeeded.\n"); */
    }
    jobject al  = (*env)->NewObject(env, al_clazz, ctor);
    if (al == NULL) {
	printf("NewObject failed for ArrayList\n");
    } else {
	jmethodID j_add = (*env)->GetMethodID(env, al_clazz, "add", "(Ljava/lang/Object;)Z");
	if (j_add == NULL) {
	    printf("Find add method Failed.\n");
	}else {
	    /* printf("Found add method succeeded.\n"); */
	}
    }

    /* now get the c data */

    /* for (int i = 0; i < n; i++) { */
    /* 	rcvdVendorSpecificHeaderOptions[i]; */
    /* } */

    /* printf("Java_org_iochibity_MessageService_getVendorHeaderOptions EXIT\n"); */
    return al;
}


