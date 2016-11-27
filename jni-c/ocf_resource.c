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
    printf("getTypes called\n");
    jclass j_clazz = (*env)->GetObjectClass(env, j_this);
    if (j_clazz != NULL) { /* cannot be null in this case */

	/* 1. get handle to OCResource */
        jfieldID handle_field = (*env)->GetFieldID(env, j_clazz, "handle", "J");
        if (handle_field != NULL) { /* make sure we got the field */
    	    jlong j_resource_handle = (*env)->GetLongField(env, j_this, handle_field);
    	    printf("HANDLE: %ld\n", j_resource_handle);
	    printf("resource uri: %s\n", ((OCResource*)j_resource_handle)->uri);
    	}
    }

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

    jmethodID j_add = (*env)->GetMethodID(env, ll_clazz, "add", "(Ljava/lang/Object;)Z");
    if (j_add == 0) {
	printf("Find add method Failed.\n");
    }else {
	/* printf("Found add method succeeded.\n"); */
    }

    jobject ll  = (*env)->NewObject(env, ll_clazz, ctor);
    jstring jString = (*env)->NewStringUTF(env, "Foo");
    jboolean b = (*env)->CallBooleanMethod(env, ll, j_add, jString);

    /* OCResourceType* resource_types = ((OCResource*)j_resource_handle)->rsrcType; */

    return ll;
}

