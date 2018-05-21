package openocf.behavior;

import openocf.constants.ConnectivityType;
import openocf.utils.Endpoint;
import openocf.utils.CoAPOption;
import openocf.app.CoResourceSP;

import java.util.List;

// wraps data necessary to support OCDoResource
// OCStackResult OCDoResource(OCDoHandle *handle,
//                             OCMethod method,
//                             const char *requestUri,
//                             const OCDevAddr *destination,
//                             OCPayload* payload,  /* Observation record */
//                             OCConnectivityType connectivityType,
//                             OCQualityOfService qos,
//                             OCCallbackData *cbData,
//                             OCHeaderOption *options,
//                             uint8_t numOptions)


// created by client, from CoResourceSP
public class OutboundStimulus extends Outbound
{
    long context;	       // void*

    long contextDeleter;       // typedef void (* OCClientContextDeleter)(void *context);

    private OutboundStimulus() { }

    public OutboundStimulus(CoResourceSP corsp) {
	_coResourceSP = corsp;
	// _ep = NULL;
    }

    public OutboundStimulus(CoResourceSP corsp, Endpoint ep) {
	_coResourceSP = corsp;
	_ep = ep;
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

    private int _method;
    public int getMethod() { return _method; }
    public OutboundStimulus setMethod(int method) { _method = method; return this; }

    private CoResourceSP _coResourceSP; // OCClientResponseHandler
    public  CoResourceSP getCoResourceSP() { return _coResourceSP; }
    public  OutboundStimulus setCoResourceSP(CoResourceSP corsp) { _coResourceSP = corsp; return this; }

    // NB: OCHeaderOptions is misnamed, s/b OCCoAPOptions (they are
    // not headers in CoAP)

    // FIXME: move to CoRSP
    // public String        _uriPath;

    // FIXME: _method is in Behavior.java?
    // public int           method;

    // FIXME: _remoteDeviceAddress is in Behavior.java?
    private Endpoint _ep;
    public Endpoint            getEndpoint() {return _ep;}
    public OutboundStimulus    setEndpoint(Endpoint ep) { _ep = ep; return this;}

    // payload
    ObservationRecord       observationRecord;

    // FIXME: comm stuff goes here, pull it from CoRSP class
    int protocol; // ConnectivityType.DEFAULT, etc.

    // QoS supplied as arg to sendRequest, not as data
    // int qualityOfService; // OCF.QOS_LOW, MED, HI, NA
    private int qos;
    public int              getQualityOfService() { return qos; }
    public OutboundStimulus setQualityOfService(int theQos) { qos = theQos; return this;}

    // OCCallbackData fields:

    // FIXME: _options is in Message
    // NB: not HeaderOptions
    List<CoAPOption> options;

    // Discovery requests: OCF.discoverPlatform, OCF.discoverDevice, OCF.discoverResources?

}

// examples:
// OutboundStimulus requestOut = new OutboundStimulus(Resource.URI_PLATFORM,
// 					     platformRequestor);

// OutboundStimulus requestOut = new OutboundStimulus(Resource.URI_RESOURCES,
// 					     devAddress, // forces unicast
// 					     platformRequestor);
