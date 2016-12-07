#include <ctype.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "org_iochibity_OCF.h"
#include "ocf_exceptions.h"
#include "jni_utils.h"

#include "octypes.h"
#include "ocresource.h"
#include "ocstack.h"
#include "oic_malloc.h"
#include "oic_string.h"

/* PRIVATE */
pthread_t pt_work;
bool g_quit_flag = false;

const char* g_config_fname;

static jclass    K_INTEGER;		/* 32-bit ints */
static jmethodID MID_INT_INTVALUE;	/* Integer.intValue() returns int */
static jclass K_LONG;			/* 64-bit ints */
static jmethodID MID_LONG_LONGVALUE;	/* Long.longValue() returns long */
/* jclass K_BIGINTEGER		/\* arbitrary bits *\/ */
static jclass K_BOOLEAN;
static jmethodID MID_BOOL_BOOLVALUE;	/* Boolean.booleanValue() returns boolean */
static jclass K_DOUBLE;
static jmethodID MID_DBL_DBLVALUE;	/* Double.doubleValue() returns double */
static jclass K_STRING;
static jclass K_LIST;		   /* OCByteString => List<Byte> */
static jclass K_BYTE;
static jclass K_OBJECT;
/* jclass K_ARRAY; - List */

static jclass   K_PAYLOAD;
static jfieldID FID_PAYLOAD_HANDLE;
static jfieldID FID_PAYLOAD_TYPE;

static jclass   K_PFRS; 			/* class for PayloadForResourceState */
static jfieldID FID_PFRS_URI;
static jfieldID FID_PFRS_RTYPES;
static jfieldID FID_PFRS_INTERFACES;
static jfieldID FID_PFRS_PROPERTIES;

static jclass   K_PMAP; 			/* class for PropertyMap */
static jmethodID MID_PMAP_SIZE;
static jmethodID MID_PMAP_ENTRYSET;
static jmethodID MID_PMAP_GET;

static jclass   K_SET; 			/* interface java.util.Set */
static jmethodID MID_SET_ITERATOR;

static jclass   K_ITER; 			/* interface java.util.Iterator */
static jmethodID MID_ITER_HASNEXT;
static jmethodID MID_ITER_NEXT;
static jmethodID MID_ITER_REMOVE;

static jclass   K_MAPENTRY; 			/* interface java.util.Map.Entry */
static jmethodID MID_ENTRY_GETKEY;
static jmethodID MID_ENTRY_GETVALUE;

/* thread routine - service client requests */
void *troutine_work(void *arg)
{
    OC_UNUSED(arg);
    printf("Entering server work thread...\n");

    while (!g_quit_flag) {
	if (OCProcess() != OC_STACK_OK) {
	    printf("OCStack process error\n");
	}
    }
    printf("Exiting server work thread...\n");
    /* we're the only thread left, pthread_exit(NULL) would kill us,
       but not the process. */
    /* exit(0); */
    return NULL;
}

FILE* server_fopen(const char *path, const char *mode)
{
    (void)path;
    printf("%s: fopening %s", __func__, g_config_fname);
    return fopen(g_config_fname, mode);
    /* return fopen(path, mode); */
}

/*
 *
 */
void prep_java(JNIEnv* env)
{
    jclass klass = NULL;
    /* printf("prep_java ENTRY\n"); */
    if (K_INTEGER == NULL) {	/* 32 bits */
	klass = (*env)->FindClass(env, "Ljava/lang/Integer;");
	if (klass == NULL) {
	    THROW_JNI_EXCEPTION("FindClass failed for java/lang/Integer");
	    return;
	}
	K_INTEGER = (jclass)(*env)->NewGlobalRef(env, klass);
	(*env)->DeleteLocalRef(env, klass);
    }
    if (MID_INT_INTVALUE == NULL) {
	MID_INT_INTVALUE = (*env)->GetMethodID(env, K_INTEGER, "intValue", "()I");
	if (MID_INT_INTVALUE == NULL) {
	    THROW_JNI_EXCEPTION("GetMethodID failed for 'intValue' of Integer\n");
	    return;
	}
    }
    if (K_LONG == NULL) {	/* 64 bits */
	klass = (*env)->FindClass(env, "Ljava/lang/Long;");
	if (klass == NULL) {
	    THROW_JNI_EXCEPTION("FindClass failed for java/lang/Long");
	    return;
	}
	K_LONG = (jclass)(*env)->NewGlobalRef(env, klass);
	(*env)->DeleteLocalRef(env, klass);
    }
    if (MID_LONG_LONGVALUE == NULL) {
	MID_LONG_LONGVALUE = (*env)->GetMethodID(env, K_LONG, "longValue", "()J");
	if (MID_LONG_LONGVALUE == NULL) {
	    THROW_JNI_EXCEPTION("GetMethodID failed for 'longValue' of Long\n");
	    return;
	}
    }

    if (K_BOOLEAN == NULL) {
	klass = (*env)->FindClass(env, "Ljava/lang/Boolean;");
	if (klass == NULL) {
	    THROW_JNI_EXCEPTION("FindClass failed for java/lang/Boolean");
	    return;
	}
	K_BOOLEAN = (jclass)(*env)->NewGlobalRef(env, klass);
	(*env)->DeleteLocalRef(env, klass);
    }
    if (MID_BOOL_BOOLVALUE == NULL) {
	MID_BOOL_BOOLVALUE = (*env)->GetMethodID(env, K_BOOLEAN, "booleanValue", "()Z");
	if (MID_BOOL_BOOLVALUE == NULL) {
	    THROW_JNI_EXCEPTION("GetMethodID failed for 'booleanValue' of Boolean\n");
	    return;
	}
    }
    if (K_DOUBLE == NULL) {
	klass = (*env)->FindClass(env, "Ljava/lang/Double;");
	if (klass == NULL) {
	    THROW_JNI_EXCEPTION("FindClass failed for java/lang/Double");
	    return;
	}
	K_DOUBLE = (jclass)(*env)->NewGlobalRef(env, klass);
	(*env)->DeleteLocalRef(env, klass);
    }
    if (MID_DBL_DBLVALUE == NULL) {
	MID_DBL_DBLVALUE = (*env)->GetMethodID(env, K_DOUBLE, "doubleValue", "()D");
	if (MID_DBL_DBLVALUE == NULL) {
	    THROW_JNI_EXCEPTION("GetMethodID failed for 'DBLValue' of Double\n");
	    return;
	}
    }
    if (K_STRING == NULL) {
	klass = (*env)->FindClass(env, "Ljava/lang/String;");
	if (klass == NULL) {
	    THROW_JNI_EXCEPTION("FindClass failed for java/lang/String");
	    return;
	}
	K_STRING = (jclass)(*env)->NewGlobalRef(env, klass);
	(*env)->DeleteLocalRef(env, klass);
    }
    /* a byte array must be stored as List<Byte> */
    if (K_LIST == NULL) {
	klass = (*env)->FindClass(env, "Ljava/util/List;");
	if (klass == NULL) {
	    THROW_JNI_EXCEPTION("FindClass failed for java/util/List");
	    return;
	}
	K_LIST = (jclass)(*env)->NewGlobalRef(env, klass);
	(*env)->DeleteLocalRef(env, klass);
    }
    if (K_BYTE == NULL) {
	klass = (*env)->FindClass(env, "Ljava/lang/Byte;");
	if (klass == NULL) {
	    THROW_JNI_EXCEPTION("FindClass failed for java/lang/Byte");
	    return;
	}
	K_BYTE = (jclass)(*env)->NewGlobalRef(env, klass);
	(*env)->DeleteLocalRef(env, klass);
    }
    if (K_OBJECT == NULL) {
	klass = (*env)->FindClass(env, "Ljava/lang/Integer;");
	if (klass == NULL) {
	    THROW_JNI_EXCEPTION("FindClass failed for java/lang/Integer");
	    return;
	}
	K_OBJECT = (jclass)(*env)->NewGlobalRef(env, klass);
	(*env)->DeleteLocalRef(env, klass);
    }
}

/*
*
*/
void prep_pfrs(JNIEnv* env)
{
    if (K_PFRS == NULL) {
	K_PFRS = (*env)->FindClass(env, "Lorg/iochibity/PayloadForResourceState;");
	if (K_PFRS == NULL) {
	    THROW_JNI_EXCEPTION("FindClass failed for org/iochibity/PayloadForResourceState\n");
	    return;
	}
    }
    if (FID_PFRS_URI == NULL) {
	FID_PFRS_URI = (*env)->GetFieldID(env, K_PFRS, "_uri", "Ljava/lang/String;");
	if (FID_PFRS_URI == NULL) {
	    THROW_JNI_EXCEPTION("GetFieldID failed for '_uri' of PFRS");
	    return;
	}
    }
    if (FID_PFRS_RTYPES == NULL) {
	FID_PFRS_RTYPES = (*env)->GetFieldID(env, K_PFRS, "_rtypes", "Ljava/util/List;");
	if (FID_PFRS_RTYPES == NULL) {
	    THROW_JNI_EXCEPTION("GetFieldID failed for '_uri' of PFRS");
	    return;
	}
    }
    if (FID_PFRS_INTERFACES == NULL) {
	FID_PFRS_INTERFACES = (*env)->GetFieldID(env, K_PFRS, "_interfaces", "Ljava/util/List;");
	if (FID_PFRS_INTERFACES == NULL) {
	    THROW_JNI_EXCEPTION("GetFieldID failed for '_interfaces' of PFRS");
	    return;
	}
    }
    if (FID_PFRS_PROPERTIES == NULL) {
	FID_PFRS_PROPERTIES = (*env)->GetFieldID(env, K_PFRS, "_properties", "Lorg/iochibity/PropertyMap;");
	if (FID_PFRS_PROPERTIES == NULL) {
	    THROW_JNI_EXCEPTION("GetFieldID failed for '_properties' of PFRS");
	    return;
	}
    }
}

/*
 *
 */
void prep_pmap(JNIEnv* env)
{
    /* printf("prep_pmap ENTRY\n"); */
    /* iteration over property map in java: */
    /* Iterator it = mp.entrySet().iterator(); */
    /* while (it.hasNext()) { */
    /*     Map.Entry pair = (Map.Entry)it.next(); */
    /*     System.out.println(pair.getKey() + " = " + pair.getValue()); */
    /*     it.remove(); // avoids a ConcurrentModificationException */
    /* } */

    /* PropertyMap */
    if (K_PMAP == NULL) {
	K_PMAP = (*env)->FindClass(env, "Lorg/iochibity/PropertyMap;");
	if (K_PMAP == NULL) {
	    THROW_JNI_EXCEPTION("FindClass failed for org/iochibity/PropertyMap\n");
	    return;
	}
    }
    if (MID_PMAP_SIZE == NULL) {
	MID_PMAP_SIZE = (*env)->GetMethodID(env, K_PMAP, "size", "()I");
	if (MID_PMAP_SIZE == NULL) {
	    THROW_JNI_EXCEPTION("GetMethodID failed for 'size' of PropertyMap\n");
	    return;
	}
    }
    if (MID_PMAP_ENTRYSET == NULL) {
	MID_PMAP_ENTRYSET = (*env)->GetMethodID(env, K_PMAP, "entrySet", "()Ljava/util/Set;");
	if (MID_PMAP_ENTRYSET == NULL) {
	    THROW_JNI_EXCEPTION("GetMethodID failed for 'entrySet' of PropertyMap\n");
	    return;
	}
    }
    if (MID_PMAP_GET == NULL) {
	MID_PMAP_GET = (*env)->GetMethodID(env, K_PMAP, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");
	if (MID_PMAP_GET == NULL) {
	    THROW_JNI_EXCEPTION("GetMethodID failed for 'get' of PropertyMap\n");
	    return;
	}
    }

     /* Map.Entry */
    if (K_MAPENTRY == NULL) {
	K_MAPENTRY = (*env)->FindClass(env, "Ljava/util/Map$Entry;");
	if (K_MAPENTRY == NULL) {
	    THROW_JNI_EXCEPTION("FindClass failed for java/util/Map$Entry\n");
	    return;
	}
    }
    if (MID_ENTRY_GETKEY == NULL) {
	MID_ENTRY_GETKEY = (*env)->GetMethodID(env, K_MAPENTRY, "getKey", "()Ljava/lang/Object;");
	if (MID_ENTRY_GETKEY == NULL) {
	    THROW_JNI_EXCEPTION("GetMethodID failed for 'getKey' of Map$Entry\n");
	    return;
	}
    }
    if (MID_ENTRY_GETVALUE == NULL) {
	MID_ENTRY_GETVALUE = (*env)->GetMethodID(env, K_MAPENTRY, "getValue", "()Ljava/lang/Object;");
	if (MID_ENTRY_GETVALUE == NULL) {
	    THROW_JNI_EXCEPTION("GetMethodID failed for 'getValue' of Map$Entry\n");
	    return;
	}
    }

   /* SET */
    if (K_SET == NULL) {
	K_SET = (*env)->FindClass(env, "Ljava/util/Set;");
	if (K_SET == NULL) {
	    THROW_JNI_EXCEPTION("FindClass failed for java/util/SET\n");
	    return;
	}
    }
    if (MID_SET_ITERATOR == NULL) {
	MID_SET_ITERATOR = (*env)->GetMethodID(env, K_SET, "iterator", "()Ljava/util/Iterator;");
	if (MID_SET_ITERATOR == NULL) {
	    THROW_JNI_EXCEPTION("GetMethodID failed for 'iterator' of Set\n");
	    return;
	}
    }

    /* ITER */
    if (K_ITER == NULL) {
	K_ITER = (*env)->FindClass(env, "Ljava/util/Iterator;");
	if (K_ITER == NULL) {
	    THROW_JNI_EXCEPTION("FindClass failed for java/util/Iterator\n");
	    return;
	}
    }
    if (MID_ITER_HASNEXT == NULL) {
	MID_ITER_HASNEXT = (*env)->GetMethodID(env, K_ITER, "hasNext", "()Z");
	if (MID_ITER_HASNEXT == NULL) {
	    THROW_JNI_EXCEPTION("GetMethodID failed for 'hasNext' of PropertyMap\n");
	    return;
	}
    }
    if (MID_ITER_NEXT == NULL) {
	MID_ITER_NEXT = (*env)->GetMethodID(env, K_ITER, "next", "()Ljava/lang/Object;");
	if (MID_ITER_NEXT == NULL) {
	    THROW_JNI_EXCEPTION("GetMethodID failed for 'next' of PropertyMap\n");
	    return;
	}
    }
    if (MID_ITER_REMOVE == NULL) {
	MID_ITER_REMOVE = (*env)->GetMethodID(env, K_ITER, "remove", "()V");
	if (MID_ITER_REMOVE == NULL) {
	    THROW_JNI_EXCEPTION("GetMethodID failed for 'remove' of PropertyMap\n");
	    return;
	}
    }
    /* printf("prep_pmap EXIT\n"); */
}

OCRepPayloadValue* props_to_OCRepPayloadValue(JNIEnv* env, jobject j_propmap)
{
    printf("props_to_OCRepPayloadValue ENTRY\n");
    OCRepPayloadValue* plv_head = NULL;

    /* test */
    /* plv_head = (OCRepPayloadValue*)OICCalloc(1, sizeof(OCRepPayloadValue)); */
    /* if (!plv_head) { */
    /* 	THROW_JNI_EXCEPTION("IOCCalloc failed for head OCRepPayloadValue"); */
    /* 	return NULL; */
    /* } */
    /* plv_head->name = "HeadPropNode"; */
    /* if (!plv_head->name) */
    /*     { */
    /*         OICFree(plv_head); */
    /* 	    THROW_JNI_EXCEPTION("Name assign failed for OCRepPayloadValue"); */
    /*         return NULL; */
    /*     } */

    /* plv_head->type = OCREP_PROP_INT; */
    /* plv_head->i    = 72; */

    /* OCRepPayloadValue* plv2 = (OCRepPayloadValue*)OICCalloc(1, sizeof(OCRepPayloadValue)); */
    /* if (!plv2) { */
    /* 	THROW_JNI_EXCEPTION("IOCCalloc failed for plv2 OCRepPayloadValue"); */
    /* 	return NULL; */
    /* } */
    /* plv2->name = "Node2"; */
    /* if (!plv2->name) */
    /*     { */
    /*         OICFree(plv2); */
    /* 	    THROW_JNI_EXCEPTION("Name assign failed for OCRepPayloadValue"); */
    /*         return NULL; */
    /*     } */
    /* plv2->type = OCREP_PROP_INT; */
    /* plv2->i    = 73; */
    /* plv_head->next = plv2; */

    /* iterate over j_propmap */
    /* Set eset = j_propmap.entrySet() */
    /* Iterator it = eset.iterator(); */
    /* while (it.hasNext()) { */
    /*     Map.Entry pair = (Map.Entry)it.next(); */
    /*     System.out.println(pair.getKey() + " = " + pair.getValue()); */
    /*     it.remove(); // avoids a ConcurrentModificationException */

    jobject j_set = (*env)->CallObjectMethod(env, j_propmap, MID_PMAP_ENTRYSET);
    if (j_set == NULL) {
	THROW_JNI_EXCEPTION("CallObjectMethod failed for MID_PMAP_ENTRYSET on j_propmap\n");
	return NULL;
    }
    jobject j_iter = (*env)->CallObjectMethod(env, j_set, MID_SET_ITERATOR);
    if (j_iter == NULL) {
	THROW_JNI_EXCEPTION("CallObjectMethod failed for MID_SET_ITERATOR on j_set\n");
	return NULL;
    }

    jobject j_entry;
    jobject j_key;
    jobject j_val;
    OCRepPayloadValue** plv = &plv_head;
    bool has_next = (bool)(*env)->CallObjectMethod(env, j_iter, MID_ITER_HASNEXT);
    printf("ITERATING %d\n", has_next);
    while ( has_next ) {
	char*   c_key;
	j_entry = (*env)->CallObjectMethod(env, j_iter, MID_ITER_NEXT);
	if (j_entry == NULL) {
	    printf("GOT NULL ENTRY, head: %s\n", plv_head->name);
	    /* THROW_JNI_EXCEPTION("CallObjectMethod failed for MID_ITER_NEXT on j_iter\n"); */
	    /* return plv_head; */
	    break;
	}
	j_key = (*env)->CallObjectMethod(env, j_entry, MID_ENTRY_GETKEY);
	if (j_key == NULL) {
	    THROW_JNI_EXCEPTION("CallObjectMethod failed for MID_ENTRY_GETKEY on j_entry\n");
	    return NULL;
	}
	c_key =  (char*) (*env)->GetStringUTFChars(env, j_key, NULL);

	j_val = (*env)->CallObjectMethod(env, j_entry, MID_ENTRY_GETVALUE);
	if (j_val == NULL) {
	    THROW_JNI_EXCEPTION("CallObjectMethod failed for MID_ENTRY_GETVAL on j_entry\n");
	    return NULL;
	}

	if ( (*env)->IsInstanceOf(env, j_val, K_INTEGER) ) {
	    int c_val = (int)(*env)->CallIntMethod(env, j_val, MID_INT_INTVALUE);
	    *plv = (OCRepPayloadValue*)OICCalloc(1, sizeof(OCRepPayloadValue));
	    if (!*plv) {
	    	THROW_JNI_EXCEPTION("IOCCalloc failed for *plv OCRepPayloadValue");
	    	return NULL;
	    }
	    (*plv)->name = OICStrdup(c_key);
	    (*plv)->type = OCREP_PROP_INT;
	    (*plv)->i = (int64_t)c_val;
	    printf("GOT ENTRY: %s = %lld (INT)\n", (*plv)->name, (int64_t)(*plv)->i);
	    /* (*plv)->next = *plv; */
	    (*plv)->next = NULL;
	    plv = &((*plv)->next);
	} else if ( (*env)->IsInstanceOf(env, j_val, K_DOUBLE) ) {
	    double c_val = (double)(*env)->CallDoubleMethod(env, j_val, MID_DBL_DBLVALUE);
	    *plv = (OCRepPayloadValue*)OICCalloc(1, sizeof(OCRepPayloadValue));
	    if (!*plv) {
	    	THROW_JNI_EXCEPTION("IOCCalloc failed for *plv OCRepPayloadValue");
	    	return NULL;
	    }
	    (*plv)->name = OICStrdup(c_key);
	    (*plv)->type = OCREP_PROP_DOUBLE;
	    (*plv)->d = (double)c_val;
	    printf("GOT ENTRY: %s = %f (DOUBLE)\n", (*plv)->name, (double)(*plv)->d);
	    (*plv)->next = NULL;
	    plv = &((*plv)->next);
	} else if ( (*env)->IsInstanceOf(env, j_val, K_BOOLEAN) ) {
	    bool c_val = (bool)(*env)->CallBooleanMethod(env, j_val, MID_BOOL_BOOLVALUE);
	    *plv = (OCRepPayloadValue*)OICCalloc(1, sizeof(OCRepPayloadValue));
	    if (!*plv) {
	    	THROW_JNI_EXCEPTION("IOCCalloc failed for *plv OCRepPayloadValue");
	    	return NULL;
	    }
	    (*plv)->name = OICStrdup(c_key);
	    (*plv)->type = OCREP_PROP_BOOL;
	    (*plv)->b = (bool)c_val;
	    printf("GOT ENTRY: %s = %d (BOOL)\n", (*plv)->name, (bool)(*plv)->b);
	    (*plv)->next = NULL;
	    plv = &((*plv)->next);
	} else if ( (*env)->IsInstanceOf(env, j_val, K_STRING) ) {
	    char* c_val = (char*) (*env)->GetStringUTFChars(env, j_val, NULL);
	    *plv = (OCRepPayloadValue*)OICCalloc(1, sizeof(OCRepPayloadValue));
	    if (!*plv) {
	    	THROW_JNI_EXCEPTION("IOCCalloc failed for *plv OCRepPayloadValue");
	    	return NULL;
	    }
	    (*plv)->name = OICStrdup(c_key);
	    (*plv)->type = OCREP_PROP_STRING;
	    (*plv)->str = (char*)OICStrdup(c_val);
	    (*env)->ReleaseStringUTFChars(env, j_val, c_val);
	    printf("GOT ENTRY: %s = %s (STRING)\n", (*plv)->name, (char*)(*plv)->str);
	    (*plv)->next = NULL;
	    plv = &((*plv)->next);
	} else {
	    printf("GOT ENTRY: %s of type ?\n", c_key);
	}
	/* (*env)->CallObjectMethod(env, j_iter, MID_ITER_REMOVE); */
	has_next = (bool)(*env)->CallObjectMethod(env, j_iter, MID_ITER_HASNEXT);
    }

    printf("props_to_OCRepPayloadValue EXIT returning %ld\n", (long)plv_head);
    return plv_head;
}

/*
 *
 */
OCRepPayload* pfrs_to_OCRepPayload(JNIEnv* env, jobject j_pfrs)
{
    OCRepPayload* c_payload = (OCRepPayload*)(*env)->GetLongField(env, j_pfrs, FID_PAYLOAD_HANDLE);
    /* int j_payload_type = (*env)->GetIntField(env, j_pfrs, FID_PAYLOAD_TYPE); */

    if (c_payload->base.type !=  PAYLOAD_TYPE_REPRESENTATION) {
	printf("ERROR: mismatch between java and c\n");
    }

    /* populate the struct: uri, types, interfaces, properties */

    jstring j_uri  = (*env)->GetObjectField(env, j_pfrs, FID_PFRS_URI);
    if (j_uri == NULL) {
	THROW_JNI_EXCEPTION("GetObjectField failed for FID_PFRS_URI on j_pfrs\n");
	return NULL;
    }
    c_payload->uri = (char*) (*env)->GetStringUTFChars(env, j_uri, NULL);
    if (c_payload->uri == NULL) {
	THROW_JNI_EXCEPTION("GetStringUTFChars failed for j_uri\n");
	return NULL;
    }

    /* properties ('values'): OCRepPayloadValue* values; */
    prep_pmap(env);		/* prep mid's etc. for iterating over pmap */

    jobject j_properties  = (*env)->GetObjectField(env, j_pfrs, FID_PFRS_PROPERTIES);
    if (j_properties == NULL) {
	THROW_JNI_EXCEPTION("GetObjectField failed for FID_PFRS_PROPERTIES on j_pfrs\n");
	return NULL;
    }
    int sz = 0;
    sz = (*env)->CallIntMethod(env, j_properties, MID_PMAP_SIZE);
    printf("PropertyMap SIZE: %d\n", sz);

    OCRepPayloadValue* properties = props_to_OCRepPayloadValue(env, j_properties);

    c_payload->values = properties;

    return c_payload;
}

/* PUBLIC */
/*
 * Class:     org_iochibity_OCF
 * Method:    Init
 * Signature: (Ljava/lang/String;CI)I
 */
/* JNIEXPORT jint JNICALL Java_org_iochibity_OCF_Init */
/*   (JNIEnv *, jobject, jstring ip_addr, jchar port, jint mode) */
JNIEXPORT void JNICALL Java_org_iochibity_OCF_Init
(JNIEnv * env, jclass clazz, jstring j_ip_addr, jint port, jint mode, jstring j_config_fname)
{
    OC_UNUSED(clazz);
    printf("Java_org_iochibity_OCF_Init\n");

    /* First configure security */
    if (j_config_fname == NULL) {
	printf("config fname is null\n");
	j_config_fname = (*env)->NewStringUTF(env, g_config_fname);
    }
    g_config_fname = (*env)->GetStringUTFChars(env, j_config_fname, NULL);
    if (g_config_fname == NULL) {
	THROW_JNI_EXCEPTION("GetStringUTFChars");
    }
    OCPersistentStorage ps = {server_fopen, fread, fwrite, fclose, unlink};
    printf("calling OCRegisterPersistentStorageHandler: %s\n", g_config_fname);
    OCRegisterPersistentStorageHandler(&ps);
    /* (*env)->ReleaseStringUTFChars(env, j_config_fname, g_config_fname); */

    /* Then initialize supervisor */
    OCStackResult op_result;
    const char *cip_addr = "";
    if (j_ip_addr == NULL) {
	printf("ip addr null\n");
	j_ip_addr = (*env)->NewStringUTF(env, cip_addr);
	cip_addr = (*env)->GetStringUTFChars(env, j_ip_addr, NULL);
    } else {
	cip_addr = (*env)->GetStringUTFChars(env, j_ip_addr, NULL);
	if (cip_addr == NULL) {
	    THROW_JNI_EXCEPTION("GetStringUTFChars");
	}
    }
    printf("ip addr: [%s]\n", cip_addr);

    op_result = OCInit(cip_addr, (uint16_t)port, mode);
    if (op_result != OC_STACK_OK) {
	printf("OCStack init error\n");
	THROW_STACK_EXCEPTION(op_result, "Initialization failure");
    }
    (*env)->ReleaseStringUTFChars(env, j_ip_addr, cip_addr);

    /* return op_result; */
}

/*
 * Class:     org_iochibity_OCF
 * Method:    OCInit1
 * Signature: (III)I
 */
JNIEXPORT void JNICALL Java_org_iochibity_OCF_OCInit1
  (JNIEnv * env, jclass clazz, jint mode, jint server_flags, jint client_flags)
{
    OC_UNUSED(env);
    OC_UNUSED(clazz);
    OC_UNUSED(mode);
    OC_UNUSED(server_flags);
    OC_UNUSED(client_flags);
}

/*
 * Class:     org_iochibity_OCF
 * Method:    OCStartMulticastServer
 * Signature: ()I
 */
/* JNIEXPORT jint JNICALL Java_org_iochibity_OCF_OCStartMulticastServer */
/*   (JNIEnv *, jobject); */

/*
 * Class:     org_iochibity_OCF
 * Method:    OCStopMulticastServer
 * Signature: ()I
 */
/* JNIEXPORT jint JNICALL Java_org_iochibity_OCF_OCStopMulticastServer */
/*   (JNIEnv *, jobject); */

/*
 * Class:     org_iochibity_OCF
 * Method:    run
 * Signature: ()I
 */
JNIEXPORT void JNICALL Java_org_iochibity_OCF_run
  (JNIEnv * env, jclass clazz)
{
    OC_UNUSED(env);
    OC_UNUSED(clazz);
    printf("Launching worker thread...\n");
    pthread_create (&pt_work, NULL, troutine_work, (void *)NULL);
    /* main thread has nothing to do. by calling pthread_exit it exits
       but the process continues, so any spawned threads do too. */
    /* pthread_exit(NULL); */
}

/*
 * Class:     org_iochibity_OCF
 * Method:    stop
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_iochibity_OCF_stop
  (JNIEnv * env, jclass clazz)
{
    OC_UNUSED(env);
    OC_UNUSED(clazz);
    g_quit_flag = true;
}

/*
 * Class:     org_iochibity_OCF
 * Method:    OCDoResource
 * Signature: (Ljava/lang/Object;ILjava/lang/String;Ljava/lang/Object;Ljava/lang/Object;IILjava/lang/Object;Ljava/lang/Object;B)I
 */
/* JNIEXPORT jint JNICALL Java_org_iochibity_OCF_OCDoResource */
/*   (JNIEnv *, jobject, jobject, jint, jstring, jobject, jobject, jint, jint, jobject, jobject, jbyte); */

/*
 * Class:     org_iochibity_OCF
 * Method:    OCCancel
 * Signature: (Ljava/lang/Object;ILjava/lang/Object;B)I
 */
/* JNIEXPORT jint JNICALL Java_org_iochibity_OCF_OCCancel */
/*   (JNIEnv *, jobject, jobject, jint, jobject, jbyte); */

/*
 * Class:     org_iochibity_OCF
 * Method:    OCDoResponse
 * Signature: (Ljava/lang/Object;)I
 */
/* JNIEXPORT jint JNICALL Java_org_iochibity_OCF_OCDoResponse */
/*   (JNIEnv *, jobject, jobject); */

/*
 * Class:     org_iochibity_OCF
 * Method:    setPlatformInfo
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT void JNICALL Java_org_iochibity_OCF_setPlatformInfo
  (JNIEnv * env, jclass clazz,
   jstring j_platform_id,
   jstring j_mfg_name,
   jstring j_mfg_url,
   jstring j_model_number,
   jstring j_mfg_date,
   jstring j_platform_version,
   jstring j_os_version,
   jstring j_hw_version,
   jstring j_fw_version,
   jstring j_support_url,
   jstring j_sys_time)
{
    OC_UNUSED(clazz);
    static OCPlatformInfo platform_info =
	{
	    .platformID			= "",
	    .manufacturerName		= "",
	    .manufacturerUrl		= "",
	    .modelNumber		= "",
	    .dateOfManufacture		= "",
	    .platformVersion		= "",
	    .operatingSystemVersion	= "",
	    .hardwareVersion		= "",
	    .firmwareVersion		= "",
	    .supportUrl			= "",
	    .systemTime			= ""   /* "2015-05-15T11.04" */
	};

    if (j_platform_id == NULL) {
	printf("platform id is null\n");
	j_platform_id = (*env)->NewStringUTF(env, platform_info.platformID);
	platform_info.platformID = (char*) (*env)->GetStringUTFChars(env, j_platform_id, NULL);
    } else {
	platform_info.platformID = (char*) (*env)->GetStringUTFChars(env, j_platform_id, NULL);
	if (platform_info.platformID == NULL) {
	    THROW_JNI_EXCEPTION("GetStringUTFChars");
	}
    }
    printf("c platform id: [%s]\n", platform_info.platformID);

    if (j_mfg_name == NULL) {
	printf("platform id is null\n");
	j_mfg_name = (*env)->NewStringUTF(env, platform_info.manufacturerName);
	platform_info.manufacturerName = (char*) (*env)->GetStringUTFChars(env, j_mfg_name, NULL);
    } else {
	platform_info.manufacturerName = (char*) (*env)->GetStringUTFChars(env, j_mfg_name, NULL);
	if (platform_info.manufacturerName == NULL) {
	    THROW_JNI_EXCEPTION("GetStringUTFChars");
	}
    }
    printf("c mfg name: [%s]\n", platform_info.manufacturerName);

    if (j_mfg_url == NULL) {
	printf("platform id is null\n");
	j_mfg_url = (*env)->NewStringUTF(env, platform_info.manufacturerUrl);
	platform_info.manufacturerUrl = (char*) (*env)->GetStringUTFChars(env, j_mfg_url, NULL);
    } else {
	platform_info.manufacturerUrl = (char*) (*env)->GetStringUTFChars(env, j_mfg_url, NULL);
	if (platform_info.manufacturerUrl == NULL) {
	    THROW_JNI_EXCEPTION("GetStringUTFChars");
	}
    }
    printf("c mfg url: [%s]\n", platform_info.manufacturerUrl);

    if (j_model_number == NULL) {
	printf("platform id is null\n");
	j_model_number = (*env)->NewStringUTF(env, platform_info.modelNumber);
	platform_info.modelNumber = (char*) (*env)->GetStringUTFChars(env, j_model_number, NULL);
    } else {
	platform_info.modelNumber = (char*) (*env)->GetStringUTFChars(env, j_model_number, NULL);
	if (platform_info.modelNumber == NULL) {
	    THROW_JNI_EXCEPTION("GetStringUTFChars");
	}
    }
    printf("c model nbr: [%s]\n", platform_info.modelNumber);

    if (j_mfg_date == NULL) {
	printf("platform id is null\n");
	j_mfg_date = (*env)->NewStringUTF(env, platform_info.dateOfManufacture);
	platform_info.dateOfManufacture = (char*) (*env)->GetStringUTFChars(env, j_mfg_date, NULL);
    } else {
	platform_info.dateOfManufacture = (char*) (*env)->GetStringUTFChars(env, j_mfg_date, NULL);
	if (platform_info.dateOfManufacture == NULL) {
	    THROW_JNI_EXCEPTION("GetStringUTFChars");
	}
    }
    printf("c mfg date: [%s]\n", platform_info.dateOfManufacture);

    if (j_platform_version == NULL) {
	printf("platform id is null\n");
	j_platform_version = (*env)->NewStringUTF(env, platform_info.platformVersion);
	platform_info.platformVersion = (char*) (*env)->GetStringUTFChars(env, j_platform_version, NULL);
    } else {
	platform_info.platformVersion = (char*) (*env)->GetStringUTFChars(env, j_platform_version, NULL);
	if (platform_info.platformVersion == NULL) {
	    THROW_JNI_EXCEPTION("GetStringUTFChars");
	}
    }
    printf("c platform version: [%s]\n", platform_info.platformVersion);

    if (j_os_version == NULL) {
	printf("platform id is null\n");
	j_os_version = (*env)->NewStringUTF(env, platform_info.operatingSystemVersion);
	platform_info.operatingSystemVersion = (char*) (*env)->GetStringUTFChars(env, j_os_version, NULL);
    } else {
	platform_info.operatingSystemVersion = (char*) (*env)->GetStringUTFChars(env, j_os_version, NULL);
	if (platform_info.operatingSystemVersion == NULL) {
	    THROW_JNI_EXCEPTION("GetStringUTFChars");
	}
    }
    printf("c os version: [%s]\n", platform_info.operatingSystemVersion);

    if (j_hw_version == NULL) {
	printf("platform id is null\n");
	j_hw_version = (*env)->NewStringUTF(env, platform_info.hardwareVersion);
	platform_info.hardwareVersion = (char*) (*env)->GetStringUTFChars(env, j_hw_version, NULL);
    } else {
	platform_info.hardwareVersion = (char*) (*env)->GetStringUTFChars(env, j_hw_version, NULL);
	if (platform_info.hardwareVersion == NULL) {
	    THROW_JNI_EXCEPTION("GetStringUTFChars");
	}
    }
    printf("c hw version: [%s]\n", platform_info.hardwareVersion);

    if (j_fw_version == NULL) {
	printf("platform id is null\n");
	j_fw_version = (*env)->NewStringUTF(env, platform_info.firmwareVersion);
	platform_info.firmwareVersion = (char*) (*env)->GetStringUTFChars(env, j_fw_version, NULL);
    } else {
	platform_info.firmwareVersion = (char*) (*env)->GetStringUTFChars(env, j_fw_version, NULL);
	if (platform_info.firmwareVersion == NULL) {
	    THROW_JNI_EXCEPTION("GetStringUTFChars");
	}
    }
    printf("c firmware version: [%s]\n", platform_info.firmwareVersion);

    if (j_support_url == NULL) {
	printf("platform id is null\n");
	j_support_url = (*env)->NewStringUTF(env, platform_info.supportUrl);
	platform_info.supportUrl = (char*) (*env)->GetStringUTFChars(env, j_support_url, NULL);
    } else {
	platform_info.supportUrl = (char*) (*env)->GetStringUTFChars(env, j_support_url, NULL);
	if (platform_info.supportUrl == NULL) {
	    THROW_JNI_EXCEPTION("GetStringUTFChars");
	}
    }
    printf("c support url: [%s]\n", platform_info.supportUrl);

    if (j_sys_time == NULL) {
	printf("platform id is null\n");
	j_sys_time = (*env)->NewStringUTF(env, platform_info.systemTime);
	platform_info.systemTime = (char*) (*env)->GetStringUTFChars(env, j_sys_time, NULL);
    } else {
	platform_info.systemTime = (char*) (*env)->GetStringUTFChars(env, j_sys_time, NULL);
	if (platform_info.systemTime == NULL) {
	    THROW_JNI_EXCEPTION("GetStringUTFChars");
	}
    }
    printf("c system time: [%s]\n", platform_info.systemTime);

    printf("Setting platform info...\n");
    OCStackResult op_result;
    op_result = OCSetPlatformInfo(platform_info);

    (*env)->ReleaseStringUTFChars(env, j_platform_id, platform_info.platformID);
    (*env)->ReleaseStringUTFChars(env, j_mfg_name, platform_info.manufacturerName);
    (*env)->ReleaseStringUTFChars(env, j_mfg_url, platform_info.manufacturerUrl);
    (*env)->ReleaseStringUTFChars(env, j_model_number, platform_info.modelNumber);
    (*env)->ReleaseStringUTFChars(env, j_mfg_date, platform_info.dateOfManufacture);
    (*env)->ReleaseStringUTFChars(env, j_platform_version, platform_info.platformVersion);
    (*env)->ReleaseStringUTFChars(env, j_os_version, platform_info.operatingSystemVersion);
    (*env)->ReleaseStringUTFChars(env, j_hw_version, platform_info.hardwareVersion);
    (*env)->ReleaseStringUTFChars(env, j_fw_version, platform_info.firmwareVersion);
    (*env)->ReleaseStringUTFChars(env, j_support_url, platform_info.supportUrl);
    (*env)->ReleaseStringUTFChars(env, j_sys_time, platform_info.systemTime);
}

/*
 * Class:     org_iochibity_OCF
 * Method:    setDeviceInfo
 * Signature: (Ljava/lang/String;Ljava/lang/Object;Ljava/lang/String;[Ljava/lang/String;)I
 */
JNIEXPORT void JNICALL Java_org_iochibity_OCF_setDeviceInfo
  (JNIEnv * env, jclass clazz,
   jstring j_device_name, jobjectArray j_types, jstring j_spec_version, jobjectArray j_data_model_versions)
{
    OC_UNUSED(env);
    OC_UNUSED(clazz);
    OC_UNUSED(j_types);
    OC_UNUSED(j_data_model_versions);
    static OCDeviceInfo device_info =
	{
	    .deviceName = "", /* Default Device Name", */
	    /* OCStringLL *types; */
	    .types = NULL,
	    .specVersion = "0.0.0", /* device specification version */
	    // .dataModelVersions = "minDeviceModelVersion"
	    .dataModelVersions = NULL
	};

    if (j_device_name == NULL) {
	printf("device name is null, using default\n");
	j_device_name = (*env)->NewStringUTF(env, device_info.deviceName);
	device_info.deviceName = (char*) (*env)->GetStringUTFChars(env, j_device_name, NULL);
    } else {
	device_info.deviceName = (char*) (*env)->GetStringUTFChars(env, j_device_name, NULL);
	if (device_info.deviceName == NULL) {
	    THROW_JNI_EXCEPTION("GetStringUTFChars");
	}
    }
    printf("c device name: [%s]\n", device_info.deviceName);

    if (j_spec_version == NULL) {
	printf("spec version is null, using default\n");
	j_spec_version = (*env)->NewStringUTF(env, device_info.specVersion);
	device_info.specVersion = (char*) (*env)->GetStringUTFChars(env, j_spec_version, NULL);
    } else {
	device_info.specVersion = (char*) (*env)->GetStringUTFChars(env, j_spec_version, NULL);
	if (device_info.specVersion == NULL) {
	    THROW_JNI_EXCEPTION("GetStringUTFChars");
	}
    }
    printf("c platform id: [%s]\n", device_info.specVersion);



    printf("Setting device info...\n");
    OCStackResult op_result;
    op_result = OCSetDeviceInfo(device_info);
    switch (op_result) {
    case OC_STACK_OK:
	break;
    case OC_STACK_INVALID_PARAM:
	THROW_STACK_EXCEPTION(op_result, "Java_org_iochibity_OCF_setDeviceInfo");
	/* throw_invalid_param(env, "Java_org_iochibity_OCF_setDeviceInfo"); */
	break;
    default:
        printf("Device Registration failed with result %d!\n", op_result);
	THROW_STACK_EXCEPTION(op_result, "UNKNOWN");
    }

    (*env)->ReleaseStringUTFChars(env, j_device_name, device_info.deviceName);
    (*env)->ReleaseStringUTFChars(env, j_spec_version, device_info.specVersion);
}

/*
 * Class:     org_iochibity_OCF
 * Method:    sendResponse
 * Signature: (Lorg/iochibity/ResponseOut;)I
 */
JNIEXPORT void JNICALL Java_org_iochibity_OCF_sendResponse
(JNIEnv * env, jclass klass, jobject j_response_out)
{
    OC_UNUSED(env);
    OC_UNUSED(klass);
    OC_UNUSED(j_response_out);
    printf("Java_org_iochibity_OCF_sendResponse ENTRY\n");

    prep_java(env);

    /* 1. alloc the OCEntityHandlerResponse struct */
    OCEntityHandlerResponse * c_response_out =
                        (OCEntityHandlerResponse *) OICCalloc(1, sizeof(*c_response_out));
    if (!c_response_out) {
	THROW_EH_EXCEPTION(OC_EH_ERROR, "OCEntityHandlerResponse calloc failure\n");
	return;
    }

    /* 2. get handles from j_response_out._requestIn */
    jclass k_response_out = (*env)->GetObjectClass(env, j_response_out);
    if (k_response_out == NULL) {
	THROW_JNI_EXCEPTION("GetObjectClass failed for ResponseOut object\n");
	return;
    }
    /* get the RequestIn object */
    jfieldID fid_rqst_in = (*env)->GetFieldID(env, k_response_out, "_requestIn", "Lorg/iochibity/RequestIn;");
    if (fid_rqst_in == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for '_requestIn' on ResponseOut\n");
	return;
    }
    jobject j_request_in = (*env)->GetObjectField(env, j_response_out, fid_rqst_in);
    if (j_request_in == NULL) {
	THROW_JNI_EXCEPTION("GetObjectField failed for fid_rqst_in on j_response_out\n");
	return;
    }
    /* now we have the original RequestIn object; let's pull the handles from it */

    /* first we need the RequestIn class so we can get the c struct handle */
    jclass k_request_in = (*env)->GetObjectClass(env, j_request_in);
    if (k_request_in == NULL) {
	THROW_JNI_EXCEPTION("GetObjectClass failed for RequestIn object\n");
	return;
    }

    /* get the localHandle */
    jfieldID fid_local_handle = (*env)->GetFieldID(env, k_request_in, "localHandle", "J");
    if (fid_local_handle == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for 'localHandle' on RequestIn\n");
	return;
    }
    OCEntityHandlerRequest* j_handle = (OCEntityHandlerRequest*)(*env)->GetLongField(env, j_request_in,
										     fid_local_handle);
    if (j_handle == NULL) {
	THROW_JNI_EXCEPTION("GetObjectField failed for fid_remote_handle on j_request_in\n");
	return;
    }
    printf("RESPONSE localHandle: %ld\n", (long)j_handle);
    printf("RESPONSE remoteHandle: %ld\n", (long)j_handle->requestHandle);
    c_response_out->requestHandle = j_handle->requestHandle;

    /* ditto for resourceHandle */
    printf("RESPONSE resourceHandle: %ld\n", (long)j_handle->resource);
    c_response_out->resourceHandle = j_handle->resource;

    /* defaults */
    c_response_out->numSendVendorSpecificHeaderOptions = 0;

    memset(c_response_out->sendVendorSpecificHeaderOptions, 0,
	   sizeof c_response_out->sendVendorSpecificHeaderOptions);
    memset(c_response_out->resourceUri, 0, sizeof(c_response_out->resourceUri));

    c_response_out->persistentBufferFlag = 0;

    /* PayloadList */
    jfieldID fid_pll = (*env)->GetFieldID(env, k_response_out,
					  "_payloadList", "Lorg/iochibity/PayloadList;");
    if (fid_pll == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for '_payloadList' on ResponseOut\n");
	return;
    }
    jobject j_pll = (*env)->GetObjectField(env, j_response_out, fid_pll);
    if (j_pll == NULL) {
	THROW_JNI_EXCEPTION("GetObjectField failed for fid_pll on j_response_out\n");
	return;
    }
    jclass k_pll = (*env)->GetObjectClass(env, j_pll);
    if (k_pll == NULL) {
	THROW_JNI_EXCEPTION("GetObjectClass failed for PayloadList object\n");
	return;
    }
    /* we're going to iterate using jni, dunno about efficiency */
    jmethodID mid_pll_size = (*env)->GetMethodID(env, k_pll, "size", "()I");
    if (mid_pll_size == NULL) {
	THROW_JNI_EXCEPTION("GetMethodID failed for 'size' of PayloadList\n");
	return;
    }
    int sz = 0;
    sz = (*env)->CallIntMethod(env, j_pll, mid_pll_size);
    printf("PayloadList SIZE: %d\n", sz);

    jmethodID mid_pll_get = (*env)->GetMethodID(env, k_pll, "get", "(I)Ljava/lang/Object;");
    if (mid_pll_get == NULL) {
	THROW_JNI_EXCEPTION("GetMethodID failed for 'get' of PayloadList\n");
	return;
    }

    /* since PayloadList contains Payload objects */
    K_PAYLOAD = (*env)->FindClass(env, "Lorg/iochibity/Payload;");
    if (K_PAYLOAD == NULL) {
	THROW_JNI_EXCEPTION("FindClass failed for org/iochibity/Payload\n");
    }
    FID_PAYLOAD_HANDLE = (*env)->GetFieldID(env, K_PAYLOAD, "_handle", "J");
    if (FID_PAYLOAD_HANDLE == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for '_handle' of Payload\n");
	return;
    }
    FID_PAYLOAD_TYPE = (*env)->GetFieldID(env, K_PAYLOAD, "type", "I");
    if (FID_PAYLOAD_TYPE == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for 'type' on Payload\n");
	return;
    }

    /* pfrs = PayloadForResourceState */
    prep_pfrs(env); /* get all the field and method ids etc. needed for iteration */

    jobject j_payload = NULL;
    int c_type;
    OCPayload* c_payload = NULL;
    for (int i=0; i<sz; i++) {
	j_payload = (*env)->CallObjectMethod(env, j_pll, mid_pll_get, i);
	c_type = (*env)->GetIntField(env, j_payload, FID_PAYLOAD_TYPE);
	printf("PAYLOAD TYPE: %d\n", c_type);

	/* the OCPayload structs are already there in the _handle fields */
	/* but we need to synch them with the Java objects */
	switch(c_type) {
	case PAYLOAD_TYPE_INVALID:
	    printf("INVALID PAYLOAD TYPE NOT SUPPORTED\n");
	    break;
	case PAYLOAD_TYPE_DISCOVERY:
	    printf("DISCOVERY PAYLOAD NOT YET SUPPORTED\n");
	    break;
	case PAYLOAD_TYPE_DEVICE:
	    printf("DEVICE PAYLOAD NOT YET SUPPORTED\n");
	    break;
	case PAYLOAD_TYPE_PLATFORM:
	    printf("PLATFORM PAYLOAD NOT YET SUPPORTED\n");
	    break;
	case PAYLOAD_TYPE_REPRESENTATION:
	    c_payload = (OCPayload*)pfrs_to_OCRepPayload(env, j_payload);
	    break;
	case PAYLOAD_TYPE_SECURITY:
	    printf("SECURITY PAYLOAD NOT YET SUPPORTED\n");
	    break;
	case PAYLOAD_TYPE_PRESENCE:
	    printf("PRESENCE PAYLOAD NOT YET SUPPORTED\n");
	    break;
	case PAYLOAD_TYPE_RD:
	    printf("RD PAYLOAD NOT YET SUPPORTED\n");
	    break;
	default:
	    printf("UNKNOWN PAYLOAD - NOT SUPPORTED\n");
	    break;
	}
    }

    /* set OCEntityHandlerResult from this._processingResult */

    /* vendor header options */

    /* payload: iterate over the PayloadList to create OCPayload linked list */

    c_response_out->payload = (OCPayload*) c_payload;

    /* finally, send it off: */
    OCStackResult op_result = OC_STACK_OK;
    op_result = OCDoResponse(c_response_out);
    if (op_result != OC_STACK_OK) {
	THROW_STACK_EXCEPTION(op_result, "OCDoResponse failed");
	/* ehResult = OC_EH_ERROR; */
	return;
    }

    printf("Java_org_iochibity_OCF_sendResponse EXIT\n");
}
