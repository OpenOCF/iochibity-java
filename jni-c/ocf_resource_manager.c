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

void populate_request_in(JNIEnv* env, jobject jrequest_in, OCEntityHandlerRequest* crequest_in)
{
    printf("populate_request_in ENTRY\n");
    jclass request_in_clazz = (*env)->GetObjectClass(env, jrequest_in);
    if (request_in_clazz == NULL) {
	printf("Failed to find RequestIn class\n");
    } else {
	/* printf("Found RequestIn class\n"); */

        jfieldID field = (*env)->GetFieldID(env, request_in_clazz, "resourceHandle", "J");
        if (field == NULL) {
	    printf("Failed to get resourceHandle fld id\n");
	} else {
	    printf("OCResourceHandle (in c): %ld\n", (long)crequest_in->resource);
	    (*env)->SetLongField(env, jrequest_in, field, (jlong)crequest_in->resource);
	}

	field = (*env)->GetFieldID(env, request_in_clazz, "handleAtOrigin", "J");
        if (field == NULL) {
	    printf("Failed to get handleAtOrigin fld id\n");
	} else {
	    printf("OCRequestHandle in c: %ld\n", (long)crequest_in->requestHandle);
	    (*env)->SetLongField(env, jrequest_in, field, (jlong)crequest_in->requestHandle);
	}
	/* method */
	field = (*env)->GetFieldID(env, request_in_clazz, "method", "I");
        if (field == NULL) {
	    printf("Failed to get method fld\n");
	} else {
	    /* printf("method in c: %d\n", crequest_in->method); */
	    (*env)->SetIntField(env, jrequest_in, field, crequest_in->method);
	}
	/* OCDevAddr */
        jfieldID dev_addr_field = (*env)->GetFieldID(env, request_in_clazz,
						     "deviceAddress", "Lorg/iochibity/DeviceAddress;");
        if (dev_addr_field == NULL) {
	    printf("Failed to get deviceAddress fld id\n");
	} else {
	    /* construct a new DeviceAddress object to insert into RequestIn obj */
	    jclass dev_addr_clazz = (*env)->FindClass(env, "Lorg/iochibity/DeviceAddress;");
	    if (dev_addr_clazz == NULL) {
		printf("Failed to find DeviceAddress class\n");
	    } else {
		/* printf("Found DeviceAddress class\n"); */
	    }

	    jmethodID dev_addr_ctor = (*env)->GetMethodID(env, dev_addr_clazz, "<init>", "()V");
	    if (dev_addr_ctor == NULL) {
		printf("Failed to find DeviceAddress ctor\n");
	    } else {
		/* printf("Found ctor\n"); */
	    }

	    jobject jdevice_address = (*env)->NewObject(env, dev_addr_clazz, dev_addr_ctor);

	    /* OCDevAddr.adapter */
	    field = (*env)->GetFieldID(env, dev_addr_clazz, "adapter", "I");
	    if (field == NULL) {
		printf("Failed to get dev addr adapter fld\n");
	    } else {
		printf("c adapter: %d\n", crequest_in->devAddr.adapter);
		(*env)->SetIntField(env, jdevice_address, field, crequest_in->devAddr.adapter);
	    }
	    /* OCDevAddr.flags */
	    field = (*env)->GetFieldID(env, dev_addr_clazz, "flags", "I");
	    if (field == NULL) {
		printf("Failed to get dev addr flags fld\n");
	    } else {
		printf("c flags: 0x%X\n", crequest_in->devAddr.flags);
		(*env)->SetIntField(env, jdevice_address, field, crequest_in->devAddr.flags);
	    }
	    /* OCDevAddr.port */
	    field = (*env)->GetFieldID(env, dev_addr_clazz, "port", "I");
	    if (field == NULL) {
		printf("Failed to get dev addr port fld\n");
	    } else {
		printf("c port: %d\n", crequest_in->devAddr.port);
		(*env)->SetIntField(env, jdevice_address, field, crequest_in->devAddr.port);
	    }
	    /* OCDevAddr.addr */
	    field = (*env)->GetFieldID(env, dev_addr_clazz, "address", "Ljava/lang/String;");
	    if (field == NULL) {
		printf("Failed to get dev addr address fld\n");
	    } else {
		printf("c address: %s\n", crequest_in->devAddr.addr);
		jstring js = (*env)->NewStringUTF(env, crequest_in->devAddr.addr);
		(*env)->SetObjectField(env, jdevice_address, field, js);
	    }
	    /* OCDevAddr.ifindex */
	    field = (*env)->GetFieldID(env, dev_addr_clazz, "ifindex", "I");
	    if (field == NULL) {
		printf("Failed to get dev addr ifindex fld\n");
	    } else {
		printf("c ifindex: %d\n", crequest_in->devAddr.ifindex);
		(*env)->SetIntField(env, jdevice_address, field, crequest_in->devAddr.ifindex);
	    }
	    /* FIXME */
	    /* /\* OCDevAddr.routeData *\/ */
	    /* if (crequest_in->devAddr.routeData) { */
	    /* 	field = (*env)->GetFieldID(env, dev_addr_clazz, "routeData", "Ljava/lang/String;"); */
	    /* 	if (field == NULL) { */
	    /* 	    printf("Failed to get dev addr routeData fld\n"); */
	    /* 	} else { */
	    /* 	    printf("c route data: %s\n", crequest_in->devAddr.routeData); */
	    /* 	    jstring js = (*env)->NewStringUTF(env, crequest_in->devAddr.routeData); */
	    /* 	    (*env)->SetObjectField(env, jdevice_address, field, js); */
	    /* 	} */
	    /* } */
	    /* now set deviceAddress field in RequestIn object */
	    (*env)->SetObjectField(env, jrequest_in, dev_addr_field, jdevice_address);
	}

	/* query */
	field = (*env)->GetFieldID(env, request_in_clazz, "query", "Ljava/lang/String;");
        if (field == NULL) { /* make sure we got the field */
	    printf("Failed to get query fld\n");
	} else {
	    printf("Got id for query fld\n");
	    printf("query in c: '%s'\n", crequest_in->query);
            jstring js = (*env)->NewStringUTF(env, crequest_in->query);
	    (*env)->SetObjectField(env, jrequest_in, field, js);
	}

	/* observation info */
	field = (*env)->GetFieldID(env, request_in_clazz, "observeAction", "I");
	if (field == NULL) {
	    printf("Failed to get observeAction fld\n");
	} else {
	    printf("c observeAction: %d\n", crequest_in->obsInfo.action);
	    (*env)->SetIntField(env, jrequest_in, field, crequest_in->obsInfo.action);
	}
	field = (*env)->GetFieldID(env, request_in_clazz, "observeId", "I");
	if (field == NULL) {
	    printf("Failed to get observeId fld\n");
	} else {
	    printf("c observation id: %d\n", crequest_in->obsInfo.obsId);
	    (*env)->SetIntField(env, jrequest_in, field, crequest_in->obsInfo.obsId);
	}

	/* vendor header options - implemented as getter method */
	field = (*env)->GetFieldID(env, request_in_clazz, "vendorHeaderOptionCount", "I");
	if (field == NULL) {
	    printf("Failed to get vendorHeaderOptionCount fld\n");
	} else {
	    printf("c nbf header options: %d\n", crequest_in->numRcvdVendorSpecificHeaderOptions);
	    (*env)->SetIntField(env, jrequest_in, field, crequest_in->numRcvdVendorSpecificHeaderOptions);
	}

	/* message ID */
	field = (*env)->GetFieldID(env, request_in_clazz, "messageId", "I");
        if (field == NULL) { /* make sure we got the field */
	    printf("Failed to get messageId fld\n");
	} else {
	    printf("message ID in c: %d\n", crequest_in->messageID);
	    (*env)->SetIntField(env, jrequest_in, field, crequest_in->messageID);
	}

	/* payload - implemented as getter, using handle */
        field = (*env)->GetFieldID(env, request_in_clazz, "payloadHandle", "J");
        if (field == NULL) {
	    printf("Failed to get payloadHandle fld id\n");
	} else {
	    printf("OCPayload ptr (in c): %ld\n", (long)crequest_in->payload);
	    /* printf("OCPayload type: %ld\n", (long)((OCPayload*)crequest_in->payload)->type); */
	    (*env)->SetLongField(env, jrequest_in, field, (jlong)crequest_in->payload);
	}
    }
    printf("populate_request_in EXIT\n");
}

/* typedef OCEntityHandlerResult (*OCEntityHandler) */
/* (OCEntityHandlerFlag flag, OCEntityHandlerRequest * entityHandlerRequest, void* callbackParam); */
OCEntityHandlerResult service_routine(OCEntityHandlerFlag flag,
				      OCEntityHandlerRequest * crequest_in, /* RequestIn */
				      void* callbackParam)
{
    OC_UNUSED(flag);
    printf("\nocf_resource_manager.c/service_routine ENTRY\n");
    /* printf("REQUEST URI: %s\n", ((OCResource*)(crequest_in->resource))->uri); */

    printf("request resource properties: 0x%X\n",
	   ((OCResource*)(crequest_in->resource))->resourceProperties);

    /* 1. set up jvm, env */
/* http://stackoverflow.com/questions/12900695/how-to-obtain-jni-interface-pointer-jnienv-for-asynchronous-calls */
/* http://adamish.com/blog/archives/327 */
    JNIEnv * env;
    // double check it's all ok
    int getEnvStat = (*g_jvm)->GetEnv(g_jvm, (void **)&env, JNI_VERSION_1_6);
    if (getEnvStat == JNI_EDETACHED) {
	/* printf("GetEnv: not attached; attaching now\n"); */
	if ((*g_jvm)->AttachCurrentThread(g_jvm, (void **) &env, NULL) != 0) {
	    printf("Failed to attach\n");
	}
    } else if (getEnvStat == JNI_OK) {
	/* printf("GetEnv: attached\n"); */
    } else if (getEnvStat == JNI_EVERSION) {
	printf("GetEnv: version not supported\n");
	return OC_EH_INTERNAL_SERVER_ERROR;
    }
    if ((*env)->ExceptionCheck(env)) {
	(*env)->ExceptionDescribe(env);
    }

    /* extract ref to service provider from callback param */
    jclass cbparam_clazz = (*env)->GetObjectClass(env, callbackParam);
    if (cbparam_clazz == NULL) {
	printf("Failed to find class for callbackParam\n");
    } else {
	printf("Found class for callbackParam\n");
    }
    jfieldID fid_rsp = (*env)->GetFieldID(env, cbparam_clazz,
					  "serviceProvider", "Lorg/iochibity/ResourceServiceProvider;");
    if (fid_rsp == NULL) {
	printf("Failed to get serviceProvider fld id\n");
    } else {
	printf("Got serviceProvider fld id\n");
    }
    jobject service_provider = (*env)->GetObjectField(env, callbackParam, fid_rsp);
    if (service_provider == NULL) {
	printf("Failed to get ResourceServiceProvider object\n");
    } else {
	printf("Got ResourceServiceProvider object\n");
    }
    /* now get the service routine, for later */
    jclass rsp_clazz = (*env)->GetObjectClass(env, service_provider);
    if (rsp_clazz == NULL) {
	printf("Failed to find class for ResourceServiceProvider\n");
    } else {
	printf("Found class for ResourceServiceProvider\n");
    }
    jmethodID mid_service = (*env)->GetMethodID(env, rsp_clazz,
					  "service",
					  "(ILorg/iochibity/RequestIn;Ljava/lang/Object;)I");
    if (mid_service == NULL) {
	printf("TEST Unable to get 'service' method id\n");
    } else {
	printf("Found id for 'service' method\n");
    }

    /* prepare args to service method: (ILorg/iochibity/RequestIn;Ljava/lang/Object;)I */

    /* create RequestIn object */
    jclass request_in_clazz = (*env)->FindClass(env, "Lorg/iochibity/RequestIn;");
    if (request_in_clazz == NULL) {
	printf("Failed to find RequestIn class\n");
    } else {
	/* printf("Found RequestIn class\n"); */
    }

    jmethodID request_in_ctor = (*env)->GetMethodID(env, request_in_clazz, "<init>", "()V");
    if (request_in_ctor == NULL) {
	printf("Failed to find ctor method\n");
    } else {
	/* printf("Found ctor\n"); */
    }

    jobject jrequest_in = (*env)->NewObject(env, request_in_clazz, request_in_ctor);
    populate_request_in(env, jrequest_in, crequest_in);
    fflush(NULL);

    OCEntityHandlerResult op_result = OC_EH_OK;
    op_result = (*env)->CallIntMethod(env, service_provider, mid_service,
				      flag, jrequest_in, callbackParam);
    if (op_result != OC_STACK_OK) {
        printf("TEST call to service routine failed!\n");
        /* exit (EXIT_FAILURE); */
    } else {
        printf("TEST call to Java service routine succeeded!\n");
    }

    (*g_jvm)->DetachCurrentThread(g_jvm);

    /* printf("Incoming request: %ld\n", (long)crequest_in); */
    /* printf("Incoming requestHandle: %ld\n", (long)crequest_in->requestHandle); */
    /* printf("Incoming request param: %ld\n", (long)callbackParam); */
    /* printf("Incoming request flag: %d\n", flag); */

    printf("ocf_resource_manager.c/service_routine EXIT\n");
    return op_result;
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
    printf("Java_org_iochibity_ResourceManager_registerResource ENTRY\n");

    const char *c_resource_type_name = "";
    if (j_resource_type_name == NULL) {
	printf("resource type is null\n");
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
	printf("resource if is null\n");
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
    /* bool returnValue = true; */

    // convert local to global references to be passed
    jobject g_resource_service_provider;
    jobject g_callback_param;

    g_resource_service_provider = (*env)->NewGlobalRef(env, j_resource_service_provider);
    jclass g_clazz = (*env)->GetObjectClass(env, g_resource_service_provider);
    if (g_clazz == NULL) {
	printf("Failed to find class\n");
    }
    jmethodID service_mid = (*env)->GetMethodID(env, g_clazz,
					  "service",
					  "(ILorg/iochibity/RequestIn;Ljava/lang/Object;)I");
    if (service_mid == NULL) {
	printf("Unable to get method ref\n");
    } else {
	/* printf("Found id for service method\n"); */
    }
    /* now malloc a callbackParam struct and pack obj and method id into it */
    /* or, just pass the global ref as cb param? */
    g_callback_param = (*env)->NewGlobalRef(env, j_callback_param);
    jclass cbparam_clazz = (*env)->GetObjectClass(env, g_callback_param);
    if (cbparam_clazz == NULL) {
	printf("Failed to find CallbackParam class\n");
    } else {
	jfieldID fid_rsp = (*env)->GetFieldID(env, cbparam_clazz,
					      "serviceProvider", "Lorg/iochibity/ResourceServiceProvider;");
        if (fid_rsp == NULL) {
	    printf("Failed to get serviceProvider fld id\n");
	} else {
	    (*env)->SetObjectField(env, g_callback_param, fid_rsp, g_resource_service_provider);
	}
    }

    printf("c POLICIES before: 0x%X\n",  j_resource_properties);

    OCResourceHandle c_resource_handle;
    OCStackResult op_result;
    op_result = OCCreateResource( &c_resource_handle,    /* **Resource */
				  c_resource_type_name,  /* const char *resourceTypeName, */
				  c_resource_if_name,    /* const char *resourceInterfaceName, */
				  c_uri,                 /* const char *uri, */
				  service_routine,       /* OCEntityHandler entityHandler, */
				  g_callback_param,
				  /* j_resource_service_provider, /\* void* callbackParam *\/ */
				  /* (*env)->NewGlobalRef(env, j_resource_service_provider), */
				  (uint8_t) j_resource_properties);  /* uint8_t resourceProperties */

    /* Populate Resource */
    printf("Resource handle: %ld\n", (long)c_resource_handle);
    printf("Resource uri: %s\n", ((OCResource*)c_resource_handle)->uri);
    jclass resource_clazz = (*env)->GetObjectClass(env, j_resource);
    /* insert handle to OCResource */
    if (resource_clazz != NULL) { /* cannot be null in this case */
        jfieldID fid_handle = (*env)->GetFieldID(env, resource_clazz, "handle", "J");
        if (fid_handle != NULL) {
	    (*env)->SetLongField(env, j_resource, fid_handle, (jlong) (OCResource*)c_resource_handle);
	}
        jfieldID urifield = (*env)->GetFieldID(env, resource_clazz, "uri", "Ljava/lang/String;");
        if (urifield != NULL) {
            jstring jString = (*env)->NewStringUTF(env, ((OCResource*)c_resource_handle)->uri);
            if (jString != NULL) {
                (*env)->SetObjectField(env, j_resource, urifield, jString);
            }
	}
	printf("c POLICIES: 0x%X\n",  ((OCResource*)c_resource_handle)->resourceProperties);

        jfieldID fid_policies = (*env)->GetFieldID(env, resource_clazz, "policies", "I");
        if (fid_policies != NULL) {
	    (*env)->SetIntField(env, j_resource, fid_policies,
				(jint)((OCResource*)c_resource_handle)->resourceProperties);
	} else {
	    printf("Failed to find policies field id\n");
	}

	/* printf("resource seq nbr: %d\n", (jint) ((OCResource*)c_resource_handle)->sequenceNum); */
        jfieldID fid_sn = (*env)->GetFieldID(env, resource_clazz, "sn", "I");
        if (fid_sn != NULL) {
	    (*env)->SetIntField(env, j_resource, fid_sn,
				(jint) ((OCResource*)c_resource_handle)->sequenceNum);
	}

	/* instance id ∪nion */
	/* printf("instance ORD:  %d\n", ((OCResource*)c_resource_handle)->ins); */
	/* printf("instance str:  '%s'\n", ((OCResource*)c_resource_handle)->uniqueStr); */
	/* printf("instance uuid ln: %d\n", ((OCResource*)c_resource_handle)->uniqueUUID.id_length); */
	/* printf("instance uuid: %s\n", ((OCResource*)c_resource_handle)->uniqueUUID.id); */

	if ( ((OCResource*)c_resource_handle)->ins > 0) {
	    printf("instance ORD:  %d\n", ((OCResource*)c_resource_handle)->ins);
	} else if ( ((OCResource*)c_resource_handle)->uniqueStr != NULL) {
	    printf("instance str:  '%s'\n", ((OCResource*)c_resource_handle)->uniqueStr);
	} else if ( ((OCResource*)c_resource_handle)->uniqueUUID.id_length > 0) {
	    printf("instance uuid: %s\n", ((OCResource*)c_resource_handle)->uniqueUUID.id);
	} else {
	    printf("NO INSTANCE ID\n");
	}
    }

    (*env)->ReleaseStringUTFChars(env, j_resource_type_name, c_resource_type_name);
    (*env)->ReleaseStringUTFChars(env, j_resource_if_name, c_resource_if_name);
    (*env)->ReleaseStringUTFChars(env, j_uri, c_uri);

    printf("org_iochibity_ResourceManager/registerResource EXIT\n");
    return 0;
}

/* int main(){} */

 /*
   PROBLEM: OCEntityHandlerRequest points to an OCEntityHandlerRequest
   object which is locally allocated by the calling routine
   (ocresource.c/HandleResourceWithEntityHandler), and after this
   routine exits, so does HandleResourceWithEntityHandler, so the
   object is destroyed.  So we need to copy the entire thing in order
   to make it available to the Java service routine.
 */
