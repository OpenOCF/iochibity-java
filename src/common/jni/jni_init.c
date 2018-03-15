/**
 * @file jni_init.c
 * @author Gregg Reynolds
 * @date December 2016
 *
 * @brief JNI_OnLoad and JNI_OnUnload
 */

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

/* #ifdef DEBUG_COAP */
#include "coap/debug.h"		/* libcoap debugging */
/* #endif */
#ifdef DEBUG_TINYDTLS
#include "tinydtls/debug.h"		/* for tinydtls debugging */
#endif

#include "_threads.h"

/* #include "openocf_engine_OCFServices.h" */
#include "org_iochibity_Exceptions.h"
#include "jni_init.h"
#include "jni_utils.h"

#include "openocf.h"
/* #include "octypes.h" */
/* #include "ocresource.h" */
/* #include "ocstack.h" */
/* #include "oic_malloc.h" */
/* #include "oic_string.h" */

/* #include "logger.h" */


JavaVM* g_JVM;

extern const char* g_config_fname;


jclass    K_INTEGER                       = NULL; /* 32-bit ints */
jmethodID MID_INT_CTOR                    = NULL;
jmethodID MID_INT_INTVALUE                = NULL; /* Integer.intValue() returns int */
jclass K_LONG                             = NULL; /* 64-bit ints */
jmethodID MID_LONG_CTOR                   = NULL;
jmethodID MID_LONG_LONGVALUE              = NULL; /* Long.longValue() returns long */
/* jclass K_BIGINTEGER		/\* arbitrary bits *\/ */
jclass K_BOOLEAN                          = NULL;
jmethodID MID_BOOL_CTOR                   = NULL;
jmethodID MID_BOOL_BOOLVALUE              = NULL; /* Boolean.booleanValue() returns boolean */
jclass K_DOUBLE                           = NULL;
jmethodID MID_DBL_CTOR                    = NULL; /* Double.doubleValue() returns double */
jmethodID MID_DBL_DBLVALUE                = NULL; /* Double.doubleValue() returns double */
jclass K_SHORT                            = NULL;
jmethodID MID_SHORT_CTOR                  = NULL;
jmethodID MID_SHORT_SHORTVALUE            = NULL;
jclass K_STRING                           = NULL;
jmethodID MID_STR_CTOR                    = NULL;
jclass K_LIST                             = NULL; /* OCByteString => List<Byte> */
jclass K_BYTE                             = NULL;
jmethodID MID_BYTE_CTOR                   = NULL;
jclass K_OBJECT                           = NULL;
/* jclass K_ARRAY; - List */

jclass    K_LINKED_LIST                   = NULL;
jmethodID MID_LL_CTOR                     = NULL;
jmethodID MID_LL_ADD                      = NULL;
jmethodID MID_LL_GET                      = NULL;

/* Messages */
jclass    K_OBSERVATION_IN                = NULL;
jmethodID MID_OBIN_CTOR               = NULL;
jfieldID  FID_OBIN_OBSERVATION_HANDLE = NULL;
jfieldID  FID_OBIN_REMOTE_DEVADDR     = NULL;
jmethodID MID_OBIN_GET_REMOTE_DEVADDR = NULL;
jfieldID  FID_OBIN_CONN_TYPE          = NULL;
jfieldID  FID_OBIN_REMOTE_SID         = NULL;
jfieldID  FID_OBIN_RESULT             = NULL;
jfieldID  FID_OBIN_SERIAL             = NULL;
jfieldID  FID_OBIN_URI                = NULL;
jmethodID MID_OBIN_GET_OBSERVATION    = NULL;
jfieldID  FID_OBIN_OPTIONS            = NULL;
jfieldID  FID_OBIN_PTR_OPTIONS        = NULL;
jmethodID MID_OBIN_GET_OPTIONS        = NULL;

jclass    K_MSG_RESPONSE_OUT              = NULL;
jmethodID MID_MsgRspOut_CTOR              = NULL;
jfieldID  FID_MsgRspOut_RQST_IN         = NULL;

/* Payloads */
jclass    K_OBSERVATION                   = NULL;
jfieldID  FID_OBSERVATION_HANDLE          = NULL;
jfieldID  FID_OBSERVATION_TYPE            = NULL;
jmethodID MID_OBSERVATION_CTOR            = NULL;
jfieldID  FID_OBSERVATION_URI_PATH        = NULL;
jmethodID MID_OBSERVATION_GET_URI_PATH    = NULL;
jfieldID  FID_OBSERVATION_RTYPES          = NULL;
jmethodID MID_OBSERVATION_GET_RTYPES      = NULL;
jfieldID  FID_OBSERVATION_IFS             = NULL;
jmethodID MID_OBSERVATION_GET_IFS         = NULL;
jfieldID  FID_OBSERVATION_PROPERTIES      = NULL;
jmethodID MID_OBSERVATION_GET_PROPERTIES  = NULL;
jfieldID  FID_OBSERVATION_CHILDREN        = NULL;
jmethodID MID_OBSERVATION_GET_CHILDREN    = NULL;

jclass    K_OBSERVATION_LIST              = NULL;
jmethodID MID_PLL_CTOR                    = NULL;
jmethodID MID_PLL_ADD                     = NULL;

/* jclass   K_PFRS                           = NULL; /\* class for PayloadForResourceState *\/ */
/* jfieldID FID_PFRS_URI                     = NULL; */
/* jfieldID FID_PFRS_RTYPES                  = NULL; */
/* jfieldID FID_PFRS_INTERFACES              = NULL; */
/* jfieldID FID_PFRS_PROPERTIES              = NULL; */

/* jclass    K_PFP                           = NULL; /\* class for PayloadForPlatform *\/ */
/* jfieldID  FID_PFP_URI                     = NULL; */
/* jfieldID  FID_PFP_RTYPES                  = NULL; */
/* jfieldID  FID_PFP_INTERFACES              = NULL; */
/* jfieldID  FID_PFP_PROPERTIES              = NULL; */
/* jmethodID MID_PFP_CTOR                    = NULL; */

jclass   K_SET                            = NULL; /* interface java.util.Set */
jmethodID MID_SET_ITERATOR                = NULL;

jclass   K_ITER                           = NULL; /* interface java.util.Iterator */
jmethodID MID_ITER_HASNEXT                = NULL;
jmethodID MID_ITER_NEXT                   = NULL;
jmethodID MID_ITER_REMOVE                 = NULL;

jclass   K_MAPENTRY                       = NULL; /* interface java.util.Map.Entry */
jmethodID MID_ENTRY_GETKEY                = NULL;
jmethodID MID_ENTRY_GETVALUE              = NULL;

jclass    K_ENDPOINT                      = NULL;
jmethodID MID_EP_CTOR                     = NULL;
jfieldID  FID_EP_NETWORK_PROTOCOL         = NULL;
jfieldID  FID_EP_NETWORK_FLAGS            = NULL;
jfieldID  FID_EP_NETWORK_POLICIES         = NULL;
jfieldID  FID_EP_NETWORK_SCOPE            = NULL;
jfieldID  FID_EP_TRANSPORT_SECURITY       = NULL;
jfieldID  FID_EP_PORT                     = NULL;
jfieldID  FID_EP_ADDRESS                  = NULL;
jfieldID  FID_EP_IFINDEX                  = NULL;
jfieldID  FID_EP_ROUTE_DATA               = NULL;

jclass    K_RESOURCE_LOCAL                = NULL;

jclass    K_MESSAGE                       = NULL;
jfieldID  FID_MSG_LOCAL_HANDLE            = NULL;
jfieldID  FID_MSG_OBSERVATION_HANDLE      = NULL;
/* jmethodID MID_MSG_GET_OBSERVATION_TYPE = NULL; */
jfieldID  FID_MSG_METHOD                  = NULL;
jmethodID MID_MSG_GET_METHOD              = NULL;
jfieldID  FID_MSG_OPTIONS                 = NULL;
jmethodID MID_MSG_GET_OPTIONS             = NULL;
/* jfieldID  FID_MSG_PTR_OPTIONS          = NULL; */

jfieldID  FID_MSG_REMOTE_DEVADDR          = NULL;
char     *NM_MSG_REMOTE_DEVADDR           = "_remoteDeviceAddress";

jmethodID MID_MSG_GET_REMOTE_DEVADDR      = NULL;

jclass   K_MSG_FOR_RESOURCE_SP       = NULL;
jfieldID FID_MFSP_REMOTE_RQST_HANDLE      = NULL;
jfieldID FID_MFSP_RESOURCE_HANDLE         = NULL;

jclass    K_OUTBOUND_STIMULUS               = NULL;
jmethodID MID_RQO_CTOR                    = NULL;
jfieldID  FID_RQO_LOCAL_HANDLE            = NULL;
jfieldID  FID_RQO_CORESOURCE_SP     = NULL;
jfieldID  FID_RQO_METHOD                  = NULL;
/* jfieldID  FID_RQO_URI_PATH                = NULL; */
jfieldID  FID_RQO_DEST                    = NULL;

jclass    K_INBOUND_STIMULUS                = NULL;
jmethodID MID_INBOUND_STIMULUS_CTOR                    = NULL;
jfieldID  FID_INBOUND_STIMULUS_LOCAL_HANDLE            = NULL;
jfieldID  FID_INBOUND_STIMULUS_CORESOURCE_SP     = NULL;
jfieldID  FID_INBOUND_STIMULUS_WATCH_ACTION            = NULL;
jfieldID  FID_INBOUND_STIMULUS_WATCH_ID                = NULL;
jfieldID  FID_INBOUND_STIMULUS_METHOD                  = NULL;
jmethodID MID_INBOUND_STIMULUS_GET_METHOD              = NULL;
jfieldID  FID_INBOUND_STIMULUS_QUERY                   = NULL;
jmethodID MID_INBOUND_STIMULUS_GET_QUERY               = NULL;
jfieldID  FID_INBOUND_STIMULUS_MSG_ID                  = NULL;
jmethodID MID_INBOUND_STIMULUS_GET_MSG_ID              = NULL;

/*
 *
 */
int init_java(JNIEnv* env)
{
    jclass klass = NULL;
    if (K_INTEGER == NULL) {	/* 32 bits */
	klass = (*env)->FindClass(env, JC_INTEGER);
	/* JNI_ASSERT_NULL(klass, "FindClass failed for java/lang/Integer"); */
	if (klass == NULL) {
	    printf("ERROR: FindClass failed for java/lang/Integer");
	    return -1;
	}
	K_INTEGER = (jclass)(*env)->NewGlobalRef(env, klass);
	(*env)->DeleteLocalRef(env, klass);
    }
    if (MID_INT_CTOR == NULL) {
	MID_INT_CTOR = (*env)->GetMethodID(env, K_INTEGER,
					   "<init>",
					   "(" J_INT ")" J_VOID);
	if (MID_INT_CTOR == NULL) {
	    printf("ERROR: GetMethodID failed for ctor of Integer\n");
	    return -1;
	}
    }
    if (MID_INT_INTVALUE == NULL) {
	MID_INT_INTVALUE = (*env)->GetMethodID(env, K_INTEGER,
					       "intValue",
					       J_NULLARY J_INT);
	if (MID_INT_INTVALUE == NULL) {
	    printf("ERROR: GetMethodID failed for 'intValue' of Integer\n");
	    return -1;
	}
    }
    if (K_SHORT == NULL) {	/* uint16_t */
	klass = (*env)->FindClass(env, JC_SHORT_URL);
	if (klass == NULL) {
	    printf("ERROR: FindClass failed for java/lang/Short");
	    return -1;
	}
	K_SHORT = (jclass)(*env)->NewGlobalRef(env, klass);
	(*env)->DeleteLocalRef(env, klass);
    }
    if (MID_SHORT_CTOR == NULL) {
	MID_SHORT_CTOR = (*env)->GetMethodID(env, K_SHORT,
					     "<init>",
					     "(" J_SHORT ")" J_VOID);
	if (MID_SHORT_CTOR == NULL) {
	    printf("ERROR: GetMethodID failed for ctor of Short\n");
	    return -1;
	}
    }
    if (MID_SHORT_SHORTVALUE == NULL) {
	MID_SHORT_SHORTVALUE = (*env)->GetMethodID(env, K_SHORT,
						   "shortValue",
						   J_NULLARY J_SHORT);
	if (MID_SHORT_SHORTVALUE == NULL) {
	    printf("ERROR: GetMethodID failed for 'shortValue' of Short\n");
	    return -1;
	}
    }

    if (K_LONG == NULL) {	/* 64 bits */
	klass = (*env)->FindClass(env, JC_LONG_URL);
	if (klass == NULL) {
	    printf("ERROR: FindClass failed for java/lang/Long");
	    return -1;
	}
	K_LONG = (jclass)(*env)->NewGlobalRef(env, klass);
	(*env)->DeleteLocalRef(env, klass);
    }
    if (MID_LONG_CTOR == NULL) {
	MID_LONG_CTOR = (*env)->GetMethodID(env, K_LONG,
					    "<init>",
					    "(" J_LONG ")" J_VOID);
	if (MID_LONG_CTOR == NULL) {
	    printf("ERROR: GetMethodID failed for ctor of Long\n");
	    return -1;
	}
    }
    if (MID_LONG_LONGVALUE == NULL) {
	MID_LONG_LONGVALUE = (*env)->GetMethodID(env, K_LONG,
						 "longValue",
						 J_NULLARY J_LONG);
	if (MID_LONG_LONGVALUE == NULL) {
	    printf("ERROR: GetMethodID failed for 'longValue' of Long\n");
	    return -1;
	}
    }


    if (K_BOOLEAN == NULL) {
	klass = (*env)->FindClass(env, JC_BOOLEAN_URL);
	if (klass == NULL) {
	    printf("ERROR: FindClass failed for java/lang/Boolean");
	    return -1;
	}
	K_BOOLEAN = (jclass)(*env)->NewGlobalRef(env, klass);
	(*env)->DeleteLocalRef(env, klass);
    }
    if (MID_BOOL_CTOR == NULL) {
	MID_BOOL_CTOR = (*env)->GetMethodID(env, K_BOOLEAN,
					    "<init>",
					    "(" J_BOOLEAN ")" J_VOID);
	if (MID_BOOL_CTOR == NULL) {
	    printf("ERROR: GetMethodID failed for ctor of Boolean\n");
	    return -1;
	}
    }
    if (MID_BOOL_BOOLVALUE == NULL) {
	MID_BOOL_BOOLVALUE = (*env)->GetMethodID(env, K_BOOLEAN, "booleanValue", "()Z");
	if (MID_BOOL_BOOLVALUE == NULL) {
	    printf("ERROR: GetMethodID failed for 'booleanValue' of Boolean\n");
	    return -1;
	}
    }
    if (K_DOUBLE == NULL) {
	klass = (*env)->FindClass(env, "java/lang/Double");
	if (klass == NULL) {
	    printf("ERROR: FindClass failed for java/lang/Double");
	    return -1;
	}
	K_DOUBLE = (jclass)(*env)->NewGlobalRef(env, klass);
	(*env)->DeleteLocalRef(env, klass);
    }
    if (MID_DBL_CTOR == NULL) {
	MID_DBL_CTOR = (*env)->GetMethodID(env, K_DOUBLE, "<init>", "(D)V");
	if (MID_DBL_CTOR == NULL) {
	    printf("ERROR: GetMethodID failed for ctor of Double\n");
	    return -1;
	}
    }
    if (MID_DBL_DBLVALUE == NULL) {
	MID_DBL_DBLVALUE = (*env)->GetMethodID(env, K_DOUBLE, "doubleValue", "()D");
	if (MID_DBL_DBLVALUE == NULL) {
	    printf("ERROR: GetMethodID failed for 'DBLValue' of Double\n");
	    return -1;
	}
    }
    if (K_STRING == NULL) {
	klass = (*env)->FindClass(env, "java/lang/String");
	if (klass == NULL) {
	    printf("ERROR: FindClass failed for java/lang/String");
	    return -1;
	}
	K_STRING = (jclass)(*env)->NewGlobalRef(env, klass);
	(*env)->DeleteLocalRef(env, klass);
    }
    if (MID_STR_CTOR == NULL) {
	MID_STR_CTOR = (*env)->GetMethodID(env, K_STRING, "<init>", "([C)V");
	if (MID_STR_CTOR == NULL) {
	    printf("ERROR: GetMethodID failed for ctor of String\n");
	    return -1;
	}
    }

    /* a byte array must be stored as List<Byte> */
    if (K_LIST == NULL) {
	klass = (*env)->FindClass(env, "java/util/List");
	if (klass == NULL) {
	    printf("ERROR: FindClass failed for java/util/List");
	    return -1;
	}
	K_LIST = (jclass)(*env)->NewGlobalRef(env, klass);
	(*env)->DeleteLocalRef(env, klass);
    }
    if (K_BYTE == NULL) {
	klass = (*env)->FindClass(env, "java/lang/Byte");
	if (klass == NULL) {
	    printf("ERROR: FindClass failed for java/lang/Byte");
	    return -1;
	}
	K_BYTE = (jclass)(*env)->NewGlobalRef(env, klass);
	(*env)->DeleteLocalRef(env, klass);
    }
    if (MID_BYTE_CTOR == NULL) {
	MID_BYTE_CTOR = (*env)->GetMethodID(env, K_BYTE, "<init>", "(B)V");
	if (MID_BYTE_CTOR == NULL) {
	    printf("ERROR: GetMethodID failed for ctor of java/lang/Byte.\n");
	    return -1;
	}
    }

    if (K_OBJECT == NULL) {
	klass = (*env)->FindClass(env, "java/lang/Object");
	if (klass == NULL) {
	    printf("ERROR: FindClass failed for java/lang/Object");
	    return -1;
	}
	K_OBJECT = (jclass)(*env)->NewGlobalRef(env, klass);
	(*env)->DeleteLocalRef(env, klass);
    }

    if (K_LINKED_LIST == NULL) {
	klass = (*env)->FindClass(env, "java/util/LinkedList");
	if (klass == NULL) {
	    printf("ERROR: FindClass failed for LinkedList\n");
	    return -1;
	}
	K_LINKED_LIST = (jclass)(*env)->NewGlobalRef(env, klass);
	(*env)->DeleteLocalRef(env, klass);
    }
    if (MID_LL_CTOR == NULL) {
	MID_LL_CTOR = (*env)->GetMethodID(env, K_LINKED_LIST, "<init>", "()V");
	if (MID_LL_CTOR == NULL) {
	    printf("ERROR: GetMethodID failed for ctor of LinkedList.\n");
	    return -1;
	}
    }
    if (MID_LL_ADD == NULL) {
	MID_LL_ADD = (*env)->GetMethodID(env, K_LINKED_LIST, "add",
					 "(" JC_OBJECT ")" J_BOOLEAN);
	if (MID_LL_ADD == NULL) {
	    printf("ERROR: GetMethodID failed for add method of LinkedList\n");
	    return -1;
	}
    }
    MID_LL_GET = (*env)->GetMethodID(env, K_LINKED_LIST, "get", "(I)Ljava/lang/Object;");
    if (MID_LL_GET == NULL) {
	printf("ERROR: GetMethodID failed for get method of LinkedList\n");
    }

    /* SET */
    if (K_SET == NULL) {
	klass = (*env)->FindClass(env, "java/util/Set");
	if (klass == NULL) {
	    printf("ERROR: FindClass failed for java/util/SET\n");
	    return -1;
	}
	K_SET = (jclass)(*env)->NewGlobalRef(env, klass);
	(*env)->DeleteLocalRef(env, klass);
    }
    if (MID_SET_ITERATOR == NULL) {
	MID_SET_ITERATOR = (*env)->GetMethodID(env, K_SET, "iterator", "()Ljava/util/Iterator;");
	if (MID_SET_ITERATOR == NULL) {
	    printf("ERROR: GetMethodID failed for 'iterator' of Set\n");
	    return -1;
	}
    }

    /* ITERATOR */
    if (K_ITER == NULL) {
	klass = (*env)->FindClass(env, "java/util/Iterator");
	if (klass == NULL) {
	    printf("ERROR: FindClass failed for java/util/Iterator\n");
	    return -1;
	}
	K_ITER = (jclass)(*env)->NewGlobalRef(env, klass);
	(*env)->DeleteLocalRef(env, klass);
    }
    if (MID_ITER_HASNEXT == NULL) {
	MID_ITER_HASNEXT = (*env)->GetMethodID(env, K_ITER, "hasNext", "()Z");
	if (MID_ITER_HASNEXT == NULL) {
	    printf("ERROR: GetMethodID failed for 'hasNext' of PropertyMap\n");
	    return -1;
	}
    }
    if (MID_ITER_NEXT == NULL) {
	MID_ITER_NEXT = (*env)->GetMethodID(env, K_ITER, "next", "()Ljava/lang/Object;");
	if (MID_ITER_NEXT == NULL) {
	    printf("ERROR: GetMethodID failed for 'next' of PropertyMap\n");
	    return -1;
	}
    }
    if (MID_ITER_REMOVE == NULL) {
	MID_ITER_REMOVE = (*env)->GetMethodID(env, K_ITER, "remove", "()V");
	if (MID_ITER_REMOVE == NULL) {
	    printf("ERROR: GetMethodID failed for 'remove' of PropertyMap\n");
	    return -1;
	}
    }
    return 0;
}

// FIXME: payload?
int init_ObservationOut(JNIEnv* env)
{
    jclass klass;
    klass = (*env)->FindClass(env, "openocf/ObservationOut");
    JNI_ASSERT_NULL(klass, "FindClass failed for openocf/ObservationOut\n", -1);
    K_MSG_RESPONSE_OUT = (jclass)(*env)->NewGlobalRef(env, klass);
    (*env)->DeleteLocalRef(env, klass);

    if (MID_MsgRspOut_CTOR == NULL) {
	MID_MsgRspOut_CTOR = (*env)->GetMethodID(env, K_MSG_RESPONSE_OUT, "<init>", "()V");
	if (MID_MsgRspOut_CTOR == NULL) {
	    printf("ERROR:  GetMethodID failed for ctor for ObservationOut");
	    return -1;
	}
    }
    FID_MsgRspOut_RQST_IN = (*env)->GetFieldID(env, K_MSG_RESPONSE_OUT,
					       "_requestIn", "Lopenocf/StimulusIn;");
    if (FID_MsgRspOut_RQST_IN == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for '_requestIn' on ObservationOut\n");
	return -1;
    }
    return 0;
}

int init_ObservationIn(JNIEnv* env)
{
    jclass klass;
    klass = (*env)->FindClass(env, "openocf/ObservationIn");
    JNI_ASSERT_NULL(klass, "FindClass failed for openocf/ObservationIn\n", -1);
    K_OBSERVATION_IN = (jclass)(*env)->NewGlobalRef(env, klass);
    (*env)->DeleteLocalRef(env, klass);

    if (MID_OBIN_CTOR == NULL) {
	MID_OBIN_CTOR = (*env)->GetMethodID(env, K_OBSERVATION_IN, "<init>", "()V");
	if (MID_OBIN_CTOR == NULL) {
	    printf("ERROR:  GetMethodID failed for ctor for ObservationIn");
	    return -1;
	}
    }
    if (FID_OBIN_OBSERVATION_HANDLE == NULL) {
    	FID_OBIN_OBSERVATION_HANDLE = (*env)->GetFieldID(env, K_OBSERVATION_IN,
    							 "_observationRecordHandle", "J");
    	if (FID_OBIN_OBSERVATION_HANDLE == NULL) {
    	    printf("ERROR: GetFieldID failed for '_observationRecordHandle' of ObservationIn\n");
    	    return -1;
    	}
    }
    if (FID_OBIN_REMOTE_DEVADDR == NULL) {
	FID_OBIN_REMOTE_DEVADDR = (*env)->GetFieldID(env, K_OBSERVATION_IN,
						     "_remoteDeviceAddress",
						     FQCS_ENDPOINT);
							 /* "Lopenocf/EndPoint;"); */
	if (FID_OBIN_REMOTE_DEVADDR == NULL) {
	    printf("ERROR: GetFieldID failed for '_remoteDeviceAddress' of ObservationIn\n");
	    return -1;
	}
    }
    /* if (MID_OBIN_GET_REMOTE_DEVADDR == NULL) { */
    /* 	MID_OBIN_GET_REMOTE_DEVADDR = (*env)->GetMethodID(env, K_OBSERVATION_IN, */
    /* 							      "getRemoteDeviceAddress", */
    /* 							      "()Lopenocf/EndPoint;"); */
    /* 	if (MID_OBIN_GET_REMOTE_DEVADDR == NULL) { */
    /* 	    printf("ERROR:  GetMethodID failed for getRemoteDeviceAddress for ObservationIn\n"); */
    /* 	    return -1; */
    /* 	} */
    /* } */
    if (FID_OBIN_CONN_TYPE == NULL) {
	FID_OBIN_CONN_TYPE = (*env)->GetFieldID(env, K_OBSERVATION_IN, "connType", "I");
	if (FID_OBIN_CONN_TYPE == NULL) {
	    printf("ERROR: GetFieldID failed for 'connType' of ObservationIn\n");
	    return -1;
	}
    }
    if (FID_OBIN_REMOTE_SID == NULL) {
	FID_OBIN_REMOTE_SID = (*env)->GetFieldID(env, K_OBSERVATION_IN,
						  "secID", "Ljava/lang/String;");
	if (FID_OBIN_REMOTE_SID == NULL) {
	    printf("ERROR: GetFieldID failed for 'secID' of ObservationIn\n");
	    return -1;
	}
    }
    if (FID_OBIN_RESULT == NULL) {
	FID_OBIN_RESULT = (*env)->GetFieldID(env, K_OBSERVATION_IN, "result", "I");
	if (FID_OBIN_RESULT == NULL) {
	    printf("ERROR: GetFieldID failed for 'result' of ObservationIn\n");
	    return -1;
	}
    }
    if (FID_OBIN_SERIAL == NULL) {
	FID_OBIN_SERIAL = (*env)->GetFieldID(env, K_OBSERVATION_IN, "serial", "I");
	if (FID_OBIN_RESULT == NULL) {
	    printf("ERROR: GetFieldID failed for 'serial' of ObservationIn\n");
	    return -1;
	}
    }
    /* if (FID_OBIN_URI == NULL) { */
    /* 	FID_OBIN_URI = (*env)->GetFieldID(env, K_OBSERVATION_IN, "_uriPath", "Ljava/lang/String;"); */
    /* 	if (FID_OBIN_URI == NULL) { */
    /* 	    printf("ERROR: GetFieldID failed for 'uri' of ObservationIn\n"); */
    /* 	    return -1; */
    /* 	} */
    /* } */
    /* if (MID_OBIN_GET_OBSERVATION == NULL) { */
    /* 	MID_OBIN_GET_OBSERVATION = (*env)->GetMethodID(env, K_OBSERVATION_IN, */
    /* 						    "getPDUObservation", */
    /* 						    "()Lopenocf/ObservationList;"); */
    /* 	if (MID_OBIN_GET_OBSERVATION == NULL) { */
    /* 	    printf("ERROR: GetFieldID failed for 'getPDUObservation' of ObservationIn\n"); */
    /* 	    return -1; */
    /* 	} */
    /* } */
    /* if (FID_OBIN_PTR_OPTIONS == NULL) { */
    /* 	FID_OBIN_PTR_OPTIONS = (*env)->GetFieldID(env, K_OBSERVATION_IN, "ptr_Options", "J"); */
    /* 	if (FID_OBIN_PTR_OPTIONS == NULL) { */
    /* 	    printf("ERROR: GetFieldID failed for 'ptr_Options' of ObservationIn\n"); */
    /* 	    return -1; */
    /* 	} */
    /* } */
    /* if (FID_OBIN_OPTION_COUNT == NULL) { */
    /* 	FID_OBIN_OPTION_COUNT = (*env)->GetFieldID(env, K_OBSERVATION_IN, "optionCount", "I"); */
    /* 	if (FID_OBIN_OPTION_COUNT == NULL) { */
    /* 	    printf("ERROR: GetFieldID failed for 'optionCount' of ObservationIn\n"); */
    /* 	    return -1; */
    /* 	} */
    /* } */
    /* if (MID_OBIN_GET_OPTIONS == NULL) { */
    /* 	MID_OBIN_GET_OPTIONS = (*env)->GetMethodID(env, K_OBSERVATION_IN, */
    /* 						    "getOptions", "()Ljava/util/List;"); */
    /* 	if (MID_OBIN_GET_OPTIONS == NULL) { */
    /* 	    printf("ERROR: GetFieldID failed for 'getOptions' of ObservationIn\n"); */
    /* 	    return -1; */
    /* 	} */
    /* } */
    return 0;
}

int init_observation(JNIEnv* env)
{
    jclass klass;
    klass = (*env)->FindClass(env, "openocf/ObservationRecord");
    JNI_ASSERT_NULL(klass, "FindClass failed for openocf/ObservationRecord\n", 0);
    K_OBSERVATION = (jclass)(*env)->NewGlobalRef(env, klass);
    (*env)->DeleteLocalRef(env, klass);

    FID_OBSERVATION_HANDLE = (*env)->GetFieldID(env, K_OBSERVATION, "_handle", "J");
    if (FID_OBSERVATION_HANDLE == NULL) {
	printf("ERROR: GetFieldID failed for '_handle' of Observation\n");
	return -1;
    }
    FID_OBSERVATION_TYPE = (*env)->GetFieldID(env, K_OBSERVATION, "_type", "I");
    if (FID_OBSERVATION_TYPE == NULL) {
	printf("ERROR: GetFieldID failed for 'type' on Observation\n");
	return -1;
    }
    if (MID_OBSERVATION_CTOR == NULL) {
	MID_OBSERVATION_CTOR = (*env)->GetMethodID(env, K_OBSERVATION, "<init>", "(Lopenocf/StimulusIn;)V");
	if (MID_OBSERVATION_CTOR == NULL) {
	    printf("ERROR: GetMethodID failed for ctor of Observation\n");
	    return -1;
	}
    }
    if (FID_OBSERVATION_URI_PATH == NULL) {
	FID_OBSERVATION_URI_PATH = (*env)->GetFieldID(env, K_OBSERVATION, "_uriPath", "Ljava/lang/String;");
	if (FID_OBSERVATION_URI_PATH == NULL) {
	printf("ERROR: GetFieldID failed for '_uriPath' on Observation\n");
	return -1;
	}
    }
    if (MID_OBSERVATION_GET_URI_PATH == NULL) {
	MID_OBSERVATION_GET_URI_PATH = (*env)->GetMethodID(env, K_OBSERVATION,
						       "getUriPath",
						       "()Ljava/lang/String;");
	if (MID_OBSERVATION_GET_URI_PATH == NULL) {
	    printf("ERROR: GetMethodID failed for MID_OBSERVATION_GET_URI_PATH of Observation\n");
	    return -1;
	}
    }

    if (FID_OBSERVATION_RTYPES == NULL) {
	FID_OBSERVATION_RTYPES = (*env)->GetFieldID(env, K_OBSERVATION,
						"_rtypes", "Ljava/util/List;");
	if (FID_OBSERVATION_RTYPES == NULL) {
	printf("ERROR: GetFieldID failed for '_rtypes' on Observation\n");
	return -1;
	}
    }
    if (MID_OBSERVATION_GET_RTYPES == NULL) {
	MID_OBSERVATION_GET_RTYPES = (*env)->GetMethodID(env, K_OBSERVATION,
						     "getResourceTypes",
						       "()Ljava/util/List;");
	if (MID_OBSERVATION_GET_RTYPES == NULL) {
	    printf("ERROR: GetMethodID failed for MID_OBSERVATION_GET_RTYPES of Observation\n");
	    return -1;
	}
    }

    if (FID_OBSERVATION_IFS == NULL) {
	FID_OBSERVATION_IFS = (*env)->GetFieldID(env, K_OBSERVATION,
						"_interfaces", "Ljava/util/List;");
	if (FID_OBSERVATION_IFS == NULL) {
	printf("ERROR: GetFieldID failed for '_interfaces' on Observation\n");
	return -1;
	}
    }
    if (MID_OBSERVATION_GET_IFS == NULL) {
	MID_OBSERVATION_GET_IFS = (*env)->GetMethodID(env, K_OBSERVATION,
						  "getInterfaces",
						  "()Ljava/util/List;");
	if (MID_OBSERVATION_GET_IFS == NULL) {
	    printf("ERROR: GetMethodID failed for MID_OBSERVATION_GET_IFS of Observation\n");
	    return -1;
	}
    }

    if (FID_OBSERVATION_PROPERTIES == NULL) {
	FID_OBSERVATION_PROPERTIES = (*env)->GetFieldID(env, K_OBSERVATION,
						    "_properties", FQCS_PMAP);
	if (FID_OBSERVATION_PROPERTIES == NULL) {
	printf("ERROR: GetFieldID failed for '_properties' on Observation\n");
	return -1;
	}
    }
    if (MID_OBSERVATION_GET_PROPERTIES == NULL) {
	MID_OBSERVATION_GET_PROPERTIES = (*env)->GetMethodID(env, K_OBSERVATION,
						       "getProperties",
						       "()Lopenocf/PropertyMap;");
	if (MID_OBSERVATION_GET_PROPERTIES == NULL) {
	    printf("ERROR: GetMethodID failed for MID_OBSERVATION_GET_PROPERTIES of Observation\n");
	    return -1;
	}
    }

    if (FID_OBSERVATION_CHILDREN == NULL) {
	FID_OBSERVATION_CHILDREN = (*env)->GetFieldID(env, K_OBSERVATION,
						  "_children", "Ljava/util/List;");
	if (FID_OBSERVATION_CHILDREN == NULL) {
	printf("ERROR: GetFieldID failed for '_children' on Observation\n");
	return -1;
	}
    }
    if (MID_OBSERVATION_GET_CHILDREN == NULL) {
	MID_OBSERVATION_GET_CHILDREN = (*env)->GetMethodID(env, K_OBSERVATION,
						       "getChildren",
						       "()Ljava/util/List;");
	if (MID_OBSERVATION_GET_CHILDREN == NULL) {
	    printf("ERROR: GetMethodID failed for MID_OBSERVATION_GET_CHILDREN of Observation\n");
	    return -1;
	}
    }

    return 0;
}

/*
 * initialize PayloadForPlatform
 */
/* int init_pfp(JNIEnv* env) */
/* { */
/*     if (K_PFP == NULL) { */
/* 	K_PFP = (*env)->FindClass(env, "openocf/PayloadForPlatform"); */
/* 	if (K_PFP == NULL) { */
/* 	    printf("ERROR: FindClass failed for openocf/PayloadForPlatform\n"); */
/* 	    return -1; */
/* 	} */
/*     } */
/*     MID_PFP_CTOR = (*env)->GetMethodID(env, K_PFP, "<init>", "()V"); */
/*     if (MID_PFP_CTOR == 0) { */
/* 	printf("ERROR: GetMethodID failed for ctor of PayloadForPlatform.\n"); */
/* 	return -1; */
/*     } */
/*     if (FID_PFP_URI == NULL) { */
/* 	FID_PFP_URI = (*env)->GetFieldID(env, K_PFP, "_uri", "Ljava/lang/String;"); */
/* 	if (FID_PFP_URI == NULL) { */
/* 	    printf("ERROR: GetFieldID failed for '_uri' of PFP"); */
/* 	    return -1; */
/* 	} */
/*     } */
/*     if (FID_PFP_RTYPES == NULL) { */
/* 	FID_PFP_RTYPES = (*env)->GetFieldID(env, K_PFP, "_rtypes", "Ljava/util/List;"); */
/* 	if (FID_PFP_RTYPES == NULL) { */
/* 	    printf("ERROR: GetFieldID failed for '_uri' of PFP"); */
/* 	    return -1; */
/* 	} */
/*     } */
/*     if (FID_PFP_INTERFACES == NULL) { */
/* 	FID_PFP_INTERFACES = (*env)->GetFieldID(env, K_PFP, "_interfaces", "Ljava/util/List;"); */
/* 	if (FID_PFP_INTERFACES == NULL) { */
/* 	    printf("ERROR: GetFieldID failed for '_interfaces' of PFP"); */
/* 	    return -1; */
/* 	} */
/*     } */
/*     if (FID_PFP_PROPERTIES == NULL) { */
/* 	FID_PFP_PROPERTIES = (*env)->GetFieldID(env, K_PFP, "_properties", FQCS_PMAP); */
/* 	if (FID_PFP_PROPERTIES == NULL) { */
/* 	    printf("ERROR: GetFieldID failed for '_properties' of PFP"); */
/* 	    return -1; */
/* 	} */
/*     } */
/*     return 0; */
/* } */

/*
 *
 */
/* void init_pfrs(JNIEnv* env) */
/* { */
/*     if (K_PFRS == NULL) { */
/* 	K_PFRS = (*env)->FindClass(env, "openocf/PayloadForResourceState"); */
/* 	if (K_PFRS == NULL) { */
/* 	    printf("ERROR: FindClass failed for openocf/PayloadForResourceState\n"); */
/* 	    return; */
/* 	} */
/*     } */
/*     if (FID_PFRS_URI == NULL) { */
/* 	FID_PFRS_URI = (*env)->GetFieldID(env, K_PFRS, "_uri", "Ljava/lang/String;"); */
/* 	if (FID_PFRS_URI == NULL) { */
/* 	    printf("ERROR: GetFieldID failed for '_uri' of PFRS"); */
/* 	    return; */
/* 	} */
/*     } */
/*     if (FID_PFRS_RTYPES == NULL) { */
/* 	FID_PFRS_RTYPES = (*env)->GetFieldID(env, K_PFRS, "_rtypes", "Ljava/util/List;"); */
/* 	if (FID_PFRS_RTYPES == NULL) { */
/* 	    printf("ERROR: GetFieldID failed for '_uri' of PFRS"); */
/* 	    return; */
/* 	} */
/*     } */
/*     if (FID_PFRS_INTERFACES == NULL) { */
/* 	FID_PFRS_INTERFACES = (*env)->GetFieldID(env, K_PFRS, "_interfaces", "Ljava/util/List;"); */
/* 	if (FID_PFRS_INTERFACES == NULL) { */
/* 	    printf("ERROR: GetFieldID failed for '_interfaces' of PFRS"); */
/* 	    return; */
/* 	} */
/*     } */
/*     if (FID_PFRS_PROPERTIES == NULL) { */
/* 	FID_PFRS_PROPERTIES = (*env)->GetFieldID(env, K_PFRS, "_properties", K_PMAP_T); */
/* 	if (FID_PFRS_PROPERTIES == NULL) { */
/* 	    printf("ERROR: GetFieldID failed for '_properties' of PFRS"); */
/* 	    return; */
/* 	} */
/*     } */
/* } */

/*
 *
 */
int init_observation_list(JNIEnv* env)
{
        /* klass = (*env)->FindClass(env, "openocf/ObservationList"); */
    /* JNI_ASSERT_NULL(klass, "FindClass failed for openocf/ObservationList\n", 0); */
    /* K_OBSERVATION_LIST = (jclass)(*env)->NewGlobalRef(env, klass); */
    /* (*env)->DeleteLocalRef(env, klass); */
    /* MID_PLL_CTOR = (*env)->GetMethodID(env, K_OBSERVATION_LIST, "<init>", "()V"); */
    /* if (MID_PLL_CTOR == 0) { */
    /* 	printf("ERROR: GetMethodID failed for ctor of ObservationList.\n"); */
    /* 	return -1; */
    /* } */
    /* MID_PLL_ADD = (*env)->GetMethodID(env, K_OBSERVATION_LIST, "add", "(Ljava/lang/Object;)Z"); */
    /* if (MID_PLL_ADD == NULL) { */
    /* 	printf("ERROR: GetMethodID failed for add method of ObservationList\n"); */
    /* } */
    return 0;
}

/**
 * @brief called when native lib is loaded, e.g. by System.loadlibrary("mylib")
 */
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
    OC_UNUSED(reserved);
    /* printf("%s ENTRY\n", __func__); */

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

    /* init_observation_list(env); */

    /* FIXME: move to openocf_utils_EndPoint.c */
    /* EndPoint = OCDevAddr */
    if (K_ENDPOINT == NULL) {
	klass = (*env)->FindClass(env, FQCN_ENDPOINT); // "openocf/utilsEndPoint");
	JNI_ASSERT_NULL(klass, FINDCLASS_FAIL(FQCN_ENDPOINT), 0);
	K_ENDPOINT = (jclass)(*env)->NewGlobalRef(env, klass);
	(*env)->DeleteLocalRef(env, klass);
    }
    MID_EP_CTOR = (*env)->GetMethodID(env, K_ENDPOINT, "<init>", "()V");
    if (MID_EP_CTOR == 0) {
	printf("ERROR: GetMethodID failed for ctor of EndPoint.\n");
	return -1;
    }

    // networkProtocol -> OCDevAddr.adapter
    /* FID_EP_NETWORK_PROTOCOL = (*env)->GetFieldID(env, K_ENDPOINT, "networkProtocol", "I"); */
    /* if (FID_EP_NETWORK_PROTOCOL == NULL) { */
    /* 	printf("ERROR:  GetFieldID failed for networkProtocol for EndPoint\n"); */
    /* 	fflush(NULL); */
    /* 	return OC_EH_INTERNAL_SERVER_ERROR; */
    /* } */
    /* FID_EP_NETWORK_FLAGS = (*env)->GetFieldID(env, K_ENDPOINT, "networkFlags", "I"); */
    /* if (FID_EP_NETWORK_FLAGS == NULL) { */
    /* 	printf("ERROR:  GetFieldID failed for networkFlags for EndPoint\n"); */
    /* 	fflush(NULL); */
    /* 	return OC_EH_INTERNAL_SERVER_ERROR; */
    /* } */
    /* FID_EP_NETWORK_POLICIES= (*env)->GetFieldID(env, K_ENDPOINT, "networkPolicies", "B"); */
    /* if (FID_EP_NETWORK_POLICIES == NULL) { */
    /* 	printf("ERROR:  GetFieldID failed for 'networkPolicy' of EndPoint\n"); */
    /* 	fflush(NULL); */
    /* 	return OC_EH_INTERNAL_SERVER_ERROR; */
    /* } */
    /* FID_EP_NETWORK_SCOPE= (*env)->GetFieldID(env, K_ENDPOINT, "networkScope", "B"); */
    /* if (FID_EP_NETWORK_SCOPE == NULL) { */
    /* 	printf("ERROR:  GetFieldID failed for 'networkPolicy' of EndPoint\n"); */
    /* 	fflush(NULL); */
    /* 	return OC_EH_INTERNAL_SERVER_ERROR; */
    /* } */
    /* FID_EP_TRANSPORT_SECURITY= (*env)->GetFieldID(env, K_ENDPOINT, "transportSecurity", "Z"); */
    /* if (FID_EP_TRANSPORT_SECURITY == NULL) { */
    /* 	printf("ERROR:  GetFieldID failed for 'transportSecurity' of EndPoint\n"); */
    /* 	fflush(NULL); */
    /* 	return OC_EH_INTERNAL_SERVER_ERROR; */
    /* } */
    /* FID_EP_PORT = (*env)->GetFieldID(env, K_ENDPOINT, "port", "I"); */
    /* if (FID_EP_PORT == NULL) { */
    /* 	printf("ERROR:  GetFieldID failed for port for EndPoint\n"); */
    /* 	fflush(NULL); */
    /* 	return OC_EH_INTERNAL_SERVER_ERROR; */
    /* } */
    /* FID_EP_ADDRESS = (*env)->GetFieldID(env, K_ENDPOINT, "address", "Ljava/lang/String;"); */
    /* if (FID_EP_ADDRESS == NULL) { */
    /* 	printf("ERROR:  GetFieldID failed for address of EndPoint\n"); */
    /* 	fflush(NULL); */
    /* 	return OC_EH_INTERNAL_SERVER_ERROR; */
    /* } */
    /* FID_EP_IFINDEX = (*env)->GetFieldID(env, K_ENDPOINT, "ifindex", "I"); */
    /* if (FID_EP_IFINDEX == NULL) { */
    /* 	printf("ERROR:  GetFieldID failed for ifindex EndPoint\n"); */
    /* 	fflush(NULL); */
    /* 	return OC_EH_INTERNAL_SERVER_ERROR; */
    /* } */
    /* FIXME */
    /* /\* OCDevAddr.routeData *\/ */
    /* if (crequest_in->devAddr.routeData) { */
    /* FID_EP_ROUTE_DATA = (*env)->GetFieldID(env, K_ENDPOINT, "routeData", "Ljava/lang/String;"); */
    /* if (FID_EP_ROUTE_DATA == NULL) { */
    /* 	    printf("ERROR: GetFieldID failed routeData of EndPoint\n"); */
    /* } */

    /* klass = (*env)->FindClass(env, "openocf/ResourceLocal"); */
    /* JNI_ASSERT_NULL(klass, "FindClass failed for openocf/ResourceLocal\n", 0); */
    /* K_RESOURCE_LOCAL = (jclass)(*env)->NewGlobalRef(env, klass); */
    /* (*env)->DeleteLocalRef(env, klass); */

    /* if (init_Messages(env) != 0) return OC_EH_INTERNAL_SERVER_ERROR; */

    init_ResourceSPs(env);

    /* init_StimulusOut(env); */

    /* init_StimulusIn(env); */

    /* init_ObservationIn(env); */

    init_java(env);

    /* if (init_observation(env) != 0) return OC_EH_INTERNAL_SERVER_ERROR; */

    /* init_pfp(env); */

    /* init_pfrs(env); */

    init_pmap(env);		/* prep mid's etc. for iterating over pmap */

    init_ICoResourceSP(env);
    init_CoResourceSP(env);

    /* printf("%s EXIT\n", __func__); */
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

    (*env)->DeleteGlobalRef(env, K_OBSERVATION);
}
