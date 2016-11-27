package org.iochibity;

public class RequestIn		// OCEntityHandlerRequest
{
    private long handle;

// typedef struct
// {
//     /** Associated resource.*/
//     OCResourceHandle resource;
    // Resource getResource();

//     /** Associated request handle.*/
//     OCRequestHandle requestHandle - i.e. handle of request on originating client?;
    long handleAtOrigin;

//     /** the REST method retrieved from received request PDU.*/
//     OCMethod method;
    int method;

//     /** description of endpoint that sent the request.*/
//     OCDevAddr devAddr;
    // DeviceAddress getDeviceAddress;

//     /** resource query send by client.*/
//     char * query;
    String query;

//     /** Information associated with observation - valid only when OCEntityHandler flag includes
//      * ::OC_OBSERVE_FLAG.*/
//     OCObservationInfo obsInfo;

//     /** Number of the received vendor specific header options.*/
//     uint8_t numRcvdVendorSpecificHeaderOptions;
    int VendorHeaderOptionsCount;

//     /** Pointer to the array of the received vendor specific header options.*/
//     OCHeaderOption * rcvdVendorSpecificHeaderOptions;

//     /** Message id.*/
//     uint16_t messageID;
    int messageId;

//     /** the payload from the request PDU.*/
//     OCPayload *payload;
    // Payload getPayload();

// } OCEntityHandlerRequest;

}
