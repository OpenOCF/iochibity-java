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
extern jmethodID MID_INT_INTVALUE;	/* Integer.intValue() returns int */
extern jclass K_LONG;			/* 64-bit ints */
extern jmethodID MID_LONG_LONGVALUE;	/* Long.longValue() returns long */
/* jclass K_BIGINTEGER		/\* arbitrary bits *\/ */
extern jclass K_BOOLEAN;
extern jmethodID MID_BOOL_BOOLVALUE;	/* Boolean.booleanValue() returns boolean */
extern jclass K_DOUBLE;
extern jmethodID MID_DBL_DBLVALUE;	/* Double.doubleValue() returns double */
extern jclass K_STRING;
extern jclass K_LIST;		   /* OCByteString => List<Byte> */
extern jclass K_BYTE;
extern jclass K_OBJECT;
/* jclass K_ARRAY; - List */

extern jclass   K_PAYLOAD;
extern jfieldID FID_PAYLOAD_HANDLE;
extern jfieldID FID_PAYLOAD_TYPE;

extern jclass   K_PFRS; 			/* class for PayloadForResourceState */
extern jfieldID FID_PFRS_URI;
extern jfieldID FID_PFRS_RTYPES;
extern jfieldID FID_PFRS_INTERFACES;
extern jfieldID FID_PFRS_PROPERTIES;

extern jclass   K_PMAP; 			/* class for PropertyMap */
extern jmethodID MID_PMAP_SIZE;
extern jmethodID MID_PMAP_ENTRYSET;
extern jmethodID MID_PMAP_GET;

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
extern jfieldID  FID_DA_ADAPTER;
extern jfieldID  FID_DA_FLAGS;
extern jfieldID  FID_DA_PORT;
extern jfieldID  FID_DA_ADDRESS;
extern jfieldID  FID_DA_IFINDEX;
extern jfieldID  FID_DA_ROUTE_DATA;

extern jclass    K_RESOURCE_LOCAL;

extern jclass   K_DOCUMENT;
extern jfieldID  FID_DOC_PAYLOAD_HANDLE;
extern jmethodID MID_DOC_GET_PAYLOAD_TYPE;
extern jfieldID  FID_DOC_OPTION_COUNT;
extern jfieldID  FID_DOC_PTR_OPTIONS;
extern jmethodID MID_DOC_GET_OPTIONS;

extern jclass    K_DOC_REQUEST_IN;

extern jclass   K_DOC_REQUEST_OUT;
extern jfieldID FID_DRQO_SERVICE_REQUESTOR;

extern jclass    K_IRESOURCE_SERVICE_REQUESTOR;
extern jmethodID MID_IRSR_SERVICE_RESPONSE_IN;

extern jclass    K_DOC_REQUEST_IN;
extern jmethodID MID_DRQI_CTOR;
extern jfieldID  FID_DRQI_SERVICE_REQUESTOR;
extern jfieldID  FID_DRQI_DEVICE_ADDRESS;
extern jfieldID  FID_DRQI_IS_REQUEST;
extern jfieldID  FID_DRQI_IS_OBSERVE;

extern jclass    K_DOC_RESPONSE_IN;
extern jmethodID MID_DRSPI_CTOR;
extern jfieldID  FID_DRSPI_PTR_RESPONSE;
extern jfieldID  FID_DRSPI_DEVICE_ADDRESS;
extern jfieldID  FID_DRSPI_CONN_TYPE;
extern jfieldID  FID_DRSPI_REMOTE_SID;
extern jfieldID  FID_DRSPI_RESULT;
extern jfieldID  FID_DRSPI_SERIAL;
extern jfieldID  FID_DRSPI_URI;
extern jfieldID  FID_DRSPI_PAYLOAD;
extern jfieldID  FID_DRSPI_OPTION_COUNT;
extern jmethodID MID_DRSPI_GET_OPTIONS;
extern jmethodID MID_DRSPI_GET_PAYLOAD;


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
						OCEntityHandlerRequest * crequest_in, /* DocRequestIn */
						void* callbackParam);
