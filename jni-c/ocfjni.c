#include <ctype.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "org_iochibity_OCF.h"

#include "octypes.h"
#include "ocresource.h"
#include "ocstack.h"

pthread_t pt_work;
bool g_quit_flag = false;

char* g_config_fname;

/* thread routine - service client requests */
void *troutine_work(void *arg)
{
    OC_UNUSED(arg);
    printf("Entering server work thread...\n");

    while (!g_quit_flag) {
	if (OCProcess() != OC_STACK_OK) {
	    printf("OCStack process error\n");
	}
    }
    printf("Exiting server work thread...\n");
    /* we're the only thread left, pthread_exit(NULL) would kill us,
       but not the process. */
    /* exit(0); */
    return NULL;
}

FILE* server_fopen(const char *path, const char *mode)
{
    (void)path;
    printf("%s: fopening %s", __func__, g_config_fname);
    return fopen(g_config_fname, mode);
    /* return fopen(path, mode); */
}

/*
 * Class:     org_iochibity_OCF
 * Method:    Init
 * Signature: (Ljava/lang/String;CI)I
 */
/* JNIEXPORT jint JNICALL Java_org_iochibity_OCF_Init */
/*   (JNIEnv *, jobject, jstring ip_addr, jchar port, jint mode) */
JNIEXPORT jint JNICALL Java_org_iochibity_OCF_Init
(JNIEnv * env, jclass clazz, jstring j_ip_addr, jint port, jint mode, jstring j_config_fname)
{
    OC_UNUSED(clazz);
    printf("Java_org_iochibity_OCF_Init\n");

    /* First configure security */
    if (j_config_fname == NULL) {
	printf("config fname is null\n");
	j_config_fname = (*env)->NewStringUTF(env, g_config_fname);
    }
    g_config_fname = (*env)->GetStringUTFChars(env, j_config_fname, NULL);
    if (g_config_fname == NULL) {
	return 0; /* OutOfMemoryError already thrown */
    }
    OCPersistentStorage ps = {server_fopen, fread, fwrite, fclose, unlink};
    printf("calling OCRegisterPersistentStorageHandler: %s\n", g_config_fname);
    OCRegisterPersistentStorageHandler(&ps);
    /* (*env)->ReleaseStringUTFChars(env, j_config_fname, g_config_fname); */

    /* Then initialize supervisor */
    OCStackResult op_result;
    const char *cip_addr = "";
    if (j_ip_addr == NULL) {
	printf("ip addr null\n");
	j_ip_addr = (*env)->NewStringUTF(env, cip_addr);
	cip_addr = (*env)->GetStringUTFChars(env, j_ip_addr, NULL);
    } else {
	cip_addr = (*env)->GetStringUTFChars(env, j_ip_addr, NULL);
	if (cip_addr == NULL) {
	    return 0; /* OutOfMemoryError already thrown */
	}
    }
    printf("ip addr: [%s]\n", cip_addr);

    op_result = OCInit(cip_addr, (uint16_t)port, mode);
    if (op_result != OC_STACK_OK) {
	printf("OCStack init error\n");
    }
    (*env)->ReleaseStringUTFChars(env, j_ip_addr, cip_addr);

    return op_result;
}

/*
 * Class:     org_iochibity_OCF
 * Method:    OCInit1
 * Signature: (III)I
 */
JNIEXPORT jint JNICALL Java_org_iochibity_OCF_OCInit1
  (JNIEnv * env, jclass clazz, jint mode, jint server_flags, jint client_flags)
{
    OC_UNUSED(env);
    OC_UNUSED(clazz);
    OC_UNUSED(mode);
    OC_UNUSED(server_flags);
    OC_UNUSED(client_flags);
    return 0;
}

/*
 * Class:     org_iochibity_OCF
 * Method:    OCStartMulticastServer
 * Signature: ()I
 */
/* JNIEXPORT jint JNICALL Java_org_iochibity_OCF_OCStartMulticastServer */
/*   (JNIEnv *, jobject); */

/*
 * Class:     org_iochibity_OCF
 * Method:    OCStopMulticastServer
 * Signature: ()I
 */
/* JNIEXPORT jint JNICALL Java_org_iochibity_OCF_OCStopMulticastServer */
/*   (JNIEnv *, jobject); */

/*
 * Class:     org_iochibity_OCF
 * Method:    run
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_iochibity_OCF_run
  (JNIEnv * env, jclass clazz)
{
    OC_UNUSED(env);
    OC_UNUSED(clazz);
    printf("Launching worker thread...\n");
    pthread_create (&pt_work, NULL, troutine_work, (void *)NULL);
    /* main thread has nothing to do. by calling pthread_exit it exits
       but the process continues, so any spawned threads do too. */
    /* pthread_exit(NULL); */
    return 0;
}

/*
 * Class:     org_iochibity_OCF
 * Method:    stop
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_iochibity_OCF_stop
  (JNIEnv * env, jclass clazz)
{
    OC_UNUSED(env);
    OC_UNUSED(clazz);
    g_quit_flag = true;
}

/*
 * Class:     org_iochibity_OCF
 * Method:    OCDoResource
 * Signature: (Ljava/lang/Object;ILjava/lang/String;Ljava/lang/Object;Ljava/lang/Object;IILjava/lang/Object;Ljava/lang/Object;B)I
 */
/* JNIEXPORT jint JNICALL Java_org_iochibity_OCF_OCDoResource */
/*   (JNIEnv *, jobject, jobject, jint, jstring, jobject, jobject, jint, jint, jobject, jobject, jbyte); */

/*
 * Class:     org_iochibity_OCF
 * Method:    OCCancel
 * Signature: (Ljava/lang/Object;ILjava/lang/Object;B)I
 */
/* JNIEXPORT jint JNICALL Java_org_iochibity_OCF_OCCancel */
/*   (JNIEnv *, jobject, jobject, jint, jobject, jbyte); */

/*
 * Class:     org_iochibity_OCF
 * Method:    OCDoResponse
 * Signature: (Ljava/lang/Object;)I
 */
/* JNIEXPORT jint JNICALL Java_org_iochibity_OCF_OCDoResponse */
/*   (JNIEnv *, jobject, jobject); */

/*
 * Class:     org_iochibity_OCF
 * Method:    setPlatformInfo
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_org_iochibity_OCF_setPlatformInfo
  (JNIEnv * env, jclass clazz,
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
    OC_UNUSED(clazz);
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
	    return 0; /* OutOfMemoryError already thrown */
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
	    return 0; /* OutOfMemoryError already thrown */
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
	    return 0; /* OutOfMemoryError already thrown */
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
	    return 0; /* OutOfMemoryError already thrown */
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
	    return 0; /* OutOfMemoryError already thrown */
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
	    return 0; /* OutOfMemoryError already thrown */
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
	    return 0; /* OutOfMemoryError already thrown */
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
	    return 0; /* OutOfMemoryError already thrown */
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
	    return 0; /* OutOfMemoryError already thrown */
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
	    return 0; /* OutOfMemoryError already thrown */
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
	    return 0; /* OutOfMemoryError already thrown */
	}
    }
    printf("c system time: [%s]\n", platform_info.systemTime);

    printf("Setting platform info...\n");
    OCStackResult op_result;
    op_result = OCSetPlatformInfo(platform_info);

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
    return 0;
}

/*
 * Class:     org_iochibity_OCF
 * Method:    setDeviceInfo
 * Signature: (Ljava/lang/String;Ljava/lang/Object;Ljava/lang/String;[Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_org_iochibity_OCF_setDeviceInfo
  (JNIEnv * env, jclass clazz,
   jstring j_device_name, jobjectArray j_types, jstring j_spec_version, jobjectArray j_data_model_versions)
{
    OC_UNUSED(env);
    OC_UNUSED(clazz);
    OC_UNUSED(j_types);
    OC_UNUSED(j_data_model_versions);
    static OCDeviceInfo device_info =
	{
	    .deviceName = "Default Device Name",
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
	    return 0; /* OutOfMemoryError already thrown */
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
	    return 0; /* OutOfMemoryError already thrown */
	}
    }
    printf("c platform id: [%s]\n", device_info.specVersion);



    printf("Setting device info...\n");
    OCStackResult op_result;
    op_result = OCSetDeviceInfo(device_info);
    if (op_result != OC_STACK_OK) {
        printf("Device Registration failed!\n");
        /* exit (EXIT_FAILURE); */
    }

    (*env)->ReleaseStringUTFChars(env, j_device_name, device_info.deviceName);
    (*env)->ReleaseStringUTFChars(env, j_spec_version, device_info.specVersion);

    return 0;
}
