package org.iochibity;

public class ResponseOut extends MessageService //  OCEntityHandlerResponse
{
    // typedef struct
    // {
    //     /** Request handle - at request ORIGIN (i.e. client)? */
    //     OCRequestHandle requestHandle;
    // in MessageService

    //     /** Resource handle.*/
    //     OCResourceHandle resourceHandle;
    // in MessageService

    //     /** Allow the entity handler to pass a result with the response.*/
    //     OCEntityHandlerResult  ehResult;

    //     /** This is the pointer to server payload data to be transferred.*/
    //     OCPayload* payload;
    // in MessageService

    // number of the vendor specific header options .*/
    // uint8_t numSendVendorSpecificHeaderOptions;
    // in MessageService
    
    // An array of the vendor specific header options the entity handler wishes to use in response.*/
    // OCHeaderOption sendVendorSpecificHeaderOptions[MAX_HEADER_OPTIONS];
    // in MessageService
    
    //     /** URI of new resource that entity handler might create.*/
    //     char resourceUri[MAX_URI_LENGTH];
    public String resourceUri;

    //     /** Server sets to true for persistent response buffer,false for non-persistent response buffer*/
    //     uint8_t persistentBufferFlag;
    // } OCEntityHandlerResponse;
}
