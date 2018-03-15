package openocf.signals;

import java.util.List;

// Wrapper for args to application-defined callback for server responses:
// typedef OCStackApplicationResult (* OCClientResponseHandler)(void *context,
// 							     OCDoHandle handle, /* GAR: misnamed, s/b txn_id */
// 							     OCClientResponse * clientResponse);

// Observation is what the CoSP does, rather than what it observes -
// in this case the client observes a InboundResponse.  I.e. the
// response behavior exhibited by the ResourceSP does not (yet) count
// as an observation. (See ing/ed distinction)
// was: ObservationIn

// an InboundResponse record is passed to application-defined coReact method of a CoResourceSP
public class InboundResponse extends Observable
{
    // context, handle, and clientresponse: stored in Behavior
    // private long _txn_id;			// OCDoHandle handle (void *)
    // private long _message;			// OCClientResponse*
    // private long _ctx;				// void *context

// fields from struct OCClientResponse:

//     /** resourceURI.*/
//   inherited from openocf.signals.Behavior: getUri,

    // Method - set (by CoResourceSP) in OutboundStimulus, but not stored in OCClientResponse,
    // so CoResourceSP.getMethod returns outbound request method, InboundResponse.getMethod undefined
    // TODO link outboundStimulus to inboundResponse

//     /** the is the result of our stack, OCStackResult should contain coap/other error codes.*/
//     OCStackResult result;
    public int resourceSPResult;		// GAR: result of server-side processing of request; check it before proceeding?

//     /** Address of remote server.*/
//     OCDevAddr devAddr;  /* contains OCTransportAdapter adapter; OCTransportFlags flags;
//     EXPOSED IN Observable.java

//IGNORE fields there for backward compatibility:
//     /** backward compatibility (points to devAddr).*/
//     OCDevAddr *addr;
//     /** backward compatibility.*/
//     OCConnectivityType connType; /* GAR: adapter type, security flag, IP version, IPv6 scope */

    // see Message: _remoteDeviceAddress;
    // // Message protocol:
    // public Endpoint getRemoteDP() // address + port?


//     /** the security identity of the remote server.*/
//     OCIdentity identity;
    public String secID;

//     /** If associated with observe, this will represent the sequence of notifications from server.*/
//     uint32_t sequenceNumber;
    public int serial;


    // /** the payload for the response PDU.*/
    // OCPayload *payload;
    // see Message.java

//     /** Number of the received vendor specific header options.*/
//     uint8_t numRcvdVendorSpecificHeaderOptions;
    // see Message.java

//     /** An array of the received vendor specific header options.*/
//     OCHeaderOption rcvdVendorSpecificHeaderOptions[MAX_HEADER_OPTIONS];
    // see Message.java

// } OCClientResponse;
}
