package openocf.behavior;

import java.util.List;

import openocf.utils.Endpoint;
import openocf.utils.CoAPOption;

// InboundResponses are server behavior records co-observed by clients

// Wrapper for args to application-defined callback for server responses:
//     void *context, OCDoHandle txn_id, OCClientResponse response
// to be passed to client coReact method
// 

// Structure (mirrors OutboundStimulus?):
// 1. SP/CoSP-specific: method, uri
// 2. comm mech: network/transport protocols, etc.
// 3. Resource state (payload)


// the InboundResponse arg is created by the jni layer, in
// _openocf_app_CoResourceSP_coReact

// typedef OCStackApplicationResult (* OCClientResponseHandler)(void *context,
// 							     OCDoHandle handle, /* GAR: misnamed, s/b txn_id */
// 							     OCClientResponse * clientResponse);

// OCClientResponse struct:
// {
//     OCDevAddr devAddr;
//     OCDevAddr *addr;  /** backward compatibility (points to devAddr).*/
//     OCConnectivityType connType; /* backward compatibility; OCTransportAdapter, OCTransportFlags */
//     OCIdentity identity; /** security id of the remote server. not used for discovery responses? */
//     OCStackResult result;  /* result of server-side processing? */
//     uint32_t sequenceNumber; /* with observe, notification sequence nbr from server.*/
//     const char * resourceUri;
//     OCPayload *payload;  /** the payload for the response PDU.*/
//     uint8_t numRcvdVendorSpecificHeaderOptions;
//     OCHeaderOption rcvdVendorSpecificHeaderOptions[MAX_HEADER_OPTIONS];
// } OCClientResponse

// Observation is what the CoSP does, rather than what it observes -
// in this case the client observes a InboundResponse.  I.e. the
// response behavior exhibited by the ResourceSP does not (yet) count
// as an observation. (See ing/ed distinction)
// was: ObservationIn

// an InboundResponse record is passed to application-defined coReact method of a CoResourceSP
public class InboundResponse extends Inbound
{
    public boolean isRetain; // app sets this to retain OCClientResponse

    // fields from struct OCClientResponse:

    // /** resourceURI.*/
    // _uri inherited from BehaviorRecord
    // native implementation uses _handle to get from OCClientResponse
    native public  String getUri();
    // no setUri for inbound msgs

    // Method? - set (by CoResourceSP) in OutboundStimulus, but not stored in OCClientResponse,
    // so CoResourceSP.getMethod returns outbound request method, InboundResponse.getMethod undefined
    // TODO link outboundStimulus to inboundResponse
    native public int getMethod();


    // /** the is the result of our stack, OCStackResult should contain coap/other error codes.*/
    // OCStackResult result;
    native public int getResult(); // GAR: result of server-side processing of request

    // OCDevAddr devAddr;  /* contains OCTransportAdapter adapter; OCTransportFlags flags;
    // should match params in OutboundStimulus?
    native public Endpoint getEndpoint();

    //IGNORE fields for backward compatibility:
    //     OCDevAddr *addr; /** backward compatibility (points to devAddr).*/
    //     OCConnectivityType connType; /** backward compatibility -dups what's in OCDevAddr */

    // OCIdentity identity; /** OCIdentity.id - security identity of the remote server.*/
    // private String _secID;
    // public String getSecID() { return _secID; }
    native public String getSecID();

    // /** If associated with observe, this will represent the sequence of notifications from server.*/
    // uint32_t sequenceNumber;
    //public int notificationSerial;
    native public int getNotificationSerial();

    // /** the payload for the response PDU.*/
    // OCPayload *payload;
    // see Message.java

    // uint8_t numRcvdVendorSpecificHeaderOptions;
    // OCHeaderOption rcvdVendorSpecificHeaderOptions[MAX_HEADER_OPTIONS];
    native public List<CoAPOption> getOptions();

// } OCClientResponse;
}
