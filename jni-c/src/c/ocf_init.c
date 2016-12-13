#include <ctype.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "org_iochibity_OCF.h"
#include "ocf_init.h"
#include "ocf_exceptions.h"
#include "jni_utils.h"

#include "octypes.h"
#include "ocresource.h"
#include "ocstack.h"
#include "oic_malloc.h"
#include "oic_string.h"

JavaVM* g_JVM;

extern const char* g_config_fname;


jclass    K_INTEGER              = NULL; /* 32-bit ints */
jmethodID MID_INT_INTVALUE       = NULL; /* Integer.intValue() returns int */
jclass K_LONG                    = NULL; /* 64-bit ints */
jmethodID MID_LONG_LONGVALUE     = NULL; /* Long.longValue() returns long */
/* jclass K_BIGINTEGER		/\* arbitrary bits *\/ */
jclass K_BOOLEAN                 = NULL;
jmethodID MID_BOOL_BOOLVALUE     = NULL; /* Boolean.booleanValue() returns boolean */
jclass K_DOUBLE                  = NULL;
jmethodID MID_DBL_DBLVALUE       = NULL; /* Double.doubleValue() returns double */
jclass K_STRING                  = NULL;
jclass K_LIST                    = NULL; /* OCByteString => List<Byte> */
jclass K_BYTE                    = NULL;
jclass K_OBJECT                  = NULL;
/* jclass K_ARRAY; - List */

jclass   K_PAYLOAD               = NULL;
jfieldID FID_PAYLOAD_HANDLE      = NULL;
jfieldID FID_PAYLOAD_TYPE        = NULL;

jclass   K_PFRS                  = NULL; /* class for PayloadForResourceState */
jfieldID FID_PFRS_URI            = NULL;
jfieldID FID_PFRS_RTYPES         = NULL;
jfieldID FID_PFRS_INTERFACES     = NULL;
jfieldID FID_PFRS_PROPERTIES     = NULL;

jclass   K_PMAP                  = NULL; /* class for PropertyMap */
jmethodID MID_PMAP_SIZE          = NULL;
jmethodID MID_PMAP_ENTRYSET      = NULL;
jmethodID MID_PMAP_GET           = NULL;

jclass   K_SET                   = NULL; /* interface java.util.Set */
jmethodID MID_SET_ITERATOR       = NULL;

jclass   K_ITER                  = NULL; /* interface java.util.Iterator */
jmethodID MID_ITER_HASNEXT       = NULL;
jmethodID MID_ITER_NEXT          = NULL;
jmethodID MID_ITER_REMOVE        = NULL;

jclass   K_MAPENTRY              = NULL; /* interface java.util.Map.Entry */
jmethodID MID_ENTRY_GETKEY       = NULL;
jmethodID MID_ENTRY_GETVALUE     = NULL;

jclass K_DEVICE_ADDRESS       = NULL;
jfieldID  FID_DA_ADAPTER      = NULL;
jfieldID  FID_DA_FLAGS        = NULL;
jfieldID  FID_DA_PORT         = NULL;
jfieldID  FID_DA_ADDRESS      = NULL;
jfieldID  FID_DA_IFINDEX      = NULL;
jfieldID  FID_DA_ROUTE_DATA   = NULL;

jclass K_RESOURCE_LOCAL       = NULL;

jclass    K_DOCUMENT            = NULL;
jfieldID  FID_DOC_PAYLOAD_HANDLE  = NULL;
jfieldID  FID_DOC_OPTION_COUNT    = NULL;
jfieldID  FID_DOC_PTR_OPTIONS     = NULL;
jmethodID MID_DOC_GET_OPTIONS     = NULL;
jmethodID MID_DOC_GET_PAYLOAD_TYPE= NULL;

jclass   K_DOC_REQUEST_OUT          = NULL;
jfieldID FID_DRQO_SERVICE_REQUESTOR  = NULL;

jclass    K_DOC_REQUEST_IN           = NULL;
jmethodID MID_DRQI_CTOR              = NULL;
jfieldID  FID_DRQI_SERVICE_REQUESTOR = NULL;
jfieldID  FID_DRQI_DEVICE_ADDRESS    = NULL;
jfieldID  FID_DRQI_IS_REQUEST        = NULL;
jfieldID  FID_DRQI_IS_OBSERVE        = NULL;

jclass    K_DOC_RESPONSE_IN          = NULL;
jmethodID MID_DRSPI_CTOR             = NULL;
jfieldID  FID_DRSPI_PTR_RESPONSE     = NULL;
jfieldID  FID_DRSPI_DEVICE_ADDRESS   = NULL;
jfieldID  FID_DRSPI_CONN_TYPE        = NULL;
jfieldID  FID_DRSPI_REMOTE_SID       = NULL;
jfieldID  FID_DRSPI_RESULT           = NULL;
jfieldID  FID_DRSPI_SERIAL           = NULL;
jfieldID  FID_DRSPI_URI              = NULL;
jmethodID MID_DRSPI_GET_PAYLOAD      = NULL;
jfieldID  FID_DRSPI_OPTION_COUNT     = NULL;
jfieldID  FID_DRSPI_OPTIONS          = NULL;
jfieldID  FID_DRSPI_PTR_OPTIONS      = NULL;
jmethodID MID_DRSPI_GET_OPTIONS      = NULL;

jclass    K_IRESOURCE_SERVICE_REQUESTOR = NULL;
jmethodID MID_IRSR_SERVICE_RESPONSE_IN  = NULL;

int init_Document(JNIEnv* env)
{
    jclass klass;
    klass = (*env)->FindClass(env, "org/iochibity/Document");
    JNI_ASSERT_NULL(klass, "FindClass failed for org/iochibity/Document\n", -1);
    K_DOCUMENT = (jclass)(*env)->NewGlobalRef(env, klass);
    (*env)->DeleteLocalRef(env, klass);

    if (MID_DOC_GET_PAYLOAD_TYPE == NULL) {
	MID_DOC_GET_PAYLOAD_TYPE = (*env)->GetMethodID(env, K_DOCUMENT, "getPayloadType", "()I");
	if (MID_DOC_GET_PAYLOAD_TYPE == NULL) {
	    printf("ERROR:  GetMethodID failed for ctor for Document");
	    return -1;
	}
    }
    if (FID_DOC_PAYLOAD_HANDLE == NULL) {
	FID_DOC_PAYLOAD_HANDLE = (*env)->GetFieldID(env, K_DOCUMENT, "payloadHandle", "J");
	if (FID_DOC_PAYLOAD_HANDLE == NULL) {
	    THROW_JNI_EXCEPTION("GetFieldID failed for 'payloadHandle' of Document\n");
	    return -1;
	}
    }
    return 0;
}

int init_DocResponseIn(JNIEnv* env)
{
    jclass klass;
    klass = (*env)->FindClass(env, "org/iochibity/DocResponseIn");
    JNI_ASSERT_NULL(klass, "FindClass failed for org/iochibity/DocResponseIn\n", -1);
    K_DOC_RESPONSE_IN = (jclass)(*env)->NewGlobalRef(env, klass);
    (*env)->DeleteLocalRef(env, klass);

    if (MID_DRSPI_CTOR == NULL) {
	MID_DRSPI_CTOR = (*env)->GetMethodID(env, K_DOC_RESPONSE_IN, "<init>", "()V");
	if (MID_DRSPI_CTOR == NULL) {
	    printf("ERROR:  GetMethodID failed for ctor for DocResponseIn");
	    return -1;
	}
    }
    if (FID_DRSPI_PTR_RESPONSE == NULL) {
	FID_DRSPI_PTR_RESPONSE = (*env)->GetFieldID(env, K_DOC_RESPONSE_IN,
						    "ptr_OCClientResponse", "J");
	if (FID_DRSPI_PTR_RESPONSE == NULL) {
	    THROW_JNI_EXCEPTION("GetFieldID failed for 'ptr_OCClientResponse' of DocResponseIn\n");
	    return -1;
	}
    }
    if (FID_DRSPI_DEVICE_ADDRESS == NULL) {
	FID_DRSPI_DEVICE_ADDRESS = (*env)->GetFieldID(env, K_DOC_RESPONSE_IN,
						      "deviceAddress",
						      "Lorg/iochibity/DeviceAddress;");
	if (FID_DRSPI_DEVICE_ADDRESS == NULL) {
	    THROW_JNI_EXCEPTION("GetFieldID failed for 'deviceAddress' of DocResponseIn\n");
	    return -1;
	}
    }
    if (FID_DRSPI_CONN_TYPE == NULL) {
	FID_DRSPI_CONN_TYPE = (*env)->GetFieldID(env, K_DOC_RESPONSE_IN, "connType", "I");
	if (FID_DRSPI_CONN_TYPE == NULL) {
	    THROW_JNI_EXCEPTION("GetFieldID failed for 'connType' of DocResponseIn\n");
	    return -1;
	}
    }
    if (FID_DRSPI_REMOTE_SID == NULL) {
	FID_DRSPI_REMOTE_SID = (*env)->GetFieldID(env, K_DOC_RESPONSE_IN,
						  "secID", "Ljava/lang/String;");
	if (FID_DRSPI_REMOTE_SID == NULL) {
	    THROW_JNI_EXCEPTION("GetFieldID failed for 'secID' of DocResponseIn\n");
	    return -1;
	}
    }
    if (FID_DRSPI_RESULT == NULL) {
	FID_DRSPI_RESULT = (*env)->GetFieldID(env, K_DOC_RESPONSE_IN, "result", "I");
	if (FID_DRSPI_RESULT == NULL) {
	    THROW_JNI_EXCEPTION("GetFieldID failed for 'result' of DocResponseIn\n");
	    return -1;
	}
    }
    if (FID_DRSPI_SERIAL == NULL) {
	FID_DRSPI_SERIAL = (*env)->GetFieldID(env, K_DOC_RESPONSE_IN, "serial", "I");
	if (FID_DRSPI_RESULT == NULL) {
	    THROW_JNI_EXCEPTION("GetFieldID failed for 'serial' of DocResponseIn\n");
	    return -1;
	}
    }
    if (FID_DRSPI_URI == NULL) {
	FID_DRSPI_URI = (*env)->GetFieldID(env, K_DOC_RESPONSE_IN, "uri", "Ljava/lang/String;");
	if (FID_DRSPI_URI == NULL) {
	    THROW_JNI_EXCEPTION("GetFieldID failed for 'uri' of DocResponseIn\n");
	    return -1;
	}
    }
    /* if (MID_DRSPI_GET_PAYLOAD == NULL) { */
    /* 	MID_DRSPI_GET_PAYLOAD = (*env)->GetMethodID(env, K_DOC_RESPONSE_IN, */
    /* 						    "getPDUPayload", */
    /* 						    "()Lorg/iochibity/PayloadList;"); */
    /* 	if (MID_DRSPI_GET_PAYLOAD == NULL) { */
    /* 	    THROW_JNI_EXCEPTION("GetFieldID failed for 'getPDUPayload' of DocResponseIn\n"); */
    /* 	    return -1; */
    /* 	} */
    /* } */
    /* if (FID_DRSPI_PTR_OPTIONS == NULL) { */
    /* 	FID_DRSPI_PTR_OPTIONS = (*env)->GetFieldID(env, K_DOC_RESPONSE_IN, "ptr_Options", "J"); */
    /* 	if (FID_DRSPI_PTR_OPTIONS == NULL) { */
    /* 	    THROW_JNI_EXCEPTION("GetFieldID failed for 'ptr_Options' of DocResponseIn\n"); */
    /* 	    return -1; */
    /* 	} */
    /* } */
    if (FID_DRSPI_OPTION_COUNT == NULL) {
    	FID_DRSPI_OPTION_COUNT = (*env)->GetFieldID(env, K_DOC_RESPONSE_IN, "optionCount", "I");
    	if (FID_DRSPI_OPTION_COUNT == NULL) {
    	    THROW_JNI_EXCEPTION("GetFieldID failed for 'optionCount' of DocResponseIn\n");
    	    return -1;
    	}
    }
    /* if (MID_DRSPI_GET_OPTIONS == NULL) { */
    /* 	MID_DRSPI_GET_OPTIONS = (*env)->GetMethodID(env, K_DOC_RESPONSE_IN, */
    /* 						    "getOptions", "()Ljava/util/List;"); */
    /* 	if (MID_DRSPI_GET_OPTIONS == NULL) { */
    /* 	    THROW_JNI_EXCEPTION("GetFieldID failed for 'getOptions' of DocResponseIn\n"); */
    /* 	    return -1; */
    /* 	} */
    /* } */
    return 0;
}

/*
 *
 */
int init_java(JNIEnv* env)
{
    jclass klass = NULL;
    if (K_INTEGER == NULL) {	/* 32 bits */
	klass = (*env)->FindClass(env, "java/lang/Integer");
	/* JNI_ASSERT_NULL(klass, "FindClass failed for java/lang/Integer"); */
	if (klass == NULL) {
	    THROW_JNI_EXCEPTION("FindClass failed for java/lang/Integer");
	    return -1;
	}
	K_INTEGER = (jclass)(*env)->NewGlobalRef(env, klass);
	(*env)->DeleteLocalRef(env, klass);
    }
    if (MID_INT_INTVALUE == NULL) {
	MID_INT_INTVALUE = (*env)->GetMethodID(env, K_INTEGER, "intValue", "()I");
	if (MID_INT_INTVALUE == NULL) {
	    THROW_JNI_EXCEPTION("GetMethodID failed for 'intValue' of Integer\n");
	    return -1;
	}
    }
    if (K_LONG == NULL) {	/* 64 bits */
	klass = (*env)->FindClass(env, "java/lang/Long");
	if (klass == NULL) {
	    THROW_JNI_EXCEPTION("FindClass failed for java/lang/Long");
	    return -1;
	}
	K_LONG = (jclass)(*env)->NewGlobalRef(env, klass);
	(*env)->DeleteLocalRef(env, klass);
    }
    if (MID_LONG_LONGVALUE == NULL) {
	MID_LONG_LONGVALUE = (*env)->GetMethodID(env, K_LONG, "longValue", "()J");
	if (MID_LONG_LONGVALUE == NULL) {
	    THROW_JNI_EXCEPTION("GetMethodID failed for 'longValue' of Long\n");
	    return -1;
	}
    }

    if (K_BOOLEAN == NULL) {
	klass = (*env)->FindClass(env, "java/lang/Boolean");
	if (klass == NULL) {
	    THROW_JNI_EXCEPTION("FindClass failed for java/lang/Boolean");
	    return -1;
	}
	K_BOOLEAN = (jclass)(*env)->NewGlobalRef(env, klass);
	(*env)->DeleteLocalRef(env, klass);
    }
    if (MID_BOOL_BOOLVALUE == NULL) {
	MID_BOOL_BOOLVALUE = (*env)->GetMethodID(env, K_BOOLEAN, "booleanValue", "()Z");
	if (MID_BOOL_BOOLVALUE == NULL) {
	    THROW_JNI_EXCEPTION("GetMethodID failed for 'booleanValue' of Boolean\n");
	    return -1;
	}
    }
    if (K_DOUBLE == NULL) {
	klass = (*env)->FindClass(env, "java/lang/Double");
	if (klass == NULL) {
	    THROW_JNI_EXCEPTION("FindClass failed for java/lang/Double");
	    return -1;
	}
	K_DOUBLE = (jclass)(*env)->NewGlobalRef(env, klass);
	(*env)->DeleteLocalRef(env, klass);
    }
    if (MID_DBL_DBLVALUE == NULL) {
	MID_DBL_DBLVALUE = (*env)->GetMethodID(env, K_DOUBLE, "doubleValue", "()D");
	if (MID_DBL_DBLVALUE == NULL) {
	    THROW_JNI_EXCEPTION("GetMethodID failed for 'DBLValue' of Double\n");
	    return -1;
	}
    }
    if (K_STRING == NULL) {
	klass = (*env)->FindClass(env, "java/lang/String");
	if (klass == NULL) {
	    THROW_JNI_EXCEPTION("FindClass failed for java/lang/String");
	    return -1;
	}
	K_STRING = (jclass)(*env)->NewGlobalRef(env, klass);
	(*env)->DeleteLocalRef(env, klass);
    }
    /* a byte array must be stored as List<Byte> */
    if (K_LIST == NULL) {
	klass = (*env)->FindClass(env, "java/util/List");
	if (klass == NULL) {
	    THROW_JNI_EXCEPTION("FindClass failed for java/util/List");
	    return -1;
	}
	K_LIST = (jclass)(*env)->NewGlobalRef(env, klass);
	(*env)->DeleteLocalRef(env, klass);
    }
    if (K_BYTE == NULL) {
	klass = (*env)->FindClass(env, "java/lang/Byte");
	if (klass == NULL) {
	    THROW_JNI_EXCEPTION("FindClass failed for java/lang/Byte");
	    return -1;
	}
	K_BYTE = (jclass)(*env)->NewGlobalRef(env, klass);
	(*env)->DeleteLocalRef(env, klass);
    }
    if (K_OBJECT == NULL) {
	klass = (*env)->FindClass(env, "java/lang/Integer");
	if (klass == NULL) {
	    THROW_JNI_EXCEPTION("FindClass failed for java/lang/Integer");
	    return -1;
	}
	K_OBJECT = (jclass)(*env)->NewGlobalRef(env, klass);
	(*env)->DeleteLocalRef(env, klass);
    }
    return 0;
}

/*
*
*/
void prep_pfrs(JNIEnv* env)
{
    if (K_PFRS == NULL) {
	K_PFRS = (*env)->FindClass(env, "org/iochibity/PayloadForResourceState");
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
	K_PMAP = (*env)->FindClass(env, "org/iochibity/PropertyMap");
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
	K_MAPENTRY = (*env)->FindClass(env, "java/util/Map$Entry");
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
	K_SET = (*env)->FindClass(env, "java/util/Set");
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
	K_ITER = (*env)->FindClass(env, "java/util/Iterator");
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

/*
 * Routine:   props_to_OCRepPayloadValue
 * Signature: (Lorg/iochibity/PropertyMap;)J
 */
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
 * Routine:   pfrs_to_OCRepPayloadValue
 * Signature: (Lorg/iochibity/PayloadForResourceState;)J
 */
OCRepPayload* pfrs_to_OCRepPayload(JNIEnv* env, jobject j_pfrs)
{
    OCRepPayload* c_payload = (OCRepPayload*)(intptr_t)
	(*env)->GetLongField(env, j_pfrs, FID_PAYLOAD_HANDLE);
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

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
    OC_UNUSED(reserved);
    printf("JNI_OnLoad\n");
    JNIEnv* env = NULL;
    g_JVM = vm;

    int getEnvStat = (*g_JVM)->GetEnv(g_JVM, (void **)&env, JNI_VERSION_1_6);
    if (getEnvStat == JNI_EDETACHED) {
	/* printf("GetEnv: not attached; attaching now\n"); */
	if ((*g_JVM)->AttachCurrentThread(g_JVM, (void **) &env, NULL) != 0) {
	    /* THROW_JNI_EXCEPTION("AttachCurrentThread failed\n"); */
	    printf("ERROR %s %d (%s): AttachCurrentThread failure\n", __FILE__, __LINE__,__func__);
	    return OC_EH_INTERNAL_SERVER_ERROR;
	}
    } else if (getEnvStat == JNI_OK) {
	/* printf("GetEnv: attached\n"); */
    } else if (getEnvStat == JNI_EVERSION) {
	printf("ERROR %s %d (%s): JNI version not supported\n", __FILE__, __LINE__,__func__);
	return OC_EH_INTERNAL_SERVER_ERROR;
    }
    if ((*env)->ExceptionCheck(env)) {
	(*env)->ExceptionDescribe(env);
    }

    JNI_ASSERT_NULL(env, "GetEnv failed", 0);

    jclass klass = NULL;

    klass = (*env)->FindClass(env, "org/iochibity/Payload");
    JNI_ASSERT_NULL(klass, "FindClass failed for org/iochibity/Payload\n", 0);
    K_PAYLOAD = (jclass)(*env)->NewGlobalRef(env, klass);
    (*env)->DeleteLocalRef(env, klass);

    FID_PAYLOAD_HANDLE = (*env)->GetFieldID(env, K_PAYLOAD, "_handle", "J");
    if (FID_PAYLOAD_HANDLE == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for '_handle' of Payload\n");
	return 0;
    }
    FID_PAYLOAD_TYPE = (*env)->GetFieldID(env, K_PAYLOAD, "type", "I");
    if (FID_PAYLOAD_TYPE == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for 'type' on Payload\n");
	return 0;
    }

    /* DeviceAddress = OCDevAddr */
    if (K_DEVICE_ADDRESS == NULL) {
	klass = (*env)->FindClass(env, "org/iochibity/DeviceAddress");
	JNI_ASSERT_NULL(klass, "FindClass failed for org/iochibity/DeviceAddress\n", 0);
	K_DEVICE_ADDRESS = (jclass)(*env)->NewGlobalRef(env, klass);
	(*env)->DeleteLocalRef(env, klass);
    }
    FID_DA_ADAPTER = (*env)->GetFieldID(env, K_DEVICE_ADDRESS, "adapter", "I");
    if (FID_DA_ADAPTER == NULL) {
	printf("ERROR:  GetFieldID failed for adapter for DeviceAddress\n");
	fflush(NULL);
    	return OC_EH_INTERNAL_SERVER_ERROR;
    }
    FID_DA_FLAGS= (*env)->GetFieldID(env, K_DEVICE_ADDRESS, "flags", "I");
    if (FID_DA_FLAGS == NULL) {
	printf("ERROR:  GetFieldID failed for 'flags' of DeviceAddress\n");
	fflush(NULL);
    	return OC_EH_INTERNAL_SERVER_ERROR;
    }
    FID_DA_PORT = (*env)->GetFieldID(env, K_DEVICE_ADDRESS, "port", "I");
    if (FID_DA_PORT == NULL) {
	printf("ERROR:  GetFieldID failed for port for DeviceAddress\n");
	fflush(NULL);
    	return OC_EH_INTERNAL_SERVER_ERROR;
    }
    FID_DA_ADDRESS = (*env)->GetFieldID(env, K_DEVICE_ADDRESS, "address", "Ljava/lang/String;");
    if (FID_DA_ADDRESS == NULL) {
	printf("ERROR:  GetFieldID failed for address of DeviceAddress\n");
	fflush(NULL);
    	return OC_EH_INTERNAL_SERVER_ERROR;
    }
    FID_DA_IFINDEX = (*env)->GetFieldID(env, K_DEVICE_ADDRESS, "ifindex", "I");
    if (FID_DA_IFINDEX == NULL) {
	printf("ERROR:  GetFieldID failed for ifindex DeviceAddress\n");
	fflush(NULL);
    	return OC_EH_INTERNAL_SERVER_ERROR;
    }
    /* FIXME */
    /* /\* OCDevAddr.routeData *\/ */
    /* if (crequest_in->devAddr.routeData) { */
    FID_DA_ROUTE_DATA = (*env)->GetFieldID(env, K_DEVICE_ADDRESS, "routeData", "Ljava/lang/String;");
    if (FID_DA_ROUTE_DATA == NULL) {
    	    printf("GetFieldID failed routeData of DeviceAddress\n");
    }


    klass = (*env)->FindClass(env, "org/iochibity/ResourceLocal");
    JNI_ASSERT_NULL(klass, "FindClass failed for org/iochibity/ResourceLocal\n", 0);
    K_RESOURCE_LOCAL = (jclass)(*env)->NewGlobalRef(env, klass);
    (*env)->DeleteLocalRef(env, klass);

    klass = (*env)->FindClass(env, "org/iochibity/DocRequestIn");
    JNI_ASSERT_NULL(klass, "FindClass failed for org/iochibity/DocRequestIn\n", 0);
    K_DOC_REQUEST_IN = (jclass)(*env)->NewGlobalRef(env, klass);
    (*env)->DeleteLocalRef(env, klass);

    MID_DRQI_CTOR = (*env)->GetMethodID(env, K_DOC_REQUEST_IN, "<init>", "()V");
    if (MID_DRQI_CTOR == NULL) {
    	printf("ERROR:  GetMethodID failed for ctor for DocRequestIn");
    	return OC_EH_INTERNAL_SERVER_ERROR;
    }
    if (FID_DRQI_IS_REQUEST == NULL) {
	FID_DRQI_IS_REQUEST = (*env)->GetFieldID(env, K_DOC_REQUEST_IN, "isRequest", "Z");
	if (FID_DRQI_IS_REQUEST == NULL) {
	    THROW_JNI_EXCEPTION("GetFieldID failed for 'isRequest' of DocRequestIn\n");
	    return 0;
	}
    }
    if (FID_DRQI_IS_OBSERVE == NULL) {
	FID_DRQI_IS_OBSERVE = (*env)->GetFieldID(env, K_DOC_REQUEST_IN, "isObserve", "Z");
	if (FID_DRQI_IS_OBSERVE == NULL) {
	    THROW_JNI_EXCEPTION("GetFieldID failed for 'isObserve' of DocRequestIn\n");
	    return 0;
	}
    }
    if (FID_DRQI_DEVICE_ADDRESS == NULL) {
	FID_DRQI_DEVICE_ADDRESS = (*env)->GetFieldID(env, K_DOC_REQUEST_IN,
						     "deviceAddress",
						     "Lorg/iochibity/DeviceAddress;");
	if (FID_DRQI_DEVICE_ADDRESS == NULL) {
	    THROW_JNI_EXCEPTION("GetFieldID failed for 'deviceAddress' of DocRequestIn\n");
	    return 0;
	}
    }

    klass = (*env)->FindClass(env, "org/iochibity/DocRequestOut");
    JNI_ASSERT_NULL(klass, "FindClass failed for org/iochibity/DocRequestOut\n", 0);
    K_DOC_REQUEST_OUT = (jclass)(*env)->NewGlobalRef(env, klass);
    (*env)->DeleteLocalRef(env, klass);

    if (FID_DRQO_SERVICE_REQUESTOR == NULL) {
	FID_DRQO_SERVICE_REQUESTOR = (*env)->GetFieldID(env, K_DOC_REQUEST_OUT,
						       "serviceRequestor",
						       "Lorg/iochibity/IResourceServiceRequestor;");
	if (FID_DRQO_SERVICE_REQUESTOR == NULL) {
	    THROW_JNI_EXCEPTION("GetFieldID failed for 'serviceRequestor' of DocRequestOut\n");
	    return 0;
	}
    }

    init_Document(env);

    init_DocResponseIn(env);

    init_java(env);

    prep_pfrs(env);

    klass = (*env)->FindClass(env, "org/iochibity/IResourceServiceRequestor");
    JNI_ASSERT_NULL(klass, "FindClass failed for org/iochibity/IResourceServiceRequestor\n", 0);
    K_IRESOURCE_SERVICE_REQUESTOR = (jclass)(*env)->NewGlobalRef(env, klass);
    (*env)->DeleteLocalRef(env, klass);

    jmethodID MID_IRSR_SERVICE_RESPONSE_IN  = NULL;
    if (MID_IRSR_SERVICE_RESPONSE_IN == NULL) {
	MID_IRSR_SERVICE_RESPONSE_IN = (*env)->GetMethodID(env,
							   K_IRESOURCE_SERVICE_REQUESTOR,
							   "serviceResponseIn",
							   "(Lorg/iochibity/DocResponseIn;)I");
	if (MID_IRSR_SERVICE_RESPONSE_IN == NULL) {
	    THROW_JNI_EXCEPTION("GetMethodID failed for 'serviceResponseIn' of IResourceServiceRequestor\n");
	    return 0;
	}
    }



    return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved)
{
    OC_UNUSED(vm);
    OC_UNUSED(reserved);
    printf("JNI_OnUnload");
    JNIEnv* env;

    int getEnvStat = (*g_JVM)->GetEnv(g_JVM, (void **)&env, JNI_VERSION_1_6);
    if (getEnvStat == JNI_EDETACHED) {
	/* printf("GetEnv: not attached; attaching now\n"); */
	if ((*g_JVM)->AttachCurrentThread(g_JVM, (void **) &env, NULL) != 0) {
	    /* THROW_JNI_EXCEPTION("AttachCurrentThread failed\n"); */
	    printf("ERROR %s %d (%s): AttachCurrentThread failure\n", __FILE__, __LINE__,__func__);
	}
    } else if (getEnvStat == JNI_OK) {
	/* printf("GetEnv: attached\n"); */
    } else if (getEnvStat == JNI_EVERSION) {
	printf("ERROR %s %d (%s): JNI version not supported\n", __FILE__, __LINE__,__func__);
    }
    if ((*env)->ExceptionCheck(env)) {
	(*env)->ExceptionDescribe(env);
    }

    (*env)->DeleteGlobalRef(env, K_PAYLOAD);
}
