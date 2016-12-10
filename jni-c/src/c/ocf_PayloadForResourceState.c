#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "org_iochibity_PayloadForResourceState.h"
#include "jni_utils.h"
#include "ocf_exceptions.h"

#include "octypes.h"
#include "ocpayload.h"
#include "ocresource.h"
#include "ocresourcehandler.h"
#include "ocstack.h"

/* PRIVATE */
OCRepPayload* getObjectHandle(JNIEnv* env, jobject this)
{
    jclass k_this = (*env)->GetObjectClass(env, this);
    if (k_this == NULL) {
	THROW_JNI_EXCEPTION("GetObjectClass failure");
	return NULL;
    }
    jfieldID fid_handle = (*env)->GetFieldID(env, k_this, "handle", "J");
    if (fid_handle == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for 'handle'\n");
	return NULL;
    }
    return (OCRepPayload*)(intptr_t) (*env)->GetLongField(env, this, fid_handle);
}

/* PUBLIC */
/*
 * Class:     org_iochibity_PayloadForResourceState
 * Method:    createPayload
 * Signature: (Lorg/iochibity/Resource;)Lorg/iochibity/PayloadForResourceState;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_PayloadForResourceState_createPayload
(JNIEnv * env, jobject this, jobject j_resource)
{
    /* printf("Java_org_iochibity_PayloadForResource_createPayload ENTRY\n"); */

    OCResource* c_resource_handle = (OCResource*) getObjectHandle(env, j_resource);
    OCStackResult op_result = OC_STACK_OK;
    OCRepPayload* c_payload = NULL;  //OCRepPayloadCreate();
#ifdef IOCHIBITY    /* 1.1.1: */
    op_result = BuildResponseRepresentation(c_resource_handle, &c_payload);
#else    /* 1.2.1: */
    op_result = BuildResponseRepresentation(c_resource_handle, &c_payload, NULL);
#endif
    if (op_result != OC_STACK_OK)
    {
        THROW_STACK_EXCEPTION(op_result, "BuildResponseRepresentation failure\n");
	return NULL;
    }

    /* /\* 1. create a new OCRepPayload struct *\/ */
    /* OCRepPayload *c_payload = (OCRepPayload *) OCRepPayloadCreate(); */
    /* if (!c_payload) { */
    /* 	THROW_EH_EXCEPTION(OC_EH_ERROR, "OCRepPayloadCreate failure"); */
    /* 	return NULL; */
    /* } */

    /* tests */
    /* OCRepPayloadSetPropInt(c_payload, "i1", -1); */
    /* OCRepPayloadSetPropInt(c_payload, "i2", 0); */
    /* OCRepPayloadSetPropInt(c_payload, "i3", 1); */

    /* OCRepPayloadSetPropDouble(c_payload, "d1", 1.1); */
    /* OCRepPayloadSetPropDouble(c_payload, "d2", -9.9); */

    /* OCRepPayloadSetPropBool(c_payload, "b1", true); */
    /* OCRepPayloadSetPropBool(c_payload, "b2", false); */

    /* OCRepPayloadSetPropString(c_payload, "s1", "Hello, world"); */
    /* OCRepPayloadSetPropString(c_payload, "s2", "Goodbye, world"); */

    /* OCRepPayloadAddResourceType(c_payload, "foo.t.test1"); */
    /* OCRepPayloadAddResourceType(c_payload, "foo.t.test2"); */

    char msg[80];
    jclass k_this = (*env)->GetObjectClass(env, this);
    if (k_this == NULL) {
	THROW_JNI_EXCEPTION("GetObjectClass failed for this PayloadForResource");
	return NULL;
    }

    /* 1. set the OCPayloadType */
    jfieldID fid_this_type = (*env)->GetFieldID(env, k_this, "type", "I");
    if (fid_this_type == NULL) {
	sprintf(msg, "GetFieldID failed for 'type' of this\n");
	THROW_JNI_EXCEPTION(msg);
	return NULL;
    }
    (*env)->SetLongField(env, this, fid_this_type, (jlong) c_payload->base.type);

    /* 2. set the handle of the payload object to the new payload struct */
    jfieldID fid_this_handle = (*env)->GetFieldID(env, k_this, "_handle", "J");
    if (fid_this_handle == NULL) {
	sprintf(msg, "GetFieldID failed for _handle of this\n");
	THROW_JNI_EXCEPTION(msg);
	return NULL;
    }
    (*env)->SetLongField(env, this, fid_this_handle, (intptr_t)c_payload);

    /* 3. populate the payload object from the resource param */
    jclass r_klass = (*env)->GetObjectClass(env, j_resource);
    if (r_klass == NULL) {
	THROW_JNI_EXCEPTION("GetObjectClass failed for Resource");
	return NULL;
    }
    /* get uri from incoming resource object */
    jfieldID fid_uri = (*env)->GetFieldID(env, r_klass, "uri", "Ljava/lang/String;");
    if (fid_uri == NULL) {
	sprintf(msg, "GetFieldID failed for 'uri' of Resource\n");
	THROW_JNI_EXCEPTION(msg);
	return NULL;
    }
    jobject j_uri = (*env)->GetObjectField(env, j_resource, fid_uri);
    if (j_uri == NULL) {
	THROW_JNI_EXCEPTION("Failed to get j_uri object\n");
	return NULL;
    }
    /* char* c_uri = (char*) (*env)->GetStringUTFChars(env, j_uri, NULL); */
    /* printf("c uri from resource: %s\n", c_uri); */

    jfieldID fid_this_uri = (*env)->GetFieldID(env, k_this, "_uri", "Ljava/lang/String;");
    if (fid_this_uri == NULL) {
	sprintf(msg, "GetFieldID failed for 'uri' of this PayloadForResource\n");
	THROW_JNI_EXCEPTION(msg);
	return NULL;
    }
    (*env)->SetObjectField(env, this, fid_this_uri, j_uri);

    /* prep LinkedList stuff */
    /* FIXME: don't create LL in jni; pull it from the java object */
    jclass ll_klass = (*env)->FindClass(env, "java/util/LinkedList");
    if (ll_klass == 0) {
	THROW_JNI_EXCEPTION("FindClass failed for LinkedList\n");
	return NULL;
    }
    jmethodID ll_ctor = (*env)->GetMethodID(env, ll_klass, "<init>", "()V");
    if (ll_ctor == 0) {
	THROW_JNI_EXCEPTION("GetMethodID failed for ll ll_ctor.\n");
	return NULL;
    }
    jmethodID mid_add = (*env)->GetMethodID(env, ll_klass, "add", "(Ljava/lang/Object;)Z");
    if (mid_add == NULL) {
	THROW_JNI_EXCEPTION("GetMethodID failed for add method of ll.\n");
    }

    /* do types */
    jobject llts  = (*env)->NewObject(env, ll_klass, ll_ctor);
    if (llts == NULL) {
	THROW_JNI_EXCEPTION("NewObject failed for _types LinkedList\n");
	return NULL;
    }
    OCStringLL* c_rtype = c_payload->types;
    jstring  j_rtype = NULL;
    jboolean j_b;
    while(c_rtype) {
	/* printf("c payload type: %s\n", c_rtype->value); */
	j_rtype = (*env)->NewStringUTF(env, c_rtype->value);
	j_b = (*env)->CallBooleanMethod(env, llts, mid_add, j_rtype);
	if (!j_b) {
	    THROW_JNI_EXCEPTION("CallBooleanMethod failed for mid_add\n");
	}
	c_rtype = c_rtype->next;
    }
    jfieldID fid_rtypes = (*env)->GetFieldID(env, k_this, "_rtypes", "Ljava/util/List;");
    if (fid_rtypes == NULL) {
    	THROW_JNI_EXCEPTION("GetFieldID failed for '_rtypes' on PayloadForResourceState\n");
    	return NULL;
    }
    (*env)->SetObjectField(env, this, fid_rtypes, llts);

    /* do interfaces */
    jobject llifs  = (*env)->NewObject(env, ll_klass, ll_ctor);
    if (llifs == NULL) {
	THROW_JNI_EXCEPTION("NewObject failed for _interfaces LinkedList\n");
	return NULL;
    }
    OCStringLL* c_iface = c_payload->interfaces;
    jstring  j_iface = NULL;
    while(c_iface) {
	/* printf("c payload interface: %s\n", c_iface->value); */
	j_iface = (*env)->NewStringUTF(env, c_iface->value);
	j_b = (*env)->CallBooleanMethod(env, llifs, mid_add, j_iface);
	if (!j_b) {
	    THROW_JNI_EXCEPTION("CallBooleanMethod failed for mid_add\n");
	}
	c_iface = c_iface->next;
    }
    jfieldID fid_ifs = (*env)->GetFieldID(env, k_this, "_interfaces", "Ljava/util/List;");
    if (fid_ifs == NULL) {
    	THROW_JNI_EXCEPTION("GetFieldID failed for _interfaces on PayloadResourceForState\n");
    	return NULL;
    }
    (*env)->SetObjectField(env, this, fid_ifs, llifs);

    /* do properties ('attributes') */
    /* first prepare for PropertyMap stuff */
    jclass pm_klass = (*env)->FindClass(env, "org/iochibity/PropertyMap");
    if (pm_klass == 0) {
	THROW_JNI_EXCEPTION("FindClass failed for PropertyMap\n");
	return NULL;
    }
    jmethodID pm_ctor = (*env)->GetMethodID(env, pm_klass, "<init>", "()V");
    if (pm_ctor == 0) {
	THROW_JNI_EXCEPTION("GetMethodID failed for PropertyMap ctor.\n");
	return NULL;
    }
    jmethodID mid_put = (*env)->GetMethodID(env, pm_klass, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
    if (mid_add == NULL) {
	THROW_JNI_EXCEPTION("GetMethodID failed for put method of pm.\n");
    }

    jobject pmps  = (*env)->NewObject(env, pm_klass, pm_ctor);
    if (pmps == NULL) {
	THROW_JNI_EXCEPTION("NewObject failed for _properties PropertyMap\n");
	return NULL;
    }
    OCRepPayloadValue* c_prop = c_payload->values;
    jstring j_pname = NULL;
    /* jobject  j_pval = NULL; */
    while(c_prop) {
	printf("**** PROPERTY name: %s\n", c_prop->name);
	j_pname = (*env)->NewStringUTF(env, c_prop->name);
	switch(c_prop->type) {
	case OCREP_PROP_NULL:
	    printf("**** FOUND NULL PROPERTY\n");
	    break;
	case OCREP_PROP_INT:
	    /* j_pval = c_prop->i; */
	    printf("**** PROPERTY int, value: %lld\n", c_prop->i);
	    jobject j_int = int2Integer(env, c_prop->i);
	    (*env)->CallObjectMethod(env, pmps, mid_put,
				     j_pname,
				     j_int);
	    /* if (!j_b) { */
	    /* 	THROW_JNI_EXCEPTION("CallObjectMethod failed for mid_put i\n"); */
	    /* } */
	    break;
	case OCREP_PROP_DOUBLE:
	    printf("**** PROPERTY double, value: %f\n", c_prop->d);
	    jobject j_d = double2Double(env, c_prop->d);
	    (*env)->CallObjectMethod(env, pmps, mid_put, j_pname, j_d);
	    if (!j_d) {
		THROW_JNI_EXCEPTION("CallObjectMethod failed for mid_put d\n");
	    }
	    break;
	case OCREP_PROP_BOOL:
	    printf("**** PROPERTY boolean, value: %d\n", c_prop->b);
	    jobject j_bool = bool2Boolean(env, c_prop->b);
	    (*env)->CallObjectMethod(env, pmps, mid_put, j_pname, j_bool);
	    /* if (!j_b) { */
	    /* 	THROW_JNI_EXCEPTION("CallObjectMethod failed for mid_put b\n"); */
	    /* } */
	    break;
	case OCREP_PROP_STRING:
	    printf("**** PROPERTY string, value: %s\n", c_prop->str);
	    jobject j_s = (*env)->NewStringUTF(env,  c_prop->str);
	    (*env)->CallObjectMethod(env, pmps, mid_put, j_pname, j_s);
	    /* if (!j_b) { */
	    /* 	THROW_JNI_EXCEPTION("CallBooleanMethod failed for mid_put str\n"); */
	    /* } */
	    break;
	case OCREP_PROP_BYTE_STRING:
	    printf("**** PROPERTY type byte string, not implemented\n");
	    break;
	case OCREP_PROP_OBJECT:
	    printf("**** PROPERTY type object, not implemented\n");
	    break;
	case OCREP_PROP_ARRAY:
	    printf("**** PROPERTY type array, not implemented\n");
	    break;
	default:
	    break;
	}
	/* j_b = (*env)->CallBooleanMethod(env, pmps, mid_put, j_pname, j_pval); */
	/* if (!j_b) { */
	/*     THROW_JNI_EXCEPTION("CallBooleanMethod failed for mid_put\n"); */
	/* } */
	c_prop = c_prop->next;
    }
    jfieldID fid_ps = (*env)->GetFieldID(env, k_this, "_properties", "Lorg/iochibity/PropertyMap;");
    if (fid_ps == NULL) {
    	THROW_JNI_EXCEPTION("GetFieldID failed for _interfaces on PayloadResource\n");
    	return NULL;
    }
    (*env)->SetObjectField(env, this, fid_ps, pmps);

    /* printf("Java_org_iochibity_PayloadForResource_createPayload EXIT\n"); */
    return this;
}


/* /\* */
/*  * Class:     org_iochibity_PayloadForResource */
/*  * Method:    getUri */
/*  * Signature: ()Ljava/lang/String; */
/*  *\/ */
/* JNIEXPORT jstring JNICALL Java_org_iochibity_PayloadForResource_getUri */
/* (JNIEnv * env, jobject this) */
/* { */
/*     /\* printf("Java_org_iochibity_PayloadForResource_getUri ENTRY\n"); *\/ */
/*     jclass klass = (*env)->GetObjectClass(env, this); */
/*     if (klass == NULL) { */
/* 	THROW_JNI_EXCEPTION("GetObjectClass failed for this\n"); */
/* 	return NULL; */
/*     } */
/*     jfieldID fid_uri = (*env)->GetFieldID(env, klass, "_uri", "Ljava/lang/String;"); */
/*     if (fid_uri == NULL) { */
/* 	THROW_JNI_EXCEPTION("GetFieldID failed for uri\n"); */
/* 	return NULL; */
/*     } */
/*     jstring j_uri = (*env)->GetObjectField(env, this, fid_uri); */
/*     if (j_uri == NULL) { */
/* 	THROW_JNI_EXCEPTION("GetObjectField failed for fid_uri on this PayloadResource\n"); */
/* 	return NULL; */
/*     } */
/*     char* c_uri = (char*) (*env)->GetStringUTFChars(env, j_uri, NULL); */
/*     if (c_uri == NULL) { */
/* 	THROW_JNI_EXCEPTION("Failed to get c_uri object\n"); */
/* 	return NULL; */
/*     } */
/*     /\* printf("Java_org_iochibity_PayloadForResource_getUri EXIT\n"); *\/ */
/*     return j_uri; // (*env)->NewStringUTF(env, (jstring)j_uri); */
/* } */

/* /\* */
/*  * Class:     org_iochibity_PayloadForResource */
/*  * Method:    setUri */
/*  * Signature: (Ljava/lang/String;)V */
/*  *\/ */
/* JNIEXPORT void JNICALL Java_org_iochibity_PayloadForResource_setUri */
/* (JNIEnv * env, jobject this, jstring j_uri) */
/* { */
/*     OC_UNUSED(env); */
/*     OC_UNUSED(this); */
/*     OC_UNUSED(j_uri); */
/*     /\* printf("Java_org_iochibity_PayloadForResource_setUri ENTRY\n"); *\/ */
/*     jclass k_this = (*env)->GetObjectClass(env, this); */
/*     if (k_this == NULL) { */
/* 	THROW_JNI_EXCEPTION("GetObjectClass failed for Resource"); */
/* 	return; */
/*     } */
/*     jfieldID fid_handle = (*env)->GetFieldID(env, k_this, "handle", "J"); */
/*     if (fid_handle == NULL) { */
/* 	THROW_JNI_EXCEPTION("GetFieldID failed for 'handle' on PayloadResource\n"); */
/* 	return; */
/*     } */
/*     OCRepPayload* c_payload_resource = (OCRepPayload*) (*env)->GetLongField(env, this, fid_handle); */
/*     c_payload_resource->uri = (char*) (*env)->GetStringUTFChars(env, j_uri, NULL); */

/*     jfieldID fid_uri = (*env)->GetFieldID(env, k_this, "_uri", "Ljava/lang/String;"); */
/*     if (fid_uri == NULL) { */
/* 	THROW_JNI_EXCEPTION("GetFieldID failed for 'uri' of Resource\n"); */
/* 	return; */
/*     } */
/*     (*env)->SetObjectField(env, this, fid_uri, j_uri); */
/* } */

/* /\* */
/*  * Class:     org_iochibity_PayloadForResource */
/*  * Method:    getTypes */
/*  * Signature: ()Ljava/util/LinkedList; */
/*  *\/ */
/* JNIEXPORT jobject JNICALL Java_org_iochibity_PayloadForResource_getTypes */
/* (JNIEnv * env, jobject this) */
/* { */
/*     printf("Java_org_iochibity_PayloadForResource_getTypes ENTRY\n"); */
/*     //     OCResourceType *rsrcType; */

/*     OCRepPayload* c_payload = (OCRepPayload*) getObjectHandle(env, this); */
/*     if (c_payload == NULL) { */
/* 	THROW_JNI_EXCEPTION("getObjectHandle failed"); */
/* 	return NULL; */
/*     } else { */
/* 	printf("getTypes FOUND payload handle %ld\n", (long)c_payload); */
/*     } */

/*     /\* create LinkedList for types *\/ */
/*     jclass ll_klass = (*env)->FindClass(env, "java/util/LinkedList"); */
/*     if (ll_klass == 0) { */
/* 	THROW_JNI_EXCEPTION("FindClass failed for LinkedList\n"); */
/* 	return NULL; */
/*     } */
/*     jmethodID ctor = (*env)->GetMethodID(env, ll_klass, "<init>", "()V"); */
/*     if (ctor == 0) { */
/* 	THROW_JNI_EXCEPTION("GetMethodID failed for ll ctor.\n"); */
/* 	return NULL; */
/*     } */
/*     jmethodID mid_add = (*env)->GetMethodID(env, ll_klass, "add", "(Ljava/lang/Object;)Z"); */
/*     if (mid_add == NULL) { */
/* 	THROW_JNI_EXCEPTION("GetMethodID failed for add method of ll.\n"); */
/*     } */
/*     jobject ll  = (*env)->NewObject(env, ll_klass, ctor); */
/*     if (ll == NULL) { */
/* 	THROW_JNI_EXCEPTION("NewObject failed for LinkedList\n"); */
/* 	return NULL; */
/*     } */

/*     OCStringLL* c_type = c_payload->types; */
/*     jstring  j_type = NULL; */
/*     jboolean j_b; */
/*     while(c_type) { */
/* 	/\* printf("c payload type: %s\n", c_type->value); *\/ */
/* 	j_type = (*env)->NewStringUTF(env, c_type->value); */
/* 	j_b = (*env)->CallBooleanMethod(env, ll, mid_add, j_type); */
/* 	if (!j_b) { */
/* 	    THROW_JNI_EXCEPTION("CallBooleanMethod failed for mid_add\n"); */
/* 	} */
/* 	c_type = c_type->next; */
/*     } */
/*     printf("Java_org_iochibity_PayloadForResource_getTypes EXIT\n"); */
/*     return ll; */
/* } */


/*
 * Class:     org_iochibity_PayloadForResourceState
 * Method:    setResourceTypes
 * Signature: (Ljava/util/LinkedList;)V
 */
JNIEXPORT void JNICALL Java_org_iochibity_PayloadForResourceState_setResourceTypes
  (JNIEnv *, jobject, jobject);

/* /\* */
/*  * Class:     org_iochibity_PayloadForResource */
/*  * Method:    addType */
/*  * Signature: (Ljava/lang/String;)V */
/*  *\/ */
/* JNIEXPORT void JNICALL Java_org_iochibity_PayloadForResource_addType */
/* (JNIEnv * env, jobject this, jstring j_type) */
/* { */
/*     /\* printf("Java_org_iochibity_PayloadForResource_addType ENTRY\n"); *\/ */
/*     OCRepPayload* c_payload = (OCRepPayload*) getObjectHandle(env, this); */
/*     if (c_payload == NULL) { */
/* 	THROW_JNI_EXCEPTION("getObjectHandle failed"); */
/* 	return; */
/*     /\* } else { *\/ */
/*     /\* 	printf("add: FOUND payload handle %ld\n", c_payload); *\/ */
/*     } */
/*     char* s = (char*) (*env)->GetStringUTFChars(env, j_type, NULL); */
/*     /\* printf("ADDING type %s\n", s); *\/ */
/*     OCRepPayloadAddResourceType(c_payload, s); */

/*     /\* now add it to the LinkedList? *\/ */
/*     /\* 1. get 'types' fld from this *\/ */
/*     jclass k_this = (*env)->GetObjectClass(env, this); */
/*     if (k_this == NULL) { */
/* 	THROW_JNI_EXCEPTION("GetObjectClass failed for Resource"); */
/* 	return; */
/*     } */
/*     jfieldID fid_types = (*env)->GetFieldID(env, k_this, "_types", "Ljava/util/LinkedList;"); */
/*     if (fid_types == NULL) { */
/*     	THROW_JNI_EXCEPTION("GetFieldID failed for 'types' on PayloadResource\n"); */
/*     	return; */
/*     } */
/*     jobject j_types = (*env)->GetObjectField(env, this, fid_types); */
/*     if (j_types == NULL) { */
/* 	THROW_JNI_EXCEPTION("GetObjectField failed for 'types'"); */
/* 	return; */
/*     } */

/*     /\* 2. get linked list add method *\/ */
/*     jclass ll_klass = (*env)->GetObjectClass(env, j_types); */
/*     if (ll_klass == NULL) { */
/*     	THROW_JNI_EXCEPTION("GetObjectClass failed for types ll"); */
/*     	return; */
/*     } */
/*     /\* jclass ll_klass = (*env)->FindClass(env, "java/util/LinkedList"); *\/ */
/*     /\* if (ll_klass == 0) { *\/ */
/*     /\* 	THROW_JNI_EXCEPTION("FindClass failed for LinkedList\n"); *\/ */
/*     /\* 	return; *\/ */
/*     /\* } *\/ */
/*     jmethodID mid_add = (*env)->GetMethodID(env, ll_klass, "add", "(Ljava/lang/Object;)Z"); */
/*     if (mid_add == NULL) { */
/* 	THROW_JNI_EXCEPTION("GetMethodID failed for add method of ll.\n"); */
/*     } */
/*     jboolean j_b = (*env)->CallBooleanMethod(env, j_types, mid_add, j_type); */
/*     if (!j_b) { */
/* 	    THROW_JNI_EXCEPTION("CallBooleanMethod failed for mid_add\n"); */
/*     } */

/*     /\* printf("Java_org_iochibity_PayloadForResource_addType EXIT\n"); *\/ */
/*     return; */
/* } */

/*
 * Class:     org_iochibity_PayloadForResourceState
 * Method:    setInterfaces
 * Signature: (Ljava/util/LinkedList;)V
 */
JNIEXPORT void JNICALL Java_org_iochibity_PayloadForResourceState_setInterfaces
  (JNIEnv *, jobject, jobject);

/* /\* */
/*  * Class:     org_iochibity_PayloadForResource */
/*  * Method:    getProperties */
/*  * Signature: ()Lorg/iochibity/PropertyMap; */
/*  *\/ */
/* JNIEXPORT jobject JNICALL Java_org_iochibity_PayloadForResource_getProperties */
/* (JNIEnv * env, jobject this) */
/* { */
/*     printf("Java_org_iochibity_PayloadForResource_getProperties ENTRY\n"); */
/*     jclass klass = NULL; */
/*     klass = (*env)->GetObjectClass(env, this); */
/*     if (klass == NULL) { */
/* 	THROW_JNI_EXCEPTION("GetObjectClass failed for PayloadResource object\n"); */
/* 	return 0; */
/*     } */
/*     /\* jlong handle_payload_resource = 0; *\/ */
/*     OCRepPayload* payload_resource = NULL; */
/*     jfieldID fid_handle = (*env)->GetFieldID(env, klass, "handle", "J"); */
/*     if (fid_handle == NULL) { */
/* 	THROW_JNI_EXCEPTION("GetFieldID failed for 'handle' on PayloadResource\n"); */
/* 	return 0; */
/*     } */
/*     payload_resource = (OCRepPayload*) (*env)->GetLongField(env, this, fid_handle); */

/*     /\* OCRepPayloadValue* c_handle_payload_properties = payload_resource->values; *\/ */

/*     /\* now create PayloadProperties hashmap *\/ */
/*     jclass hm_klass = (*env)->FindClass(env, "org/iochibity/PayloadProperties"); */
/*     if (hm_klass == 0) { */
/* 	THROW_JNI_EXCEPTION("FindClass failed on org/iochibity/PayloadProperties\n"); */
/*     } */
/*     jmethodID ctor = (*env)->GetMethodID(env, hm_klass, "<init>", "()V"); */
/*     if (ctor == 0) { */
/* 	THROW_JNI_EXCEPTION("GetMethodID failed on ctor for PayloadProperties\n"); */
/*     } */
/*     jobject payload_properties  = (*env)->NewObject(env, hm_klass, ctor); */
/*     if (payload_properties == NULL) { */
/* 	THROW_JNI_EXCEPTION("NewObject failed for PayloadProperties\n"); */
/*     } */
/*     /\* now set OCRepPayload* handle in PayloadProperties object *\/ */
/*     jfieldID fid_payload_rep_handle = (*env)->GetFieldID(env, hm_klass, "handleOCRepPayload", "J"); */
/*     if (fid_payload_rep_handle != NULL) { */
/* 	(*env)->SetLongField(env, payload_properties, */
/* 			     fid_payload_rep_handle, */
/* 			     (jlong) payload_resource); */
/*     } */
/*     /\* /\\* now set OCRepPayloadValue* handle in PayloadProperties object *\\/ *\/ */
/*     /\* jfieldID fid_payload_props_handle = (*env)->GetFieldID(env, hm_klass, "handle", "J"); *\/ */
/*     /\* if (fid_payload_props_handle != NULL) { *\/ */
/*     /\* 	(*env)->SetLongField(env, payload_properties, *\/ */
/*     /\* 			     fid_payload_props_handle, *\/ */
/*     /\* 			     (jlong) c_handle_payload_properties); *\/ */
/*     /\* } *\/ */
/*     printf("Java_org_iochibity_PayloadForResource_getProperties EXIT\n"); */
/*     return payload_properties; */
/* } */

/*
 * Class:     org_iochibity_PayloadForResourceState
 * Method:    setProperties
 * Signature: (Lorg/iochibity/PropertyForResource;)V
 */
JNIEXPORT void JNICALL Java_org_iochibity_PayloadForResourceState_setProperties
  (JNIEnv *, jobject, jobject);


