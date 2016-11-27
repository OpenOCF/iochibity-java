#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "org_iochibity_ResourceManager.h"

#include "ocpayload.h"
#include "ocresource.h"
#include "ocstack.h"
#include "octypes.h"

JavaVM* g_jvm;

/* INTERNAL ONLY */
/* typedef OCEntityHandlerResult (*OCEntityHandler) */
/* (OCEntityHandlerFlag flag, OCEntityHandlerRequest * entityHandlerRequest, void* callbackParam); */
OCEntityHandlerResult service_routine(OCEntityHandlerFlag flag,
				      OCEntityHandlerRequest * request, /* RequestIn */
				      void* callbackParam)
{
    OC_UNUSED(flag);
    OC_UNUSED(request);
    printf("\nservice_routine called\n");

    /* 1. set up jvm, env */
    /* 2. callbackParam is ref to Java callback obj (ResourceServiceProvider) */
    /* 3.  get id for service method */
    /* 4.  prep args for service method */
    /* 5. call service method */

/* http://stackoverflow.com/questions/12900695/how-to-obtain-jni-interface-pointer-jnienv-for-asynchronous-calls */
/* http://adamish.com/blog/archives/327 */
    JNIEnv * g_env;
    // double check it's all ok
    int getEnvStat = (*g_jvm)->GetEnv(g_jvm, (void **)&g_env, JNI_VERSION_1_6);
    if (getEnvStat == JNI_EDETACHED) {
	printf("GetEnv: not attached; attaching now\n");
	if ((*g_jvm)->AttachCurrentThread(g_jvm, (void **) &g_env, NULL) != 0) {
	    printf("Failed to attach\n");
	}
    } else if (getEnvStat == JNI_OK) {
	printf("GetEnv: attached\n");
    } else if (getEnvStat == JNI_EVERSION) {
	printf("GetEnv: version not supported\n");
    }
    /* g_env->CallVoidMethod(g_obj, g_mid, val); */
    printf("C\n");

    jclass g_clazz = (*g_env)->GetObjectClass(g_env, callbackParam);
    if (g_clazz == NULL) {
	printf("Failed to find class\n");
    }
    /* TODO: verify this is a subclass of ResourceServiceProvider */

    jmethodID g_mid = (*g_env)->GetMethodID(g_env, g_clazz,
					  "service",
					  "(ILorg/iochibity/RequestIn;Ljava/lang/Object;)I");
    if (g_mid == NULL) {
	printf("Unable to get method ref\n");
    } else {
	printf("Found id for service method\n");
    }
    if ((*g_env)->ExceptionCheck(g_env)) {
	(*g_env)->ExceptionDescribe(g_env);
    }

    (*g_jvm)->DetachCurrentThread(g_jvm);

    /* callbackParam contains ref to the java ResourceServiceProvider jobject.
       use it to invoke service routine.
    */

    /* jclass clazz = (*env)->GetObjectClass(env, (jobject) callbackParam); */
    /* if (clazz != NULL) { */
    /*     /\* Same as clazz.getMethod("setBar", String.class) - assuming non-static *\/ */
    /*     jmethodID method = (*env)->GetMethodID(env, clazz, "service", "(Ljava/lang/String;)V"); */
    /*     if (method != NULL) { /\* make sure we found the method *\/ */
    /*         /\* Create a new Java String *\/ */
    /*         jstring jString = (*env)->NewStringUTF(env, "Bar2"); */
    /*         if (jString != null) { */
    /*             /\* Same as method.invoke(object, jString) *\/ */
    /*             (*env)->CallVoidMethod(env, object, method, jString); */
    /*         } */
    /*     } */
    /* } */

    printf("Incoming request: %ld\n", request);
    printf("Incoming requestHandle: %ld\n", request->requestHandle);
    printf("Incoming request param: %ld\n", callbackParam);
    printf("Incoming request flag: %ld\n", flag);

/* 1. create a Payload object */
/* 2. create a ResponseOut object */
/* 3. populate the ResponseOut */
/* 4. send the ResponseOut */

    OCEntityHandlerResult ehResult = OC_EH_OK;
    OCEntityHandlerResponse response = { .requestHandle = NULL }; /* ResponseOut */
    OCRepPayload* new_payload = OCRepPayloadCreate();
    if(!new_payload)
    {
        printf("Failed to allocate Payload\n");
	ehResult = OC_EH_ERROR;
    }

#define RSC_URI_TEMPERATURE  "/a/temperature"
    OCRepPayloadSetUri(new_payload, RSC_URI_TEMPERATURE);
    OCRepPayloadSetPropInt(new_payload, "temp", 72);

    /* *payload = new_payload; */
    // If the result isn't an error or forbidden, send response
    if (!((ehResult == OC_EH_ERROR) || (ehResult == OC_EH_FORBIDDEN)))
	{
	    // Format the response.  Note this requires some info about the request
	    response.requestHandle = request->requestHandle;
	    response.resourceHandle = request->resource;
	    response.ehResult = ehResult;
	    response.payload = (OCPayload*)new_payload; /* testing */
	    // Indicate that response is NOT in a persistent buffer
	    response.persistentBufferFlag = 0;

	    // Send the response
	    if (OCDoResponse(&response) != OC_STACK_OK)
		{
		    printf("Error sending response\n");
		    ehResult = OC_EH_ERROR;
		}
	}
    return ehResult;
    /* return OC_EH_OK; */
}

/*
 * Class:     org_iochibity_ResourceManager
 * Method:    RegisterDefaultServiceRoutine
 * Signature: (Ljava/lang/Object;Ljava/lang/Object;)I
 */
JNIEXPORT jint JNICALL Java_org_iochibity_ResourceManager_registerDefaultServiceRoutine
  (JNIEnv *, jclass, jobject, jobject);

/*
 * Class:     org_iochibity_ResourceManager
 * Method:    resourceCount
 * Signature: (B)I
 */
JNIEXPORT jint JNICALL Java_org_iochibity_ResourceManager_resourceCount
(JNIEnv * env, jclass clazz)
{
    OC_UNUSED(env);
    OC_UNUSED(clazz);
    OCStackResult op_result;
    uint8_t count = 0;
    op_result = OCGetNumberOfResources(&count);
    printf("COUNT: %d\n", count);
    return count;
}

/*
 * Class:     org_iochibity_ResourceManager
 * Method:    RegisterResource
 * Signature: (Lorg/iochibity/Resource;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Lorg/iochibity/ResourceServiceProvider;Ljava/lang/Object;B)I
 */
JNIEXPORT jint JNICALL Java_org_iochibity_ResourceManager_registerResource
  (JNIEnv * env, jclass clazz,
   jobject j_resource,
   jstring j_resource_type_name, jstring j_resource_if_name, jstring j_uri,
   jobject j_resource_service_provider,
   jobject j_callback_param,
   jbyte   j_resource_properties)
{
    OC_UNUSED(clazz);
    OC_UNUSED(j_resource);
    OC_UNUSED(j_callback_param);
    printf("registering resource with service provider %ld\n", j_resource_service_provider);

    const char *c_resource_type_name = "";
    if (j_resource_type_name == NULL) {
	printf("platform id is null\n");
	j_resource_type_name = (*env)->NewStringUTF(env, c_resource_type_name);
	c_resource_type_name = (char*) (*env)->GetStringUTFChars(env, j_resource_type_name, NULL);
    } else {
	c_resource_type_name = (char*) (*env)->GetStringUTFChars(env, j_resource_type_name, NULL);
	if (c_resource_type_name == NULL) {
	    return 0; /* OutOfMemoryError already thrown */
	}
    }
    printf("c resource type name: [%s]\n", c_resource_type_name);

    const char *c_resource_if_name = "";
    if (j_resource_if_name == NULL) {
	printf("platform id is null\n");
	j_resource_if_name = (*env)->NewStringUTF(env, c_resource_if_name);
	c_resource_if_name = (char*) (*env)->GetStringUTFChars(env, j_resource_if_name, NULL);
    } else {
	c_resource_if_name = (char*) (*env)->GetStringUTFChars(env, j_resource_if_name, NULL);
	if (c_resource_if_name == NULL) {
	    return 0; /* OutOfMemoryError already thrown */
	}
    }
    printf("c resource if name: [%s]\n", c_resource_if_name);

    const char *c_uri = "";
    if (j_uri == NULL) {
	printf("platform id is null\n");
	j_uri = (*env)->NewStringUTF(env, c_uri);
	c_uri = (char*) (*env)->GetStringUTFChars(env, j_uri, NULL);
    } else {
	c_uri = (char*) (*env)->GetStringUTFChars(env, j_uri, NULL);
	if (c_uri == NULL) {
	    return 0; /* OutOfMemoryError already thrown */
	}
    }
    printf("c resource uri: [%s]\n", c_uri);

    /* prep callback */
    (*env)->GetJavaVM(env, &g_jvm);
    bool returnValue = true;
    // convert local to global reference
    jobject g_resource_service_provider;
    g_resource_service_provider = (*env)->NewGlobalRef(env, j_resource_service_provider);
    jclass g_clazz = (*env)->GetObjectClass(env, g_resource_service_provider);
    if (g_clazz == NULL) {
	printf("Failed to find class\n");
    }
    jmethodID g_mid = (*env)->GetMethodID(env, g_clazz,
					  "service",
					  "(ILorg/iochibity/RequestIn;Ljava/lang/Object;)I");
    if (g_mid == NULL) {
	printf("Unable to get method ref\n");
    } else {
	printf("Found id for service method\n");
    }

    /* now malloc a callbackParam struct and pack obj and method id into it */
    /* or, just pass the global ref as cb param? */

    OCResourceHandle c_resource_handle;
    OCStackResult op_result;
    op_result = OCCreateResource( &c_resource_handle,    /* **Resource */
				  c_resource_type_name,  /* const char *resourceTypeName, */
				  c_resource_if_name,    /* const char *resourceInterfaceName, */
				  c_uri,                 /* const char *uri, */
				  service_routine,       /* OCEntityHandler entityHandler, */
				  /* j_resource_service_provider, /\* void* callbackParam *\/ */
				  (*env)->NewGlobalRef(env, j_resource_service_provider),
				  (uint8_t) j_resource_properties);  /* uint8_t resourceProperties */

    /* Populate Resource */
    /* printf("Resource handle: %ld\n", c_resource_handle); */
    /* printf("Resource uri: %s\n", ((OCResource*)c_resource_handle)->uri); */
    jclass resource_clazz = (*env)->GetObjectClass(env, j_resource);
    if (resource_clazz != NULL) { /* cannot be null in this case */
        jfieldID field = (*env)->GetFieldID(env, resource_clazz, "handle", "J");
        if (field != NULL) { /* make sure we got the field */
	    /* jlong j_resource_handle = (*env)->GetLongField(env, j_resource_service_provider, field); */
	    (*env)->SetLongField(env, j_resource, field, (jlong)c_resource_handle);
	}
        jfieldID urifield = (*env)->GetFieldID(env, resource_clazz, "uri", "Ljava/lang/String;");
        if (urifield != NULL) { /* make sure we got the field */
            jstring jString = (*env)->NewStringUTF(env, ((OCResource*)c_resource_handle)->uri);
            if (jString != NULL) { /* make sure we are not out of memory */
                /* Same as field.set(object, jString) */
                (*env)->SetObjectField(env, j_resource, urifield, jString);
            }
	}
    }

    (*env)->ReleaseStringUTFChars(env, j_resource_type_name, c_resource_type_name);
    (*env)->ReleaseStringUTFChars(env, j_resource_if_name, c_resource_if_name);
    (*env)->ReleaseStringUTFChars(env, j_uri, c_uri);

    return 0;
}

/* int main(){} */
