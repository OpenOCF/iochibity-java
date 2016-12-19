/* ocf_MsgForServiceProvider.c */

#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "org_iochibity_MsgForServiceProvider.h"
#include "ocf_init.h"
#include "jni_utils.h"
#include "ocf_exceptions.h"

#include "octypes.h"
#include "ocresource.h"
#include "ocstack.h"

/* /\* */
/*  * Class:     org_iochibity_MsgForServiceProvider */
/*  * Method:    getServiceProvider */
/*  * Signature: ()Lorg/iochibity/IServiceProvider; */
/*  *\/ */
/* JNIEXPORT jobject JNICALL Java_org_iochibity_MsgForServiceProvider_getServiceProvider */
/* (JNIEnv * env, jobject this) */
/* { */
/*     /\* printf("org_iochibity_MessageService/getServiceProvider ENTRY\n"); *\/ */

/*     /\* get handle to OCResource from MsgForServiceProvider *\/ */
/*     jlong j_resource_handle = */
/* 	(intptr_t) (*env)->GetLongField(env, this, FID_MFSP_RESOURCE_HANDLE); */

/*     OCResourceType* resource_types = ((OCResource*)(intptr_t)j_resource_handle)->rsrcType; */
/*     while(resource_types) { */
/* 	/\* printf("c resource type: %s\n", resource_types->resourcetypename); *\/ */
/* 	resource_types = resource_types->next; */
/*     } */

/*     /\* now prepare a ServiceProvider (Resource) object from the OCResource struct *\/ */

/*     /\* create and populate Resource java object *\/ */
/*     jobject j_IServiceProvider = (*env)->NewObject(env, K_ISERVICE_PROVIDER, MID_ISP_CTOR); */
/*     (*env)->SetLongField(env, j_IServiceProvider, FID_ISP_, j_resource_handle); */

/*     /\* printf("c resource uri: %s\n", ((OCResource*)j_resource_handle)->uri); *\/ */
/*     jfieldID jurifield = (*env)->GetFieldID(env, K_ISERVICE_PROVIDER, "uri", "Ljava/lang/String;"); */
/*     if (jurifield != NULL) { */
/* 	jstring  juri  = (*env)->NewStringUTF(env, ((OCResource*)(intptr_t)j_resource_handle)->uri); */
/* 	if (juri != NULL) { */
/* 	    (*env)->SetObjectField(env, j_IServiceProvider, jurifield, juri); */
/* 	} else { */
/* 	    printf("Failed to get UTF string from c uri\n"); */
/* 	} */
/*     } else { */
/* 	printf("Failed to get resource uri fld id\n"); */
/*     } */

/*     jfieldID fid_policies = (*env)->GetFieldID(env, K_ISERVICE_PROVIDER, "policies", "I"); */
/*     if (fid_policies != NULL) { */
/* 	(*env)->SetIntField(env, j_IServiceProvider, fid_policies, */
/* 			    (jint)((OCResource*)(intptr_t)j_resource_handle)->resourceProperties); */
/*     } else { */
/* 	printf("Failed to find policies field id\n"); */
/*     } */


/*     /\* printf("org_iochibity_MessageService/getServiceProvider EXIT\n"); *\/ */
/*     return j_IServiceProvider; */
/* } */

/* /\* */
/*  * Class:     org_iochibity_MsgForServiceProvider */
/*  * Method:    getPDUPayload */
/*  * Signature: ()Lorg/iochibity/PayloadList; */
/*  *\/ */
/* JNIEXPORT jobject JNICALL Java_org_iochibity_MsgForServiceProvider_getPDUPayload */
/* (JNIEnv * env, jobject this) */
/* { */
/*     /\* printf("Java_org_iochibity_MessageService_getPayload ENTRY\n"); *\/ */



/*     /\* 1. create a linked list *\/ */
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

/*     /\* 3.  get reeady to create java Payload objects *\/ */
/*     jclass request_in_clazz = (*env)->GetObjectClass(env, this); */
/*     if (request_in_clazz == NULL) { */
/* 	printf("Failed to find MessageService class\n"); */
/*     } else { */
/* 	/\* printf("Found MessageService class\n"); *\/ */
/*     } */

/*     /\* get handle to OCPayload* from MessageService *\/ */
/*     OCPayload* c_payload = NULL; */
/*     jfieldID field = (*env)->GetFieldID(env, request_in_clazz, "payloadHandle", "J"); */
/*     if (field == NULL) { */
/* 	printf("Failed to get payload handle fld id from MessageService object\n"); */
/*     } else { */
/* 	c_payload = (OCPayload*)(intptr_t) (*env)->GetLongField(env, this, field); */
/* 	/\* printf("Found payload handle %ld from MessageService\n", (long)c_payload); *\/ */
/*     } */

/*     jobject jpayload = 0; */
/*     /\* prepare for Payload objects *\/ */
/*     /\* for this to work we must test the type of each Payload obj */
/*        (OCPayload*) and create the appropriate java object *\/ */
/*     switch(c_payload->type) { */
/* 	// OCPayloadType enum */
/*     case PAYLOAD_TYPE_INVALID: */
/* 	break; */
/*     case PAYLOAD_TYPE_DISCOVERY: */
/* 	break; */
/*     case PAYLOAD_TYPE_DEVICE: */
/* 	break; */
/*     case PAYLOAD_TYPE_PLATFORM: */
/* 	break; */
/*     case PAYLOAD_TYPE_REPRESENTATION: */
/* 	break; */
/*     case PAYLOAD_TYPE_SECURITY: */
/* 	break; */
/*     case PAYLOAD_TYPE_PRESENCE: */
/* 	break; */
/*     /\* case PAYLOAD_TYPE_RD: *\/ */
/*     default: */
/*     	break; */
/*     } */


/*     jclass payload_clazz = (*env)->FindClass(env, "Lorg/iochibity/Payload;"); */
/*     if (payload_clazz == NULL) { */
/* 	printf("Failed to find Payload class\n"); */
/*     } else { */
/* 	/\* printf("Found Payload class\n"); *\/ */
/*     } */

/*     jmethodID payload_ctor = (*env)->GetMethodID(env, payload_clazz, "<init>", "()V"); */
/*     if (payload_ctor == NULL) { */
/* 	printf("Failed to find ctor method for Payload class\n"); */
/*     } else { */
/* 	/\* printf("Found Payload ctor\n"); *\/ */
/*     } */

/*     /\* now create and populate one Payload java object for each OCPayload *\/ */
/*     while(c_payload) { */
	
/* 	jobject j_payload = (*env)->NewObject(env, payload_clazz, payload_ctor); */
/* 	if (j_payload == NULL) { */
/* 	    THROW_JNI_EXCEPTION("NewObject failed for Payload\n"); */
/* 	} */
/* 	/\* populate it *\/ */
/*     } */
/*     /\* printf("Java_org_iochibity_MessageService_getPayload EXIT\n"); *\/ */
/*     return jpayload; */
/* } */

/* /\* */
/*  * Class:     org_iochibity_MsgForServiceProvider */
/*  * Method:    getVendorHeaderOptions */
/*  * Signature: ()Ljava/util/ArrayList; */
/*  *\/ */
/* JNIEXPORT jobject JNICALL Java_org_iochibity_MsgForServiceProvider_getVendorHeaderOptions */
/* (JNIEnv * env, jobject this) */
/* { */
/*     OC_UNUSED(env); */
/*     OC_UNUSED(this); */
/*     /\* printf("Java_org_iochibity_MessageService_getVendorHeaderOptions ENTRY\n"); *\/ */

/*     /\* first create an ArrayList *\/ */
/*     jclass al_clazz = (*env)->FindClass(env, "java/util/ArrayList"); */
/*     if (al_clazz == 0) { */
/* 	printf("Find ArrayList class method Failed.\n"); */
/*     }else { */
/* 	/\* printf("Found class method succeeded.\n"); *\/ */
/*     } */

/*     jmethodID ctor = (*env)->GetMethodID(env, al_clazz, "<init>", "()V"); */
/*     if (ctor == 0) { */
/* 	printf("Find ctor method Failed.\n"); */
/*     }else { */
/* 	/\* printf("Found ctor method succeeded.\n"); *\/ */
/*     } */
/*     jobject al  = (*env)->NewObject(env, al_clazz, ctor); */
/*     if (al == NULL) { */
/* 	printf("NewObject failed for ArrayList\n"); */
/*     } else { */
/* 	jmethodID j_add = (*env)->GetMethodID(env, al_clazz, "add", "(Ljava/lang/Object;)Z"); */
/* 	if (j_add == NULL) { */
/* 	    printf("Find add method Failed.\n"); */
/* 	}else { */
/* 	    /\* printf("Found add method succeeded.\n"); *\/ */
/* 	} */
/*     } */

/*     /\* now get the c data *\/ */

/*     /\* for (int i = 0; i < n; i++) { *\/ */
/*     /\* 	rcvdVendorSpecificHeaderOptions[i]; *\/ */
/*     /\* } *\/ */

/*     /\* printf("Java_org_iochibity_MessageService_getVendorHeaderOptions EXIT\n"); *\/ */
/*     return al; */
/* } */


