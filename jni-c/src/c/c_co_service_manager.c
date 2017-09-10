/* c_co_service_manager.c */

#include "c_co_service_manager.h"

#include "oic_malloc.h"
#include "oic_string.h"
#include "ocpayload.h"
#include "logger.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define TAG  "c_co_service_manager.c"

/**
 * store discovered platform in local database
 */
void register_platform(OCClientResponse* c_OCClientResponse)
{
    OIC_LOG_V(DEBUG, TAG, "%s:%d ENTRY", __func__, __LINE__);

    if (c_OCClientResponse->payload->type != PAYLOAD_TYPE_PLATFORM) {
	OIC_LOG_V(FATAL, TAG,
		  "FATAL %s %d (%s): wrong payload type\n", __FILE__, __LINE__,__func__);
	exit(1);
    }

    OIC_LOG_DISCOVERY_RESPONSE(DEBUG, TAG, c_OCClientResponse);

    /* Incoming response record is allocated on the stack of the
       calling routine, so it will be removed after we return.  To
       keep it around we need to make a deep copy. */

    g_Platform = OICCalloc(1, (sizeof (OCClientResponse)) );
    if (g_Platform == NULL) {
	OIC_LOG_V(FATAL, TAG,
		  "FATAL %s %d (%s): calloc failure\n", __FILE__, __LINE__,__func__);
    }
    memcpy(g_Platform, c_OCClientResponse, (sizeof (OCClientResponse)));

    g_Platform->addr = &(c_OCClientResponse->devAddr);
    /* copy the uripath string */
    /* g_Platform->resourceUri = */
    g_Platform->resourceUri = OICStrdup(c_OCClientResponse->resourceUri);

    OCPlatformPayload  *fromPP = (OCPlatformPayload*)c_OCClientResponse->payload;
    OCPlatformPayload **toPP   = (OCPlatformPayload**) &(g_Platform->payload);

    OCStringLL *fromSLL, **toSLL; // , *toSLLHead;
    /* 1. create new OCPlatformPayload */
    (*toPP)                    = (OCPlatformPayload*)OICCalloc(1, sizeof(OCPlatformPayload));
    if (!*toPP){
	OIC_LOG_V(FATAL, TAG,
		  "FATAL %s %d (%s): calloc failure\n", __FILE__, __LINE__,__func__);
    }
    (*toPP)->base.type = PAYLOAD_TYPE_PLATFORM;

    /* 1a. copy strings */
    (*toPP)->uri           = OICStrdup(fromPP->uri);

    /* 1b. OCPlatformInfo */

    /* 1c. copy fromPP types and interfaces */
    /* types */
    fromSLL                    = fromPP->rt;
    toSLL                      = &((*toPP)->rt);
    while(fromSLL) {
	(*toSLL)               = CloneOCStringLL(fromSLL);
	toSLL                  = &((*toSLL)->next);
	fromSLL                = fromSLL->next;
    }

    /* interfaces */
    fromSLL                    = fromPP->interfaces;
    toSLL                      = &((*toPP)->interfaces);
    while(fromSLL) {
	(*toSLL)               = CloneOCStringLL(fromSLL);
	toSLL                  = &((*toSLL)->next);
	fromSLL                = fromSLL->next;
    }

}

/**
 * store discovered devices in local database
 */
void register_device(OCClientResponse* c_OCClientResponse)
{
    OIC_LOG_V(DEBUG, TAG, "%s:%d ENTRY", __func__, __LINE__);

    if (c_OCClientResponse->payload->type != PAYLOAD_TYPE_DEVICE) {
	OIC_LOG_V(FATAL, TAG,
		  "FATAL %s %d (%s): wrong payload type\n", __FILE__, __LINE__,__func__);
	exit(1);
    }

    OIC_LOG_DISCOVERY_RESPONSE(DEBUG, TAG, c_OCClientResponse);

    /* Incoming response record is allocated on the stack of the
       calling routine, so it will be removed after we return.  To
       keep it around we need to make a deep copy. */

    g_Device = OICCalloc(1, (sizeof (OCClientResponse)) );
    if (g_Device == NULL) {
	OIC_LOG_V(FATAL, TAG,
		  "FATAL %s %d (%s): calloc failure\n", __FILE__, __LINE__,__func__);
    }
    memcpy(g_Device, c_OCClientResponse, (sizeof (OCClientResponse)));

    g_Device->addr = &(c_OCClientResponse->devAddr);
    /* copy the uripath string */
    /* g_Device->resourceUri = */
    g_Device->resourceUri = OICStrdup(c_OCClientResponse->resourceUri);

    OCDevicePayload  *fromPP = (OCDevicePayload*)c_OCClientResponse->payload;
    OCDevicePayload **toPP   = (OCDevicePayload**) &(g_Device->payload);

    OCStringLL *fromSLL, **toSLL; // , *toSLLHead;
    /* 1. create new OCDevicePayload */
    (*toPP)                  = (OCDevicePayload*)OICCalloc(1, sizeof(OCDevicePayload));
    if (!*toPP){
	OIC_LOG_V(FATAL, TAG,
		  "FATAL %s %d (%s): calloc failure\n", __FILE__, __LINE__,__func__);
    }
    (*toPP)->base.type = PAYLOAD_TYPE_DEVICE;

    /* 1a. copy strings */
    (*toPP)->sid           = OICStrdup(fromPP->sid);
    (*toPP)->deviceName    = OICStrdup(fromPP->deviceName);
    (*toPP)->specVersion   = OICStrdup(fromPP->specVersion);

    /* 1b. copy OCStringLLs */
    /* dataModelVersions */
    fromSLL                    = fromPP->dataModelVersions;
    toSLL                      = &((*toPP)->dataModelVersions);
    while(fromSLL) {
	(*toSLL)               = CloneOCStringLL(fromSLL);
	toSLL                  = &((*toSLL)->next);
	fromSLL                = fromSLL->next;
    }

    /* types */
    fromSLL                    = fromPP->types;
    toSLL                      = &((*toPP)->types);
    while(fromSLL) {
	(*toSLL)               = CloneOCStringLL(fromSLL);
	toSLL                  = &((*toSLL)->next);
	fromSLL                = fromSLL->next;
    }

    /* interfaces */
    fromSLL                    = fromPP->interfaces;
    toSLL                      = &((*toPP)->interfaces);
    while(fromSLL) {
	(*toSLL)               = CloneOCStringLL(fromSLL);
	toSLL                  = &((*toSLL)->next);
	fromSLL                = fromSLL->next;
    }
    OIC_LOG_V(DEBUG, TAG, "%s:%d EXIT", __func__, __LINE__);
}

/**
 * store discovered resources in local database
 */
void register_resources(OCClientResponse* c_OCClientResponse)
{
    OIC_LOG_V(DEBUG, TAG, "%s:%d ENTRY", __func__, __LINE__);

    if (c_OCClientResponse->payload->type != PAYLOAD_TYPE_DISCOVERY) {
	OIC_LOG_V(FATAL, TAG,
		  "FATAL %s %d (%s): wrong payload type\n", __FILE__, __LINE__,__func__);
	exit(1);
    }

    OIC_LOG_V(DEBUG, TAG, "resource count: %ld\n",
	      OCDiscoveryPayloadGetResourceCount((OCDiscoveryPayload*)c_OCClientResponse->payload));

    OIC_LOG_DISCOVERY_RESPONSE(DEBUG, TAG, c_OCClientResponse);

    /* Incoming response record is allocated on the stack of the
       calling routine, so it will be removed after we return.  To
       keep it around we need to make a deep copy. */
    g_Resources = (OCClientResponse *)OICCalloc(1, sizeof (OCClientResponse));
    memcpy(g_Resources, c_OCClientResponse, sizeof (OCClientResponse));

    g_Resources->addr = &(g_Resources->devAddr);
    /* copy the uripath string */
    /* g_Resources->resourceUri = */
    g_Resources->resourceUri = OICStrdup(c_OCClientResponse->resourceUri);

    /* copy payload; NB: fromDP is a linked list */
    OCDiscoveryPayload  *fromDP = (OCDiscoveryPayload*)c_OCClientResponse->payload;
    OCDiscoveryPayload **toDP   = (OCDiscoveryPayload**) &(g_Resources->payload);

    OCStringLL *fromSLL, **toSLL; // , *toSLLHead;
    while(fromDP) {
	/* 1. create new OCDiscoveryPayload */
	(*toDP)                    = OCDiscoveryPayloadCreate();

	/* 1a. copy fromDP sid, baseURI, name, uri strings */
	(*toDP)->sid               = OICStrdup(fromDP->sid);
	(*toDP)->baseURI           = OICStrdup(fromDP->baseURI);
	(*toDP)->name              = OICStrdup(fromDP->name);
	(*toDP)->uri               = OICStrdup(fromDP->uri);

	/* 1b. copy fromDP types and interfaces */
	/* types */
	fromSLL                    = fromDP->type;
	toSLL                      = &((*toDP)->type);
	while(fromSLL) {
	    (*toSLL)               = CloneOCStringLL(fromSLL);
	    toSLL                  = &((*toSLL)->next);
	    fromSLL                = fromSLL->next;
	}
	/* interfaces */
	fromSLL                    = fromDP->iface;
	toSLL                      = &((*toDP)->iface);
	while(fromSLL != 0x0) {
	    (*toSLL)               = CloneOCStringLL(fromSLL);
	    toSLL                  = &((*toSLL)->next);
	    fromSLL                = fromSLL->next;
	}
	/* resources (i.e. OCResourcePayload list) */
	OCResourcePayload*  fromRP = fromDP->resources;

	OCResourcePayload **toRP   = &((*toDP)->resources);
	while(fromRP != 0x0) {
	    (*toRP)                = (OCResourcePayload*)OICCalloc(sizeof (OCResourcePayload), 1);
	    (*toRP)->uri           = OICStrdup(fromRP->uri);
	    (*toRP)->bitmap        = fromRP->bitmap;
	    (*toRP)->secure        = fromRP->secure;
	    (*toRP)->port          = fromRP->port;
#ifdef TCP_ADAPTER
	    (*toRP)->tcpPort       = fromRP->tcpPort;
#endif

	    /* types */
	    fromSLL                = fromRP->types;
	    toSLL                  = &((*toRP)->types);
	    while(fromSLL) {
		(*toSLL)           = CloneOCStringLL(fromSLL);
		toSLL              = &((*toSLL)->next);
		fromSLL            = fromSLL->next;
	    }
	    /* interfaces */
	    fromSLL                = fromRP->interfaces;
	    toSLL                  = &((*toRP)->interfaces);
	    while(fromSLL) {
		(*toSLL)           = CloneOCStringLL(fromSLL);
		toSLL              = &((*toSLL)->next);
		fromSLL            = fromSLL->next;
	    }
	    fromRP                 = fromRP->next;
	    toRP                   = &((*toRP)->next);
	}
	fromDP = fromDP->next;
	toDP   = &((*toDP)->next);
    }
}

