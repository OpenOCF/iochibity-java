/**
 * @file org_iochibity_CoServiceManager.c
 * @author Gregg Reynolds
 * @date December 2016
 *
 * @brief implementations of `registerX` routines etc.
 */

#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "org_iochibity_CoServiceManager.h"
#include "c_co_service_manager.h"
#include "jni_init.h"
#include "org_iochibity_Exceptions.h"
#include "jni_utils.h"

#include "openocf.h"
/* #include "ocresource.h" */
/* #include "ocstack.h" */
/* #include "octypes.h" */
/* #include "logger.h" */

/* PRIVATE */

#define TAG  "CO_SERVICE_MANAGER"

/* **************************************************************** */
/* PUBLIC */
/* **************************************************************** */

/*
 * Class:     org_iochibity_CoServiceManager
 * Method:    configurePlatformSP
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_iochibity_CoServiceManager_configurePlatformSP
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
 * Class:     org_iochibity_CoServiceManager
 * Method:    configureDeviceSP
 * Signature: (Ljava/lang/String;[Ljava/lang/String;Ljava/lang/String;[Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_iochibity_CoServiceManager_configureDeviceSP
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
 * Class:     org_iochibity_CoServiceManager
 * Method:    RegisterDefaultServiceRoutine
 * Signature: (Ljava/lang/Object;Ljava/lang/Object;)I
 */
JNIEXPORT void JNICALL Java_org_iochibity_CoServiceManager_registerDefaultServiceRoutine
(JNIEnv * env, jclass klass, jobject j_resource_service_provider, jobject j_callback_param)
{
    OC_UNUSED(env);
    OC_UNUSED(klass);
    OC_UNUSED(j_resource_service_provider);
    OC_UNUSED(j_callback_param);
}

/*
 * Class:     org_iochibity_CoServiceManager
 * Method:    registerDefaultCoServiceProvider
 * Signature: (Lorg/iochibity/CoServiceProvider;)Lorg/iochibity/CoServiceProvider;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_CoServiceManager_registerDefaultCoServiceProvider
(JNIEnv * env, jclass klass, jobject j_CoServiceProvider)
{
    return j_CoServiceProvider;
}

/*
 * Class:     org_iochibity_CoServiceManager
 * Method:    registerCoServiceProvider
 * Signature: (Lorg/iochibity/CoServiceProvider;)Lorg/iochibity/CoServiceProvider;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_CoServiceManager_registerCoServiceProvider
(JNIEnv * env, jclass klass, jobject j_CoServiceProvider)
{
    OIC_LOG_V(DEBUG, TAG, "%s ENTRY", __func__);
    /* pseudo-registration - coSP's call CoServiceProvider.coExhibit,
       which is a native method
       Java_org_iochibity_CoServiceProvider_coExhibit, which runs
       OCDoResource, which registers the coSP (resource).

       IOW, the low-level service manager registers the cosp at the
       start of each transaction, then deregisters it after response
       is handled, unless it is a multicast request and tls_deactivate
       is false

       iow, coSPs are one-off affairs, unlike SPs, which are
       registered once and for all and then start observing.
     */
    OIC_LOG_V(DEBUG, TAG, "%s EXIT", __func__);
    return NULL;
}

/*
 * Class:     org_iochibity_CoServiceManager
 * Method:    registeredCoServiceProviders
 * Signature: ()Ljava/util/List;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_CoServiceManager_registeredCoServiceProviders
(JNIEnv * env, jclass klass)
{
    OIC_LOG_V(DEBUG, __FILE__, "[%d] %s ENTRY", __LINE__, __func__);
    /* OIC_LOG_DISCOVERY_RESPONSE(DEBUG, TAG, g_OCClientResponse); */
    // return coSP list object
    OIC_LOG_V(DEBUG, __FILE__, "[%d] %s EXIT", __LINE__, __func__);
    return NULL;
}

/*
 * Class:     org_iochibity_CoServiceManager
 * Method:    getRelatedCoServiceProviders
 * Signature: ()Ljava/util/List;
 */
JNIEXPORT jobject JNICALL Java_org_iochibity_CoServiceManager_getRelatedCoServiceProviders
 (JNIEnv * env, jclass klass)
{
    return NULL;
}

