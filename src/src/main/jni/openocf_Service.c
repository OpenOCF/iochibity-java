#include "openocf.h"
#include "openocf_Service.h"

#include "org_iochibity_Exceptions.h"

#include <jni.h>
#include <stdlib.h>
#include <string.h>

FILE                   *logfd;

const char *SVRS_CONFIG_FNAME = "svrs_config.cbor";
const char* g_config_fname;

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
Java_openocf_Service_config_1svrs (JNIEnv *env, jclass klass, jstring j_svrs_config_fname)
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
Java_openocf_Service_config_1logging(JNIEnv *env, jclass klass, jstring j_logfname)
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
Java_openocf_Service_configuration (JNIEnv * env, jclass klass)
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
