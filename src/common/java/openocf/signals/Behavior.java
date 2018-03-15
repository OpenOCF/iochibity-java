package openocf.signals;

import openocf.utils.Endpoint;

import java.util.List;

// was: Message.java

// Behaviors are exhibited by routines:

// server:  OCStackResult OC_CALL OCDoResponse(OCEntityHandlerResponse *ehResponse)
// OCEntityHandlerResponse:
//    OCRequestHandle requestHandle; ->  OCServerRequest* - contains method, URL, endpoint, etc.
//    OCResourceHandle resourceHandle;
//    OCEntityHandlerResult  ehResult;
//    OCPayload* payload;
//    uint8_t numSendVendorSpecificHeaderOptions;
//    OCHeaderOption sendVendorSpecificHeaderOptions[MAX_HEADER_OPTIONS];
//    char resourceUri[MAX_URI_LENGTH];
//    uint8_t persistentBufferFlag;

// client:  OCStackResult OC_CALL OCDoRequest(
//    OCDoHandle *handle,  - Txn ID
//    OCMethod method,
//    const char *requestUri,
//    const OCDevAddr *destination,
//    OCPayload* payload,
//    OCConnectivityType connectivityType,
//    OCQualityOfService qos,
//    OCCallbackData *cbData,
//    OCHeaderOption *options,
//    uint8_t numOptions)

// Behaviors are observed by routines:

//  server: OCEntityHandler (OCEntityHandlerFlag flag, OCEntityHandlerRequest * entityHandlerRequest, void* callbackParam)
//  OCEntityHandlerRequest:
//    OCResourceHandle resource;
//    OCRequestHandle requestHandle; - contains URL, etc.
//    OCMethod method;
//    OCDevAddr devAddr;
//    char * query;
//    OCObservationInfo obsInfo;
//    uint8_t numRcvdVendorSpecificHeaderOptions;
//    OCHeaderOption * rcvdVendorSpecificHeaderOptions;
//    uint16_t messageID;
//    OCPayload *payload;

//  client: OCClientResponseHandler (void *context, OCDoHandle handle, OCClientResponse * clientResponse)
//  OCClientResponse:
//    OCDevAddr devAddr;
//    OCDevAddr *addr;
//    OCConnectivityType connType; /* corresponds to OCTransportAdapter, OCTransportFlags: adapter type, sec flag, IP ver., IPv6 scope */
//    OCIdentity identity;	/* GAR: not used for discovery responses? */
//    OCStackResult result;
//    uint32_t sequenceNumber;
//    const char * resourceUri;
//    OCPayload *payload;
//    uint8_t numRcvdVendorSpecificHeaderOptions;
//    OCHeaderOption rcvdVendorSpecificHeaderOptions[MAX_HEADER_OPTIONS];

// what they have in common: OCDevAddr (remote Endpoint); handle
// (OCDoHandle, entityHandlerRequest.requestHandle; vendor-specific
// header options; payload

// i.e. base Message (OCEntityHandlerRequest, OCClientResponse)
public class Behavior implements IBehavior
{
    // private Endpoint _remoteEP;
    private long _endpoint_handle;	  // c ptr to EP
    native public Endpoint getRemoteEP(); // { return _remoteEP; } // IMessage

    private long _handle;	// _txn_id? DoHandle, requestHandle

    // clientresponse:
    // uint8_t numRcvdVendorSpecificHeaderOptions;
    // OCHeaderOption rcvdVendorSpecificHeaderOptions[MAX_HEADER_OPTIONS];
    // server (inbound stimulus):
    // uint8_t numRcvdVendorSpecificHeaderOptions;
    // OCHeaderOption * rcvdVendorSpecificHeaderOptions;

    // local handle for c struct underlying this object: OCClientResponse, etc.
    private long _message;	// was: _localHandle

    // OCEntityHandlerRequest.resource.uri
    // OCClientResponse.resourceUri
    // OCDiscoveryPayload.uri, OCRepPayload.uri, OCPlatformPayload.uri,
    private String _Uri;
    public  String getUri() { return _Uri; }
    // FIXME: setUri only for Outbound msgs:
    public  void   setUri(String theUri) { _Uri = theUri; }

    // network stuff? OCClientResponse.connType, OCEntityHandlerRequest.OCDevAddr.adapter/.flags

    //FIXME: only for Observable/Inbound:
    // the payload from the request PDU.
    //FIXME: just call it Payload instead of Observe etc.?
    private long _observationRecordHandle;
    public  long getObservationRecordHandle() { return _observationRecordHandle; } // IMessage

    // public native PayloadList<Payload> getPayloadList(); // IMessage

    public native ObservationList<ObservationRecord> getObservationRecords(); // IMessage

    // public native int getObservationType(); // IMessage

    // Number of vendor specific header options sent or recd.
    // InboundStimulus:   uint8_t numRcvdVendorSpecificHeaderOptions;
    //              OCHeaderOption * rcvdVendorSpecificHeaderOptions;
    // ObservationOut: uint8_t numSendVendorSpecificHeaderOptions;
    //              OCHeaderOption sendVendorSpecificHeaderOptions[MAX_HEADER_OPTIONS];
    // public  int  optionCount;
    // private long ptr_Options;	// OCHeaderOption*


    private List<HeaderOption> _options;
    public native List<HeaderOption> getOptions(); // IMessage
}
