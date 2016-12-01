package org.iochibity;

public class RequestIn extends MessageService // OCEntityHandlerRequest
{

    // typedef struct
    // {
    // OCResourceHandle resource;
    // in MessageService

    // OCRequestHandle requestHandle - i.e. handle of request at ORIGIN (i.e. client)?;
    // in MessageService

    // the REST method retrieved from received request PDU.
    // OCMethod method;
    public int method;

    // description of endpoint that sent the request.
    // OCDevAddr devAddr;
    public DeviceAddress deviceAddress;

    // resource query sent by client.
    // char * query;
    public String query;

    // Information associated with observation - valid only when OCEntityHandler flag includes
    // * ::OC_OBSERVE_FLAG.
    // (i.e. the flag passed to the resource provider service routine)
    // OCObservationInfo obsInfo;
    // typedef struct
    // {
    // Action associated with observation request.*/
    // OCObserveAction action;  /* enum */
    // Identifier for observation being registered/deregistered.*/
    // OCObservationId obsId;   /* uint8_t */
    // } OCObservationInfo;
    public int observeAction;
    public int observeId;

    // Number of the received vendor specific header options.*/
    // uint8_t numRcvdVendorSpecificHeaderOptions;
    // in MessageService

    // Pointer to the array of the received vendor specific header options.*/
    // OCHeaderOption * rcvdVendorSpecificHeaderOptions;
    // in MessageService

    // Message id.
    // uint16_t messageID;
    public int messageId;

    // the payload from the request PDU.
    // OCPayload *payload;
    // in MessageService

    // } OCEntityHandlerRequest;

}
