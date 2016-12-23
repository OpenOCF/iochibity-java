#include <jni.h>

#include "octypes.h"

/* #include <ctype.h> */
/* #include <pthread.h> */
/* #include <string.h> */
/* #include <stdlib.h> */
/* #include <stdbool.h> */
/* #include <unistd.h> */

/* #include "org_iochibity_OCF.h" */
/* #include "ocf_exceptions.h" */
/* #include "jni_utils.h" */

/* #include "ocresource.h" */
/* #include "ocstack.h" */
/* #include "oic_malloc.h" */
/* #include "oic_string.h" */

#define JNI_ASSERT_NULL(arg, msg, ...)		\
    if (NULL == (arg)) \
    { \
        THROW_JNI_EXCEPTION(msg); \
        return __VA_ARGS__; \
    } \

extern JavaVM* g_JVM;

extern jclass    K_INTEGER;		/* 32-bit ints */
extern jmethodID MID_INT_CTOR;
extern jmethodID MID_INT_INTVALUE;	/* Integer.intValue() returns int */
extern jclass K_LONG;			/* 64-bit ints */
extern jmethodID MID_LONG_CTOR;
extern jmethodID MID_LONG_LONGVALUE;	/* Long.longValue() returns long */
/* jclass K_BIGINTEGER		/\* arbitrary bits *\/ */
extern jclass K_BOOLEAN;
extern jmethodID MID_BOOL_CTOR;
extern jmethodID MID_BOOL_BOOLVALUE;	/* Boolean.booleanValue() returns boolean */
extern jclass K_DOUBLE;
extern jmethodID MID_DBL_CTOR;
extern jmethodID MID_DBL_DBLVALUE;	/* Double.doubleValue() returns double */
extern jclass K_SHORT;
extern jmethodID MID_SHORT_CTOR;
extern jmethodID MID_SHORT_SHORTVALUE;
extern jclass K_STRING;
extern jmethodID MID_STR_CTOR;
extern jclass K_LIST;		   /* OCByteString => List<Byte> */
extern jclass K_BYTE;
extern jmethodID MID_BYTE_CTOR;
extern jclass K_OBJECT;
/* jclass K_ARRAY; - List */

extern jclass    K_LINKED_LIST;
extern jmethodID MID_LL_CTOR;
extern jmethodID MID_LL_ADD;
extern jmethodID MID_LL_GET;

extern jclass    K_OBSERVATION;
extern jfieldID  FID_OBSERVATION_HANDLE;
extern jfieldID  FID_OBSERVATION_TYPE;
extern jmethodID MID_OBSERVATION_CTOR;
extern jfieldID  FID_OBSERVATION_URI_PATH;
extern jmethodID MID_OBSERVATION_GET_URI_PATH;
extern jfieldID  FID_OBSERVATION_RTYPES;
extern jmethodID MID_OBSERVATION_GET_RTYPES;
extern jfieldID  FID_OBSERVATION_IFS;
extern jmethodID MID_OBSERVATION_GET_IFS;
extern jfieldID  FID_OBSERVATION_PROPERTIES;
extern jmethodID MID_OBSERVATION_GET_PROPERTIES;
extern jfieldID  FID_OBSERVATION_CHILDREN;
extern jmethodID MID_OBSERVATION_GET_CHILDREN;

extern jclass    K_OBSERVATION_LIST;
extern jmethodID MID_PLL_CTOR;
extern jmethodID MID_PLL_ADD;

extern jclass   K_PFRS; 			/* class for PayloadForResourceState */
extern jfieldID FID_PFRS_URI;
extern jfieldID FID_PFRS_RTYPES;
extern jfieldID FID_PFRS_INTERFACES;
extern jfieldID FID_PFRS_PROPERTIES;

extern jclass    K_PFP;   			/* class for PayloadForPlatform */
extern jfieldID  FID_PFP_URI;
extern jfieldID  FID_PFP_RTYPES;
extern jfieldID  FID_PFP_INTERFACES;
extern jfieldID  FID_PFP_PROPERTIES;
extern jmethodID MID_PFP_CTOR;

extern jclass   K_PMAP; 			/* class for PropertyMap */
extern jmethodID MID_PMAP_CTOR;
extern jmethodID MID_PMAP_SIZE;
extern jmethodID MID_PMAP_ENTRYSET;
extern jmethodID MID_PMAP_GET;
extern jmethodID MID_PMAP_PUT;

extern jclass   K_SET; 			/* interface java.util.Set */
extern jmethodID MID_SET_ITERATOR;

extern jclass   K_ITER; 			/* interface java.util.Iterator */
extern jmethodID MID_ITER_HASNEXT;
extern jmethodID MID_ITER_NEXT;
extern jmethodID MID_ITER_REMOVE;

extern jclass    K_MAPENTRY; 			/* interface java.util.Map.Entry */
extern jmethodID MID_ENTRY_GETKEY;
extern jmethodID MID_ENTRY_GETVALUE;

extern jclass    K_DEVICE_ADDRESS;
extern jmethodID MID_DA_CTOR;
extern jfieldID  FID_DA_NETWORK_PROTOCOL;
extern jfieldID  FID_DA_NETWORK_POLICIES;
extern jfieldID  FID_DA_NETWORK_SCOPE;
extern jfieldID  FID_DA_TRANSPORT_SECURITY;
extern jfieldID  FID_DA_PORT;
extern jfieldID  FID_DA_ADDRESS;
extern jfieldID  FID_DA_IFINDEX;
extern jfieldID  FID_DA_ROUTE_DATA;

extern jclass    K_ISERVICE_PROVIDER;
/* extern jmethodID MID_ISP_CTOR; */
extern jmethodID MID_ISP_OBSERVE_STIMULUS;

extern jclass    K_SERVICE_PROVIDER;
/* extern jmethodID MID_ISP_CTOR; */
extern jfieldID FID_SP_HANDLE;
extern jfieldID FID_SP_ID;
extern jfieldID FID_SP_URI_PATH;
extern jfieldID FID_SP_TYPES;
extern jfieldID FID_SP_INTERFACES;
extern jfieldID FID_SP_PROPERTIES;
extern jfieldID FID_SP_CHILDREN;
extern jfieldID FID_SP_ACTION_SET;
extern jfieldID FID_SP_POLICIES;

extern jclass    K_RESOURCE_LOCAL;

extern jclass   K_MESSAGE;
extern jfieldID  FID_MSG_LOCAL_HANDLE;
extern jfieldID  FID_MSG_OBSERVATION_HANDLE;
/* extern jmethodID MID_MSG_GET_OBSERVATION_TYPE; */
extern jfieldID  FID_MSG_METHOD;
extern jmethodID MID_MSG_GET_METHOD;
extern jfieldID  FID_MSG_OPTIONS;
extern jmethodID MID_MSG_GET_OPTIONS;
/* extern jfieldID  FID_MSG_PTR_OPTIONS; */
extern jfieldID  FID_MSG_REMOTE_DEVADDR;
extern jmethodID MID_MSG_GET_REMOTE_DEVADDR;

extern jclass    K_MSG_FOR_SERVICE_PROVIDER;
extern jfieldID  FID_MFSP_REMOTE_RQST_HANDLE;
extern jfieldID  FID_MFSP_RESOURCE_HANDLE;

extern jclass    K_MSG_REQUEST_IN;

extern jclass    K_MSG_REQUEST_OUT;
extern jmethodID MID_RQO_CTOR;
extern jfieldID  FID_RQO_LOCAL_HANDLE;
extern jfieldID  FID_RQO_CO_SERVICE_PROVIDER;
extern jfieldID  FID_RQO_METHOD;
extern jfieldID  FID_RQO_URI_PATH;
extern jfieldID  FID_RQO_DEST;

extern jclass    K_MSG_REQUEST_IN;
extern jmethodID MID_RQI_CTOR;
extern jfieldID  FID_RQI_LOCAL_HANDLE;
extern jfieldID  FID_RQI_CO_SERVICE_PROVIDER;
extern jfieldID  FID_RQI_WATCH_ACTION;
extern jfieldID  FID_RQI_WATCH_ID;
extern jfieldID  FID_RQI_METHOD;
extern jmethodID MID_RQI_GET_METHOD;
extern jfieldID  FID_RQI_QUERY;
extern jmethodID MID_RQI_GET_QUERY;
extern jfieldID  FID_RQI_MSG_ID;
extern jmethodID MID_RQI_GET_MSG_ID;

extern jclass    K_OBSERVATION_IN;
extern jmethodID MID_OBIN_CTOR;
extern jfieldID  FID_OBIN_OBSERVATION_HANDLE;
extern jfieldID  FID_OBIN_REMOTE_DEVADDR;
extern jmethodID MID_OBIN_GET_REMOTE_DEVADDR;
extern jfieldID  FID_OBIN_CONN_TYPE;
extern jfieldID  FID_OBIN_REMOTE_SID;
extern jfieldID  FID_OBIN_RESULT;
extern jfieldID  FID_OBIN_SERIAL;
extern jfieldID  FID_OBIN_URI;
extern jfieldID  FID_OBIN_PAYLOAD;
extern jmethodID MID_OBIN_GET_OPTIONS;
extern jmethodID MID_OBIN_GET_OBSERVATION;

extern jclass    K_MSG_RESPONSE_OUT;
extern jmethodID MID_MsgRspOut_CTOR;
extern jfieldID  FID_MsgRspOut_RQST_IN;

extern jclass    K_I_CO_SERVICE_PROVIDER;
extern jmethodID MID_ICOSP_OBSERVE_BEHAVIOR;

extern jclass    K_A_CO_SERVICE_PROVIDER;
extern jfieldID  FID_COSP_HANDLE;
extern jfieldID  FID_COSP_METHOD;
extern jfieldID  FID_COSP_URI_PATH;
extern jfieldID  FID_COSP_DESTINATION;
extern jmethodID MID_COSP_EXHIBIT_STIMULUS;

extern FILE* server_fopen(const char *path, const char *mode);

/*
 *
 */
/* extern int prep_java(JNIEnv* env); */

/*
*
*/
extern void prep_pfrs(JNIEnv* env);

/*
 *
 */
extern void prep_pmap(JNIEnv* env);

/*
 * Routine:   props_to_OCRepPayloadValue
 * Signature: (Lorg/iochibity/PropertyMap;)J
 */
extern OCRepPayloadValue* props_to_OCRepPayloadValue(JNIEnv* env, jobject j_propmap);

/*
 * Routine:   pfrs_to_OCRepPayloadValue
 * Signature: (Lorg/iochibity/PayloadForResourceState;)J
 */
extern OCRepPayload* pfrs_to_OCRepPayload(JNIEnv* env, jobject j_pfrs);


extern OCEntityHandlerResult c_ServiceProvider_observe_stimulus(OCEntityHandlerFlag flag,
						OCEntityHandlerRequest * crequest_in, /* StimulusIn */
						void* callbackParam);
