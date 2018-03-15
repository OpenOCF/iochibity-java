package openocf.signals;

import java.util.ArrayList;
import java.util.LinkedList;

// OCFServerService observes InboundStimulus
// dispatches it to ResourceServiceProvider.coReact
// ResourceServiceProvider.coReact constructs OutboundResponse,
// tells OCFServerService to exhibit OutboundResponse
//
// InboundStimulus observed by OCFServerService matches OutboundStimulus exhibited by OCFClientService

// OutboundResponse exhibited by OCFServerService matches InboundResponse observed by OCFClientService

// All four are Behaviors associated with an OCFService

// OutboundStimulus: wraps OCDoResource (client)
// OutboundResponse: wraps OCEntityHandlerResponse* (server)

// base for OutboundStimulus (client) and OutboundResponse (server)

    // data necessary to support OCDoResource (client's OutboundStimulus):
    // OCStackResult OCDoResource(OCDoHandle *handle,
    //                             OCMethod method,
    //                             const char *requestUri,
    //                             const OCDevAddr *destination,
    // ExhibitedBehavior:          OCPayload* payload,
    //                             OCConnectivityType connectivityType,
    //                             OCQualityOfService qos,
    //                             OCCallbackData *cbData,
    //                             OCHeaderOption *options,
    //                             uint8_t numOptions)

// NB: intuitively the payload is the exhibitable behavior; the rest
// is metadata about the message/transport. The latter could be
// thought of as channel params.

// So when app calls OCFClientSP.exhibit(OutboundStimulus so) it instructs
// the engine to exhibit the payload in the channel (mode?) specified
// by the metadata. That's a messaging metaphor, but it fits mere
// exhibition of a behavior as well - Pavlov's stimulus was conveyed
// over a channel as well - air at a certain density, temp, etc. If
// not "channel" at least medium. IOW you cannot "exhibit" a behavior
// without a medium of exhibition.


public class Exhibitable extends Behavior
{
    // Request remote handle - request handle at ORIGIN (i.e. client)? */

    private long                     _remoteRequestHandle; // OCRequestHandle requestHandle = OCRequest*
    // public  long                    getRemoteRequestHandle() { return _remoteHandle; }

    private long                    _resourceHandle; // OCResource*
    // public  long                    getResourceHandle() { return _resourceHandle; }

    // common to OCDoResource and OCEntityHandlerResponse:
    // OCPayload *payload

    // public  native IServiceProvider getServiceProvider();   // getResource();

    //     /** Pointer of ActionSet which to support group action.*/
    //     OCActionSet *actionsetHead;

    //     /** The instance identifier for this web link in an array of web links - used in links. */
    //     union
    //     {
    //         /** An ordinal number that is not repeated - must be unique in the collection context. */
    //         uint8_t ins;
    //         /** Any unique string including a URI. */
    //         char *uniqueStr;
    //         /** Use UUID for universal uniqueness - used in /oic/res to identify the device. */
    //         OCIdentity uniqueUUID;
    //     };
    // } OCResource;
}
