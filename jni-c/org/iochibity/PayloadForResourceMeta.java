package org.iochibity;

public class PayloadForResourceMeta extends Payload // OCResourcePayload*
{
    // in struct OCDiscoveryPayload:
    //     /** This structure holds the old /oic/res response. */
    //     OCResourcePayload *resources;

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
}
