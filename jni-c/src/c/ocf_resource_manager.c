#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "org_iochibity_ResourceManager.h"
#include "ocf_init.h"
#include "ocf_exceptions.h"
#include "jni_utils.h"

#include "ocpayload.h"
#include "ocresource.h"
#include "ocstack.h"
#include "octypes.h"

/* PRIVATE */


/* **************************************************************** */
/* PUBLIC */
/* **************************************************************** */

/*
 * Class:     org_iochibity_ResourceManager
 * Method:    resourceCount
 * Signature: (B)I
 */
JNIEXPORT jint JNICALL Java_org_iochibity_ResourceManager_resourceCount
(JNIEnv * env, jclass klass)
{
    OC_UNUSED(env);
    OC_UNUSED(klass);
    uint8_t count = 0;
    OCStackResult op_result = OCGetNumberOfResources(&count);
    if ( op_result != OC_STACK_OK ) {
	// err
    }
    return count;
}

/*
 * Class:     org_iochibity_ResourceManager
 * Method:    registerPlatform
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_iochibity_ResourceManager_registerPlatform
  (JNIEnv * env, jclass klass,
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
    OC_UNUSED(klass);
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
    if (op_result != OC_STACK_OK) {
	// FIXME do sth!
    }

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
 * Class:     org_iochibity_ResourceManager
 * Method:    registerDevice
 * Signature: (Ljava/lang/String;[Ljava/lang/String;Ljava/lang/String;[Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_iochibity_ResourceManager_registerDevice
  (JNIEnv * env, jclass klass,
   jstring j_device_name,
   jobjectArray j_types,
   jstring j_spec_version,
   jobjectArray j_data_model_versions)
{
    OC_UNUSED(env);
    OC_UNUSED(klass);
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
 * Class:     org_iochibity_ResourceManager
 * Method:    RegisterDefaultServiceRoutine
 * Signature: (Ljava/lang/Object;Ljava/lang/Object;)I
 */
JNIEXPORT void JNICALL Java_org_iochibity_ResourceManager_registerDefaultServiceRoutine
(JNIEnv * env, jclass klass, jobject j_resource_service_provider, jobject j_callback_param)
{
    OC_UNUSED(env);
    OC_UNUSED(klass);
    OC_UNUSED(j_resource_service_provider);
    OC_UNUSED(j_callback_param);
}

/*
 * Class:     org_iochibity_ResourceManager
 * Method:    registerResource
 * Signature: (Ljava/lang/String;[Ljava/lang/String;[Ljava/lang/String;Lorg/iochibity/IResourceServiceProvider;B)Lorg/iochibity/ResourceLocal;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_ResourceManager_registerResource
(JNIEnv * env, jclass klass,
 jstring j_uri,
 jobjectArray j_tnames,
 jobjectArray j_ifnames,
 jobject j_IResourceServiceProvider,
   /* jobject j_callback_param, */
   jbyte   j_policies)
{
    OC_UNUSED(klass);
    printf("Java_org_iochibity_ResourceManager_registerResource ENTRY\n");

    jobject j_resource_type_name = (*env)->GetObjectArrayElement(env, j_tnames, 0);
    const char *c_resource_type_name = "";
    if (j_resource_type_name == NULL) {
	j_resource_type_name = (*env)->NewStringUTF(env, c_resource_type_name);
	c_resource_type_name = (char*) (*env)->GetStringUTFChars(env, j_resource_type_name, NULL);
    } else {
	c_resource_type_name = (char*) (*env)->GetStringUTFChars(env, j_resource_type_name, NULL);
	if (c_resource_type_name == NULL) {
	    THROW_JNI_EXCEPTION("GetStringUTFChars");
	    return NULL;
	}
    }
    jobject j_resource_if_name = (*env)->GetObjectArrayElement(env, j_ifnames, 0);
    const char *c_resource_if_name = "";
    if (j_resource_if_name == NULL) {
	j_resource_if_name = (*env)->NewStringUTF(env, c_resource_if_name);
	c_resource_if_name = (char*) (*env)->GetStringUTFChars(env, j_resource_if_name, NULL);
    } else {
	c_resource_if_name = (char*) (*env)->GetStringUTFChars(env, j_resource_if_name, NULL);
	if (c_resource_if_name == NULL) {
	    THROW_JNI_EXCEPTION("GetStringUTFChars");
	    return NULL;
	}
    }

    const char *c_uri = "";
    if (j_uri == NULL) {
	j_uri = (*env)->NewStringUTF(env, c_uri);
	c_uri = (char*) (*env)->GetStringUTFChars(env, j_uri, NULL);
    } else {
	c_uri = (char*) (*env)->GetStringUTFChars(env, j_uri, NULL);
	if (c_uri == NULL) {
	    THROW_JNI_EXCEPTION("GetStringUTFChars");
	}
    }

    /* prep callback */
    /* (*env)->GetJavaVM(env, &g_jvm); */
    /* bool returnValue = true; */

    jobject x_IResourceServiceProvider = NULL;
    /* jobject x_callback_param = NULL; */
    if (j_IResourceServiceProvider == NULL) {
	THROW_STACK_EXCEPTION(OC_STACK_INVALID_PARAM, "ResourceServiceProvider must not be null");
	return NULL;
    }
    // convert local to global references to be passed
    x_IResourceServiceProvider = (*env)->NewGlobalRef(env, j_IResourceServiceProvider);
    jclass k_rsp = (*env)->GetObjectClass(env, x_IResourceServiceProvider);
    if (k_rsp == NULL) {
	THROW_JNI_EXCEPTION("GetObjectClass for ResourceServiceProvider impl");
    }

    /* jmethodID mid_service = (*env)->GetMethodID(env, k_rsp, */
    /* 						"serviceRequestIn", */
    /* 						"(ILorg/iochibity/DocRequestIn;Lorg/iochibity/CallbackParam;)I"); */
    /* if (mid_service == NULL) { */
    /* 	THROW_JNI_EXCEPTION("GetMethodID failed for serviceRequestIn of ResourceServiceProvider"); */
    /* 	return NULL; */
    /* } */

    /* now malloc a callbackParam struct and pack obj and method id into it */
    /* x_callback_param = (*env)->NewGlobalRef(env, j_callback_param); */
    /* jclass k_cbparam = (*env)->GetObjectClass(env, x_callback_param); */
    /* if (k_cbparam == NULL) { */
    /* 	THROW_JNI_EXCEPTION("GetObjectClass failed for CallbackParam\n"); */
    /* 	return NULL; */
    /* } */

    /* jfieldID fid_rsp = (*env)->GetFieldID(env, k_cbparam, */
    /* 					  "serviceProvider", "Lorg/iochibity/ResourceServiceProvider;"); */
    /* if (fid_rsp == NULL) { */
    /* 	THROW_JNI_EXCEPTION("GetFieldID failed for serviceProvider"); */
    /* } else { */
    /* 	(*env)->SetObjectField(env, x_callback_param, fid_rsp, x_IResourceServiceProvider); */
    /* } */

    OCResourceHandle c_resource_handle;
    OCStackResult op_result;
    /* NB: callback service_request_in is in ocf_api_server.c */
    op_result = OCCreateResource( &c_resource_handle,    /* **Resource */
				  c_resource_type_name,  /* const char *resourceTypeName, */
				  c_resource_if_name,    /* const char *resourceInterfaceName, */
				  c_uri,                 /* const char *uri, */
				  service_request_in, /* OCEntityHandler entityHandler, */
				  /* x_callback_param, */
				  x_IResourceServiceProvider,
				  (uint8_t) j_policies);  /* uint8_t resourceProperties */

    if (op_result != OC_STACK_OK) THROW_STACK_EXCEPTION(op_result, "OCCreateResource failed");

    jmethodID mid_resource_ctor = (*env)->GetMethodID(env, K_RESOURCE_LOCAL, "<init>", "()V");
    if (mid_resource_ctor == NULL) {
	THROW_JNI_EXCEPTION("GetMethodID failed for ResourceLocal ctor\n");
    	return NULL;
    }
    jobject j_resource = (*env)->NewObject(env, K_RESOURCE_LOCAL, mid_resource_ctor);

    /* insert handle to OCResource */
    if (K_RESOURCE_LOCAL != NULL) { /* cannot be null in this case */
        jfieldID fid_handle = (*env)->GetFieldID(env, K_RESOURCE_LOCAL, "handle", "J");
        if (fid_handle != NULL) {
	    (*env)->SetLongField(env, j_resource, fid_handle, (intptr_t) (OCResource*)c_resource_handle);
	}
        jfieldID urifield = (*env)->GetFieldID(env, K_RESOURCE_LOCAL, "uri", "Ljava/lang/String;");
        if (urifield != NULL) {
            jstring jString = (*env)->NewStringUTF(env, ((OCResource*)c_resource_handle)->uri);
            if (jString != NULL) {
                (*env)->SetObjectField(env, j_resource, urifield, jString);
            }
	}

        jfieldID fid_policies = (*env)->GetFieldID(env, K_RESOURCE_LOCAL, "policies", "I");
        if (fid_policies != NULL) {
	    (*env)->SetIntField(env, j_resource, fid_policies,
				(jint)((OCResource*)c_resource_handle)->resourceProperties);
	} else {
	    THROW_JNI_EXCEPTION("SetIntField failed for fid_policies\n");
	}

	/* printf("resource seq nbr: %d\n", (jint) ((OCResource*)c_resource_handle)->sequenceNum); */
        jfieldID fid_sn = (*env)->GetFieldID(env, K_RESOURCE_LOCAL, "sn", "I");
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
	    /* printf("NO INSTANCE ID\n"); */
	}
    }

    (*env)->ReleaseStringUTFChars(env, j_resource_type_name, c_resource_type_name);
    (*env)->ReleaseStringUTFChars(env, j_resource_if_name, c_resource_if_name);
    (*env)->ReleaseStringUTFChars(env, j_uri, c_uri);

    printf("org_iochibity_ResourceManager/registerResource EXIT\n");
    return j_resource;
}
