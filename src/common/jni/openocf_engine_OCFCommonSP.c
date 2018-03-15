#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <jni.h>


#include "openocf.h"
#include "openocf_engine_OCFCommonSP.h"

/* FIXME: split these out: */
#include "openocf_standard_OCFServices.h"
#include "openocf_android_OCFServices.h"

#include "org_iochibity_Exceptions.h"
#include "_threads.h"

#include "jni_init.h"
#include "jni_utils.h"

/* PRIVATE */
#define TAG  "openocf_engine_OCFCommonSP"

FILE                   *logfd;

const char *SVRS_CONFIG_FNAME = "svrs_config.cbor";
const char* g_config_fname;

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

FILE* server_fopen(const char *path, const char *mode)
{
    OIC_LOG_V(DEBUG, TAG, "%s ENTRY, path: %s", __func__, path);
    (void)path;
    /* printf("%s path: %s\n", __func__, path); */
    if (0 == strcmp(path, SVR_DB_DAT_FILE_NAME)) /* "oic_svr_db.dat" */
    {
    	/* override default file */
	OIC_LOG_V(DEBUG, TAG, "Overriding path with: %s", g_config_fname);
        return fopen(g_config_fname, mode);
    }
    else
    {
        return fopen(path, mode);
    }
    return NULL;
}

static OCPersistentStorage ps = {server_fopen, fread, fwrite, fclose, unlink};

JNIEXPORT void JNICALL
Java_openocf_standard_OCFServices_config_1svrs (JNIEnv *env, jclass klass, jstring j_svrs_config_fname)
{
    OIC_LOG_V(DEBUG, TAG, "%s ENTRY", __func__);
    if (j_svrs_config_fname == NULL) {
    	OIC_LOG_V(DEBUG, TAG, "svrs config fname is null; defaulting to %s", SVRS_CONFIG_FNAME);
	g_config_fname = SVRS_CONFIG_FNAME;
    	/* j_svrs_config_fname = (*env)->NewStringUTF(env, SVRS_CONFIG_FNAME); */
    } else {
	g_config_fname = (*env)->GetStringUTFChars(env, j_svrs_config_fname, NULL);
    	OIC_LOG_V(DEBUG, TAG, "svrs config fname: %s", g_config_fname);
    }
    if (g_config_fname == NULL) {
    	THROW_JNI_EXCEPTION("GetStringUTFChars");
    }
    OIC_LOG_V(DEBUG, TAG, "calling OCRegisterPersistentStorageHandler: %s", g_config_fname);
    OCRegisterPersistentStorageHandler(&ps);
    printf("called OCRegisterPersistentStorageHandler: %s\n", g_config_fname);
}

JNIEXPORT void JNICALL
Java_openocf_standard_OCFServices_config_1logging(JNIEnv *env, jclass klass, jstring j_logfname)
{
    printf("config_logging ENTRY: %s\n",
	   (j_logfname == NULL)? "NULL"
	   : (char*) (*env)->GetStringUTFChars(env, j_logfname, NULL));
    if (j_logfname) {
	logfd = fopen((char*) (*env)->GetStringUTFChars(env, j_logfname, NULL), "w");
	if (logfd)
	    OCLogInit(logfd);
	else {
	    printf("Logfile fopen failed\n");
	    exit(EXIT_FAILURE);
	}
    } else {
	OCLogInit(NULL);
#ifdef TB_LOG
	OCSetLogLevel(DEBUG, false);
#endif
	OIC_LOG_V(DEBUG, TAG, "%s OCLogInit done", __func__);
    /* LOG_EMERG, LOG_ALERT, LOG_CRIT, LOG_WARNING, LOG_NOTICE, LOG_INFO, LOG_DEBUG */
    /* coap_set_log_level(LOG_WARNING); */

    /* dtls_set_log_level(DTLS_LOG_WARN); /\* DEBUG, INFO,  NOTICE, WARN, CRIT ... *\/ */


    }
}

JNIEXPORT jstring JNICALL
Java_openocf_engine_OCFCommonSP_configuration (JNIEnv * env, jclass klass)
{
    const char * str = configuration();
    int len = strlen(str);
    jchar *str1;
    str1 = (jchar *)(malloc(len * sizeof(jchar)));

    for (int i = 0; i < len; i++) {
	str1[i] = (unsigned char)str[i];
    }
    jstring result = (*env)->NewString(env, str1, len);
    free(str1);
    return result;
}

/* Android API */
JNIEXPORT void JNICALL
Java_openocf_android_OCFServices_config_1logging (JNIEnv *env, jclass klass, jstring s)
{
    OCLogInit(NULL);
#ifdef TB_LOG
	OCSetLogLevel(DEBUG, false);
#endif
	OIC_LOG_V(DEBUG, TAG, "%s OCLogInit done", __func__);
}

/*
 * Class:     openocf_android_OCFServices
 * Method:    config_svrs
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL
Java_openocf_android_OCFServices_config_1svrs (JNIEnv *env, jclass klass, jstring j_svrs_config_fname)
{
    OIC_LOG_V(DEBUG, TAG, "%s ENTRY", __func__);
    if (j_svrs_config_fname == NULL) {
    	OIC_LOG_V(DEBUG, TAG, "svrs config fname is null; defaulting to %s", SVRS_CONFIG_FNAME);
	g_config_fname = SVRS_CONFIG_FNAME;
    	/* j_svrs_config_fname = (*env)->NewStringUTF(env, SVRS_CONFIG_FNAME); */
    } else {
	g_config_fname = (*env)->GetStringUTFChars(env, j_svrs_config_fname, NULL);
    	OIC_LOG_V(DEBUG, TAG, "svrs config fname: %s", g_config_fname);
    }
    if (g_config_fname == NULL) {
    	THROW_JNI_EXCEPTION("GetStringUTFChars");
    }
    OIC_LOG_V(DEBUG, TAG, "calling OCRegisterPersistentStorageHandler: %s", g_config_fname);
    OCRegisterPersistentStorageHandler(&ps);
    printf("called OCRegisterPersistentStorageHandler: %s\n", g_config_fname);
}

/* PUBLIC */

/*
 * Class:     openocf_engine_OCFCommonSP
 * Method:    Init
 * Signature: (ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL
Java_openocf_engine_OCFCommonSP_Init__I(JNIEnv * env, jclass klass, jint mode)
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
 * Class:     openocf_engine_OCFCommonSP
 * Method:    OCInit
 * Signature: (III)I
 */
JNIEXPORT void JNICALL Java_openocf_engine_OCFCommonSP_Init__III
  (JNIEnv * env, jclass clazz, jint mode, jint server_flags, jint client_flags)
{
    OC_UNUSED(env);
    OC_UNUSED(clazz);
    OC_UNUSED(mode);
    OC_UNUSED(server_flags);
    OC_UNUSED(client_flags);
}

/*
 * Class:     openocf_engine_OCFCommonSP
 * Method:    OCStartMulticastServer
 * Signature: ()I
 */
/* JNIEXPORT jint JNICALL Java_openocf_engine_OCFCommonSP_OCStartMulticastServer */
/*   (JNIEnv *, jobject); */

/*
 * Class:     openocf_engine_OCFCommonSP
 * Method:    OCStopMulticastServer
 * Signature: ()I
 */
/* JNIEXPORT jint JNICALL Java_openocf_engine_OCFCommonSP_OCStopMulticastServer */
/*   (JNIEnv *, jobject); */

/*
 * Class:     openocf_engine_OCFCommonSP
 * Method:    run
 * Signature: ()I
 */

 /* FIXME: call this "observe"? */
JNIEXPORT void JNICALL Java_openocf_engine_OCFCommonSP_run
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
 * Class:     openocf_engine_OCFCommonSP
 * Method:    stop
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_openocf_engine_OCFCommonSP_stop
  (JNIEnv * env, jclass clazz)
{
    OC_UNUSED(env);
    OC_UNUSED(clazz);
    g_quit_flag = true;
}

/*
 * Class:     openocf_engine_OCFCommonSP
 * Method:    OCCancel
 * Signature: (Ljava/lang/Object;ILjava/lang/Object;B)I
 */
/* JNIEXPORT jint JNICALL Java_openocf_engine_OCFCommonSP_OCCancel */
/*   (JNIEnv *, jobject, jobject, jint, jobject, jbyte); */
