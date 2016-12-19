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

extern jclass    K_PAYLOAD;
extern jfieldID  FID_PAYLOAD_HANDLE;
extern jfieldID  FID_PAYLOAD_TYPE;
extern jmethodID MID_PAYLOAD_CTOR;
extern jfieldID  FID_PAYLOAD_URI_PATH;
extern jmethodID MID_PAYLOAD_GET_URI_PATH;
extern jfieldID  FID_PAYLOAD_RTYPES;
extern jmethodID MID_PAYLOAD_GET_RTYPES;
extern jfieldID  FID_PAYLOAD_IFS;
extern jmethodID MID_PAYLOAD_GET_IFS;
extern jfieldID  FID_PAYLOAD_PROPERTIES;
extern jmethodID MID_PAYLOAD_GET_PROPERTIES;
extern jfieldID  FID_PAYLOAD_CHILDREN;
extern jmethodID MID_PAYLOAD_GET_CHILDREN;

extern jclass    K_PAYLOAD_LIST;
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
extern jfieldID  FID_DA_ADAPTER;
extern jfieldID  FID_DA_FLAGS;
extern jfieldID  FID_DA_PORT;
extern jfieldID  FID_DA_ADDRESS;
extern jfieldID  FID_DA_IFINDEX;
extern jfieldID  FID_DA_ROUTE_DATA;

extern jclass    K_ISERVICE_PROVIDER;
/* extern jmethodID MID_ISP_CTOR; */
extern jmethodID MID_ISP_SERVICE_REQUEST_IN;

extern jclass    K_ASERVICE_PROVIDER;
/* extern jmethodID MID_ISP_CTOR; */
extern jfieldID FID_ASP_HANDLE;
extern jfieldID FID_ASP_ID;
extern jfieldID FID_ASP_URI_PATH;
extern jfieldID FID_ASP_TYPES;
extern jfieldID FID_ASP_INTERFACES;
extern jfieldID FID_ASP_PROPERTIES;
extern jfieldID FID_ASP_CHILDREN;
extern jfieldID FID_ASP_ACTION_SET;
extern jfieldID FID_ASP_POLICIES;

extern jclass    K_RESOURCE_LOCAL;

extern jclass   K_MESSAGE;
extern jfieldID  FID_MSG_LOCAL_HANDLE;
extern jfieldID  FID_MSG_PAYLOAD_HANDLE;
extern jmethodID MID_MSG_GET_PAYLOAD_TYPE;
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

extern jclass   K_MSG_REQUEST_OUT;
extern jfieldID FID_RQO_SERVICE_REQUESTOR;
extern jfieldID FID_RQO_URI;
extern jfieldID FID_RQO_DEST;

extern jclass    K_IRESOURCE_SERVICE_REQUESTOR;
extern jmethodID MID_IRSR_SERVICE_RESPONSE_IN;

extern jclass    K_MSG_REQUEST_IN;
extern jmethodID MID_RQI_CTOR;
extern jfieldID  FID_RQI_SERVICE_REQUESTOR;
extern jfieldID  FID_RQI_IS_WATCH;
/* extern jfieldID  FID_RQI_LOCAL_HANDLE; */
/* extern jfieldID  FID_RQI_REMOTE_RQST_HANDLE; */
/* extern jfieldID  FID_RQI_RESOURCE_HANDLE; */
extern jfieldID  FID_RQI_METHOD;
extern jmethodID MID_RQI_GET_METHOD;
extern jfieldID  FID_RQI_QUERY;
extern jmethodID MID_RQI_GET_QUERY;
extern jfieldID  FID_RQI_MSG_ID;
extern jmethodID MID_RQI_GET_MSG_ID;

extern jclass    K_MSG_RESPONSE_IN;
extern jmethodID MID_MsgRspIn_CTOR;
extern jfieldID  FID_MsgRspIn_PAYLOAD_HANDLE;
extern jfieldID  FID_MsgRspIn_REMOTE_DEVADDR;
extern jmethodID MID_MsgRspIn_GET_REMOTE_DEVADDR;
extern jfieldID  FID_MsgRspIn_CONN_TYPE;
extern jfieldID  FID_MsgRspIn_REMOTE_SID;
extern jfieldID  FID_MsgRspIn_RESULT;
extern jfieldID  FID_MsgRspIn_SERIAL;
extern jfieldID  FID_MsgRspIn_URI;
extern jfieldID  FID_MsgRspIn_PAYLOAD;
extern jmethodID MID_MsgRspIn_GET_OPTIONS;
extern jmethodID MID_MsgRspIn_GET_PAYLOAD;


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


extern OCEntityHandlerResult service_request_in(OCEntityHandlerFlag flag,
						OCEntityHandlerRequest * crequest_in, /* MsgRequestIn */
						void* callbackParam);
