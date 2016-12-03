/* #include <ctype.h> */
/* #include <string.h> */
/* #include <stdlib.h> */

#include "ocf_exceptions.h"

jint throw_no_class_def_error( JNIEnv *env, char *message )
{
    jclass clazz;
    char *className = "java/lang/NoClassDefFoundError";

    clazz = (*env)->FindClass( env, className);
    if (clazz == NULL) {
        return throw_no_class_def_error( env, className );
    }

    return (*env)->ThrowNew( env, clazz, message );
}


jint throw_not_implemented( JNIEnv *env, char *message )
{
    jclass clazz;
    char *className = "org/iochibity/exceptions/OCFNotImplementedException";

    clazz = (*env)->FindClass( env, className);
    /* if (clazz == NULL) { */
    /*     return throw_no_class_def_error( env, className ); */
    /* } */

    return (*env)->ThrowNew( env, clazz, message );
}

/* see https://monochrome.sutic.nu/2013/09/01/nice-jni-exceptions.html */

void throw_jni_exception(// The first four parameters are common for all
			 // throwables
			 JNIEnv* env,
			 const char* functionName,
			 const char* file,
			 const int line,

			 // These are specific to JNIException
			 const char* message)
{
    jclass clazz = (*env)->FindClass(env, "org/iochibity/exceptions/JNIRuntimeException");
    if (clazz == 0) (*env)->ThrowNew(env,
				     (*env)->FindClass( env, "java/lang/ClassNotFoundException"),
				     "org/iochibity/exceptions/JNIRuntimeException" );

    jmethodID ctor = (*env)->GetMethodID(env, clazz, "<init>", "(ILjava/lang/String;)V");
    if (ctor == 0) (*env)->ThrowNew( env,
				     (*env)->FindClass( env, "java/lang/NoSuchMethod"),
				     "ctor for JNIRuntimeException" );

    jthrowable e = (*env)->NewObject(env, clazz, ctor, (*env)->NewStringUTF(env, message));

    // Find the __jni_setLocation method and call it with
    // the function name, file and line parameters
    jmethodID mid_set_location = (*env)->GetMethodID (env, clazz, "__jni_setLocation",
						      "(Ljava/lang/String;"
						      "Ljava/lang/String;"
						      "I)V");
    if (mid_set_location == 0) (*env)->ThrowNew(env,
						(*env)->FindClass( env, "java/lang/NoSuchMethod"),
						"__jni_setLocation" );

    (*env)->CallVoidMethod(env, e, mid_set_location,
			   (*env)->NewStringUTF(env, functionName),
			   (*env)->NewStringUTF(env, file),
			   line);

    // Throw the exception. Since this is native code,
    // execution continues, and the execution will be abruptly
    // interrupted at the point in time when we return to the VM.
    // The calling code will perform the early return back to Java code.
    (*env)->Throw(env, e);

    // Clean up local reference
    (*env)->DeleteLocalRef(env, clazz);
}

char* getCodeString(int code) {
    switch(code) {
	// typedef enum
	// {
	/** Success status code - START HERE.*/
	// case OC_STACK_OK:
	//     return "OC_STACK_OK";
	//     break;
	// case OC_STACK_RESOURCE_CREATED:
	//     return "OC_STACK_RESOURCE_CREATED";
	//     break;
	// case OC_STACK_RESOURCE_DELETED:
	//     return "OC_STACK_RESOURCE_DELETED";
	//     break;
	// OC_STACK_CONTINUE,
	// OC_STACK_RESOURCE_CHANGED,
	// /** Success status code - END HERE.*/

	// /** Error status code - START HERE.*/

    case OC_STACK_INVALID_URI:
	return "OC_STACK_INVALID_URI";
	break;
	// OC_STACK_INVALID_QUERY,
	// OC_STACK_INVALID_IP,
	// OC_STACK_INVALID_PORT,
	// OC_STACK_INVALID_CALLBACK,
	// OC_STACK_INVALID_METHOD,

	/** Invalid parameter.*/
    case OC_STACK_INVALID_PARAM:
	return "OC_STACK_INVALID_PARAM";
	break;
	// OC_STACK_INVALID_OBSERVE_PARAM,
	// OC_STACK_NO_MEMORY,
	// OC_STACK_COMM_ERROR,
	// OC_STACK_TIMEOUT,
	// OC_STACK_ADAPTER_NOT_ENABLED,
	// OC_STACK_NOTIMPL,

	// /** Resource not found.*/
	// OC_STACK_NO_RESOURCE,

	// /** e.g: not supported method or interface.*/
	// OC_STACK_RESOURCE_ERROR,
	// OC_STACK_SLOW_RESOURCE,
	// OC_STACK_DUPLICATE_REQUEST,

	// /** Resource has no registered observers.*/
	// OC_STACK_NO_OBSERVERS,
	// OC_STACK_OBSERVER_NOT_FOUND,
	// OC_STACK_VIRTUAL_DO_NOT_HANDLE,
	// OC_STACK_INVALID_OPTION,

	// /** The remote reply contained malformed data.*/
	// OC_STACK_MALFORMED_RESPONSE,
	// OC_STACK_PERSISTENT_BUFFER_REQUIRED,
	// OC_STACK_INVALID_REQUEST_HANDLE,
	// OC_STACK_INVALID_DEVICE_INFO,
	// OC_STACK_INVALID_JSON,

	// /** Request is not authorized by Resource Server. */
	// OC_STACK_UNAUTHORIZED_REQ,
	// OC_STACK_TOO_LARGE_REQ,

	// /** Error code from PDM */
	// OC_STACK_PDM_IS_NOT_INITIALIZED,
	// OC_STACK_DUPLICATE_UUID,
	// OC_STACK_INCONSISTENT_DB,

	// /**
	//  * Error code from OTM
	//  * This error is pushed from DTLS interface when handshake failure happens
	//  */
	// OC_STACK_AUTHENTICATION_FAILURE,

	// /** Insert all new error codes here!.*/
	// #ifdef WITH_PRESENCE
	// OC_STACK_PRESENCE_STOPPED = 128,
	// OC_STACK_PRESENCE_TIMEOUT,
	// OC_STACK_PRESENCE_DO_NOT_HANDLE,
	// #endif
	/** ERROR in stack.*/
    case OC_STACK_ERROR:
	return "OC_STACK_ERROR";
	break;
	/** Error status code - END HERE.*/
    default:
	return "UNKNOWN ERROR";
	// } OCStackResult;
    }
}

void throw_stack_exception (// The first four parameters are common for all
			    // throwables
			    JNIEnv* env,
			    const char* functionName,
			    const char* file,
			    const int line,

			    // These are specific to the OCFStackException
			    const int code,
			    const char* message)
{
    jclass clazz = (*env)->FindClass(env, "org/iochibity/exceptions/OCFStackException");
    if (clazz == 0) (*env)->ThrowNew(env,
				     (*env)->FindClass( env, "java/lang/ClassNotFoundException"),
				     "org/iochibity/exceptions/OCFStackException" );

    jmethodID ctor = (*env)->GetMethodID(env, clazz, "<init>", "(ILjava/lang/String;Ljava/lang/String;)V");
    if (ctor == 0) (*env)->ThrowNew( env,
				     (*env)->FindClass( env, "java/lang/NoSuchMethod"),
				     "ctor for OCFStackException" );

    jthrowable e = (*env)->NewObject(env, clazz, ctor,
				     code,
				     (*env)->NewStringUTF(env, getCodeString(code)),
				     (*env)->NewStringUTF(env, message));

    // Find the __jni_setLocation method and call it with
    // the function name, file and line parameters
    jmethodID mid_set_location = (*env)->GetMethodID(env, clazz, "__jni_setLocation",
						     "(Ljava/lang/String;"
						     "Ljava/lang/String;"
						     "I)V");

    (*env)->CallVoidMethod(env, e, mid_set_location,
			   (*env)->NewStringUTF(env, functionName),
			   (*env)->NewStringUTF(env, file),
			   line);

    // Throw the exception. Since this is native code,
    // execution continues, and the execution will be abruptly
    // interrupted at the point in time when we return to the VM.
    // The calling code will perform the early return back to Java code.
    (*env)->Throw(env, e);

    // Clean up local reference
    (*env)->DeleteLocalRef(env, clazz);
}

/* jint throw_invalid_param( JNIEnv *env, char *message ) */
/* { */
/*     jclass clazz; */
/*     char *className = "org/iochibity/exceptions/OCFInvalidParamException"; */

/*     /\* clazz = (*env)->FindClass( env, className); *\/ */
/*     /\* if (clazz == NULL) { *\/ */
/*     /\*     return throw_invalid_param( env, className ); *\/ */
/*     /\* } *\/ */

/*     return (*env)->ThrowNew( env, clazz, message ); */
/* } */

/* jint throw_out_of_memory( JNIEnv *env, char *message ) */
/* { */
/*     jclass clazz; */
/*     char *className = "org/iochibity/exceptions/OCFNoMemory"; */

/*     clazz = (*env)->FindClass( env, className); */
/*     if (clazz == NULL) { */
/*         return throw_invalid_param( env, className ); */
/*     } */

/*     return (*env)->ThrowNew( env, clazz, message ); */
/* } */
