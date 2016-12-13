package org.iochibity;

import java.util.List;

public class DocResponseIn extends DocForServiceRequestor //  OCClientResponse
{
    public long ptr_OCClientResponse; // ptr to c struct

    // a ResponseIn doc is passed to the ResourceServiceRequestor.serviceResponse

// typedef struct OCClientResponse
// {
//     /** Address of remote server.*/
//     OCDevAddr devAddr;
    public DeviceAddress deviceAddress;

//     /** backward compatibility (points to devAddr).*/
//     OCDevAddr *addr;

//     /** backward compatibility.*/
//     OCConnectivityType connType;
    public int connType;
    // public int adapterType;
    // public int adapterFlags;

//     /** the security identity of the remote server.*/
//     OCIdentity identity;
    public String secID;

//     /** the is the result of our stack, OCStackResult should contain coap/other error codes.*/
//     OCStackResult result;
    public int result;

//     /** If associated with observe, this will represent the sequence of notifications from server.*/
//     uint32_t sequenceNumber;
    public int serial;

//     /** resourceURI.*/
//     const char * resourceUri;
    public String uri;

// /** the payload for the response PDU.*/
// OCPayload *payload;
// see Document.java

//     /** Number of the received vendor specific header options.*/
//     uint8_t numRcvdVendorSpecificHeaderOptions;
    // see Document.java

//     /** An array of the received vendor specific header options.*/
//     OCHeaderOption rcvdVendorSpecificHeaderOptions[MAX_HEADER_OPTIONS];
    // see Document.java

// } OCClientResponse;
}
