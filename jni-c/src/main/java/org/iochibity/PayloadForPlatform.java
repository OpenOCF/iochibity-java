package org.iochibity;

public class PayloadForPlatform extends Payload
{
    // octypes.h:
    // typedef struct
    // {
    //     OCPayload base;
    //     char* uri;
    //     OCPlatformInfo info;
    //     OCStringLL* rt;
    //     OCStringLL* interfaces;
    // } OCPlatformPayload;

    // ocpayload.h
    // OC_EXPORT OCPlatformPayload* OCPlatformPayloadCreate(const OCPlatformInfo* platformInfo);
    // OC_EXPORT OCPlatformPayload* OCPlatformPayloadCreateAsOwner(OCPlatformInfo* platformInfo);
    // OC_EXPORT void OCPlatformInfoDestroy(OCPlatformInfo *info);
    // OC_EXPORT void OCPlatformPayloadDestroy(OCPlatformPayload* payload);
}
