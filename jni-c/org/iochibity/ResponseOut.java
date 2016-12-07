package org.iochibity;

public class ResponseOut extends MessageForResource //  OCEntityHandlerResponse
{
    private long _handle;	// OCEntityHandlerResponse*

    public RequestIn _requestIn;
    private PayloadList<Payload> _payloadList;

    // private native long createResponseOut(RequestIn r, PayloadList<Payload> pll);
    public ResponseOut(RequestIn r, PayloadList<Payload> pll)
    {
	_requestIn = r;
	_payloadList = pll;
    }

    // OCEntityHandlerResponse response = { .requestHandle = NULL };

    // typedef struct  /* OCEntityHandlerResponse */
    // {
    //     /** Request handle - at request ORIGIN (i.e. client)? */
    //     OCRequestHandle requestHandle;
    // in MessageForResource

    //     /** Resource handle.*/
    //     OCResourceHandle resourceHandle;
    // in MessageForResource

    //     /** Allow the entity handler to pass a result with the response.*/
    //     OCEntityHandlerResult  ehResult;
    private int _processingResult;
    public  int getProcessingResult() { return _processingResult; }
    public  void setProcessingResult(int result) { _processingResult = result; }

    //     /** This is the pointer to server payload data to be transferred.*/
    //     OCPayload* payload;
    // in MessageForResource

    // number of the vendor specific header options .*/
    // uint8_t numSendVendorSpecificHeaderOptions;
    // in MessageForResource

    // An array of the vendor specific header options the entity handler wishes to use in response.*/
    // OCHeaderOption sendVendorSpecificHeaderOptions[MAX_HEADER_OPTIONS];
    // in MessageForResource

    //     /** URI of new resource that entity handler might create.*/
    //     char resourceUri[MAX_URI_LENGTH];
    private String resourceUri;
    public  String getResourceUri() { return resourceUri; }
    public  void setResourceUri(String uri) { resourceUri = uri; }

    //     /** Server sets to true for persistent response buffer,false for non-persistent response buffer*/
    //     uint8_t persistentBufferFlag;
    public int persistentBufferFlag;
    // } OCEntityHandlerResponse;
}
