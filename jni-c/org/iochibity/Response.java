package org.iochibity;

// Fields and methods common to OCEntityHandlerRequest and OCEntityHandlerResponse
public class Response
{
    // Request handle - at request ORIGIN (i.e. client)?
    // OCRequestHandle requestHandle;
    public long handleAtOrigin;

    // Resource handle.
    // OCResourceHandle resourceHandle;
    protected long resourceHandle;
    public native Resource getResource();

    // OCPayload *payload;
    // RequestIn:  payload from request PDU, received
    // RequestOut: payload created on server, to be sent
    protected long payloadHandle;
    public native Payload getPayload();

    // Number of vendor specific header options.*/
    // RequestIn:  number recd
    // RequestOut: number to send
    // uint8_t numRcvdVendorSpecificHeaderOptions;
    public int vendorHeaderOptionsCount;

    // Pointer to the array of the received vendor specific header options.*/
    // OCHeaderOption * rcvdVendorSpecificHeaderOptions;
    HeaderOption[] vendorHeaderOptions;

}
