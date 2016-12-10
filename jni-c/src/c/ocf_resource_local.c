#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "org_iochibity_ResourceLocal.h"
#include "org_iochibity_ResourceLocal_InstanceId.h"
#include "jni_utils.h"
#include "ocf_exceptions.h"

#include "octypes.h"
#include "ocpayload.h"
#include "ocresource.h"
#include "ocresourcehandler.h"
#include "ocstack.h"

/* INTERNAL */

jlong getResourceHandle(JNIEnv* env, jobject this)
{
    /* first get handle to OCResource struct */
    jclass j_clazz = NULL;
    jlong j_resource_handle = 0;
    j_clazz = (*env)->GetObjectClass(env, this);
    if (j_clazz != NULL) {
	/* 1. get handle to OCResource */
        jfieldID fid_resource_handle = (*env)->GetFieldID(env, j_clazz, "handle", "J");
        if (fid_resource_handle == NULL) {
	    printf("Failed to get handle fld for Resource\n");
	    return 0;
	} else {
	    /* printf("Got handle fld for Resource\n"); */
    	    j_resource_handle = (*env)->GetLongField(env, this, fid_resource_handle);
    	    /* printf("HANDLE: %ld\n", j_resource_handle); */
	    /* c_resource_handle = (OCResource*) j_resource_handle; */
	    /* printf("c resource uri: %s\n", c_resource_handle->uri); */
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
    return j_resource_handle;
}

/*
 * Class:     org_iochibity_ResourceLocal
 * Method:    getTypes
 * Signature: ()Ljava/util/LinkedList;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_ResourceLocal_getTypes
(JNIEnv * env, jobject this)
{
    /* printf("Java_org_iochibity_ResourceLocal_getTypes ENTRY\n"); */
    //     OCResourceType *rsrcType;

    jclass j_clazz = NULL;
    jlong j_resource_handle = 0;
    /* OCResource* c_resource_handle = NULL; */
    j_clazz = (*env)->GetObjectClass(env, this);
    if (j_clazz != NULL) {
	/* printf("Got clazz for Resource\n"); */
	/* 1. get handle to OCResource */
        jfieldID handle_field = (*env)->GetFieldID(env, j_clazz, "handle", "J");
        if (handle_field == NULL) {
	    printf("Failed to get handle fld for Resource\n");
	    return NULL;
	} else {
	    /* printf("Got handle fld for Resource\n"); */
    	    j_resource_handle = (*env)->GetLongField(env, this, handle_field);
    	    /* printf("HANDLE: %ld\n", j_resource_handle); */
	    /* c_resource_handle = (OCResource*)(intptr_t) j_resource_handle; */
	    /* printf("c resource uri: %s\n", c_resource_handle->uri); */
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

	OCResourceType* resource_types = ((OCResource*)(intptr_t) j_resource_handle)->rsrcType;
	jstring j_resource_type;
	while(resource_types) {
	    /* printf("c resource type: %s\n", resource_types->resourcetypename); */
	    j_resource_type = (*env)->NewStringUTF(env, resource_types->resourcetypename);
	    jboolean jb = (*env)->CallBooleanMethod(env, ll, j_add, j_resource_type);
	    if (!jb) {
		printf("CallBooleanMethod Failed.\n");
	    }
	    resource_types = resource_types->next;
    	}
    }
    /* printf("Java_org_iochibity_ResourceLocal_getTypes EXIT\n"); */
    return ll;
}
/*
 * Class:     org_iochibity_ResourceLocal
 * Method:    getInterfaces
 * Signature: ()Ljava/util/LinkedList;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_ResourceLocal_getInterfaces
(JNIEnv * env, jobject this)
{
    /* printf("Java_org_iochibity_ResourceLocal_getInterfaces ENTRY\n"); */

    //     OCResourceInterface *rsrcInterface;

    jclass j_clazz = NULL;
    jlong j_resource_handle = 0;
    /* OCResource* c_resource_handle = NULL; */
    j_clazz = (*env)->GetObjectClass(env, this);
    if (j_clazz != NULL) {
	/* printf("Got clazz for Resource\n"); */
	/* 1. get handle to OCResource */
        jfieldID handle_field = (*env)->GetFieldID(env, j_clazz, "handle", "J");
        if (handle_field == NULL) {
	    printf("Failed to get handle fld for Resource\n");
	    return NULL;
	} else {
	    /* printf("Got handle fld for Resource\n"); */
    	    j_resource_handle = (*env)->GetLongField(env, this, handle_field);
    	    /* printf("HANDLE: %ld\n", j_resource_handle); */
	    /* c_resource_handle = (OCResource*)(intptr_t) j_resource_handle; */
	    /* printf("c resource uri: %s\n", c_resource_handle->uri); */
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
	/* exit? */
    }
    jmethodID j_add = (*env)->GetMethodID(env, ll_clazz, "add", "(Ljava/lang/Object;)Z");
    if (j_add == NULL) {
	printf("Find add method Failed.\n");
	/* exit? */
    }

    /* stack/ocresource.h: */
    /* typedef struct resourceinterface_t { */
    /*     struct resourceinterface_t *next; */
    /*     char *name ; */
    /* } OCResourceInterface; */

    OCResourceInterface* resource_ifs = ((OCResource*)(intptr_t)j_resource_handle)->rsrcInterface;
    jstring j_resource_if;
    while(resource_ifs) {
	/* printf("c resource interface: %s\n", resource_ifs->name); */
	j_resource_if = (*env)->NewStringUTF(env, resource_ifs->name);
	jboolean jb = (*env)->CallBooleanMethod(env, ll, j_add, j_resource_if);
	if (!jb) {
	    printf("CallBooleanMethod Failed.\n");
	}
	resource_ifs = resource_ifs->next;
    }
    /* printf("Java_org_iochibity_ResourceLocal_getInterfaces EXIT\n"); */
    return ll;
}

/*
 * Class:     org_iochibity_ResourceLocal
 * Method:    getProperties
 * Signature: ()Ljava/util/LinkedList;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_ResourceLocal_getProperties
(JNIEnv * env, jobject this)
{
    /* printf("Java_org_iochibity_ResourceLocal_getProperties ENTRY\n"); */

    /* first get handle to OCResource struct */
    jclass j_clazz = NULL;
    jlong j_resource_handle = 0;
    /* OCResource* c_resource_handle = NULL; */
    j_clazz = (*env)->GetObjectClass(env, this);
    if (j_clazz != NULL) {
	/* 1. get handle to OCResource */
        jfieldID handle_field = (*env)->GetFieldID(env, j_clazz, "handle", "J");
        if (handle_field == NULL) {
	    printf("Failed to get handle fld for Resource\n");
	    return NULL;
	} else {
	    /* printf("Got handle fld for Resource\n"); */
    	    j_resource_handle = (*env)->GetLongField(env, this, handle_field);
    	    /* printf("HANDLE: %ld\n", j_resource_handle); */
	    /* c_resource_handle = (OCResource*)(intptr_t) j_resource_handle; */
	    /* printf("c resource uri: %s\n", c_resource_handle->uri); */
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

    /* now create LinkedList for properties */
    jclass ll_clazz = (*env)->FindClass(env, "java/util/LinkedList");
    if (ll_clazz == 0) {
	printf("Find class method Failed.\n");
    }
    jmethodID ctor = (*env)->GetMethodID(env, ll_clazz, "<init>", "()V");
    if (ctor == 0) {
	printf("Find ctor method Failed.\n");
    }
    jobject ll  = (*env)->NewObject(env, ll_clazz, ctor);
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

    /* stack/ocresource.h: */
    /* typedef struct resourceinterface_t { */
    /*     struct resourceinterface_t *next; */
    /*     char *name ; */
    /* } OCResourceInterface; */

    /* now iterate over properties ("attributes"), creating a
       Property for each and adding it to the list */

    /* first, prepare to create new Property objects */
    jclass rp_clazz = (*env)->FindClass(env, "org/iochibity/PropertyString");
    if (rp_clazz == 0) {
	printf("Find PropertyString class Failed.\n");
    }
    jmethodID rp_ctor = (*env)->GetMethodID(env, rp_clazz, "<init>", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (ctor == 0) {
	printf("Find ctor method Failed.\n");
    } else {
	/* printf("FOUND ctor method for rp.\n"); */
    }

    jstring j_resource_pname, j_resource_pval;

    /* testing: add one rp */
    /* j_resource_pname = (*env)->NewStringUTF(env, "foo"); */
    /* j_resource_pval = (*env)->NewStringUTF(env, "99"); */
    /* printf("creating Property(\"foo\", \"99\");\n"); */
    /* jobject j_resource_p  = (*env)->NewObject(env, rp_clazz, rp_ctor, j_resource_pname, j_resource_pval); */
    /* if (j_resource_p == NULL) { */
    /* 	printf("NewObject failed for Property %s\n", (char*)j_resource_pname); */
    /* 	/\* exit? *\/ */
    /* } */
    /* printf("adding Property to list\n"); */
    /* jboolean jb = (*env)->CallBooleanMethod(env, ll, j_add, j_resource_p); */
    /* if (!jb) { */
    /* 	printf("CallBooleanMethod Failed.\n"); */
    /* } */


    /* now iterate over props */
    OCAttribute* resource_ps = ((OCResource*)(intptr_t)j_resource_handle)->rsrcAttributes;
    while(resource_ps) {
	printf("c resource property name: %s, value: %s\n", resource_ps->attrName, resource_ps->attrValue);
	j_resource_pname = (*env)->NewStringUTF(env, resource_ps->attrName);
	j_resource_pval = (*env)->NewStringUTF(env, resource_ps->attrValue);
	jobject j_resource_p  = (*env)->NewObject(env, rp_clazz, rp_ctor, j_resource_pname, j_resource_pval);
	if (j_resource_p == NULL) {
	    printf("NewObject failed for Property %s\n", (char*)j_resource_pname);
	    /* exit? */
	}
	jboolean jb = (*env)->CallBooleanMethod(env, ll, j_add, j_resource_p);
	if (!jb) {
	    printf("CallBooleanMethod Failed for add.\n");
	}
	resource_ps = resource_ps->next;
    }

    /* printf("Java_org_iochibity_ResourceLocal_getProperties EXIT\n"); */
    return ll;
}

/*
 * Class:     org_iochibity_ResourceLocal
 * Method:    getChildren
 * Signature: ()Ljava/util/LinkedList;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_ResourceLocal_getChildren
(JNIEnv * env, jobject this)
{
    /* printf("Java_org_iochibity_Resource_getChildren ENTRY\n"); */

    /* first get handle to OCResource struct */
    OCResource* c_resource_handle = (OCResource*)(intptr_t) getResourceHandle(env, this);

    OCChildResource* child_resources = c_resource_handle->rsrcChildResourcesHead;

    /* now create LinkedList for children */
    jclass ll_clazz = (*env)->FindClass(env, "java/util/LinkedList");
    if (ll_clazz == 0) {
	printf("Find class method Failed.\n");
    }
    jmethodID ctor = (*env)->GetMethodID(env, ll_clazz, "<init>", "()V");
    if (ctor == 0) {
	printf("Find ctor method Failed.\n");
    }
    jobject ll  = (*env)->NewObject(env, ll_clazz, ctor);
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

    // OCChildResource *rsrcChildResourcesHead;   /* linked list */
    /* typedef struct OCChildResource { */
    /*     struct OCResource *rsrcResource; */
    /*     struct OCChildResource *next; */
    /* } OCChildResource; */

    /* first, prepare to create new child objects */
    jclass r_clazz = (*env)->FindClass(env, "org/iochibity/ResourceLocal");
    if (r_clazz == 0) {
	printf("Find Resource class Failed.\n");
	/* TODO exit */
    }
    jmethodID r_ctor = (*env)->GetMethodID(env, r_clazz, "<init>", "()V");
    /* jmethodID r_ctor = (*env)->GetMethodID(env, r_clazz, "<init>", "(Lorg/iochibity/ResourceLocal;)V"); */
    if (ctor == 0) {
	printf("Find ctor Failed.\n");
    } else {
	/* printf("FOUND copy ctor for rp.\n"); */
    }
    jfieldID fid_handle = (*env)->GetFieldID(env, r_clazz, "handle", "J");
    jfieldID fid_uri = (*env)->GetFieldID(env, r_clazz, "uri", "Ljava/lang/String;");
    jfieldID fid_serial = (*env)->GetFieldID(env, r_clazz, "sn", "I");
    jfieldID fid_id = (*env)->GetFieldID(env, r_clazz, "id", "Lorg/iochibity/ResourceLocal$InstanceId;");

    /* now iterate over children, creating a Resource for each and
       adding it to the list */
    OCResource*      child = NULL;
    while(child_resources) {
	child = child_resources->rsrcResource;
	printf("c got child resource\n");
	jobject j_resource  = (*env)->NewObject(env, r_clazz, r_ctor);
	if (j_resource == NULL) {
	    printf("NewObject failed for child Resource\n");
	    /* exit? */
	}
	/* now copy fields */
        if (fid_handle != NULL) {
	    (*env)->SetLongField(env, j_resource, fid_handle, (intptr_t)child);
	}
        if (fid_uri != NULL) {
            jstring jString = (*env)->NewStringUTF(env, child->uri);
            if (jString != NULL) {
                (*env)->SetObjectField(env, j_resource, fid_uri, jString);
            }
	}
        if (fid_serial != NULL) {
	    (*env)->SetIntField(env, j_resource, fid_serial, (jlong)child->sequenceNum);
	}

	/* testing */
	printf("Child instance ord:  '%d'\n",  child->ins);
	printf("Child instance str:  '%s'\n",  child->uniqueStr);
	printf("Child instance uuid ln: %d\n", child->uniqueUUID.id_length);
	printf("Child instance uuid: '%s'\n",  child->uniqueUUID.id);

        if (fid_id != NULL) {
	    (*env)->SetIntField(env, j_resource, fid_serial, (jlong)child->sequenceNum);
	    if ( ((OCResource*)c_resource_handle)->ins > 0) {
		printf("instance ORD:  %d\n", ((OCResource*)c_resource_handle)->ins);
		/* FIXME: create InstanceOrdinal */
	    } else if ( ((OCResource*)c_resource_handle)->uniqueStr != NULL) {
		printf("instance str:  '%s'\n", ((OCResource*)c_resource_handle)->uniqueStr);
		/* FIXME: create InstanceString */
	    } else if ( ((OCResource*)c_resource_handle)->uniqueUUID.id_length > 0) {
		printf("instance uuid: %s\n", ((OCResource*)c_resource_handle)->uniqueUUID.id);
		/* FIXME: create InstanceUuid */
	    } else {
		printf("NO INSTANCE ID\n");
	    }
	}

	jboolean jb = (*env)->CallBooleanMethod(env, ll, j_add, j_resource);
	if (!jb) {
	    printf("CallBooleanMethod Failed for linkedlist.add.\n");
	}

	child_resources = child_resources->next;
    }

    /* printf("Java_org_iochibity_Resource_getChildren EXIT\n"); */
    return ll;
}

/*
 * Class:     org_iochibity_ResourceLocal
 * Method:    getServiceProvider
 * Signature: ()Lorg/iochibity/ResourceServiceProvider;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_ResourceLocal_getServiceProvider
(JNIEnv * env, jobject this)
{
    /* printf("Java_org_iochibity_ResourceLocal_getServiceProvider ENTRY\n"); */

    /* first get handle to OCResource struct */
    OCResource* c_resource_handle = (OCResource*)(intptr_t) getResourceHandle(env, this);

    void * c_cbparam = c_resource_handle->entityHandlerCallbackParam;
    jclass cbparam_clazz = (*env)->GetObjectClass(env, c_cbparam);
    if (cbparam_clazz == NULL) {
	printf("Failed to find class for cbparam \n");
    } else {
	/* printf("Found class for callbackParam:"); */
	/* print_class_name(env, (jobject)c_cbparam); */
    }

    jfieldID fid_rsp = (*env)->GetFieldID(env, cbparam_clazz,
					  "serviceProvider", "Lorg/iochibity/ResourceServiceProvider;");
    if (fid_rsp == NULL) {
	printf("Failed to get serviceProvider fld id\n");
    } else {
	/* printf("Got serviceProvider fld id\n"); */
    }
    jobject service_provider = (*env)->GetObjectField(env, (jobject)c_cbparam, fid_rsp);
    if (service_provider == NULL) {
	printf("Failed to get ResourceServiceProvider object\n");
    } else {
	/* printf("Got ResourceServiceProvider object\n"); */
    }

    /* printf("Java_org_iochibity_ResourceLocal_getServiceProvider EXIT\n"); */
    return service_provider;
}

/*
 * Class:     org_iochibity_ResourceLocal
 * Method:    getCallbackParam
 * Signature: ()Lorg/iochibity/CallbackParam;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_ResourceLocal_getCallbackParam
(JNIEnv * env, jobject this)
{
    /* printf("Java_org_iochibity_ResourceLocal_getCallbackParam ENTRY\n"); */

    /* first get handle to OCResource struct */
    OCResource* c_resource_handle = (OCResource*)(intptr_t) getResourceHandle(env, this);

    /* printf("Java_org_iochibity_ResourceLocal_getCallbackParam EXIT\n"); */
    /* extract the cbparam, which subclasses CallbackParam */
    return c_resource_handle->entityHandlerCallbackParam;
}

/*
 * Class:     org_iochibity_ResourceLocal
 * Method:    getActionSet
 * Signature: ()Ljava/util/LinkedList;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_ResourceLocal_getActionSet
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    throw_not_implemented(env, "Java_org_iochibity_Resource_getActionSet");
    return 0;
}

/* /\* */
/*  * Class:     org_iochibity_Resource */
/*  * Method:    getPayload */
/*  * Signature: ()Lorg/iochibity/PayloadResource; */
/*  * This method wraps  BuildResponseRepresentation (in ocresourcehandler.h/ocresource.c) */
/*  *\/ */
/* JNIEXPORT jobject JNICALL Java_org_iochibity_Resource_getPayload */
/* (JNIEnv * env, jobject this) */
/* { */
/*     /\* printf("Java_org_iochibity_Resource_getPayload ENTRY\n"); *\/ */
/*     /\*  ocresourcehandler.h: *\/ */
/*     /\* OCStackResult BuildResponseRepresentation(const OCResource *resourcePtr, *\/ */
/*     /\* 					      OCRepPayload** payload) *\/ */

/*     /\* 1. build payload for resource *\/ */
/*     OCResource* c_resource_handle = (OCResource*)(intptr_t) getResourceHandle(env, this); */
/*     OCStackResult op_result = OC_STACK_OK; */
/*     OCRepPayload* c_payload = NULL;  //OCRepPayloadCreate(); */
/* #ifdef IOCHIBITY    /\* 1.1.1: *\/ */
/*     op_result = BuildResponseRepresentation(c_resource_handle, &c_payload); */
/* #else    /\* 1.2.1: *\/ */
/*     op_result = BuildResponseRepresentation(c_resource_handle, &c_payload, NULL); */
/* #endif */
/*     if (op_result != OC_STACK_OK) */
/*     { */
/*         printf("Failed to build payload from resource\n"); */
/* 	return NULL; */
/*     } */

/*     /\* 2. create a linked list *\/ */
/*     jclass ll_clazz = (*env)->FindClass(env, "org/iochibity/PayloadList"); */
/*     if (ll_clazz == 0) { */
/* 	printf("Find class method Failed.\n"); */
/*     } */
/*     jmethodID ll_ctor = (*env)->GetMethodID(env, ll_clazz, "<init>", "()V"); */
/*     if (ll_ctor == 0) { */
/* 	printf("Find ctor method for ll Failed.\n"); */
/*     } */
/*     jobject ll  = (*env)->NewObject(env, ll_clazz, ll_ctor); */
/*     if (ll == NULL) { */
/* 	printf("NewObject failed for LinkedList\n"); */
/* 	/\* exit *\/ */
/*     } */
/*     jmethodID j_add = (*env)->GetMethodID(env, ll_clazz, "add", "(Ljava/lang/Object;)Z"); */
/*     if (j_add == NULL) { */
/* 	printf("Find add method Failed.\n"); */
/*     }else { */
/* 	/\* printf("Found add method succeeded.\n"); *\/ */
/*     } */

/*     /\* 3.  get reeady to create java PayloadResource objects *\/ */
/*     jclass p_clazz = (*env)->FindClass(env, "org/iochibity/PayloadResource"); */
/*     if (p_clazz == 0) { */
/* 	printf("Find class method for PayloadResource Failed.\n"); */
/*     } */
/*     jmethodID p_ctor = (*env)->GetMethodID(env, p_clazz, "<init>", "()V"); */
/*     if (p_ctor == 0) { */
/* 	printf("Find ctor method for PayloadResource Failed.\n"); */
/*     } */

/*     /\* 4. for each payload in the OCRepPayload linked list, create */
/*        java obj and add to java LinkedList *\/ */

/*     while (c_payload) { */
/* 	jobject j_payload  = (*env)->NewObject(env, p_clazz, p_ctor); */
/* 	if (j_payload == NULL) { */
/* 	    THROW_JNI_EXCEPTION("NewObject failed for PayloadResource\n"); */
/* 	} */

/* 	jfieldID fid_handle = (*env)->GetFieldID(env, p_clazz, "handle", "J"); */
/* 	if (fid_handle != NULL) { */
/* 		(*env)->SetLongField(env, j_payload, fid_handle, (intptr_t)c_payload); */
/* 	} else { */
/* 	    THROW_JNI_EXCEPTION("SetLongField failed for handle of PayloadResource\n"); */
/* 	} */
/* 	jmethodID mid_uri = (*env)->GetMethodID(env, p_clazz, "setUri", "(Ljava/lang/String;)V"); */
/* 	if (mid_uri == NULL) { */
/* 	    THROW_JNI_EXCEPTION("GetMethodID failed for setUri of PayloadResource\n"); */
/* 	} */
/* 	/\* jstring s = (*env)->CallObjectMethod(env, j_payload, mid_uri); *\/ */
/* 	/\* if (s == NULL) { *\/ */
/* 	/\*     THROW_STACK_EXCEPTION(op_result, "CallObjectMethod failed on getUri for PayloadResource\n"); *\/ */
/* 	/\* } else { *\/ */
/* 	/\*     printf(":::: PayloadResource Uri: %s\n", *\/ */
/* 	/\* 	   (char*) (*env)->GetStringUTFChars(env, s, NULL)); *\/ */
/* 	/\* } *\/ */

/* 	/\* 4. add PayloadResource obj to linked list *\/ */
/* 	jboolean jb = (*env)->CallBooleanMethod(env, ll, j_add, j_payload); */
/* 	if (!jb) { */
/* 	    printf("Failed add payload for linkedlist.\n"); */
/* 	} */

/* 	c_payload = c_payload->next; */
/*     } */

/*     /\* printf("Java_org_iochibity_Resource_getPayload EXIT\n"); *\/ */
/*     return ll; */
/* } */

