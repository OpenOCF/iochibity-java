#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "org_iochibity_MessageService.h"

#include "octypes.h"
#include "ocresource.h"
#include "ocstack.h"

/*
 * Class:     org_iochibity_MessageService
 * Method:    getResource
 * Signature: ()Lorg/iochibity/Resource;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_MessageService_getResource
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

    OCResourceType* resource_types = ((OCResource*)j_resource_handle)->rsrcType;
    while(resource_types) {
	/* printf("c resource type: %s\n", resource_types->resourcetypename); */
	resource_types = resource_types->next;
    }


    /* now prepare a Resource object from the OCResource struct */
    jclass resource_clazz = (*env)->FindClass(env, "Lorg/iochibity/Resource;");
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
	jstring  juri  = (*env)->NewStringUTF(env, ((OCResource*)j_resource_handle)->uri);
	if (juri != NULL) {
	    (*env)->SetObjectField(env, jresource, jurifield, juri);
	} else {
	    printf("Failed to get UTF string from c uri\n");
	}
    } else {
	printf("Failed to get resource uri fld id\n");
    }
    /* printf("org_iochibity_MessageService/getResource EXIT\n"); */
    return jresource;
}

/*
 * Class:     org_iochibity_MessageService
 * Method:    getPayload
 * Signature: ()Lorg/iochibity/Payload;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_MessageService_getPayload
(JNIEnv * env, jobject this)
{
    /* printf("Java_org_iochibity_MessageService_getPayload ENTRY\n"); */

    jclass request_in_clazz = (*env)->GetObjectClass(env, this);
    if (request_in_clazz == NULL) {
	printf("Failed to find MessageService class\n");
    } else {
	/* printf("Found MessageService class\n"); */
    }

    /* get handle to OCPayload* from MessageService */
    OCPayload* payload_handle = NULL;
    jfieldID field = (*env)->GetFieldID(env, request_in_clazz, "payloadHandle", "J");
    if (field == NULL) {
	printf("Failed to get payload handle fld id from MessageService object\n");
    } else {
	payload_handle = (void*) (*env)->GetLongField(env, this, field);
	/* printf("Found payload handle %ld from MessageService\n", (long)payload_handle); */
    }

    jobject jpayload = 0;
    if (payload_handle == NULL) {
	/* no payload */
    } else {
	/* prepare a Payload object */
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

	/* now create and populate Payload java object */
	jpayload = (*env)->NewObject(env, payload_clazz, payload_ctor);
    }
    /* printf("Java_org_iochibity_MessageService_getPayload EXIT\n"); */
    return jpayload;
}

/*
 * Class:     org_iochibity_MessageService
 * Method:    getVendorHeaderOptions
 * Signature: ()Ljava/util/ArrayList;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_MessageService_getVendorHeaderOptions
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


