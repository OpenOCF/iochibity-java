#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "org_iochibity_Resource.h"

#include "octypes.h"
#include "ocresource.h"
#include "ocstack.h"

/*
 * Class:     org_iochibity_Resource
 * Method:    getTypes
 * Signature: ()Ljava/util/LinkedList;
 */
JNIEXPORT jobject /*LinkedList<String> */ JNICALL Java_org_iochibity_Resource_getTypes
  (JNIEnv * env, jobject j_this)
{
    printf("Java_org_iochibity_Resource_getTypes ENTRY\n");

    //     OCResourceType *rsrcType;

    jclass j_clazz = NULL;
    jlong j_resource_handle = 0;
    OCResource* c_resource_handle = NULL;
    j_clazz = (*env)->GetObjectClass(env, j_this);
    if (j_clazz != NULL) {
	printf("Got clazz for Resource\n");
	/* 1. get handle to OCResource */
        jfieldID handle_field = (*env)->GetFieldID(env, j_clazz, "handle", "J");
        if (handle_field == NULL) {
	    printf("Failed to get handle fld for Resource\n");
	    return NULL;
	} else {
	    printf("Got handle fld for Resource\n");
    	    j_resource_handle = (*env)->GetLongField(env, j_this, handle_field);
    	    printf("HANDLE: %ld\n", j_resource_handle);
	    c_resource_handle = (OCResource*) j_resource_handle;
	    printf("c resource uri: %s\n", c_resource_handle->uri);
	}
	/* stack/ocresource.h:
	   typedef struct resourcetype_t {
	     struct resourcetype_t *next;
	     char *resourcetypename;
	   } OCResourceType;
	*/

    } else {
	printf("Failed to get handle fld for Resource\n");
    }

    /* create LinkedList for types */
    jclass ll_clazz = (*env)->FindClass(env, "java/util/LinkedList");
    if (ll_clazz == 0) {
	printf("Find class method Failed.\n");
    }else {
	/* printf("Found class method succeeded.\n"); */
    }

    jmethodID ctor = (*env)->GetMethodID(env, ll_clazz, "<init>", "()V");
    if (ctor == 0) {
	printf("Find ctor method Failed.\n");
    }else {
	/* printf("Found ctor method succeeded.\n"); */
    }
    jobject ll  = (*env)->NewObject(env, ll_clazz, ctor);
    if (ll == NULL) {
	printf("NewObject failed for LinkedList\n");
    } else {
	jmethodID j_add = (*env)->GetMethodID(env, ll_clazz, "add", "(Ljava/lang/Object;)Z");
	if (j_add == NULL) {
	    printf("Find add method Failed.\n");
	}else {
	    /* printf("Found add method succeeded.\n"); */
	}

	OCResourceType* resource_types = ((OCResource*)j_resource_handle)->rsrcType;
	jstring jresource_type;
	while(resource_types) {
	    printf("c resource type: %s\n", resource_types->resourcetypename);
	    jresource_type = (*env)->NewStringUTF(env, resource_types->resourcetypename);
	    jboolean jb = (*env)->CallBooleanMethod(env, ll, j_add, jresource_type);
	    if (!jb) {
		printf("CallBooleanMethod Failed.\n");
	    }
	    resource_types = resource_types->next;
    	}
    }
    return ll;
}

/*
 * Class:     org_iochibity_Resource
 * Method:    getInterfaces
 * Signature: ()Ljava/util/LinkedList;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_Resource_getInterfaces
(JNIEnv * env, jobject j_this)
{
    printf("Java_org_iochibity_Resource_getInterfaces ENTRY\n");

    //     OCResourceInterface *rsrcInterface;

    jclass j_clazz = NULL;
    jlong j_resource_handle = 0;
    OCResource* c_resource_handle = NULL;
    j_clazz = (*env)->GetObjectClass(env, j_this);
    if (j_clazz != NULL) {
	printf("Got clazz for Resource\n");
	/* 1. get handle to OCResource */
        jfieldID handle_field = (*env)->GetFieldID(env, j_clazz, "handle", "J");
        if (handle_field == NULL) {
	    printf("Failed to get handle fld for Resource\n");
	    return NULL;
	} else {
	    printf("Got handle fld for Resource\n");
    	    j_resource_handle = (*env)->GetLongField(env, j_this, handle_field);
    	    printf("HANDLE: %ld\n", j_resource_handle);
	    c_resource_handle = (OCResource*) j_resource_handle;
	    printf("c resource uri: %s\n", c_resource_handle->uri);
	}
	/* stack/ocresource.h:
	   typedef struct resourcetype_t {
	     struct resourcetype_t *next;
	     char *resourcetypename;
	   } OCResourceType;
	*/

    } else {
	printf("Failed to get handle fld for Resource\n");
    }

    /* create LinkedList for types */
    jclass ll_clazz = (*env)->FindClass(env, "java/util/LinkedList");
    if (ll_clazz == 0) {
	printf("Find class method Failed.\n");
    }else {
	/* printf("Found class method succeeded.\n"); */
    }

    jmethodID ctor = (*env)->GetMethodID(env, ll_clazz, "<init>", "()V");
    if (ctor == 0) {
	printf("Find ctor method Failed.\n");
    }else {
	/* printf("Found ctor method succeeded.\n"); */
    }
    jobject ll  = (*env)->NewObject(env, ll_clazz, ctor);
    if (ll == NULL) {
	printf("NewObject failed for LinkedList\n");
    } else {
	jmethodID j_add = (*env)->GetMethodID(env, ll_clazz, "add", "(Ljava/lang/Object;)Z");
	if (j_add == NULL) {
	    printf("Find add method Failed.\n");
	}else {
	    /* printf("Found add method succeeded.\n"); */
	}

	/* stack/ocresource.h: */
	/* typedef struct resourceinterface_t { */
	/*     struct resourceinterface_t *next; */
	/*     char *name ; */
	/* } OCResourceInterface; */

	OCResourceInterface* resource_ifs = ((OCResource*)j_resource_handle)->rsrcInterface;
	jstring jresource_if;
	while(resource_ifs) {
	    printf("c resource interface: %s\n", resource_ifs->name);
	    jresource_if = (*env)->NewStringUTF(env, resource_ifs->name);
	    jboolean jb = (*env)->CallBooleanMethod(env, ll, j_add, jresource_if);
	    if (!jb) {
		printf("CallBooleanMethod Failed.\n");
	    }
	    resource_ifs = resource_ifs->next;
    	}
    }
    return ll;
}

/*
 * Class:     org_iochibity_Resource
 * Method:    getProperties
 * Signature: ()Ljava/util/LinkedList;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_Resource_getProperties
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
}

/*
 * Class:     org_iochibity_Resource
 * Method:    getChildren
 * Signature: ()Ljava/util/LinkedList;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_Resource_getChildren
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
}

/*
 * Class:     org_iochibity_Resource
 * Method:    getServiceProvider
 * Signature: ()Lorg/iochibity/ResourceServiceProvider;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_Resource_getServiceProvider
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
}

/*
 * Class:     org_iochibity_Resource
 * Method:    getCallbackParam
 * Signature: ()Ljava/lang/Object;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_Resource_getCallbackParam
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
}

/*
 * Class:     org_iochibity_Resource
 * Method:    getPolicies
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_iochibity_Resource_getPolicies
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
}


