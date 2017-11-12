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

#include "org_iochibity_OCF.h"
#include "org_iochibity_Exceptions.h"
#include "jni_init.h"
#include "jni_utils.h"

#include "octypes.h"
#include "ocresource.h"
#include "ocstack.h"
#include "oic_malloc.h"
#include "oic_string.h"

#include "logger.h"


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

jclass   K_PMAP                           = NULL; /* class for PropertyMap */
jmethodID MID_PMAP_CTOR                   = NULL;
jmethodID MID_PMAP_SIZE                   = NULL;
jmethodID MID_PMAP_ENTRYSET               = NULL;
jmethodID MID_PMAP_GET                    = NULL;
jmethodID MID_PMAP_PUT                    = NULL;

jclass   K_SET                            = NULL; /* interface java.util.Set */
jmethodID MID_SET_ITERATOR                = NULL;

jclass   K_ITER                           = NULL; /* interface java.util.Iterator */
jmethodID MID_ITER_HASNEXT                = NULL;
jmethodID MID_ITER_NEXT                   = NULL;
jmethodID MID_ITER_REMOVE                 = NULL;

jclass   K_MAPENTRY                       = NULL; /* interface java.util.Map.Entry */
jmethodID MID_ENTRY_GETKEY                = NULL;
jmethodID MID_ENTRY_GETVALUE              = NULL;

jclass    K_DEVICE_ADDRESS                = NULL;
jmethodID MID_DA_CTOR                     = NULL;
jfieldID  FID_DA_NETWORK_PROTOCOL         = NULL;
jfieldID  FID_DA_NETWORK_FLAGS            = NULL;
jfieldID  FID_DA_NETWORK_POLICIES         = NULL;
jfieldID  FID_DA_NETWORK_SCOPE            = NULL;
jfieldID  FID_DA_TRANSPORT_SECURITY       = NULL;
jfieldID  FID_DA_PORT                     = NULL;
jfieldID  FID_DA_ADDRESS                  = NULL;
jfieldID  FID_DA_IFINDEX                  = NULL;
jfieldID  FID_DA_ROUTE_DATA               = NULL;

jclass    K_ISERVICE_PROVIDER             = NULL;
/* jmethodID MID_ISP_CTOR                 = NULL; */
/* jmethodID MID_ISP_OBSERVE_STIMULUS      = NULL; */

jclass    K_SERVICE_PROVIDER             = NULL;
jmethodID MID_SP_REACT                   = NULL;
jfieldID  FID_SP_HANDLE                   = NULL;
/* jfieldID  FID_SP_ID                       = NULL; */
jfieldID  FID_SP_URI_PATH                 = NULL;
jfieldID  FID_SP_TYPES                    = NULL;
jfieldID  FID_SP_INTERFACES               = NULL;
jfieldID  FID_SP_PROPERTIES               = NULL;
jfieldID  FID_SP_CHILDREN                 = NULL;
jfieldID  FID_SP_ACTION_SET               = NULL;
jfieldID  FID_SP_POLICIES                 = NULL;

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
jmethodID MID_MSG_GET_REMOTE_DEVADDR      = NULL;

jclass   K_MSG_FOR_SERVICE_PROVIDER       = NULL;
jfieldID FID_MFSP_REMOTE_RQST_HANDLE      = NULL;
jfieldID FID_MFSP_RESOURCE_HANDLE         = NULL;

jclass    K_MSG_REQUEST_OUT               = NULL;
jmethodID MID_RQO_CTOR                    = NULL;
jfieldID  FID_RQO_LOCAL_HANDLE            = NULL;
jfieldID  FID_RQO_CO_SERVICE_PROVIDER     = NULL;
jfieldID  FID_RQO_METHOD                  = NULL;
/* jfieldID  FID_RQO_URI_PATH                = NULL; */
jfieldID  FID_RQO_DEST                    = NULL;

jclass    K_MSG_REQUEST_IN                = NULL;
jmethodID MID_RQI_CTOR                    = NULL;
jfieldID  FID_RQI_LOCAL_HANDLE            = NULL;
jfieldID  FID_RQI_CO_SERVICE_PROVIDER     = NULL;
jfieldID  FID_RQI_WATCH_ACTION            = NULL;
jfieldID  FID_RQI_WATCH_ID                = NULL;
jfieldID  FID_RQI_METHOD                  = NULL;
jmethodID MID_RQI_GET_METHOD              = NULL;
jfieldID  FID_RQI_QUERY                   = NULL;
jmethodID MID_RQI_GET_QUERY               = NULL;
jfieldID  FID_RQI_MSG_ID                  = NULL;
jmethodID MID_RQI_GET_MSG_ID              = NULL;

jclass    K_I_CO_SERVICE_PROVIDER         = NULL;
jmethodID MID_ICOSP_COREACT               = NULL;

jclass    K_CO_SERVICE_PROVIDER           = NULL;
/* jfieldID  FID_COSP_HANDLE              = NULL; */
/* jfieldID  FID_COSP_MSG_RESPONSE_IN  = NULL; */
/* jfieldID  FID_COSP_METHOD              = NULL; */
/* jfieldID  FID_COSP_URI_PATH            = NULL; */
/* jfieldID  FID_COSP_DESTINATION         = NULL; */
jmethodID MID_COSP_EXHIBIT    = NULL;


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
	    printf("ERROR: FindClass failed for java/lang/Integer");
	    return -1;
	}
	K_INTEGER = (jclass)(*env)->NewGlobalRef(env, klass);
	(*env)->DeleteLocalRef(env, klass);
    }
    if (MID_INT_CTOR == NULL) {
	MID_INT_CTOR = (*env)->GetMethodID(env, K_INTEGER, "<init>", "(I)V");
	if (MID_INT_CTOR == NULL) {
	    printf("ERROR: GetMethodID failed for ctor of Integer\n");
	    return -1;
	}
    }
    if (MID_INT_INTVALUE == NULL) {
	MID_INT_INTVALUE = (*env)->GetMethodID(env, K_INTEGER, "intValue", "()I");
	if (MID_INT_INTVALUE == NULL) {
	    printf("ERROR: GetMethodID failed for 'intValue' of Integer\n");
	    return -1;
	}
    }
    if (K_SHORT == NULL) {	/* uint16_t */
	klass = (*env)->FindClass(env, "java/lang/Short");
	if (klass == NULL) {
	    printf("ERROR: FindClass failed for java/lang/Short");
	    return -1;
	}
	K_SHORT = (jclass)(*env)->NewGlobalRef(env, klass);
	(*env)->DeleteLocalRef(env, klass);
    }
    if (MID_SHORT_CTOR == NULL) {
	MID_SHORT_CTOR = (*env)->GetMethodID(env, K_SHORT, "<init>", "(S)V");
	if (MID_SHORT_CTOR == NULL) {
	    printf("ERROR: GetMethodID failed for ctor of Short\n");
	    return -1;
	}
    }
    if (MID_SHORT_SHORTVALUE == NULL) {
	MID_SHORT_SHORTVALUE = (*env)->GetMethodID(env, K_SHORT, "shortValue", "()S");
	if (MID_SHORT_SHORTVALUE == NULL) {
	    printf("ERROR: GetMethodID failed for 'shortValue' of Short\n");
	    return -1;
	}
    }

    if (K_LONG == NULL) {	/* 64 bits */
	klass = (*env)->FindClass(env, "java/lang/Long");
	if (klass == NULL) {
	    printf("ERROR: FindClass failed for java/lang/Long");
	    return -1;
	}
	K_LONG = (jclass)(*env)->NewGlobalRef(env, klass);
	(*env)->DeleteLocalRef(env, klass);
    }
    if (MID_LONG_CTOR == NULL) {
	MID_LONG_CTOR = (*env)->GetMethodID(env, K_LONG, "<init>", "(J)V");
	if (MID_LONG_CTOR == NULL) {
	    printf("ERROR: GetMethodID failed for ctor of Long\n");
	    return -1;
	}
    }
    if (MID_LONG_LONGVALUE == NULL) {
	MID_LONG_LONGVALUE = (*env)->GetMethodID(env, K_LONG, "longValue", "()J");
	if (MID_LONG_LONGVALUE == NULL) {
	    printf("ERROR: GetMethodID failed for 'longValue' of Long\n");
	    return -1;
	}
    }


    if (K_BOOLEAN == NULL) {
	klass = (*env)->FindClass(env, "java/lang/Boolean");
	if (klass == NULL) {
	    printf("ERROR: FindClass failed for java/lang/Boolean");
	    return -1;
	}
	K_BOOLEAN = (jclass)(*env)->NewGlobalRef(env, klass);
	(*env)->DeleteLocalRef(env, klass);
    }
    if (MID_BOOL_CTOR == NULL) {
	MID_BOOL_CTOR = (*env)->GetMethodID(env, K_BOOLEAN, "<init>", "(Z)V");
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
	MID_LL_ADD = (*env)->GetMethodID(env, K_LINKED_LIST, "add", "(Ljava/lang/Object;)Z");
	if (MID_LL_ADD == NULL) {
	    printf("ERROR: GetMethodID failed for add method of LinkedList\n");
	    return -1;
	}
    }
    MID_LL_GET = (*env)->GetMethodID(env, K_LINKED_LIST, "get", "(I)Ljava/lang/Object;");
    if (MID_LL_ADD == NULL) {
	printf("ERROR: GetMethodID failed for add method of LinkedList\n");
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

int init_ServiceProviders(JNIEnv* env)
{
    /* first, the interface IServiceProvider */
    jclass klass = NULL;
    klass = (*env)->FindClass(env, "org/iochibity/IServiceProvider");
    JNI_ASSERT_NULL(klass, "FindClass failed for org/iochibity/IServiceProvider\n", 0);
    K_ISERVICE_PROVIDER = (jclass)(*env)->NewGlobalRef(env, klass);
    (*env)->DeleteLocalRef(env, klass);

    /* if (MID_ISP_OBSERVE_STIMULUS == NULL) { */
    /* 	MID_ISP_OBSERVE_STIMULUS = (*env)->GetMethodID(env, */
    /* 							 K_ISERVICE_PROVIDER, */
    /* 							 "observeStimulus", */
    /* 							 "(Lorg/iochibity/StimulusIn;)I"); */
    /* 	if (MID_ISP_OBSERVE_STIMULUS == NULL) { */
    /* 	    printf("ERROR: GetMethodID failed for 'observeStimulus' of IServiceProvider\n"); */
    /* 	    return -1; */
    /* 	} */
    /* } */

    /* then, the abstract class ServiceProvider */
    klass = (*env)->FindClass(env, "org/iochibity/ServiceProvider");
    JNI_ASSERT_NULL(klass, "FindClass failed for org/iochibity/ServiceProvider\n", 0);
    K_SERVICE_PROVIDER = (jclass)(*env)->NewGlobalRef(env, klass);
    (*env)->DeleteLocalRef(env, klass);

    if (MID_SP_REACT == NULL) {
	MID_SP_REACT = (*env)->GetMethodID(env, K_SERVICE_PROVIDER,
					  "react", "()V");
	if (MID_SP_REACT == NULL) {
	    printf("ERROR: GetFieldID failed for 'react' of ServiceProvider\n");
	    return -1;
	}
    }
    if (FID_SP_HANDLE == NULL) {
	FID_SP_HANDLE = (*env)->GetFieldID(env, K_SERVICE_PROVIDER, "_handle", "J");
	if (FID_SP_HANDLE == NULL) {
	    printf("ERROR: GetFieldID failed for '_handle' of ServiceProvider\n");
	    return -1;
	}
    }
    /* if (FID_SP_ID == NULL) { */
    /* 	FID_SP_ID = (*env)->GetFieldID(env, K_SERVICE_PROVIDER, */
    /* 					"_id", "Lorg/iochibity/InstanceId;"); */
    /* 	if (FID_SP_ID == NULL) { */
    /* 	    printf("ERROR: GetFieldID failed for '_id' of ServiceProvider\n"); */
    /* 	    return -1; */
    /* 	} */
    /* } */
    if (FID_SP_URI_PATH == NULL) {
	FID_SP_URI_PATH = (*env)->GetFieldID(env, K_SERVICE_PROVIDER,
					      "_uriPath", "Ljava/lang/String;");
	if (FID_SP_URI_PATH == NULL) {
	    printf("ERROR: GetFieldID failed for 'x' of ServiceProvider\n");
	    return -1;
	}
    }
    if (FID_SP_TYPES == NULL) {
	FID_SP_TYPES = (*env)->GetFieldID(env, K_SERVICE_PROVIDER,
					   "_types", "Ljava/util/List;");
	if (FID_SP_TYPES == NULL) {
	    printf("ERROR: GetFieldID failed for '_types' of ServiceProvider\n");
	    return -1;
	}
    }
    if (FID_SP_INTERFACES == NULL) {
	FID_SP_INTERFACES = (*env)->GetFieldID(env, K_SERVICE_PROVIDER,
						"_interfaces", "Ljava/util/List;");
	if (FID_SP_INTERFACES == NULL) {
	    printf("ERROR: GetFieldID failed for '_interfaces' of ServiceProvider\n");
	    return -1;
	}
    }
    if (FID_SP_PROPERTIES == NULL) {
	FID_SP_PROPERTIES = (*env)->GetFieldID(env, K_SERVICE_PROVIDER,
						"_properties", "Lorg/iochibity/PropertyMap;");
	if (FID_SP_PROPERTIES == NULL) {
	    printf("ERROR: GetFieldID failed for '_properties' of ServiceProvider\n");
	    return -1;
	}
    }
    if (FID_SP_CHILDREN == NULL) {
	FID_SP_CHILDREN = (*env)->GetFieldID(env, K_SERVICE_PROVIDER,
					      "_children", "Ljava/util/List;");
	if (FID_SP_CHILDREN == NULL) {
	    printf("ERROR: GetFieldID failed for '_children' of ServiceProvider\n");
	    return -1;
	}
    }
    if (FID_SP_ACTION_SET == NULL) {
	FID_SP_ACTION_SET = (*env)->GetFieldID(env, K_SERVICE_PROVIDER,
						"_actionSet", "Ljava/util/List;");
	if (FID_SP_ACTION_SET == NULL) {
	    printf("ERROR: GetFieldID failed for '_actionSet' of ServiceProvider\n");
	    return -1;
	}
    }
    if (FID_SP_POLICIES == NULL) {
	FID_SP_POLICIES = (*env)->GetFieldID(env, K_SERVICE_PROVIDER, "_policies", "I");
	if (FID_SP_POLICIES == NULL) {
	    printf("ERROR: GetFieldID failed for '_policies' of ServiceProvider\n");
	    return -1;
	}
    }

    return 0;
}

int init_Messages(JNIEnv* env)
{
    jclass klass;
    klass = (*env)->FindClass(env, "org/iochibity/Message");
    JNI_ASSERT_NULL(klass, "FindClass failed for org/iochibity/Message\n", -1);
    K_MESSAGE = (jclass)(*env)->NewGlobalRef(env, klass);
    (*env)->DeleteLocalRef(env, klass);

    /* if (MID_MSG_GET_OBSERVATION_TYPE == NULL) { */
    /* 	MID_MSG_GET_OBSERVATION_TYPE = (*env)->GetMethodID(env, K_MESSAGE, "getObservationType", "()I"); */
    /* 	if (MID_MSG_GET_OBSERVATION_TYPE == NULL) { */
    /* 	    printf("ERROR:  GetMethodID failed for 'getObservationType' for Message"); */
    /* 	    return -1; */
    /* 	} */
    /* } */
    if (FID_MSG_OBSERVATION_HANDLE == NULL) {
	FID_MSG_OBSERVATION_HANDLE = (*env)->GetFieldID(env, K_MESSAGE, "_observationRecordHandle", "J");
	if (FID_MSG_OBSERVATION_HANDLE == NULL) {
	    printf("ERROR: GetFieldID failed for '_observationRecordHandle' of Message\n");
	    return -1;
	}
    }
    if (FID_MSG_LOCAL_HANDLE == NULL) {
	FID_MSG_LOCAL_HANDLE = (*env)->GetFieldID(env, K_MESSAGE, "_localHandle", "J");
	if (FID_MSG_LOCAL_HANDLE == NULL) {
	    printf("ERROR: GetFieldID failed for '_localHandle' of Message\n");
	    return -1;
	}
    }
    if (FID_MSG_REMOTE_DEVADDR == NULL) {
	FID_MSG_REMOTE_DEVADDR = (*env)->GetFieldID(env, K_MESSAGE,
						    "_remoteDeviceAddress",
						    "Lorg/iochibity/DeviceAddress;");
	if (FID_MSG_REMOTE_DEVADDR == NULL) {
	    printf("ERROR:  GetFieldID failed for '_remoteDeviceAddress' of Message\n");
    	return OC_EH_INTERNAL_SERVER_ERROR;
	}
    }
    if (MID_MSG_GET_REMOTE_DEVADDR == NULL) {
	MID_MSG_GET_REMOTE_DEVADDR = (*env)->GetMethodID(env, K_MESSAGE,
							 "getRemoteDeviceAddress",
							 "()Lorg/iochibity/DeviceAddress;");
	if (MID_MSG_GET_REMOTE_DEVADDR == NULL) {
	    printf("ERROR:  GetMethodID failed for getRemoteDeviceAddress for Message\n");
	    return -1;
	}
    }
    if (FID_MSG_METHOD == NULL) {
	FID_MSG_METHOD = (*env)->GetFieldID(env, K_MESSAGE, "_method", "I");
	if (FID_MSG_METHOD == NULL) {
	    printf("ERROR:  GetFieldID failed for '_method' of Message\n");
    	return OC_EH_INTERNAL_SERVER_ERROR;
	}
    }
    if (MID_MSG_GET_METHOD == NULL) {
	MID_MSG_GET_METHOD = (*env)->GetMethodID(env, K_MESSAGE, "getMethod", "()I");
	if (MID_MSG_GET_METHOD == NULL) {
	    printf("ERROR:  GetMethodID failed for getMethod for Message\n");
	    return -1;
	}
    }

    /* MsgForServiceProvider */
    klass = (*env)->FindClass(env, "org/iochibity/MsgForServiceProvider");
    JNI_ASSERT_NULL(klass, "FindClass failed for org/iochibity/MsgForServiceProvider\n", -1);
    K_MSG_FOR_SERVICE_PROVIDER = (jclass)(*env)->NewGlobalRef(env, klass);
    (*env)->DeleteLocalRef(env, klass);

    if (FID_MFSP_RESOURCE_HANDLE == NULL) {
	FID_MFSP_RESOURCE_HANDLE = (*env)->GetFieldID(env,
						      K_MSG_FOR_SERVICE_PROVIDER,
						      "_resourceHandle", "J");
	if (FID_MFSP_RESOURCE_HANDLE == NULL) {
	    printf("ERROR:  GetFieldID failed for '_resourceHandle' for MsgForServiceProvider");
	    return -1;
	}
    }
    if (FID_MFSP_REMOTE_RQST_HANDLE == NULL) {
	FID_MFSP_REMOTE_RQST_HANDLE = (*env)->GetFieldID(env,
						      K_MSG_FOR_SERVICE_PROVIDER,
						      "_resourceHandle", "J");
	if (FID_MFSP_REMOTE_RQST_HANDLE == NULL) {
	    printf("ERROR:  GetFieldID failed for '_resourceHandle' for MsgForServiceProvider");
	    return -1;
	}
    }

    return 0;
}

int init_StimulusOut(JNIEnv* env)
{
    jclass klass;
    klass = (*env)->FindClass(env, "org/iochibity/StimulusOut");
    JNI_ASSERT_NULL(klass, "FindClass failed for org/iochibity/StimulusOut\n", 0);
    K_MSG_REQUEST_OUT = (jclass)(*env)->NewGlobalRef(env, klass);
    (*env)->DeleteLocalRef(env, klass);

    if (MID_RQO_CTOR == NULL) {
	MID_RQO_CTOR = (*env)->GetMethodID(env, K_MSG_REQUEST_OUT,
					   "<init>", "(Lorg/iochibity/ICoServiceProvider;)V");
	if (MID_RQO_CTOR == NULL) {
	    printf("ERROR:  GetMethodID failed for ctor for StimulusOut\n");
	    return OC_EH_INTERNAL_SERVER_ERROR;
	}
    }
    if (FID_RQO_LOCAL_HANDLE == NULL) {
	FID_RQO_LOCAL_HANDLE = (*env)->GetFieldID(env, K_MSG_REQUEST_OUT, "_localHandle", "J");
	if (FID_RQO_LOCAL_HANDLE == NULL) {
	    printf("ERROR: GetFieldID failed for '_localHandle' of StimulusOut\n");
	    return OC_EH_INTERNAL_SERVER_ERROR;
	}
    }
    if (FID_RQO_CO_SERVICE_PROVIDER == NULL) {
	FID_RQO_CO_SERVICE_PROVIDER = (*env)->GetFieldID(env, K_MSG_REQUEST_OUT,
						       "coServiceProvider",
						       "Lorg/iochibity/ICoServiceProvider;");
	if (FID_RQO_CO_SERVICE_PROVIDER == NULL) {
	    printf("ERROR: GetFieldID failed for 'coServiceProvider' of StimulusOut\n");
	    return OC_EH_INTERNAL_SERVER_ERROR;
	}
    }
    if (FID_RQO_METHOD == NULL) {
	FID_RQO_METHOD = (*env)->GetFieldID(env, K_MSG_REQUEST_OUT, "_method", "I");
	if (FID_RQO_METHOD == NULL) {
	    printf("ERROR: GetFieldID failed for '_method' of StimulusOut\n");
	    return OC_EH_INTERNAL_SERVER_ERROR;
	}
    }
    /* if (FID_RQO_URI_PATH == NULL) { */
    /* 	FID_RQO_URI_PATH = (*env)->GetFieldID(env, K_MSG_REQUEST_OUT, */
    /* 					      "_uriPath", */
    /* 					      "Ljava/lang/String;"); */
    /* 	if (FID_RQO_URI_PATH == NULL) { */
    /* 	    printf("ERROR: GetFieldID failed for '_uriPath' of StimulusOut\n"); */
    /* 	    return OC_EH_INTERNAL_SERVER_ERROR; */
    /* 	} */
    /* } */
    if (FID_RQO_DEST == NULL) {
	FID_RQO_DEST = (*env)->GetFieldID(env, K_MSG_REQUEST_OUT,
					   "_remoteDeviceAddress",
					   "Lorg/iochibity/DeviceAddress;");
	if (FID_RQO_DEST == NULL) {
	    printf("ERROR: GetFieldID failed for 'dest' of StimulusOut\n");
	    return OC_EH_INTERNAL_SERVER_ERROR;
	}
    }
    return 0;
}

int init_StimulusIn(JNIEnv* env)
{
    jclass klass;
    klass = (*env)->FindClass(env, "org/iochibity/StimulusIn");
    JNI_ASSERT_NULL(klass, "FindClass failed for org/iochibity/StimulusIn\n", 0);
    K_MSG_REQUEST_IN = (jclass)(*env)->NewGlobalRef(env, klass);
    (*env)->DeleteLocalRef(env, klass);

    if (MID_RQI_CTOR == NULL) {
	MID_RQI_CTOR = (*env)->GetMethodID(env, K_MSG_REQUEST_IN, "<init>", "()V");
	if (MID_RQI_CTOR == NULL) {
	    printf("ERROR:  GetMethodID failed for ctor for StimulusIn");
	    return OC_EH_INTERNAL_SERVER_ERROR;
	}
    }
    if (FID_RQI_LOCAL_HANDLE == NULL) {
    	FID_RQI_LOCAL_HANDLE = (*env)->GetFieldID(env, K_MSG_REQUEST_IN, "_localHandle", "J");
    	if (FID_RQI_LOCAL_HANDLE == NULL) {
    	    printf("ERROR: GetFieldID failed for '_localHandle' of StimulusIn\n");
    	    return OC_EH_INTERNAL_SERVER_ERROR;
    	}
    }
    if (FID_RQI_WATCH_ACTION == NULL) {
    	FID_RQI_WATCH_ACTION = (*env)->GetFieldID(env, K_MSG_REQUEST_IN, "watchAction", "I");
    	if (FID_RQI_WATCH_ACTION == NULL) {
    	    printf("ERROR: GetFieldID failed for 'watchAction' of StimulusIn\n");
    	    return OC_EH_INTERNAL_SERVER_ERROR;
    	}
    }
    if (FID_RQI_WATCH_ID == NULL) {
    	FID_RQI_WATCH_ID = (*env)->GetFieldID(env, K_MSG_REQUEST_IN, "watchId", "I");
    	if (FID_RQI_WATCH_ID == NULL) {
    	    printf("ERROR: GetFieldID failed for 'watchId' of StimulusIn\n");
    	    return OC_EH_INTERNAL_SERVER_ERROR;
    	}
    }

    /* method */
    if (FID_RQI_METHOD == NULL) {
	FID_RQI_METHOD = (*env)->GetFieldID(env, K_MSG_REQUEST_IN, "_method", "I");
	if (FID_RQI_METHOD == NULL) {
	    printf("ERROR:  GetFieldID failed for '_method' of StimulusIn\n");
    	return OC_EH_INTERNAL_SERVER_ERROR;
	}
    }
    /* if (MID_RQI_GET_METHOD == NULL) { */
    /* 	MID_RQI_GET_METHOD = (*env)->GetMethodID(env, K_MSG_REQUEST_IN, "getMethod", "I"); */
    /* 	if (MID_RQI_GET_METHOD == NULL) { */
    /* 	    printf("ERROR:  GetMethodID failed for 'getMethod' for StimulusIn\n"); */
    /* 	    return -1; */
    /* 	} */
    /* } */

    /* query */
    if (FID_RQI_QUERY == NULL) {
	FID_RQI_QUERY = (*env)->GetFieldID(env, K_MSG_REQUEST_IN, "_query", "Ljava/lang/String;");
	if (FID_RQI_QUERY == NULL) {
	    printf("ERROR:  GetFieldID failed for '_query' of StimulusIn\n");
	    return OC_EH_INTERNAL_SERVER_ERROR;
	}
    }
    if (MID_RQI_GET_QUERY == NULL) {
	MID_RQI_GET_QUERY = (*env)->GetMethodID(env, K_MSG_REQUEST_IN,
						"getQueryString", "()Ljava/lang/String;");
	if (MID_RQI_GET_QUERY == NULL) {
	    printf("ERROR:  GetMethodID failed for 'getQueryString' for StimulusIn\n");
	    return -1;
	}
    }


    /* remote device address - handled by Message*/


    /* msg id */
    if (FID_RQI_MSG_ID == NULL) {
	FID_RQI_MSG_ID = (*env)->GetFieldID(env, K_MSG_REQUEST_IN, "_messageId", "I");
	if (FID_RQI_MSG_ID == NULL) {
	    printf("ERROR:  GetFieldID failed for '_messageId' of StimulusIn\n");
	    return OC_EH_INTERNAL_SERVER_ERROR;
	}
    }
    /* if (MID_RQI_GET_MSG_ID == NULL) { */
    /* 	MID_RQI_GET_MSG_ID = (*env)->GetMethodID(env, K_MSG_REQUEST_IN, */
    /* 							 "getRemoteDeviceAddress", */
    /* 							 "()Lorg/iochibity/DeviceAddress;"); */
    /* 	if (MID_RQI_GET_MSG_ID == NULL) { */
    /* 	    printf("ERROR:  GetMethodID failed for getRemoteDeviceAddress for StimulusIn\n"); */
    /* 	    return -1; */
    /* 	} */
    /* } */

    return 0;
}

int init_ObservationOut(JNIEnv* env)
{
    jclass klass;
    klass = (*env)->FindClass(env, "org/iochibity/ObservationOut");
    JNI_ASSERT_NULL(klass, "FindClass failed for org/iochibity/ObservationOut\n", -1);
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
					       "_requestIn", "Lorg/iochibity/StimulusIn;");
    if (FID_MsgRspOut_RQST_IN == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for '_requestIn' on ObservationOut\n");
	return -1;
    }
    return 0;
}

int init_ObservationIn(JNIEnv* env)
{
    jclass klass;
    klass = (*env)->FindClass(env, "org/iochibity/ObservationIn");
    JNI_ASSERT_NULL(klass, "FindClass failed for org/iochibity/ObservationIn\n", -1);
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
							 "Lorg/iochibity/DeviceAddress;");
	if (FID_OBIN_REMOTE_DEVADDR == NULL) {
	    printf("ERROR: GetFieldID failed for '_remoteDeviceAddress' of ObservationIn\n");
	    return -1;
	}
    }
    /* if (MID_OBIN_GET_REMOTE_DEVADDR == NULL) { */
    /* 	MID_OBIN_GET_REMOTE_DEVADDR = (*env)->GetMethodID(env, K_OBSERVATION_IN, */
    /* 							      "getRemoteDeviceAddress", */
    /* 							      "()Lorg/iochibity/DeviceAddress;"); */
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
    /* 						    "()Lorg/iochibity/ObservationList;"); */
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
    klass = (*env)->FindClass(env, "org/iochibity/ObservationRecord");
    JNI_ASSERT_NULL(klass, "FindClass failed for org/iochibity/ObservationRecord\n", 0);
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
	MID_OBSERVATION_CTOR = (*env)->GetMethodID(env, K_OBSERVATION, "<init>", "(Lorg/iochibity/StimulusIn;)V");
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
						    "_properties", "Lorg/iochibity/PropertyMap;");
	if (FID_OBSERVATION_PROPERTIES == NULL) {
	printf("ERROR: GetFieldID failed for '_properties' on Observation\n");
	return -1;
	}
    }
    if (MID_OBSERVATION_GET_PROPERTIES == NULL) {
	MID_OBSERVATION_GET_PROPERTIES = (*env)->GetMethodID(env, K_OBSERVATION,
						       "getProperties",
						       "()Lorg/iochibity/PropertyMap;");
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
/* 	K_PFP = (*env)->FindClass(env, "org/iochibity/PayloadForPlatform"); */
/* 	if (K_PFP == NULL) { */
/* 	    printf("ERROR: FindClass failed for org/iochibity/PayloadForPlatform\n"); */
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
/* 	FID_PFP_PROPERTIES = (*env)->GetFieldID(env, K_PFP, "_properties", "Lorg/iochibity/PropertyMap;"); */
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
/* 	K_PFRS = (*env)->FindClass(env, "org/iochibity/PayloadForResourceState"); */
/* 	if (K_PFRS == NULL) { */
/* 	    printf("ERROR: FindClass failed for org/iochibity/PayloadForResourceState\n"); */
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
/* 	FID_PFRS_PROPERTIES = (*env)->GetFieldID(env, K_PFRS, "_properties", "Lorg/iochibity/PropertyMap;"); */
/* 	if (FID_PFRS_PROPERTIES == NULL) { */
/* 	    printf("ERROR: GetFieldID failed for '_properties' of PFRS"); */
/* 	    return; */
/* 	} */
/*     } */
/* } */

/*
 *
 */
int init_pmap(JNIEnv* env)
{
    /* printf("init_pmap ENTRY\n"); */
    /* iteration over property map in java: */
    /* Iterator it = mp.entrySet().iterator(); */
    /* while (it.hasNext()) { */
    /*     Map.Entry pair = (Map.Entry)it.next(); */
    /*     System.out.println(pair.getKey() + " = " + pair.getValue()); */
    /*     it.remove(); // avoids a ConcurrentModificationException */
    /* } */

    jclass klass;
    /* PropertyMap */
    if (K_PMAP == NULL) {
	klass = (*env)->FindClass(env, "org/iochibity/PropertyMap");
	JNI_ASSERT_NULL(klass, "FindClass failed for org/iochibity/PropertyMap\n", 0);
	K_PMAP = (jclass)(*env)->NewGlobalRef(env, klass);
	(*env)->DeleteLocalRef(env, klass);
    }
    if (MID_PMAP_CTOR == NULL) {
	MID_PMAP_CTOR = (*env)->GetMethodID(env, K_PMAP, "<init>", "()V");
	if (MID_PMAP_CTOR == 0) {
	    printf("ERROR: GetMethodID failed for PropertyMap ctor.\n");
	    return -1;
	}
    }
    MID_PMAP_PUT = (*env)->GetMethodID(env, K_PMAP,
				     "put",
				     "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
    if (MID_PMAP_PUT == NULL) {
	printf("ERROR: GetMethodID failed for put method of pm.\n");
	return -1;
    }
    if (MID_PMAP_SIZE == NULL) {
	MID_PMAP_SIZE = (*env)->GetMethodID(env, K_PMAP, "size", "()I");
	if (MID_PMAP_SIZE == NULL) {
	    printf("ERROR: GetMethodID failed for 'size' of PropertyMap\n");
	    return -1;
	}
    }
    if (MID_PMAP_ENTRYSET == NULL) {
	MID_PMAP_ENTRYSET = (*env)->GetMethodID(env, K_PMAP, "entrySet", "()Ljava/util/Set;");
	if (MID_PMAP_ENTRYSET == NULL) {
	    printf("ERROR: GetMethodID failed for 'entrySet' of PropertyMap\n");
	    return -1;
	}
    }
    if (MID_PMAP_GET == NULL) {
	MID_PMAP_GET = (*env)->GetMethodID(env, K_PMAP, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");
	if (MID_PMAP_GET == NULL) {
	    printf("ERROR: GetMethodID failed for 'get' of PropertyMap\n");
	    return -1;
	}
    }
     /* Map.Entry */
    if (K_MAPENTRY == NULL) {
	klass = (*env)->FindClass(env, "java/util/Map$Entry");
	JNI_ASSERT_NULL(klass, "FindClass failed for java/util/Map$Entry\n", 0);
	K_MAPENTRY = (jclass)(*env)->NewGlobalRef(env, klass);
	(*env)->DeleteLocalRef(env, klass);
    }
    if (MID_ENTRY_GETKEY == NULL) {
	MID_ENTRY_GETKEY = (*env)->GetMethodID(env, K_MAPENTRY, "getKey", "()Ljava/lang/Object;");
	if (MID_ENTRY_GETKEY == NULL) {
	    printf("ERROR: GetMethodID failed for 'getKey' of Map$Entry\n");
	    return -1;
	}
    }
    if (MID_ENTRY_GETVALUE == NULL) {
	MID_ENTRY_GETVALUE = (*env)->GetMethodID(env, K_MAPENTRY, "getValue", "()Ljava/lang/Object;");
	if (MID_ENTRY_GETVALUE == NULL) {
	    printf("ERROR: GetMethodID failed for 'getValue' of Map$Entry\n");
	    return -1;
	}
    }
    /* printf("init_pmap EXIT\n"); */
    return 0;
}

int init_observation_list(JNIEnv* env)
{
        /* klass = (*env)->FindClass(env, "org/iochibity/ObservationList"); */
    /* JNI_ASSERT_NULL(klass, "FindClass failed for org/iochibity/ObservationList\n", 0); */
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

int init_ICoServiceProvider(JNIEnv* env)
{
    jclass klass;
    klass = (*env)->FindClass(env, "org/iochibity/ICoServiceProvider");
    JNI_ASSERT_NULL(klass, "FindClass failed for org/iochibity/ICoServiceProvider\n", 0);
    K_I_CO_SERVICE_PROVIDER = (jclass)(*env)->NewGlobalRef(env, klass);
    (*env)->DeleteLocalRef(env, klass);

    jmethodID MID_ICOSP_COREACT  = NULL;
    if (MID_ICOSP_COREACT == NULL) {
	MID_ICOSP_COREACT = (*env)->GetMethodID(env, K_I_CO_SERVICE_PROVIDER,
						"coReact",
						"()I");
	if (MID_ICOSP_COREACT == NULL) {
	    printf("ERROR: GetMethodID failed for 'coReact' of ICoServiceProvider\n");
	    return OC_EH_INTERNAL_SERVER_ERROR;
	}
    }
    return 0;
}

int init_CoServiceProvider(JNIEnv* env)
{
    jclass klass;
    klass = (*env)->FindClass(env, "org/iochibity/CoServiceProvider");
    JNI_ASSERT_NULL(klass, "FindClass failed for org/iochibity/CoServiceProvider\n", 0);
    K_CO_SERVICE_PROVIDER = (jclass)(*env)->NewGlobalRef(env, klass);
    (*env)->DeleteLocalRef(env, klass);

    /* if (FID_COSP_HANDLE == NULL) { */
    /* 	FID_COSP_HANDLE = (*env)->GetFieldID(env, K_CO_SERVICE_PROVIDER, "_handle", "J"); */
    /* 	if (FID_COSP_HANDLE == NULL) { */
    /* 	    printf("ERROR: GetFieldID failed for '_handle' of CoServiceProvider"); */
    /* 	    return OC_EH_INTERNAL_SERVER_ERROR; */
    /* 	} */
    /* } */
    /* obs handle moved to thread-local in CoServiceProvider.c */
    /* if (FID_COSP_MSG_RESPONSE_IN == NULL) { */
    /* 	FID_COSP_MSG_RESPONSE_IN = (*env)->GetFieldID(env, */
    /* 							 K_CO_SERVICE_PROVIDER, */
    /* 							 "_observationHandle", "J"); */
    /* 	if (FID_COSP_MSG_RESPONSE_IN == NULL) { */
    /* 	    printf("ERROR: GetFieldID failed for '_observationHandle' of CoServiceProvider"); */
    /* 	    return OC_EH_INTERNAL_SERVER_ERROR; */
    /* 	} */
    /* } */
    /* if (FID_COSP_URI_PATH == NULL) { */
    /* 	FID_COSP_URI_PATH = (*env)->GetFieldID(env, K_CO_SERVICE_PROVIDER, */
    /* 						"_uriPath", "Ljava/lang/String;"); */
    /* 	if (FID_COSP_URI_PATH == NULL) { */
    /* 	    printf("ERROR: GetFieldID failed for '_uriPath' of CoServiceProvider"); */
    /* 	    return OC_EH_INTERNAL_SERVER_ERROR; */
    /* 	} */
    /* } */
    /* if (FID_COSP_METHOD == NULL) { */
    /* 	FID_COSP_METHOD = (*env)->GetFieldID(env, K_CO_SERVICE_PROVIDER, "_method", "I"); */
    /* 	if (FID_COSP_METHOD == NULL) { */
    /* 	    printf("ERROR: GetFieldID failed for '_method' of CoServiceProvider"); */
    /* 	    return OC_EH_INTERNAL_SERVER_ERROR; */
    /* 	} */
    /* } */
    /* if (FID_COSP_DESTINATION == NULL) { */
    /* 	FID_COSP_DESTINATION = (*env)->GetFieldID(env, K_CO_SERVICE_PROVIDER, */
    /* 						   "_destination", "Lorg/iochibity/DeviceAddress;"); */
    /* 	if (FID_COSP_DESTINATION == NULL) { */
    /* 	    printf("ERROR: GetFieldID failed for '_destination' of CoServiceProvider"); */
    /* 	    return OC_EH_INTERNAL_SERVER_ERROR; */
    /* 	} */
    /* } */
    jmethodID MID_COSP_EXHIBIT  = NULL;
    if (MID_COSP_EXHIBIT == NULL) {
	MID_COSP_EXHIBIT = (*env)->GetMethodID(env, K_I_CO_SERVICE_PROVIDER,
							 "coExhibit", "()V");
	if (MID_COSP_EXHIBIT == NULL) {
	    printf("ERROR: GetMethodID failed for 'coExhibit' of CoServiceProvider\n");
	    return OC_EH_INTERNAL_SERVER_ERROR;
	}
    }

    return 0;
}

// FIXME: this doesn't belong here
/*
 * Routine:   props_to_OCRepPayloadValue
 * Signature: (Lorg/iochibity/PropertyMap;)J
 */
/* OCRepPayloadValue* props_to_OCRepPayloadValue(JNIEnv* env, jobject j_propmap) */
/* { */
/*     printf("props_to_OCRepPayloadValue ENTRY\n"); */
/*     OCRepPayloadValue* plv_head = NULL; */

/*     /\* test *\/ */
/*     /\* plv_head = (OCRepPayloadValue*)OICCalloc(1, sizeof(OCRepPayloadValue)); *\/ */
/*     /\* if (!plv_head) { *\/ */
/*     /\* 	printf("ERROR: IOCCalloc failed for head OCRepPayloadValue"); *\/ */
/*     /\* 	return NULL; *\/ */
/*     /\* } *\/ */
/*     /\* plv_head->name = "HeadPropNode"; *\/ */
/*     /\* if (!plv_head->name) *\/ */
/*     /\*     { *\/ */
/*     /\*         OICFree(plv_head); *\/ */
/*     /\* 	    printf("ERROR: Name assign failed for OCRepPayloadValue"); *\/ */
/*     /\*         return NULL; *\/ */
/*     /\*     } *\/ */

/*     /\* plv_head->type = OCREP_PROP_INT; *\/ */
/*     /\* plv_head->i    = 72; *\/ */

/*     /\* OCRepPayloadValue* plv2 = (OCRepPayloadValue*)OICCalloc(1, sizeof(OCRepPayloadValue)); *\/ */
/*     /\* if (!plv2) { *\/ */
/*     /\* 	printf("ERROR: IOCCalloc failed for plv2 OCRepPayloadValue"); *\/ */
/*     /\* 	return NULL; *\/ */
/*     /\* } *\/ */
/*     /\* plv2->name = "Node2"; *\/ */
/*     /\* if (!plv2->name) *\/ */
/*     /\*     { *\/ */
/*     /\*         OICFree(plv2); *\/ */
/*     /\* 	    printf("ERROR: Name assign failed for OCRepPayloadValue"); *\/ */
/*     /\*         return NULL; *\/ */
/*     /\*     } *\/ */
/*     /\* plv2->type = OCREP_PROP_INT; *\/ */
/*     /\* plv2->i    = 73; *\/ */
/*     /\* plv_head->next = plv2; *\/ */

/*     /\* iterate over j_propmap *\/ */
/*     /\* Set eset = j_propmap.entrySet() *\/ */
/*     /\* Iterator it = eset.iterator(); *\/ */
/*     /\* while (it.hasNext()) { *\/ */
/*     /\*     Map.Entry pair = (Map.Entry)it.next(); *\/ */
/*     /\*     System.out.println(pair.getKey() + " = " + pair.getValue()); *\/ */
/*     /\*     it.remove(); // avoids a ConcurrentModificationException *\/ */

/*     jobject j_set = (*env)->CallObjectMethod(env, j_propmap, MID_PMAP_ENTRYSET); */
/*     if (j_set == NULL) { */
/* 	THROW_JNI_EXCEPTION("CallObjectMethod failed for MID_PMAP_ENTRYSET on j_propmap\n"); */
/* 	return NULL; */
/*     } */
/*     jobject j_iter = (*env)->CallObjectMethod(env, j_set, MID_SET_ITERATOR); */
/*     if (j_iter == NULL) { */
/* 	printf("ERROR: CallObjectMethod failed for MID_SET_ITERATOR on j_set\n"); */
/* 	return NULL; */
/*     } */

/*     jobject j_entry; */
/*     jobject j_key; */
/*     jobject j_val; */
/*     OCRepPayloadValue** plv = &plv_head; */
/*     bool has_next = (bool)(*env)->CallObjectMethod(env, j_iter, MID_ITER_HASNEXT); */
/*     printf("ITERATING %d\n", has_next); */
/*     while ( has_next ) { */
/* 	char*   c_key; */
/* 	j_entry = (*env)->CallObjectMethod(env, j_iter, MID_ITER_NEXT); */
/* 	if (j_entry == NULL) { */
/* 	    printf("GOT NULL ENTRY, head: %s\n", plv_head->name); */
/* 	    /\* printf("CallObjectMethod failed for MID_ITER_NEXT on j_iter\n"); *\/ */
/* 	    /\* return plv_head; *\/ */
/* 	    break; */
/* 	} */
/* 	j_key = (*env)->CallObjectMethod(env, j_entry, MID_ENTRY_GETKEY); */
/* 	if (j_key == NULL) { */
/* 	    printf("CallObjectMethod failed for MID_ENTRY_GETKEY on j_entry\n"); */
/* 	    return NULL; */
/* 	} */
/* 	c_key =  (char*) (*env)->GetStringUTFChars(env, j_key, NULL); */

/* 	j_val = (*env)->CallObjectMethod(env, j_entry, MID_ENTRY_GETVALUE); */
/* 	if (j_val == NULL) { */
/* 	    THROW_JNI_EXCEPTION("CallObjectMethod failed for MID_ENTRY_GETVAL on j_entry\n"); */
/* 	    return NULL; */
/* 	} */

/* 	if ( (*env)->IsInstanceOf(env, j_val, K_INTEGER) ) { */
/* 	    int c_val = (int)(*env)->CallIntMethod(env, j_val, MID_INT_INTVALUE); */
/* 	    *plv = (OCRepPayloadValue*)OICCalloc(1, sizeof(OCRepPayloadValue)); */
/* 	    if (!*plv) { */
/* 	    	THROW_JNI_EXCEPTION("IOCCalloc failed for *plv OCRepPayloadValue"); */
/* 	    	return NULL; */
/* 	    } */
/* 	    (*plv)->name = OICStrdup(c_key); */
/* 	    (*plv)->type = OCREP_PROP_INT; */
/* 	    (*plv)->i = (int64_t)c_val; */
/* 	    printf("GOT ENTRY: %s = %lld (INT)\n", (*plv)->name, (int64_t)(*plv)->i); */
/* 	    /\* (*plv)->next = *plv; *\/ */
/* 	    (*plv)->next = NULL; */
/* 	    plv = &((*plv)->next); */
/* 	} else if ( (*env)->IsInstanceOf(env, j_val, K_DOUBLE) ) { */
/* 	    double c_val = (double)(*env)->CallDoubleMethod(env, j_val, MID_DBL_DBLVALUE); */
/* 	    *plv = (OCRepPayloadValue*)OICCalloc(1, sizeof(OCRepPayloadValue)); */
/* 	    if (!*plv) { */
/* 	    	THROW_JNI_EXCEPTION("IOCCalloc failed for *plv OCRepPayloadValue"); */
/* 	    	return NULL; */
/* 	    } */
/* 	    (*plv)->name = OICStrdup(c_key); */
/* 	    (*plv)->type = OCREP_PROP_DOUBLE; */
/* 	    (*plv)->d = (double)c_val; */
/* 	    printf("GOT ENTRY: %s = %f (DOUBLE)\n", (*plv)->name, (double)(*plv)->d); */
/* 	    (*plv)->next = NULL; */
/* 	    plv = &((*plv)->next); */
/* 	} else if ( (*env)->IsInstanceOf(env, j_val, K_BOOLEAN) ) { */
/* 	    bool c_val = (bool)(*env)->CallBooleanMethod(env, j_val, MID_BOOL_BOOLVALUE); */
/* 	    *plv = (OCRepPayloadValue*)OICCalloc(1, sizeof(OCRepPayloadValue)); */
/* 	    if (!*plv) { */
/* 	    	THROW_JNI_EXCEPTION("IOCCalloc failed for *plv OCRepPayloadValue"); */
/* 	    	return NULL; */
/* 	    } */
/* 	    (*plv)->name = OICStrdup(c_key); */
/* 	    (*plv)->type = OCREP_PROP_BOOL; */
/* 	    (*plv)->b = (bool)c_val; */
/* 	    printf("GOT ENTRY: %s = %d (BOOL)\n", (*plv)->name, (bool)(*plv)->b); */
/* 	    (*plv)->next = NULL; */
/* 	    plv = &((*plv)->next); */
/* 	} else if ( (*env)->IsInstanceOf(env, j_val, K_STRING) ) { */
/* 	    char* c_val = (char*) (*env)->GetStringUTFChars(env, j_val, NULL); */
/* 	    *plv = (OCRepPayloadValue*)OICCalloc(1, sizeof(OCRepPayloadValue)); */
/* 	    if (!*plv) { */
/* 	    	THROW_JNI_EXCEPTION("IOCCalloc failed for *plv OCRepPayloadValue"); */
/* 	    	return NULL; */
/* 	    } */
/* 	    (*plv)->name = OICStrdup(c_key); */
/* 	    (*plv)->type = OCREP_PROP_STRING; */
/* 	    (*plv)->str = (char*)OICStrdup(c_val); */
/* 	    (*env)->ReleaseStringUTFChars(env, j_val, c_val); */
/* 	    printf("GOT ENTRY: %s = %s (STRING)\n", (*plv)->name, (char*)(*plv)->str); */
/* 	    (*plv)->next = NULL; */
/* 	    plv = &((*plv)->next); */
/* 	} else { */
/* 	    printf("GOT ENTRY: %s of type ?\n", c_key); */
/* 	} */
/* 	/\* (*env)->CallObjectMethod(env, j_iter, MID_ITER_REMOVE); *\/ */
/* 	has_next = (bool)(*env)->CallObjectMethod(env, j_iter, MID_ITER_HASNEXT); */
/*     } */

/*     printf("props_to_OCRepPayloadValue EXIT returning %ld\n", (long)plv_head); */
/*     return plv_head; */
/* } */

// FIXME: this doesn't belong here
/*
 * Routine:   pfrs_to_OCRepPayloadValue
 * Signature: (Lorg/iochibity/PayloadForResourceState;)J
 */
/* OCRepPayload* pfrs_to_OCRepPayload(JNIEnv* env, jobject j_pfrs) */
/* { */
/*     OCRepPayload* c_payload = (OCRepPayload*)(intptr_t) */
/* 	(*env)->GetLongField(env, j_pfrs, FID_OBSERVATION_HANDLE); */
/*     /\* int j_payload_type = (*env)->GetIntField(env, j_pfrs, FID_PAYLOAD_TYPE); *\/ */

/*     if (c_payload->base.type !=  PAYLOAD_TYPE_REPRESENTATION) { */
/* 	printf("ERROR: mismatch between java and c\n"); */
/*     } */

/*     /\* populate the struct: uri, types, interfaces, properties *\/ */

/*     jstring j_uri  = (*env)->GetObjectField(env, j_pfrs, FID_PFRS_URI); */
/*     if (j_uri == NULL) { */
/* 	THROW_JNI_EXCEPTION("GetObjectField failed for FID_PFRS_URI on j_pfrs\n"); */
/* 	return NULL; */
/*     } */
/*     c_payload->uri = (char*) (*env)->GetStringUTFChars(env, j_uri, NULL); */
/*     if (c_payload->uri == NULL) { */
/* 	THROW_JNI_EXCEPTION("GetStringUTFChars failed for j_uri\n"); */
/* 	return NULL; */
/*     } */

/*     /\* properties ('values'): OCRepPayloadValue* values; *\/ */
/*     jobject j_properties  = (*env)->GetObjectField(env, j_pfrs, FID_PFRS_PROPERTIES); */
/*     if (j_properties == NULL) { */
/* 	THROW_JNI_EXCEPTION("GetObjectField failed for FID_PFRS_PROPERTIES on j_pfrs\n"); */
/* 	return NULL; */
/*     } */
/*     int sz = 0; */
/*     sz = (*env)->CallIntMethod(env, j_properties, MID_PMAP_SIZE); */
/*     printf("PropertyMap SIZE: %d\n", sz); */

/*     if (sz > 0) { */
/* 	OCRepPayloadValue* properties = props_to_OCRepPayloadValue(env, j_properties); */
/* 	c_payload->values = properties; */
/*     } */

/*     return c_payload; */
/* } */

/**
 * @brief called when native lib is loaded, e.g. by System.loadlibrary("mylib")
 */
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
    OC_UNUSED(reserved);
    printf("JNI_OnLoad\n");
    JNIEnv* env = NULL;
    g_JVM = vm;

#ifdef TB_LOG
    OCSetLogLevel(DEBUG, false);
#endif

    /* LOG_EMERG, LOG_ALERT, LOG_CRIT, LOG_WARNING, LOG_NOTICE, LOG_INFO, LOG_DEBUG */
    /* coap_set_log_level(LOG_WARNING); */

    /* dtls_set_log_level(DTLS_LOG_WARN); /\* DEBUG, INFO,  NOTICE, WARN, CRIT ... *\/ */

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

    /* DeviceAddress = OCDevAddr */
    if (K_DEVICE_ADDRESS == NULL) {
	klass = (*env)->FindClass(env, "org/iochibity/DeviceAddress");
	JNI_ASSERT_NULL(klass, "FindClass failed for org/iochibity/DeviceAddress\n", 0);
	K_DEVICE_ADDRESS = (jclass)(*env)->NewGlobalRef(env, klass);
	(*env)->DeleteLocalRef(env, klass);
    }
    MID_DA_CTOR = (*env)->GetMethodID(env, K_DEVICE_ADDRESS, "<init>", "()V");
    if (MID_DA_CTOR == 0) {
	printf("ERROR: GetMethodID failed for ctor of DeviceAddress.\n");
	return -1;
    }

    // networkProtocol -> OCDevAddr.adapter
    /* FID_DA_NETWORK_PROTOCOL = (*env)->GetFieldID(env, K_DEVICE_ADDRESS, "networkProtocol", "I"); */
    /* if (FID_DA_NETWORK_PROTOCOL == NULL) { */
    /* 	printf("ERROR:  GetFieldID failed for networkProtocol for DeviceAddress\n"); */
    /* 	fflush(NULL); */
    /* 	return OC_EH_INTERNAL_SERVER_ERROR; */
    /* } */
    /* FID_DA_NETWORK_FLAGS = (*env)->GetFieldID(env, K_DEVICE_ADDRESS, "networkFlags", "I"); */
    /* if (FID_DA_NETWORK_FLAGS == NULL) { */
    /* 	printf("ERROR:  GetFieldID failed for networkFlags for DeviceAddress\n"); */
    /* 	fflush(NULL); */
    /* 	return OC_EH_INTERNAL_SERVER_ERROR; */
    /* } */
    /* FID_DA_NETWORK_POLICIES= (*env)->GetFieldID(env, K_DEVICE_ADDRESS, "networkPolicies", "B"); */
    /* if (FID_DA_NETWORK_POLICIES == NULL) { */
    /* 	printf("ERROR:  GetFieldID failed for 'networkPolicy' of DeviceAddress\n"); */
    /* 	fflush(NULL); */
    /* 	return OC_EH_INTERNAL_SERVER_ERROR; */
    /* } */
    /* FID_DA_NETWORK_SCOPE= (*env)->GetFieldID(env, K_DEVICE_ADDRESS, "networkScope", "B"); */
    /* if (FID_DA_NETWORK_SCOPE == NULL) { */
    /* 	printf("ERROR:  GetFieldID failed for 'networkPolicy' of DeviceAddress\n"); */
    /* 	fflush(NULL); */
    /* 	return OC_EH_INTERNAL_SERVER_ERROR; */
    /* } */
    /* FID_DA_TRANSPORT_SECURITY= (*env)->GetFieldID(env, K_DEVICE_ADDRESS, "transportSecurity", "Z"); */
    /* if (FID_DA_TRANSPORT_SECURITY == NULL) { */
    /* 	printf("ERROR:  GetFieldID failed for 'transportSecurity' of DeviceAddress\n"); */
    /* 	fflush(NULL); */
    /* 	return OC_EH_INTERNAL_SERVER_ERROR; */
    /* } */
    /* FID_DA_PORT = (*env)->GetFieldID(env, K_DEVICE_ADDRESS, "port", "I"); */
    /* if (FID_DA_PORT == NULL) { */
    /* 	printf("ERROR:  GetFieldID failed for port for DeviceAddress\n"); */
    /* 	fflush(NULL); */
    /* 	return OC_EH_INTERNAL_SERVER_ERROR; */
    /* } */
    /* FID_DA_ADDRESS = (*env)->GetFieldID(env, K_DEVICE_ADDRESS, "address", "Ljava/lang/String;"); */
    /* if (FID_DA_ADDRESS == NULL) { */
    /* 	printf("ERROR:  GetFieldID failed for address of DeviceAddress\n"); */
    /* 	fflush(NULL); */
    /* 	return OC_EH_INTERNAL_SERVER_ERROR; */
    /* } */
    /* FID_DA_IFINDEX = (*env)->GetFieldID(env, K_DEVICE_ADDRESS, "ifindex", "I"); */
    /* if (FID_DA_IFINDEX == NULL) { */
    /* 	printf("ERROR:  GetFieldID failed for ifindex DeviceAddress\n"); */
    /* 	fflush(NULL); */
    /* 	return OC_EH_INTERNAL_SERVER_ERROR; */
    /* } */
    /* FIXME */
    /* /\* OCDevAddr.routeData *\/ */
    /* if (crequest_in->devAddr.routeData) { */
    /* FID_DA_ROUTE_DATA = (*env)->GetFieldID(env, K_DEVICE_ADDRESS, "routeData", "Ljava/lang/String;"); */
    /* if (FID_DA_ROUTE_DATA == NULL) { */
    /* 	    printf("ERROR: GetFieldID failed routeData of DeviceAddress\n"); */
    /* } */

    /* klass = (*env)->FindClass(env, "org/iochibity/ResourceLocal"); */
    /* JNI_ASSERT_NULL(klass, "FindClass failed for org/iochibity/ResourceLocal\n", 0); */
    /* K_RESOURCE_LOCAL = (jclass)(*env)->NewGlobalRef(env, klass); */
    /* (*env)->DeleteLocalRef(env, klass); */

    /* if (init_Messages(env) != 0) return OC_EH_INTERNAL_SERVER_ERROR; */

    init_ServiceProviders(env);

    /* init_StimulusOut(env); */

    /* init_StimulusIn(env); */

    /* init_ObservationIn(env); */

    init_java(env);

    /* if (init_observation(env) != 0) return OC_EH_INTERNAL_SERVER_ERROR; */

    /* init_pfp(env); */

    /* init_pfrs(env); */

    init_pmap(env);		/* prep mid's etc. for iterating over pmap */

    init_ICoServiceProvider(env);
    init_CoServiceProvider(env);

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
