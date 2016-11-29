package org.iochibity;

public class RequestIn		// OCEntityHandlerRequest
{

    // typedef struct
    // {
    //     OCRequestHandle requestHandle - i.e. handle of request at ORIGIN (i.e. client)?;
    public long handleAtOrigin;

    //     OCResourceHandle resource;
    private long resourceHandle;
    public native Resource getResource();

    //     /** the REST method retrieved from received request PDU.*/
    //     OCMethod method;
    public int method;

    //     /** description of endpoint that sent the request.*/
    //     OCDevAddr devAddr;
    public DeviceAddress deviceAddress;
    // public native DeviceAddress getDeviceAddress();

    //     /** resource query send by client.*/
    //     char * query;
    public String query;

    //     /** Information associated with observation - valid only when OCEntityHandler flag includes
    //      * ::OC_OBSERVE_FLAG.*/
    //     OCObservationInfo obsInfo;

    //     /** Number of the received vendor specific header options.*/
    //     uint8_t numRcvdVendorSpecificHeaderOptions;
    public int vendorHeaderOptionsCount;

    //     /** Pointer to the array of the received vendor specific header options.*/
    //     OCHeaderOption * rcvdVendorSpecificHeaderOptions;

    //     /** Message id.*/
    //     uint16_t messageID;
    public int messageId;

    //     /** the payload from the request PDU.*/
    //     OCPayload *payload;
    private long payloadHandle;
    public native Payload getPayload();

    // } OCEntityHandlerRequest;

}
