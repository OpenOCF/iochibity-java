#include <ctype.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "org_iochibity_OCF.h"
#include "ocf_init.h"
#include "ocf_exceptions.h"
#include "jni_utils.h"

#include "octypes.h"
#include "ocresource.h"
#include "ocstack.h"
#include "oic_malloc.h"
#include "oic_string.h"

#include "debug.h"		/* libcoap debugging */
#include "tinydtls/debug.h"		/* for tinydtls debugging */


/* PRIVATE */
pthread_t pt_work;
bool g_quit_flag = false;

const char* g_config_fname;

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
    printf("%s: fopening %s\n", __func__, g_config_fname);
    return fopen(g_config_fname, mode);
    /* return fopen(path, mode); */
}

/* PUBLIC */

/*
 * Class:     org_iochibity_OCF
 * Method:    Init
 * Signature: (Ljava/lang/String;IILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_iochibity_OCF_Init
(JNIEnv * env, jclass clazz, jstring j_ip_addr, jint port, jint mode, jstring j_config_fname)
{
    OC_UNUSED(clazz);
    printf("Java_org_iochibity_OCF_Init\n");

    coap_set_log_level(LOG_DEBUG);
    dtls_set_log_level(DTLS_LOG_DEBUG);

    /* First configure security */
    if (j_config_fname == NULL) {
	printf("config fname is null\n");
	j_config_fname = (*env)->NewStringUTF(env, g_config_fname);
    }
    g_config_fname = (*env)->GetStringUTFChars(env, j_config_fname, NULL);
    if (g_config_fname == NULL) {
	THROW_JNI_EXCEPTION("GetStringUTFChars");
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
	/* j_ip_addr = (*env)->NewStringUTF(env, cip_addr); */
	cip_addr = NULL;  /* (*env)->GetStringUTFChars(env, j_ip_addr, NULL); */
    } else {
	cip_addr = (*env)->GetStringUTFChars(env, j_ip_addr, NULL);
	if (cip_addr == NULL) {
	    THROW_JNI_EXCEPTION("GetStringUTFChars");
	}
    }
    printf("ip addr: [%s]\n", cip_addr);

    /* op_result = OCInit(cip_addr, (uint16_t)port, mode); */
    op_result = OCInit1(mode, OC_FLAG_SECURE, OC_FLAG_SECURE);
    if (op_result != OC_STACK_OK) {
	printf("OCStack init error\n");
	THROW_STACK_EXCEPTION(op_result, "Initialization failure");
    }
    (*env)->ReleaseStringUTFChars(env, j_ip_addr, cip_addr);

    /* return op_result; */
}

/*
 * Class:     org_iochibity_OCF
 * Method:    OCInit1
 * Signature: (III)I
 */
JNIEXPORT void JNICALL Java_org_iochibity_OCF_OCInit1
  (JNIEnv * env, jclass clazz, jint mode, jint server_flags, jint client_flags)
{
    OC_UNUSED(env);
    OC_UNUSED(clazz);
    OC_UNUSED(mode);
    OC_UNUSED(server_flags);
    OC_UNUSED(client_flags);
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
JNIEXPORT void JNICALL Java_org_iochibity_OCF_run
  (JNIEnv * env, jclass clazz)
{
    OC_UNUSED(env);
    OC_UNUSED(clazz);
    printf("Launching worker thread...\n");
    pthread_create (&pt_work, NULL, troutine_work, (void *)NULL);
    /* main thread has nothing to do. by calling pthread_exit it exits
       but the process continues, so any spawned threads do too. */
    /* pthread_exit(NULL); */
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
 * Method:    OCCancel
 * Signature: (Ljava/lang/Object;ILjava/lang/Object;B)I
 */
/* JNIEXPORT jint JNICALL Java_org_iochibity_OCF_OCCancel */
/*   (JNIEnv *, jobject, jobject, jint, jobject, jbyte); */

