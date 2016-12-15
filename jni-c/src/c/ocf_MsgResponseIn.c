/* ocf_MsgResponseIn.c */

#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "org_iochibity_MsgResponseIn.h"
#include "jni_utils.h"
#include "ocf_init.h"
#include "ocf_exceptions.h"

#include "octypes.h"
#include "oic_malloc.h"
#include "ocpayload.h"
#include "ocresource.h"
#include "ocresourcehandler.h"
#include "ocstack.h"

/* PRIVATE */


/* PUBLIC */

/*
 * Class:     org_iochibity_MsgResponseIn
 * Method:    logPayload
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_iochibity_MsgResponseIn_logPayload
(JNIEnv * env, jobject this)
{
    printf("Java_org_iochibity_MsgResponseIn_logPayload ENTRY\n");
    OCClientResponse* c_OCClientResponse = NULL;
    c_OCClientResponse = (OCClientResponse*)(intptr_t)
	(*env)->GetLongField(env, this, FID_MSG_LOCAL_HANDLE);
    if (c_OCClientResponse == NULL) {
	THROW_JNI_EXCEPTION("GetLongField failed for FID_MsgRspIn_PTR_RESPONSE on MsgRequestIn\n");
	return;
    }
    OCPayload* c_payload = c_OCClientResponse->payload;
    switch (c_payload->type) {
    case PAYLOAD_TYPE_PLATFORM:
    	printf("\tPLATFORM\n");
    	break;
    default:
    	printf("\tFOOBAR\n");
    	break;
    }
}
