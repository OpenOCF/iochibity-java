package org.iochibity;

public class PayloadDiscovery extends Payload
{
    //     /* ocpayload.h */
    // OC_EXPORT OCDiscoveryPayload* OCDiscoveryPayloadCreate();

    // OC_EXPORT OCSecurityPayload* OCSecurityPayloadCreate(const uint8_t* securityData, size_t size);
    // OC_EXPORT void OCSecurityPayloadDestroy(OCSecurityPayload* payload);

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
