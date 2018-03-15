package openocf.signals;

import openocf.constants.ConnectivityType;
import openocf.utils.Endpoint;
import openocf.app.ICoResourceSP;

import java.util.List;

// created by client
public class OutboundStimulus extends Exhibitable
{
    // data necessary to support OCDoResource
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

    public String        _uriPath;

    // FIXME: _method is in Behavior.java?
    // public int           method;

    // FIXME: _remoteDeviceAddress is in Behavior.java?
    // public Endpoint dest;
    // native public Endpoint getRemoteEP(); // { return _remoteDeviceAddress; }

    ObservationRecord       observationRecord;

    // FIXME: handle the OCTransportX/CT_x enums appropriately
    int protocol; // ConnectivityType.DEFAULT, etc.

    // QoS supplied as arg to sendRequest, not as data
    int qualityOfService; // OCF.QOS_LOW, MED, HI, NA

    // OCCallbackData fields:

    ICoResourceSP coResourceSP; // OCClientResponseHandler

    long context;	       // void*

    long contextDeleter;       // typedef void (* OCClientContextDeleter)(void *context);

    // FIXME: _options is in Message
    List<HeaderOption> options;

    // Discovery requests: OCF.discoverPlatform, OCF.discoverDevice, OCF.discoverResources?

    public OutboundStimulus(ICoResourceSP client) {
	coResourceSP = client;
    }

    // public OutboundStimulus(String theUri, ICoServiceProvider requestor) {
    // 	uri              = theUri;
    // 	// method           = theMethod;
    // 	dest             = null;
    // 	payload          = null;
    // 	protocol         = ConnectivityType.DEFAULT;
    // 	// qos              = OCF.QOS_LOW;
    // 	serviceRequestor = requestor;
    // 	context          = 0;
    // 	contextDeleter   = 0;
    // }
}

// examples:
// OutboundStimulus requestOut = new OutboundStimulus(Resource.URI_PLATFORM,
// 					     platformRequestor);

// OutboundStimulus requestOut = new OutboundStimulus(Resource.URI_RESOURCES,
// 					     devAddress, // forces unicast
// 					     platformRequestor);
