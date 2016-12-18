#include <ctype.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "org_iochibity_Message.h"
#include "ocf_init.h"
#include "ocf_exceptions.h"
#include "jni_utils.h"

#include "octypes.h"
#include "ocresource.h"
#include "ocstack.h"
#include "oic_malloc.h"
#include "oic_string.h"

/* INTERNAL */

#ifdef IOCHIBITY    /* 1.1.1: */
/*
 * OCPlatformPayload_to_Payload
 * compensate for OCPlatformInfo wierdness
 */
jobject OCPlatformPayload_to_Payload(JNIEnv* env, OCPlatformPayload* c_payload)
{
    /* printf("OCPlatformPayload_to_Payload ENTRY\n"); */
    jobject j_Payload = (*env)->NewObject(env, K_PAYLOAD, MID_PAYLOAD_CTOR);
    if (j_Payload == NULL) {
	THROW_JNI_EXCEPTION("NewObject failed for PayloadForPlatform\n");
	return NULL;
    }

    /* _uri */
    jstring j_uri = (*env)->NewStringUTF(env, c_payload->uri);
    (*env)->SetObjectField(env, j_Payload, FID_PAYLOAD_URI_PATH, j_uri);

    /* _rtypes */
    jobject j_rtypes  = (*env)->NewObject(env, K_LINKED_LIST, MID_LL_CTOR);
    if (j_rtypes == NULL) {
	printf("NewObject failed for rtypes LinkedList\n");
	return NULL;
    }
    OCStringLL* c_rtypes = c_payload->rt;
    jstring j_rtype;
    while(c_rtypes) {
	j_rtype = (*env)->NewStringUTF(env, c_rtypes->value);
	jboolean jb = (*env)->CallBooleanMethod(env, j_rtypes, MID_LL_ADD, j_rtype);
	if (!jb) {
	    printf("CallBooleanMethod failed on MID_LL_ADD for rtypes of Platform\n");
	}
	c_rtypes = c_rtypes->next;
    }
    (*env)->SetObjectField(env, j_Payload, FID_PAYLOAD_RTYPES, j_rtypes);

    /* _interfaces */
    jobject j_ifaces  = (*env)->NewObject(env, K_LINKED_LIST, MID_LL_CTOR);
    if (j_ifaces == NULL) {
	printf("NewObject failed for ifaces LinkedList\n");
	return NULL;
    }
    OCStringLL* c_ifaces = c_payload->interfaces;
    jstring j_iface;
    while(c_ifaces) {
	/* printf("c resource type: %s\n", resource_types->resourcetypename); */
	j_iface = (*env)->NewStringUTF(env, c_ifaces->value);
	jboolean jb = (*env)->CallBooleanMethod(env, j_ifaces, MID_LL_ADD, j_iface);
	if (!jb) {
	    printf("CallBooleanMethod failed on MID_LL_ADD for ifaces of Platform\n");
	}
	c_ifaces = c_ifaces->next;
    }
    (*env)->SetObjectField(env, j_Payload, FID_PAYLOAD_IFS, j_ifaces);

    /* OCPlatformInfo - convert to PropertyMap */
    jobject j_pmap  = (*env)->NewObject(env, K_PMAP, MID_PMAP_CTOR);
    if (j_pmap == NULL) {
	THROW_JNI_EXCEPTION("NewObject failed for PropertyMap\n");
	return NULL;
    }
    jstring j_n = (*env)->NewStringUTF(env, OC_RSRVD_PLATFORM_ID);
    jstring j_s = (*env)->NewStringUTF(env, c_payload->info.platformID);
    (*env)->CallObjectMethod(env, j_pmap, MID_PMAP_PUT, j_n, j_s);

    j_n = (*env)->NewStringUTF(env, OC_RSRVD_MFG_NAME);
    j_s = (*env)->NewStringUTF(env,  c_payload->info.manufacturerName);
    (*env)->CallObjectMethod(env, j_pmap, MID_PMAP_PUT, j_n, j_s);

    j_n = (*env)->NewStringUTF(env, OC_RSRVD_MFG_URL);
    j_s = (*env)->NewStringUTF(env,  c_payload->info.manufacturerUrl);
    (*env)->CallObjectMethod(env, j_pmap, MID_PMAP_PUT, j_n, j_s);

    j_n = (*env)->NewStringUTF(env, OC_RSRVD_MODEL_NUM);
    j_s = (*env)->NewStringUTF(env,  c_payload->info.modelNumber);
    (*env)->CallObjectMethod(env, j_pmap, MID_PMAP_PUT, j_n, j_s);

    j_n = (*env)->NewStringUTF(env, OC_RSRVD_MFG_DATE);
    j_s = (*env)->NewStringUTF(env,  c_payload->info.dateOfManufacture);
    (*env)->CallObjectMethod(env, j_pmap, MID_PMAP_PUT, j_n, j_s);

    j_n = (*env)->NewStringUTF(env, OC_RSRVD_PLATFORM_VERSION);
    j_s = (*env)->NewStringUTF(env,  c_payload->info.platformVersion);
    (*env)->CallObjectMethod(env, j_pmap, MID_PMAP_PUT, j_n, j_s);

    j_n = (*env)->NewStringUTF(env, OC_RSRVD_OS_VERSION);
    j_s = (*env)->NewStringUTF(env,  c_payload->info.operatingSystemVersion);
    (*env)->CallObjectMethod(env, j_pmap, MID_PMAP_PUT, j_n, j_s);

    j_n = (*env)->NewStringUTF(env, OC_RSRVD_HARDWARE_VERSION);
    j_s = (*env)->NewStringUTF(env,  c_payload->info.hardwareVersion);
    (*env)->CallObjectMethod(env, j_pmap, MID_PMAP_PUT, j_n, j_s);

    j_n = (*env)->NewStringUTF(env, OC_RSRVD_FIRMWARE_VERSION);
    j_s = (*env)->NewStringUTF(env,  c_payload->info.firmwareVersion);
    (*env)->CallObjectMethod(env, j_pmap, MID_PMAP_PUT, j_n, j_s);

    j_n = (*env)->NewStringUTF(env, OC_RSRVD_SUPPORT_URL);
    j_s = (*env)->NewStringUTF(env,  c_payload->info.supportUrl);
    (*env)->CallObjectMethod(env, j_pmap, MID_PMAP_PUT, j_n, j_s);

    j_n = (*env)->NewStringUTF(env, OC_RSRVD_SYSTEM_TIME);
    j_s = (*env)->NewStringUTF(env,  c_payload->info.systemTime);
    (*env)->CallObjectMethod(env, j_pmap, MID_PMAP_PUT, j_n, j_s);

    (*env)->SetObjectField(env, j_Payload, FID_PAYLOAD_PROPERTIES, j_pmap);

    /* printf("OCPlatformPayload_to_Payload EXIT\n"); */
    return j_Payload;
}

/*
 * OCDevicePayload_to_Payload
 * compensate for OCDeviceInfo wierdness
 */
jobject OCDevicePayload_to_Payload(JNIEnv* env, OCDevicePayload* c_payload)
{
    /* printf("OCDevicePayload_to_Payload ENTRY\n"); */
    jobject j_Payload = (*env)->NewObject(env, K_PAYLOAD, MID_PAYLOAD_CTOR);
    if (j_Payload == NULL) {
	THROW_JNI_EXCEPTION("NewObject failed for PayloadForDevice\n");
	return NULL;
    }

    /* _rtypes */
    jobject j_rtypes  = (*env)->NewObject(env, K_LINKED_LIST, MID_LL_CTOR);
    if (j_rtypes == NULL) {
	printf("NewObject failed for rtypes LinkedList\n");
	return NULL;
    }
    OCStringLL* c_rtypes = c_payload->types;
    jstring j_rtype;
    while(c_rtypes) {
	j_rtype = (*env)->NewStringUTF(env, c_rtypes->value);
	jboolean jb = (*env)->CallBooleanMethod(env, j_rtypes, MID_LL_ADD, j_rtype);
	if (!jb) {
	    printf("CallBooleanMethod failed on MID_LL_ADD for rtypes for Devices\n");
	}
	c_rtypes = c_rtypes->next;
    }
    (*env)->SetObjectField(env, j_Payload, FID_PAYLOAD_RTYPES, j_rtypes);

    /* _interfaces */
    jobject j_ifaces  = (*env)->NewObject(env, K_LINKED_LIST, MID_LL_CTOR);
    if (j_ifaces == NULL) {
	printf("NewObject failed for ifaces LinkedList\n");
	return NULL;
    }
    OCStringLL* c_ifaces = c_payload->interfaces;
    jstring j_iface;
    while(c_ifaces) {
	/* printf("c resource type: %s\n", resource_types->resourcetypename); */
	j_iface = (*env)->NewStringUTF(env, c_ifaces->value);
	jboolean jb = (*env)->CallBooleanMethod(env, j_ifaces, MID_LL_ADD, j_iface);
	if (!jb) {
	    printf("CallBooleanMethod failed on MID_LL_ADD for ifaces of Devices\n");
	}
	c_ifaces = c_ifaces->next;
    }
    (*env)->SetObjectField(env, j_Payload, FID_PAYLOAD_IFS, j_ifaces);

    /* OCDeviceInfo - convert to PropertyMap */
    jobject j_pmap  = (*env)->NewObject(env, K_PMAP, MID_PMAP_CTOR);
    if (j_pmap == NULL) {
	THROW_JNI_EXCEPTION("NewObject failed for PropertyMap\n");
	return NULL;
    }
    jstring j_n = (*env)->NewStringUTF(env, OC_RSRVD_DEVICE_ID);         /* di */
    jstring j_s = (*env)->NewStringUTF(env, c_payload->sid);
    (*env)->CallObjectMethod(env, j_pmap, MID_PMAP_PUT, j_n, j_s);

    j_n = (*env)->NewStringUTF(env, OC_RSRVD_DEVICE_NAME);               /* n */
    j_s = (*env)->NewStringUTF(env,  c_payload->deviceName);
    (*env)->CallObjectMethod(env, j_pmap, MID_PMAP_PUT, j_n, j_s);

    j_n = (*env)->NewStringUTF(env, OC_RSRVD_SPEC_VERSION);              /* icv */
    j_s = (*env)->NewStringUTF(env,  c_payload->specVersion);
    (*env)->CallObjectMethod(env, j_pmap, MID_PMAP_PUT, j_n, j_s);

    /* data model version(s) - csv list */
    j_n = (*env)->NewStringUTF(env, OC_RSRVD_DATA_MODEL_VERSION);       /* dmv */
    jobject j_dmvs  = (*env)->NewObject(env, K_LINKED_LIST, MID_LL_CTOR);
    if (j_dmvs == NULL) {
	printf("NewObject failed for dmvs LinkedList\n");
	return NULL;
    }
    OCStringLL* c_dmvs = c_payload->dataModelVersions;
    jstring j_dmv;
    while(c_dmvs) {
	j_dmv = (*env)->NewStringUTF(env, c_dmvs->value);
	jboolean jb = (*env)->CallBooleanMethod(env, j_dmvs, MID_LL_ADD, j_dmv);
	if (!jb) {
	    printf("CallBooleanMethod failed on MID_LL_ADD for dmvs of Devices\n");
	}
	c_dmvs = c_dmvs->next;
    }
    (*env)->CallObjectMethod(env, j_pmap, MID_PMAP_PUT, j_n, j_dmvs);

    (*env)->SetObjectField(env, j_Payload, FID_PAYLOAD_PROPERTIES, j_pmap);

    /* printf("OCDevicePayload_to_Payload EXIT\n"); */
    return j_Payload;
}

#endif	/* IOCHIBITY defined */

/*
 * OCResourcePayload_to_Payload
 */
jobject OCResourcePayload_to_Payload(JNIEnv* env, OCResourcePayload* c_payload)
{
    /* printf("OCResourcePayload_to_Payload ENTRY\n"); */
    jobject j_Payload = (*env)->NewObject(env, K_PAYLOAD, MID_PAYLOAD_CTOR);
    if (j_Payload == NULL) {
	THROW_JNI_EXCEPTION("NewObject failed for PayloadForDevice\n");
	return NULL;
    }

    /* _rtypes */
    jobject j_rtypes  = (*env)->NewObject(env, K_LINKED_LIST, MID_LL_CTOR);
    if (j_rtypes == NULL) {
	printf("NewObject failed for rtypes LinkedList\n");
	return NULL;
    }
    OCStringLL* c_rtypes = c_payload->types;
    jstring j_rtype;
    while(c_rtypes) {
	j_rtype = (*env)->NewStringUTF(env, c_rtypes->value);
	jboolean jb = (*env)->CallBooleanMethod(env, j_rtypes, MID_LL_ADD, j_rtype);
	if (!jb) {
	    printf("CallBooleanMethod failed on MID_LL_ADD for rtypes for Devices\n");
	}
	c_rtypes = c_rtypes->next;
    }
    (*env)->SetObjectField(env, j_Payload, FID_PAYLOAD_RTYPES, j_rtypes);

    /* _interfaces */
    jobject j_ifaces  = (*env)->NewObject(env, K_LINKED_LIST, MID_LL_CTOR);
    if (j_ifaces == NULL) {
	printf("NewObject failed for ifaces LinkedList\n");
	return NULL;
    }
    OCStringLL* c_ifaces = c_payload->interfaces;
    jstring j_iface;
    while(c_ifaces) {
	/* printf("c resource type: %s\n", resource_types->resourcetypename); */
	j_iface = (*env)->NewStringUTF(env, c_ifaces->value);
	jboolean jb = (*env)->CallBooleanMethod(env, j_ifaces, MID_LL_ADD, j_iface);
	if (!jb) {
	    printf("CallBooleanMethod failed on MID_LL_ADD for ifaces of Devices\n");
	}
	c_ifaces = c_ifaces->next;
    }
    (*env)->SetObjectField(env, j_Payload, FID_PAYLOAD_IFS, j_ifaces);

    /* _uri */
    jstring j_uri = (*env)->NewStringUTF(env, c_payload->uri);
    (*env)->SetObjectField(env, j_Payload, FID_PAYLOAD_URI_PATH, j_uri);

    /* properties */
    jobject j_pmap  = (*env)->NewObject(env, K_PMAP, MID_PMAP_CTOR);
    if (j_pmap == NULL) {
	THROW_JNI_EXCEPTION("NewObject failed for PropertyMap\n");
	return NULL;
    }
    jstring j_n = (*env)->NewStringUTF(env, "bitmap");
    jobject j_obj =  (*env)->NewObject(env, K_BYTE, MID_BYTE_CTOR, c_payload->bitmap);
    (*env)->CallObjectMethod(env, j_pmap, MID_PMAP_PUT, j_n, j_obj);

    j_n = (*env)->NewStringUTF(env, "secure");
    j_obj =  (*env)->NewObject(env, K_BOOLEAN, MID_BOOL_CTOR, c_payload->secure);
    (*env)->CallObjectMethod(env, j_pmap, MID_PMAP_PUT, j_n, j_obj);

    j_n = (*env)->NewStringUTF(env, "port");
    j_obj =  (*env)->NewObject(env, K_INTEGER, MID_INT_CTOR, (int)c_payload->port);
    (*env)->CallObjectMethod(env, j_pmap, MID_PMAP_PUT, j_n, j_obj);

    (*env)->SetObjectField(env, j_Payload, FID_PAYLOAD_PROPERTIES, j_pmap);

    return j_Payload;
}

/*
 * OCDevicePayload_to_Payload
 */
jobject OCDiscoveryPayload_to_Payload(JNIEnv* env, OCDiscoveryPayload* c_payload)
{
    /* printf("OCDiscoveryPayload_to_Payload ENTRY\n"); */
    jobject j_Payload = (*env)->NewObject(env, K_PAYLOAD, MID_PAYLOAD_CTOR);
    if (j_Payload == NULL) {
	THROW_JNI_EXCEPTION("NewObject failed for PayloadForDevice\n");
	return NULL;
    }

    /* _rtypes */
    jobject j_rtypes  = (*env)->NewObject(env, K_LINKED_LIST, MID_LL_CTOR);
    if (j_rtypes == NULL) {
	printf("NewObject failed for rtypes LinkedList\n");
	return NULL;
    }
    OCStringLL* c_rtypes = c_payload->type;
    jstring j_rtype;
    while(c_rtypes) {
	j_rtype = (*env)->NewStringUTF(env, c_rtypes->value);
	jboolean jb = (*env)->CallBooleanMethod(env, j_rtypes, MID_LL_ADD, j_rtype);
	if (!jb) {
	    printf("CallBooleanMethod failed on MID_LL_ADD for rtypes for Devices\n");
	}
	c_rtypes = c_rtypes->next;
    }
    (*env)->SetObjectField(env, j_Payload, FID_PAYLOAD_RTYPES, j_rtypes);

    /* _interfaces */
    jobject j_ifaces  = (*env)->NewObject(env, K_LINKED_LIST, MID_LL_CTOR);
    if (j_ifaces == NULL) {
	printf("NewObject failed for ifaces LinkedList\n");
	return NULL;
    }
    OCStringLL* c_ifaces = c_payload->iface;
    jstring j_iface;
    while(c_ifaces) {
	/* printf("c resource type: %s\n", resource_types->resourcetypename); */
	j_iface = (*env)->NewStringUTF(env, c_ifaces->value);
	jboolean jb = (*env)->CallBooleanMethod(env, j_ifaces, MID_LL_ADD, j_iface);
	if (!jb) {
	    printf("CallBooleanMethod failed on MID_LL_ADD for ifaces of Devices\n");
	}
	c_ifaces = c_ifaces->next;
    }
    (*env)->SetObjectField(env, j_Payload, FID_PAYLOAD_IFS, j_ifaces);

    /* _uri */
    jstring j_uri = (*env)->NewStringUTF(env, c_payload->uri);
    (*env)->SetObjectField(env, j_Payload, FID_PAYLOAD_URI_PATH, j_uri);

    /* convert remaining flds to PropertyMap */
    jobject j_pmap  = (*env)->NewObject(env, K_PMAP, MID_PMAP_CTOR);
    if (j_pmap == NULL) {
	THROW_JNI_EXCEPTION("NewObject failed for Discovery PropertyMap\n");
	return NULL;
    }

    jstring j_n = (*env)->NewStringUTF(env, "baseURI");
    jstring j_s = (*env)->NewStringUTF(env, c_payload->baseURI);
    (*env)->CallObjectMethod(env, j_pmap, MID_PMAP_PUT, j_n, j_s);

    j_n = (*env)->NewStringUTF(env, OC_RSRVD_DEVICE_ID);         /* di */
    j_s = (*env)->NewStringUTF(env, c_payload->sid);
    (*env)->CallObjectMethod(env, j_pmap, MID_PMAP_PUT, j_n, j_s);

    j_n = (*env)->NewStringUTF(env, OC_RSRVD_DEVICE_NAME);               /* n */
    j_s = (*env)->NewStringUTF(env,  c_payload->name);
    (*env)->CallObjectMethod(env, j_pmap, MID_PMAP_PUT, j_n, j_s);

    (*env)->SetObjectField(env, j_Payload, FID_PAYLOAD_PROPERTIES, j_pmap);

    /* now the resource list */
    jobject j_resource_list  = (*env)->NewObject(env, K_LINKED_LIST, MID_LL_CTOR);
    if (j_resource_list == NULL) {
	printf("NewObject failed for resource_list LinkedList\n");
	return NULL;
    }
    OCResourcePayload* c_resource_list = c_payload->resources;
    jobject j_resource;
    while(c_resource_list) {
	j_resource = OCResourcePayload_to_Payload(env, c_resource_list);
	jboolean jb = (*env)->CallBooleanMethod(env, j_resource_list, MID_LL_ADD, j_resource);
	if (!jb) {
	    printf("CallBooleanMethod failed on MID_LL_ADD for resource_list of Devices\n");
	}
	c_resource_list = c_resource_list->next;
    }

    (*env)->SetObjectField(env, j_Payload, FID_PAYLOAD_CHILDREN, j_resource_list);

    /* printf("OCDiscoveryPayload_to_Payload EXIT\n"); */
    return j_Payload;
}

/*
 * OCRepPayload_to_Payload
 */
jobject OCRepPayload_to_Payload(JNIEnv* env, OCRepPayload* c_payload)
{
    printf("OCRepPayload_to_Payload ENTRY\n");
    jobject j_Payload = (*env)->NewObject(env, K_PAYLOAD, MID_PAYLOAD_CTOR);
    if (j_Payload == NULL) {
	THROW_JNI_EXCEPTION("NewObject failed for PayloadForDevice\n");
	return NULL;
    }

    /* _rtypes */
    jobject j_rtypes  = (*env)->NewObject(env, K_LINKED_LIST, MID_LL_CTOR);
    if (j_rtypes == NULL) {
	printf("NewObject failed for rtypes LinkedList\n");
	return NULL;
    }
    OCStringLL* c_rtypes = c_payload->types;
    jstring j_rtype;
    while(c_rtypes) {
	j_rtype = (*env)->NewStringUTF(env, c_rtypes->value);
	jboolean jb = (*env)->CallBooleanMethod(env, j_rtypes, MID_LL_ADD, j_rtype);
	if (!jb) {
	    printf("CallBooleanMethod failed on MID_LL_ADD for rtypes for Devices\n");
	}
	c_rtypes = c_rtypes->next;
    }
    (*env)->SetObjectField(env, j_Payload, FID_PAYLOAD_RTYPES, j_rtypes);
    /* _interfaces */
    jobject j_ifaces  = (*env)->NewObject(env, K_LINKED_LIST, MID_LL_CTOR);
    if (j_ifaces == NULL) {
	printf("NewObject failed for ifaces LinkedList\n");
	return NULL;
    }
    OCStringLL* c_ifaces = c_payload->interfaces;
    jstring j_iface;
    while(c_ifaces) {
	/* printf("c resource type: %s\n", resource_types->resourcetypename); */
	j_iface = (*env)->NewStringUTF(env, c_ifaces->value);
	jboolean jb = (*env)->CallBooleanMethod(env, j_ifaces, MID_LL_ADD, j_iface);
	if (!jb) {
	    printf("CallBooleanMethod failed on MID_LL_ADD for ifaces of Devices\n");
	}
	c_ifaces = c_ifaces->next;
    }
    (*env)->SetObjectField(env, j_Payload, FID_PAYLOAD_IFS, j_ifaces);

    /* _uri */
    jstring j_uri = (*env)->NewStringUTF(env, c_payload->uri);
    (*env)->SetObjectField(env, j_Payload, FID_PAYLOAD_URI_PATH, j_uri);

    /* properties */
    jobject j_pmap  = (*env)->NewObject(env, K_PMAP, MID_PMAP_CTOR);
    if (j_pmap == NULL) {
	THROW_JNI_EXCEPTION("NewObject failed for Discovery PropertyMap\n");
	return NULL;
    }

    OCRepPayloadValue* vmap = c_payload->values;
    jstring j_key;
    jobject j_val;
    while (vmap) {
	j_key = (*env)->NewStringUTF(env, vmap->name);
	switch(vmap->type) {
	case OCREP_PROP_NULL:
	    break;
	case OCREP_PROP_INT:
	    j_val = (*env)->NewObject(env, K_INTEGER, MID_INT_CTOR, vmap->i);
	    (*env)->CallObjectMethod(env, j_pmap, MID_PMAP_PUT, j_key, j_val);
	    break;
	case OCREP_PROP_DOUBLE:
	    j_val = (*env)->NewObject(env, K_DOUBLE, MID_DBL_CTOR, vmap->d);
	    (*env)->CallObjectMethod(env, j_pmap, MID_PMAP_PUT, j_key, j_val);
	    break;
	case OCREP_PROP_BOOL:
	    j_val = (*env)->NewObject(env, K_BOOLEAN, MID_BOOL_CTOR, vmap->b);
	    (*env)->CallObjectMethod(env, j_pmap, MID_PMAP_PUT, j_key, j_val);
	    break;
	case OCREP_PROP_STRING:
	    j_val = (*env)->NewStringUTF(env, vmap->str);
	    /* j_val = (*env)->NewObject(env, K_STRING, MID_STR_CTOR, j_s); */
	    (*env)->CallObjectMethod(env, j_pmap, MID_PMAP_PUT, j_key, j_val);
	    break;
	case OCREP_PROP_BYTE_STRING:
	    break;
	case OCREP_PROP_OBJECT:
	    break;
	case OCREP_PROP_ARRAY:
	    break;
	default:
	    break;
	}
	vmap = vmap->next;
    }
    (*env)->SetObjectField(env, j_Payload, FID_PAYLOAD_PROPERTIES, j_pmap);

    printf("OCRepPayload_to_Payload EXIT\n");
    return j_Payload;
}

/* EXTERNAL */

/*
 * Class:     org_iochibity_Message
 * Method:    getPayloadList
 * Signature: ()Lorg/iochibity/PayloadList;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_Message_getPayloadList
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    printf("Java_org_iochibity_Message_getPayloadList ENTRY\n");

    OCPayload* c_payload = (OCPayload*)(intptr_t)
	(*env)->GetLongField(env, this, FID_MSG_PAYLOAD_HANDLE);
    if (c_payload == NULL) {
	// ok - maybe no payload
	/* THROW_JNI_EXCEPTION("GetLongField failed for FID_MSG_PAYLOAD_HANDLE on Message"); */
	return NULL;
    }

    /* create a new PayloadList */
    jobject pll  = (*env)->NewObject(env, K_PAYLOAD_LIST, MID_PLL_CTOR);
    if (pll == NULL) {
	THROW_JNI_EXCEPTION("NewObject failed for PayloadList\n");
	return NULL;
    }

    /* create new Payload object and adding to list */
    jobject j_payload = NULL;
    jboolean j_b;
    while (c_payload != NULL) {
	switch (c_payload->type) {
	case PAYLOAD_TYPE_DISCOVERY:
	    j_payload = OCDiscoveryPayload_to_Payload(env, (OCDiscoveryPayload*) c_payload);
	    /* add payload to payload list */
	    j_b = (*env)->CallBooleanMethod(env, pll, MID_PLL_ADD, j_payload);
	    if (!j_b) {
		THROW_JNI_EXCEPTION("CallBooleanMethod failed for mid_add\n");
	    }
	    c_payload = (OCPayload*) ((OCDiscoveryPayload*)c_payload)->next;
	    break;
	case PAYLOAD_TYPE_DEVICE:
#ifdef IOCHIBITY    /* 1.1.1: */
	    j_payload = OCDevicePayload_to_Payload(env, (OCDevicePayload*) c_payload);
#else
	    // OCDevicePayload has been removed.
	    printf("XXXXXXXXXXXXXXXX: received PAYLOAD_TYPE_DEVICE\n");
#endif
	    /* add payload to payload list */
	    j_b = (*env)->CallBooleanMethod(env, pll, MID_PLL_ADD, j_payload);
	    if (!j_b) {
		THROW_JNI_EXCEPTION("CallBooleanMethod failed for mid_add\n");
	    }
	    c_payload = NULL;
	    break;
	case PAYLOAD_TYPE_PLATFORM:
#ifdef IOCHIBITY    /* 1.1.1: */
	    j_payload = OCPlatformPayload_to_Payload(env, (OCPlatformPayload*) c_payload);
#else
	    // OCPlatformPayload has been removed.
	    printf("XXXXXXXXXXXXXXXX: received PAYLOAD_TYPE_PLATFORM\n");
#endif
	    /* add payload to payload list */
	    j_b = (*env)->CallBooleanMethod(env, pll, MID_PLL_ADD, j_payload);
	    if (!j_b) {
		THROW_JNI_EXCEPTION("CallBooleanMethod failed for mid_add\n");
	    }
	    c_payload = NULL;
	    break;
	case PAYLOAD_TYPE_REPRESENTATION:
	    j_payload = OCRepPayload_to_Payload(env, (OCRepPayload*) c_payload);
	    /* add payload to payload list */
	    j_b = (*env)->CallBooleanMethod(env, pll, MID_PLL_ADD, j_payload);
	    if (!j_b) {
		THROW_JNI_EXCEPTION("CallBooleanMethod failed for mid_add\n");
	    }
	    c_payload = (OCPayload*) ((OCRepPayload*)c_payload)->next;
	    break;
	case PAYLOAD_TYPE_INVALID:
	case PAYLOAD_TYPE_SECURITY:
	case PAYLOAD_TYPE_PRESENCE:
	/* case PAYLOAD_TYPE_RD: */
	default:
	    c_payload = NULL;
	    break;
	}
    }
    printf("Java_org_iochibity_Message_getPayloadList EXIT\n");
    return pll;
}

/*
 * Class:     org_iochibity_Message
 * Method:    getPayloadType
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_iochibity_Message_getPayloadType
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    /* printf("Java_org_iochibity_Message_getPayloadType ENTRY\n"); */
    /* 1. get payloadHandle from this */
    OCPayload* j_payload = (OCPayload*)(intptr_t)
	(*env)->GetLongField(env, this, FID_MsgRspIn_PAYLOAD_HANDLE);
    if (j_payload == NULL) {
	// ok, maybe no payload?
	/* THROW_JNI_EXCEPTION("GetLongField failed for FID_MSG_PAYLOAD_HANDLE on Message"); */
	return -1;
    } else
	return j_payload->type;
}

/*
 * Class:     org_iochibity_Message
 * Method:    getOptions
 * Signature: ()Ljava/util/List;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_Message_getOptions
(JNIEnv * env, jobject this)
{
    OC_UNUSED(env);
    OC_UNUSED(this);
    printf("Java_org_iochibity_Message_getOptions\n");
    /* if (FID_DRSPI_PTR_OPTIONS == NULL) { */
    /* 	FID_DRSPI_PTR_OPTIONS = (*env)->GetFieldID(env, K_MSG_RESPONSE_IN, "ptr_Options", "J"); */
    /* 	if (FID_DRSPI_PTR_OPTIONS == NULL) { */
    /* 	    THROW_JNI_EXCEPTION("GetFieldID failed for 'ptr_Options' of MsgResponseIn\n"); */
    /* 	    return -1; */
    /* 	} */
    /* } */
    /* if (FID_DRSPI_OPTION_COUNT == NULL) { */
    /* 	FID_DRSPI_OPTION_COUNT = (*env)->GetFieldID(env, K_MSG_RESPONSE_IN, "optionCount", "I"); */
    /* 	if (FID_DRSPI_OPTION_COUNT == NULL) { */
    /* 	    THROW_JNI_EXCEPTION("GetFieldID failed for 'optionCount' of MsgResponseIn\n"); */
    /* 	    return -1; */
    /* 	} */
    /* } */
    /* if (MID_DRSPI_GET_OPTIONS == NULL) { */
    /* 	MID_DRSPI_GET_OPTIONS = (*env)->GetMethodID(env, K_MSG_RESPONSE_IN, */
    /* 						    "getOptions", "()Ljava/util/List;"); */
    /* 	if (MID_DRSPI_GET_OPTIONS == NULL) { */
    /* 	    THROW_JNI_EXCEPTION("GetFieldID failed for 'getOptions' of MsgResponseIn\n"); */
    /* 	    return -1; */
    /* 	} */
    /* } */
    return NULL;
}

