package org.iochibity;

public class PayloadForDiscovery // extends Payload
{
    // typedef struct OCDiscoveryPayload
    // {
    //     OCPayload base;

    //     /** Device Id */
    //     char *sid;

    //     /** A special case for handling RD address. */
    //     char* baseURI;

    //     /** Name */
    //     char *name;

    //     /** HREF */
    //     char *uri;

    //     /** Resource Type */
    //     OCStringLL *type;

    //     /** Interface */
    //     OCStringLL *iface;

    //     /** This structure holds the old /oic/res response. */
    //     OCResourcePayload *resources;
    // see PayloadForResourceState.java (as opposed to PayloadForResourceStateState)

    //     /** Holding address of the next DiscoveryPayload. */
    //     struct OCDiscoveryPayload *next;

    // } OCDiscoveryPayload;

    //     octypes.h:
    //     // used inside a discovery payload
    // typedef struct OCResourcePayload
    // {
    //     char* uri;
    //     OCStringLL* types;
    //     OCStringLL* interfaces;
    //     uint8_t bitmap;
    //     bool secure;
    //     uint16_t port;
    // #ifdef TCP_ADAPTER
    //     uint16_t tcpPort;
    // #endif
    //     struct OCResourcePayload* next;
    // } OCResourcePayload;


    //     /* ocpayload.h */
    // OC_EXPORT OCDiscoveryPayload* OCDiscoveryPayloadCreate();

    // #ifndef TCP_ADAPTER
    // OC_EXPORT void OCDiscoveryPayloadAddResource(OCDiscoveryPayload* payload, const OCResource* res,
    //                                              uint16_t securePort);
    // #else
    // OC_EXPORT void OCDiscoveryPayloadAddResource(OCDiscoveryPayload* payload, const OCResource* res,
    //                                              uint16_t securePort, uint16_t tcpPort);
    // #endif
    // OC_EXPORT void OCDiscoveryPayloadAddNewResource(OCDiscoveryPayload* payload, OCResourcePayload* res);
    // OC_EXPORT bool OCResourcePayloadAddStringLL(OCStringLL **payload, const char* type);

    // OC_EXPORT size_t OCDiscoveryPayloadGetResourceCount(OCDiscoveryPayload* payload);
    // OC_EXPORT OCResourcePayload* OCDiscoveryPayloadGetResource(OCDiscoveryPayload* payload, size_t index);

    // OC_EXPORT void OCDiscoveryResourceDestroy(OCResourcePayload* payload);
    // OC_EXPORT void OCDiscoveryPayloadDestroy(OCDiscoveryPayload* payload);
}
