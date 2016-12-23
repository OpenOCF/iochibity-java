/**
 * @file ocf_ObservationOut.c
 * @author Gregg Reynolds
 * @date December 2016
 *
 * @brief unused
 */

#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "org_iochibity_ObservationOut.h"
#include "jni_utils.h"
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
 * Class:     org_iochibity_ResponseOut
 * Method:    createResponseOut
 * Signature: (Lorg/iochibity/RequestIn;Lorg/iochibity/PayloadList;)J
 */
JNIEXPORT jlong JNICALL Java_org_iochibity_ResponseOut_createResponseOut
  (JNIEnv * env, jobject this, jobject j_request_in, jobject j_payload_list)

/* OBSOLETE */

{
    OC_UNUSED(this);
    OC_UNUSED(j_request_in);
    OC_UNUSED(j_payload_list);
    printf("Java_org_iochibity_ResponseOut_createResponseOut ENTRY\n");
    OCEntityHandlerResponse * response =
                        (OCEntityHandlerResponse *) OICCalloc(1, sizeof(*response));
    if (!response) {
	THROW_EH_EXCEPTION(OC_EH_ERROR, "OCEntityHandlerResponse calloc failure\n");
	return 0;
    }

    printf("Java_org_iochibity_ResponseOut_createResponseOut EXIT\n");
    return (intptr_t) response;
}

