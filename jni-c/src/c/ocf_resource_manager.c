#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "org_iochibity_ResourceManager.h"
#include "ocf_exceptions.h"
#include "jni_utils.h"

#include "ocpayload.h"
#include "ocresource.h"
#include "ocstack.h"
#include "octypes.h"

JavaVM* g_jvm;

/* PRIVATE */

/* FIXME: exceptions when called by c stack */
/* THROW_JNI_EXCEPTION won't work since we're not called from Java */
void populate_request_in(JNIEnv* env, jobject jrequest_in, OCEntityHandlerRequest* crequest_in)
{
    /* printf("populate_request_in ENTRY\n"); */
    jclass k_request_in = (*env)->GetObjectClass(env, jrequest_in);
    if (k_request_in == NULL) {
	    THROW_JNI_EXCEPTION("GetObjectClass failed for DocRequestIn\n");
	    return;
    }

    /* first, store pointer to the incoming request */
    jfieldID field = (*env)->GetFieldID(env, k_request_in, "localHandle", "J");
    if (field == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for localHandle\n");
    }
    (*env)->SetLongField(env, jrequest_in, field, (intptr_t)crequest_in);

    field = (*env)->GetFieldID(env, k_request_in, "remoteHandle", "J");
    if (field == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for remoteHandle\n");
    } else {
	printf("c request remote handle: %ld\n", (long)crequest_in->requestHandle);
	(*env)->SetLongField(env, jrequest_in, field, (intptr_t)crequest_in->requestHandle);
    }
    /* then do the rest */
    field = (*env)->GetFieldID(env, k_request_in, "resourceHandle", "J");
    if (field == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for resourceHandle\n");
    } else {
	/* printf("OCResourceHandle (in c): %ld\n", (long)crequest_in->resource); */
	(*env)->SetLongField(env, jrequest_in, field, (intptr_t)crequest_in->resource);
    }

    /* method */
    field = (*env)->GetFieldID(env, k_request_in, "method", "I");
    if (field == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for 'method' fld\n");
    } else {
	/* printf("method in c: %d\n", crequest_in->method); */
	(*env)->SetIntField(env, jrequest_in, field, crequest_in->method);
    }
    /* OCDevAddr */
    jfieldID dev_addr_field = (*env)->GetFieldID(env, k_request_in,
						 "deviceAddress", "Lorg/iochibity/DeviceAddress;");
    if (dev_addr_field == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for deviceAddress\n");
    }
    /* construct a new DeviceAddress object to insert into DocRequestIn obj */
    jclass k_devaddr = (*env)->FindClass(env, "Lorg/iochibity/DeviceAddress;");
    if (k_devaddr == NULL) {
	THROW_JNI_EXCEPTION("FindClass failed for org/iochibity/DeviceAddress\n");
    }
    jmethodID dev_addr_ctor = (*env)->GetMethodID(env, k_devaddr, "<init>", "()V");
    if (dev_addr_ctor == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for ctor of DeviceAddress\n");
    }
    jobject jdevice_address = (*env)->NewObject(env, k_devaddr, dev_addr_ctor);

    /* OCDevAddr.adapter */
    field = (*env)->GetFieldID(env, k_devaddr, "adapter", "I");
    if (field == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for adapter\n");
    } else {
	/* printf("c adapter: %d\n", crequest_in->devAddr.adapter); */
	(*env)->SetIntField(env, jdevice_address, field, crequest_in->devAddr.adapter);
    }
    /* OCDevAddr.flags */
    field = (*env)->GetFieldID(env, k_devaddr, "flags", "I");
    if (field == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for 0 fld\n");
    } else {
	/* printf("c flags: 0x%X\n", crequest_in->devAddr.flags); */
	(*env)->SetIntField(env, jdevice_address, field, crequest_in->devAddr.flags);
    }
    /* OCDevAddr.port */
    field = (*env)->GetFieldID(env, k_devaddr, "port", "I");
    if (field == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for port fld\n");
    } else {
	/* printf("c port: %d\n", crequest_in->devAddr.port); */
	(*env)->SetIntField(env, jdevice_address, field, crequest_in->devAddr.port);
    }
    /* OCDevAddr.addr */
    field = (*env)->GetFieldID(env, k_devaddr, "address", "Ljava/lang/String;");
    if (field == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for address fld\n");
    } else {
	/* printf("c address: %s\n", crequest_in->devAddr.addr); */
	jstring js = (*env)->NewStringUTF(env, crequest_in->devAddr.addr);
	(*env)->SetObjectField(env, jdevice_address, field, js);
    }
    /* OCDevAddr.ifindex */
    field = (*env)->GetFieldID(env, k_devaddr, "ifindex", "I");
    if (field == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for ifindex fld\n");
    } else {
	/* printf("c ifindex: %d\n", crequest_in->devAddr.ifindex); */
	(*env)->SetIntField(env, jdevice_address, field, crequest_in->devAddr.ifindex);
    }
    /* FIXME */
    /* /\* OCDevAddr.routeData *\/ */
    /* if (crequest_in->devAddr.routeData) { */
    /* 	field = (*env)->GetFieldID(env, k_devaddr, "routeData", "Ljava/lang/String;"); */
    /* 	if (field == NULL) { */
    /* 	    printf("Failed to get dev addr routeData fld\n"); */
    /* 	} else { */
    /* 	    printf("c route data: %s\n", crequest_in->devAddr.routeData); */
    /* 	    jstring js = (*env)->NewStringUTF(env, crequest_in->devAddr.routeData); */
    /* 	    (*env)->SetObjectField(env, jdevice_address, field, js); */
    /* 	} */
    /* } */
    /* now set deviceAddress field in DocRequestIn object */
    (*env)->SetObjectField(env, jrequest_in, dev_addr_field, jdevice_address);

    /* query */
    field = (*env)->GetFieldID(env, k_request_in, "query", "Ljava/lang/String;");
    if (field == NULL) { /* make sure we got the field */
	THROW_JNI_EXCEPTION("GetFieldID failed for query fld\n");
    } else {
	/* printf("query in c: '%s'\n", crequest_in->query); */
	jstring js = (*env)->NewStringUTF(env, crequest_in->query);
	(*env)->SetObjectField(env, jrequest_in, field, js);
    }

    /* observation info */
    field = (*env)->GetFieldID(env, k_request_in, "observeAction", "I");
    if (field == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for observeAction fld\n");
    } else {
	/* printf("c observeAction: %d\n", crequest_in->obsInfo.action); */
	(*env)->SetIntField(env, jrequest_in, field, crequest_in->obsInfo.action);
    }
    field = (*env)->GetFieldID(env, k_request_in, "observeId", "I");
    if (field == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for observeId fld\n");
    } else {
	/* printf("c observation id: %d\n", crequest_in->obsInfo.obsId); */
	(*env)->SetIntField(env, jrequest_in, field, crequest_in->obsInfo.obsId);
    }

    /* vendor header options - implemented as getter method */
    field = (*env)->GetFieldID(env, k_request_in, "vendorHeaderOptionCount", "I");
    if (field == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for vendorHeaderOptionCount fld\n");
    } else {
	/* printf("c nbf header options: %d\n", crequest_in->numRcvdVendorSpecificHeaderOptions); */
	(*env)->SetIntField(env, jrequest_in, field, crequest_in->numRcvdVendorSpecificHeaderOptions);
    }

    /* message ID */
    field = (*env)->GetFieldID(env, k_request_in, "messageId", "I");
    if (field == NULL) { /* make sure we got the field */
	THROW_JNI_EXCEPTION("GetFieldID failed for messageId fld\n");
    } else {
	/* printf("message ID in c: %d\n", crequest_in->messageID); */
	(*env)->SetIntField(env, jrequest_in, field, crequest_in->messageID);
    }

    /* payload - implemented as getter, using handle */
    field = (*env)->GetFieldID(env, k_request_in, "payloadHandle", "J");
    if (field == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for payloadHandle fld\n");
    } else {
	/* printf("OCPayload ptr (in c): %ld\n", (long)crequest_in->payload); */
	/* printf("OCPayload type: %ld\n", (long)((OCPayload*)crequest_in->payload)->type); */
	(*env)->SetLongField(env, jrequest_in, field, (intptr_t)crequest_in->payload);
    }
    /* printf("populate_request_in EXIT\n"); */
}

/* typedef OCEntityHandlerResult (*OCEntityHandler) */
/* (OCEntityHandlerFlag flag, OCEntityHandlerRequest * entityHandlerRequest, void* callbackParam); */
OCEntityHandlerResult service_routine(OCEntityHandlerFlag flag,
				      OCEntityHandlerRequest * crequest_in, /* DocRequestIn */
				      void* callbackParam)
{
    OC_UNUSED(flag);
    printf("\nocf_resource_manager.c/service_routine ENTRY\n");
    /* printf("REQUEST URI: %s\n", ((OCResource*)(crequest_in->resource))->uri); */

    /* printf("request resource properties: 0x%X\n", */
    /* 	   ((OCResource*)(crequest_in->resource))->resourceProperties); */

    /* 1. set up jvm, env */
/* http://stackoverflow.com/questions/12900695/how-to-obtain-jni-interface-pointer-jnienv-for-asynchronous-calls */
/* http://adamish.com/blog/archives/327 */
    JNIEnv * env;
    // double check it's all ok
    int getEnvStat = (*g_jvm)->GetEnv(g_jvm, (void **)&env, JNI_VERSION_1_6);
    if (getEnvStat == JNI_EDETACHED) {
	/* printf("GetEnv: not attached; attaching now\n"); */
	if ((*g_jvm)->AttachCurrentThread(g_jvm, (void **) &env, NULL) != 0) {
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

    if (callbackParam == NULL) {
	/* FIXME: use proper logging */
	printf("ERROR %s %d (%s): callbackParam is NULL\n", __FILE__, __LINE__,__func__);
	return OC_EH_INTERNAL_SERVER_ERROR;
    }

    /* extract ref to service provider from callback param */
    jclass k_cbparam = (*env)->GetObjectClass(env, callbackParam);
    if (k_cbparam == NULL) {
	printf("ERROR %s %d (%s): GetObjectClass failed for callbackParam\n", __FILE__, __LINE__,__func__);
	return OC_EH_INTERNAL_SERVER_ERROR;
    }
    jfieldID fid_rsp = (*env)->GetFieldID(env, k_cbparam,
					  "serviceProvider", "Lorg/iochibity/ResourceServiceProvider;");
    if (fid_rsp == NULL) {
	printf("Failed to get serviceProvider fld id\n");
    }
    jobject service_provider = (*env)->GetObjectField(env, callbackParam, fid_rsp);
    if (service_provider == NULL) {
	printf("Failed to get ResourceServiceProvider object\n");
    }

    /* now get the service routine, for later */
    jclass k_rsp = (*env)->GetObjectClass(env, service_provider);
    if (k_rsp == NULL) {
	printf("Failed to find class for ResourceServiceProvider\n");
    }


    jmethodID mid_service = (*env)->GetMethodID(env, k_rsp,
					  "service",
					  "(ILorg/iochibity/DocRequestIn;Lorg/iochibity/CallbackParam;)I");
    if (mid_service == NULL) {
    	THROW_JNI_EXCEPTION("GetMethodID: service routine");
    	return OC_EH_INTERNAL_SERVER_ERROR;
    }

    /* prepare args to service method: (ILorg/iochibity/DocRequestIn;Ljava/lang/Object;)I */

    /* create RequestIn object */
    jclass k_request_in = (*env)->FindClass(env, "Lorg/iochibity/DocRequestIn;");
    if (k_request_in == NULL) {
	printf("Failed to find RequestIn class\n");
    }

    jmethodID request_in_ctor = (*env)->GetMethodID(env, k_request_in, "<init>", "()V");
    if (request_in_ctor == NULL) {
	printf("Failed to find ctor method\n");
    }

    jobject jrequest_in = (*env)->NewObject(env, k_request_in, request_in_ctor);
    populate_request_in(env, jrequest_in, crequest_in);
    fflush(NULL);

    int op_result = OC_EH_OK;
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
 * Signature: (Ljava/lang/String;[Ljava/lang/String;[Ljava/lang/String;Lorg/iochibity/ResourceServiceProvider;Ljava/lang/Object;B)Lorg/iochibity/ResourceLocal;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_ResourceManager_registerResource
  (JNIEnv * env, jclass klass,
   jstring j_uri,
   jobjectArray j_tnames,
   jobjectArray j_ifnames,
   jobject j_resource_service_provider,
   jobject j_callback_param,
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
    (*env)->GetJavaVM(env, &g_jvm);
    /* bool returnValue = true; */

    jobject g_resource_service_provider = NULL;
    jobject g_callback_param = NULL;
    if (j_resource_service_provider == NULL) {
	THROW_STACK_EXCEPTION(OC_STACK_INVALID_PARAM, "ResourceServiceProvider must not be null");
	return NULL;
    }
    // convert local to global references to be passed
    g_resource_service_provider = (*env)->NewGlobalRef(env, j_resource_service_provider);
    /* printf("j_resource_service_provider Classname:\n"); */
    /* print_class_name(env, g_resource_service_provider); */
    jclass k_g_rsp = (*env)->GetObjectClass(env, g_resource_service_provider);
    if (k_g_rsp == NULL) {
	THROW_JNI_EXCEPTION("GetObjectClass for ResourceServiceProvider impl");
    }

    jmethodID mid_service = (*env)->GetMethodID(env, k_g_rsp,
						"service",
						"(ILorg/iochibity/DocRequestIn;Lorg/iochibity/CallbackParam;)I");
    if (mid_service == NULL) {
    	THROW_JNI_EXCEPTION("GetMethodID: service routine");
    	return NULL;
    }

    /* now malloc a callbackParam struct and pack obj and method id into it */
    g_callback_param = (*env)->NewGlobalRef(env, j_callback_param);
    jclass k_cbparam = (*env)->GetObjectClass(env, g_callback_param);
    if (k_cbparam == NULL) {
	THROW_JNI_EXCEPTION("GetObjectClass failed for CallbackParam\n");
	return NULL;
    }

    jfieldID fid_rsp = (*env)->GetFieldID(env, k_cbparam,
					  "serviceProvider", "Lorg/iochibity/ResourceServiceProvider;");
    if (fid_rsp == NULL) {
	THROW_JNI_EXCEPTION("GetFieldID failed for serviceProvider");
    } else {
	(*env)->SetObjectField(env, g_callback_param, fid_rsp, g_resource_service_provider);
    }

    OCResourceHandle c_resource_handle;
    OCStackResult op_result;
    op_result = OCCreateResource( &c_resource_handle,    /* **Resource */
				  c_resource_type_name,  /* const char *resourceTypeName, */
				  c_resource_if_name,    /* const char *resourceInterfaceName, */
				  c_uri,                 /* const char *uri, */
				  service_routine,       /* OCEntityHandler entityHandler, */
				  g_callback_param,
				  (uint8_t) j_policies);  /* uint8_t resourceProperties */

    if (op_result != OC_STACK_OK) THROW_STACK_EXCEPTION(op_result, "OCCreateResource failed");

    /* if multiple types/ifs supplied, add them here */

    /* Populate Resource */
    /* printf("Resource handle: %ld\n", (long)c_resource_handle); */
    /* printf("Resource uri: %s\n", ((OCResource*)c_resource_handle)->uri); */

    /* create a Resource object to return */
    jclass k_resource = (*env)->FindClass(env, "Lorg/iochibity/ResourceLocal;");
    if (k_resource == NULL) {
	THROW_JNI_EXCEPTION("FindClass failed for org/iochibigy/ResourceLocal\n");
    }
    jmethodID resource_ctor = (*env)->GetMethodID(env, k_resource, "<init>", "()V");
    if (resource_ctor == NULL) {
	THROW_JNI_EXCEPTION("GetMethodID failed for ResourceLocal ctor\n");
    }
    jobject j_resource = (*env)->NewObject(env, k_resource, resource_ctor);

    /* insert handle to OCResource */
    if (k_resource != NULL) { /* cannot be null in this case */
        jfieldID fid_handle = (*env)->GetFieldID(env, k_resource, "handle", "J");
        if (fid_handle != NULL) {
	    (*env)->SetLongField(env, j_resource, fid_handle, (intptr_t) (OCResource*)c_resource_handle);
	}
        jfieldID urifield = (*env)->GetFieldID(env, k_resource, "uri", "Ljava/lang/String;");
        if (urifield != NULL) {
            jstring jString = (*env)->NewStringUTF(env, ((OCResource*)c_resource_handle)->uri);
            if (jString != NULL) {
                (*env)->SetObjectField(env, j_resource, urifield, jString);
            }
	}

        jfieldID fid_policies = (*env)->GetFieldID(env, k_resource, "policies", "I");
        if (fid_policies != NULL) {
	    (*env)->SetIntField(env, j_resource, fid_policies,
				(jint)((OCResource*)c_resource_handle)->resourceProperties);
	} else {
	    THROW_JNI_EXCEPTION("SetIntField failed for fid_policies\n");
	}

	/* printf("resource seq nbr: %d\n", (jint) ((OCResource*)c_resource_handle)->sequenceNum); */
        jfieldID fid_sn = (*env)->GetFieldID(env, k_resource, "sn", "I");
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
