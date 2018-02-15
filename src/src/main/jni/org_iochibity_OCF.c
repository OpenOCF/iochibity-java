/**
 * @file org_iochibity_OCF.c
 * @author Gregg Reynolds
 * @date December 2016
 *
 * @brief Wrappers for Init, OCProcess, etc.
 */

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "_threads.h"

#include "org_iochibity_OCF.h"
#include "jni_init.h"
#include "org_iochibity_Exceptions.h"
#include "jni_utils.h"

#include "openocf.h"
/* #include "octypes.h" */
/* #include "ocresource.h" */
/* #include "ocstack.h" */
/* #include "oic_malloc.h" */
/* #include "oic_string.h" */
/* #include "logger.h" */

/* PRIVATE */
#define TAG  "OCF"

THREAD_T tid_work;
bool g_quit_flag = false;

/* thread routine - service client requests */
THREAD_EXIT_T troutine_work(void *arg)
{
    OC_UNUSED(arg);
    printf("Entering server work thread...\n");

    while (!g_quit_flag) {
	if (OCProcess() != OC_STACK_OK) {
	    printf("OCStack process error\n");
	}
	sleep(1);
    }
    printf("Exiting server work thread...\n");
    /* we're the only thread left, pthread_exit(NULL) would kill us,
       but not the process. */
    /* exit(0); */

    /* FIXME: pthreads return void*, c11 threads return int */
    return THREAD_EXIT_OK;
}

/* PUBLIC */

/*
 * Class:     org_iochibity_OCF
 * Method:    Init
 * Signature: (ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL
Java_org_iochibity_OCF_Init__I (JNIEnv * env, jclass klass, jint mode)
/* JNIEXPORT void JNICALL Java_org_iochibity_OCF_Init__ILjava_lang_String_2 */
/* (JNIEnv * env, jclass klass, jint mode, jstring j_config_fname) */
{
    OC_UNUSED(klass);
    printf("%s | %s ENTRY, mode: %d\n", __FILE__, __func__, mode);

    /* moved to openocf.Service::config */
    /* /\* First configure security *\/ */
    /* if (j_config_fname == NULL) { */
    /* 	printf("config fname is null\n"); */
    /* 	j_config_fname = (*env)->NewStringUTF(env, g_config_fname); */
    /* } */
    /* g_config_fname = (*env)->GetStringUTFChars(env, j_config_fname, NULL); */
    /* if (g_config_fname == NULL) { */
    /* 	THROW_JNI_EXCEPTION("GetStringUTFChars"); */
    /* } */
    /* OCPersistentStorage ps = {server_fopen, fread, fwrite, fclose, unlink}; */
    /* printf("calling OCRegisterPersistentStorageHandler: %s\n", g_config_fname); */
    /* OCRegisterPersistentStorageHandler(&ps); */

    /* (*env)->ReleaseStringUTFChars(env, j_config_fname, g_config_fname); */

    /* const char *cip_addr = ""; */
    /* if (j_ip_addr == NULL) { */
    /* 	printf("ip addr null\n"); */
    /* 	/\* j_ip_addr = (*env)->NewStringUTF(env, cip_addr); *\/ */
    /* 	cip_addr = NULL;  /\* (*env)->GetStringUTFChars(env, j_ip_addr, NULL); *\/ */
    /* } else { */
    /* 	cip_addr = (*env)->GetStringUTFChars(env, j_ip_addr, NULL); */
    /* 	if (cip_addr == NULL) { */
    /* 	    THROW_JNI_EXCEPTION("GetStringUTFChars"); */
    /* 	} */
    /* } */
    /* printf("ip addr: [%s]\n", cip_addr); */


    /* Then initialize supervisor */
    OCStackResult op_result;
    /* op_result = OCInit(cip_addr, (uint16_t)port, mode); */
    op_result = OCInit1(mode, OC_FLAG_SECURE, OC_FLAG_SECURE);
    if (op_result != OC_STACK_OK) {
	printf("OCStack init error\n");
	THROW_STACK_EXCEPTION(op_result, "Initialization failure");
    }
    /* (*env)->ReleaseStringUTFChars(env, j_ip_addr, cip_addr); */

    /* return op_result; */
}

/*
 * Class:     org_iochibity_OCF
 * Method:    OCInit
 * Signature: (III)I
 */
JNIEXPORT void JNICALL Java_org_iochibity_OCF_Init__III
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

 /* FIXME: call this "observe"? */
JNIEXPORT void JNICALL Java_org_iochibity_OCF_run
  (JNIEnv * env, jclass clazz)
{
    OC_UNUSED(env);
    OC_UNUSED(clazz);
    OIC_LOG_V(DEBUG, TAG, "%s ENTRY", __func__);
    /* FIXME: broken! OCProcess is not thread safe */
    /* printf("Launching worker thread...\n"); */
    /* THREAD_CREATE_DEFAULT((THREAD_T*)&tid_work, */
    /* 			  (THREAD_START_T)troutine_work, */
    /* 			  (void *)NULL); */
    while (!g_quit_flag) {
	if (OCProcess() != OC_STACK_OK) {
	    printf("OCStack process error\n");
	}
	sleep(1);
    }

    /* main thread has nothing to do. by calling pthread_exit it exits
       but the process continues, so any spawned threads do too. */
    /* THREAD_EXIT(THREAD_EXIT_OK); */
    OIC_LOG_V(DEBUG, TAG, "%s EXIT", __func__);
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

