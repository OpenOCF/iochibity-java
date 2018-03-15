/**
 * @file openocf_engine_OCFClientSP.c
 * @author Gregg Reynolds
 * @date December 2016
 *
 * @brief implementations of `registerX` routines etc.
 */

#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "openocf_engine_OCFClientSP.h"
#include "_openocf_app_CoResourceSP.h"
#include "jni_init.h"
#include "org_iochibity_Exceptions.h"
#include "jni_utils.h"

#include "openocf.h"
/* #include "ocresource.h" */
/* #include "ocstack.h" */
/* #include "octypes.h" */
/* #include "logger.h" */

/* PRIVATE */

/* We use tls_CoRSP to "pin" the Java object implementing the coReact callback. */
/* FIXME: make sure it gets freed appropriately */
THREAD_LOCAL jobject tls_CoRSP			= NULL;

extern THREAD_LOCAL request_out_t* tls_request_out;

#define TAG  "CO_SERVICE_MANAGER"

/* **************************************************************** */
/* PUBLIC */
/* **************************************************************** */

/*
 * Class:     openocf_engine_OCFClientSP
 * Method:    configurePlatformSP
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_openocf_engine_OCFClientSP_configurePlatformSP
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
 * Class:     openocf_engine_OCFClientSP
 * Method:    configureDeviceSP
 * Signature: (Ljava/lang/String;[Ljava/lang/String;Ljava/lang/String;[Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_openocf_engine_OCFClientSP_configureDeviceSP
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
 * Class:     openocf_engine_OCFClientSP
 * Method:    RegisterDefaultServiceRoutine
 * Signature: (Ljava/lang/Object;Ljava/lang/Object;)I
 */
JNIEXPORT void JNICALL Java_openocf_engine_OCFClientSP_registerDefaultServiceRoutine
(JNIEnv * env, jclass klass, jobject j_resource_service_provider, jobject j_callback_param)
{
    OC_UNUSED(env);
    OC_UNUSED(klass);
    OC_UNUSED(j_resource_service_provider);
    OC_UNUSED(j_callback_param);
}

/*
 * Class:     openocf_engine_OCFClientSP
 * Method:    registerDefaultCoResourceSP
 * Signature: (Lorg/iochibity/CoResourceSP;)Lorg/iochibity/CoResourceSP;
 */
JNIEXPORT jobject JNICALL Java_openocf_engine_OCFClientSP_registerDefaultCoResourceSP
(JNIEnv * env, jclass klass, jobject j_CoResourceSP)
{
    return j_CoResourceSP;
}

/*
 * Class:     openocf_engine_OCFClientSP
 * Method:    registerCoResourceSP
 * Signature: (Lorg/iochibity/CoResourceSP;)Lorg/iochibity/CoResourceSP;
 */
JNIEXPORT jobject JNICALL Java_openocf_engine_OCFClientSP_registerCoResourceSP
(JNIEnv * env, jclass klass, jobject j_CoResourceSP)
{
    OIC_LOG_V(DEBUG, TAG, "%s ENTRY", __func__);
    /* pseudo-registration - coSPs call CoResourceSP.coExhibit,
       which is a native method
       Java_org_iochibity_CoResourceSP_coExhibit, which runs
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
 * Class:     openocf_engine_OCFClientSP
 * Method:    registeredCoResourceSPs
 * Signature: ()Ljava/util/List;
 */
JNIEXPORT jobject JNICALL Java_openocf_engine_OCFClientSP_registeredCoResourceSPs
(JNIEnv * env, jclass klass)
{
    OIC_LOG_V(DEBUG, __FILE__, "[%d] %s ENTRY", __LINE__, __func__);
    /* OIC_LOG_DISCOVERY_RESPONSE(DEBUG, TAG, g_OCClientResponse); */

    // int oocf_cosp_mgr_list_coresource_uris(/* out */ const char ***uri_list)

    // return list of URLs?
    OIC_LOG_V(DEBUG, __FILE__, "[%d] %s EXIT", __LINE__, __func__);
    return NULL;
}

/*
 * Class:     openocf_engine_OCFClientSP
 * Method:    getRelatedCoResourceSPs
 * Signature: ()Ljava/util/List;
 */
JNIEXPORT jobject JNICALL Java_openocf_engine_OCFClientSP_getRelatedCoResourceSPs
 (JNIEnv * env, jclass klass)
{
    return NULL;
}

/*
 * Class:     openocf_CoResourceSP
 * Method:    coExhibit
 * Signature: ()V
 */
/**
 * @brief Wrapper for `OCDoResource`; called by called by the user to
 * send a request to a server.
 *
 * It wraps `OCDoResource`.  The parameters to `OCDoResource` are
 * reified in a hidden request_out_t struct.
 *
 * implicit params: tls_request_out, tls_response_in
 *
 * @param [in] env JNIEnv pointer
 *
 * @param [in] this_CoSP reference to the calling `CoResourceSP`
 * instance
 *
 * @return void
 *
 * @throws org.iochibity.exceptions.JNIRuntimeException Indicates a
 * bad result from a JNI API routine (e.g. `GetIntField`)
 *
 * @see Java_openocf_ServiceProvider_exhibit
 * @see c_CoResourceSP_coReact
 */

/*
 * Class:     openocf_engine_OCFClientSP
 * Method:    coExhibit
 * Signature: (Lopenocf/app/CoResourceSP;)V
 */
JNIEXPORT void JNICALL
Java_openocf_engine_OCFClientSP_coExhibit(JNIEnv *env, jclass klass, jobject the_CoRSP)
{
    OC_UNUSED(env);
    OIC_LOG_V(DEBUG, TAG, "%s: ENTRY, tid %ld\n", __func__, (intptr_t)THREAD_ID);

    /* We will pass the_CoRSP as the callbackParam to OCDoResource.
       It will be passed back on a different thread, so after we call
       OCDoResource, we will store our thread-local request_out struct
       in the global map, so it can later be retrieved for the
       matching response.  We already have our thread-local
       request_out - created by the ctor, possibly updated by app code
       - so we can use it directly to parameterize OCDoReource.  Note
       that request_out is an internal, implementation struct; the API
       only presents CoSP getters and setters, which use request_out
       as needed.
    */

    /* in order to be able to reliably call-back our react method, we
       must pin the object */
    /* FIXME: don't forget to free it. */
    tls_CoRSP = (*env)->NewGlobalRef(env, the_CoRSP);
    OIC_LOG_V(DEBUG, TAG, "%s: this CoRSP %p\n", __func__, (void*)tls_CoRSP);

    OCDoHandle c_TxnId = NULL;
    txn_t* txn;

    OCStackResult ret;
    OCCallbackData cbData;

    /* does this request follow receipt of a response? */
    /* FIXME: this logic is semi-broken.  tls_response_in will be on a
       react thread, we're on an coExhibit thread, which may be
       different. */

    if (tls_response_in == NULL) {
	/* NULL response does not necessarily mean we're starting from
	   scratch!  it just means we're not on a react thread.  but we
	   could be (co-)reacting to a response that was recd on another
	   thread. */

	/* FIXME: use case: we're using a predefined address */
	printf("tls_response_in is NULL\n");
	printf("tls_request_out %ld\n", (intptr_t)tls_request_out);
	printf("tls_request_out->destination %ld\n", (intptr_t)tls_request_out->destination );
	if (tls_request_out->routingIsMulticast) {
	    /* if (tls_request_out->destination->flags & OC_MULTICAST) { */
		/* if (tls_request_out->method == OC_REST_GET) { */
		tls_request_out->method = OC_REST_DISCOVER;
		/* this is broken, but it's the way the C API works: */
		tls_request_out->destination = NULL;
	    /* } */
	}
	cbData.cb = _openocf_app_CoResourceSP_coReact; /* in spi_coresource_sp.c */
	cbData.context = (void*)(long)tls_CoRSP;
	cbData.cd = NULL;
	ret = OCDoResource(&c_TxnId,	/* OCDoHandle = void* */
			   tls_request_out->method,      // (OCMethod)c_method,
			   tls_request_out->requestUri,  // c_uri,
			   tls_request_out->destination, // c_destDevAddr,
			   tls_request_out->payload,     // OCPayload* payload
			   tls_request_out->connectivityType,
			   /* CT_ADAPTER_IP */
			   /* CT_FLAG_SECURE,	 /\* OCConnectivityType conn_type *\/ */
			   OC_LOW_QOS,
			   &cbData,	/* OCCallbackData* cbData */
			   NULL,	/* OCHeaderOptions* options */
			   0);	/* uint8_t numOptions */

	/* first: update the TLS request data with the newly coined txnId token */
	tls_request_out->txnId = c_TxnId;

	/* now copy tls request_out to the global list, so that we can
	 * correlate with response_in in the react routine. we cannot
	 * just refer to the TLS data since it will have gone away
	 * if/when the thread died. */
	/* why not just use the global list directly, no tls? because we
	   don't have a key (txnId) until we call OCDoResource. */
	/* FIXME: do we really need anything other than method?
	   everything else will be available in response. */

	txn                         = OICCalloc(sizeof (txn_t), 1);
	txn->txnId                  = tls_request_out->txnId;
	txn->method                 = tls_request_out->method;
	if (tls_request_out->destination) {
	    txn->routingIsMulticast = tls_request_out->destination->flags & OC_MULTICAST;
	} else {
	    txn->routingIsMulticast =tls_request_out->routingIsMulticast;
	}
	txn->next                   = NULL;

    /* FIXME: now delete tls_request_out?  no point in keeping it around */

	/* size_t length; /\* this will hold the length of string copied *\/ */
	/* char buffer[MAX_URI_LENGTH]; /\* fast mem on stack *\/ */
	/* length			= strlcpy(buffer, */
	/* 				  tls_request_out->requestUri, */
	/* 				  sizeof(buffer)); */
	/* if (length < sizeof(buffer)) { */
	/*     rqst->requestUri	= buffer;	/\* it fit, use the stack buffer *\/ */
	/* } else { */
	/*     THROW_JNI_EXCEPTION("URI too long!"); */
	/* } */

	/* // FIXME: verify the ptr stuff is heap-allocated */
	/* rqst->destination = tls_request_out->destination; */
	/* rqst->payload = tls_request_out->payload; */
	/* rqst->connectivityType = tls_request_out->connectivityType; */
	/* rqst->qos = tls_request_out->qos; */
	/* rqst->cbData = tls_request_out->cbData; */
	/* rqst->options = tls_request_out->options; */
	/* rqst->numOptions = tls_request_out->numOptions; */
    } else {

	printf("tls_response_in is NOT null\n");
	/* we have an OCClientResponse - this CoSP is interacting with an SP */
	/* that probably - but not necessarily - means that we're
	   sending a request following a discovery? */

      /* But we're coExhibiting, which means we we have a
       * tls_request_out, and we may be on a "driver" thread, or we
       * could be on a coReacting thread, e.g. if we are coreacting to
       * a server response.  In that case, the args we need for
       * OCDoResource are on a react thread, in tls_response_in.  so
       * if we have a tls_response_in, that must mean that we are in
       * fact on the react thread - maybe we discovered a resource and
       * are now trying to read it.  in that case what we're doing now
       * may be part of our reaction; but since reactions are
       * triggered by requests, not responses, we should call this a
       * co-response, since requests are not transactionally paired
       * with previous responses.
       */

	/* so a rule is: if you want to react to a server response,
	   you must do it on the thread that is running coReact. do
	   not spin off a worker thread and run coExhibit from there.
	*/

	/* FIXME: a little doco and terminology will help: if a client
	   wants to react to a server's response, it can only do so by
	   starting a new interaction, which has no protocol-defined
	   relation to the previous one.  So we can call this a
	   co-reaction; the CoSP's co-reaction to a response is dual
	   to the SP's reaction to a request.  Maybe we should even
	   call the CoSP's response handler "coReact" instead of
	   "react" to highlight the difference.  Reaction will always
	   result in a response that is paired with the request,
	   whereas CoReaction never results in a response, although it
	   may involve new transactions.
	*/

      /* FIXME: pull method from the dual react thread */
      /* TEMPORARY: pull from java object: */
      /* OCMethod c_method = (OCMethod)(*env)->GetIntField(env, */
      /* 							the_CoRSP, */
      /* 							FID_COSP_METHOD); */
      /* if (c_method == 0) { */
      /* 	THROW_JNI_EXCEPTION("Method OC_REST_NOMETHOD (0) not allowed"); */
      /* 	return; */
      /* } */
      /* uri */
      /* For thread safety, we must use the Uri from the response */
      /* char* c_uri tls_response_in->resourceUri, NULL); */

      /* if (c_uri == NULL) { */
      /* 	THROW_JNI_EXCEPTION("No resourceUri in tls_response_in\n"); */
      /* 	return; */
      /* } */

      /* ADDRESSING */
      /* MULTICAST: dev addr is null, connectivity type is used */
      /* UNICAST:   dev addr is used, connectivity type is ignored? */

      OCDevAddr* c_destDevAddr = NULL;
      /* jobject j_destDevAddr = NULL; */
      /* j_destDevAddr = (*env)->GetObjectField(env, tls_CoRSP, FID_COSP_DESTINATION); */

      printf("UNICASTING\n");
      /* we have an OCDevAddr from a response */
      c_destDevAddr = &(tls_response_in->response->devAddr);

      cbData.cb = _openocf_app_CoResourceSP_coReact;
      cbData.context = (void*)(long)tls_CoRSP;
      cbData.cd = NULL;
      /* ret = OCDoResource(&c_TxnId,	/\* OCDoHandle = void* *\/ */
      /* 			 (OCMethod)c_method, */
      /* 			 c_uri, */
      /* 			 c_destDevAddr, */
      /* 			 NULL,		 /\* OCPayload* payload *\/ */
      /* 			 CT_DEFAULT, */
      /* 			 /\* CT_ADAPTER_IP *\/ */
      /* 			 /\* CT_FLAG_SECURE,	 /\\* OCConnectivityType conn_type *\\/ *\/ */
      /* 			 OC_LOW_QOS, */
      /* 			 &cbData,	/\* OCCallbackData* cbData *\/ */
      /* 			 NULL,	/\* OCHeaderOptions* options *\/ */
      /* 			 0);	/\* uint8_t numOptions *\/ */

      /* now update the handle field */
      /* FIXME: thread safety */
      /* (*env)->SetLongField(env, tls_CoRSP, FID_COSP_HANDLE, (intptr_t)c_TxnId); */
    }

    OIC_LOG_V(DEBUG, TAG, "%s txn id:", __func__);
    OIC_LOG_BUFFER(DEBUG, TAG, (const uint8_t *) c_TxnId, CA_MAX_TOKEN_LEN);

    /* FIXME: synch access to global g_txn_list */
    if (g_txn_list) {
	txn_t*  t = g_txn_list;
	while (t) {
	    if (t->next) {
		t = t->next;
	    } else {
		break;
	    }
	}
	t->next = txn;
    } else {
	g_txn_list = txn;
    }

    OIC_LOG_V(DEBUG, TAG, "%s: EXIT", __func__);
    OIC_LOG(DEBUG, TAG, "");
    return;
}
