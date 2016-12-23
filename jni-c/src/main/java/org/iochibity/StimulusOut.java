package org.iochibity;

import org.iochibity.constants.ConnectivityType;

import java.util.List;

// created by client
public class StimulusOut extends MsgForCoServiceProvider
{
    // data necessary to support OCDoResource
    // OCStackResult OCDoResource(OCDoHandle *handle,
    //                             OCMethod method,
    //                             const char *requestUri,
    //                             const OCDevAddr *destination,
    //                             OCPayload* payload,
    //                             OCConnectivityType connectivityType,
    //                             OCQualityOfService qos,
    //                             OCCallbackData *cbData,
    //                             OCHeaderOption *options,
    //                             uint8_t numOptions)

    public String        _uriPath;

    // FIXME: _method is in Message
    // public int           method;

    // FIXME: _remoteDeviceAddress is in Message
    // public DeviceAddress dest;
    public DeviceAddress getDestination() { return _remoteDeviceAddress; }

    Observation       observation;

    // FIXME: handle the OCTransportX/CT_x enums appropriately
    int protocol; // ConnectivityType.DEFAULT, etc.

    // QoS supplied as arg to sendRequest, not as data
    int qualityOfService; // OCF.QOS_LOW, MED, HI, NA

    // OCCallbackData fields:

    ICoServiceProvider coServiceProvider; // OCClientResponseHandler

    long context;	       // void*

    long contextDeleter;       // typedef void (* OCClientContextDeleter)(void *context);

    // FIXME: _options is in Message
    List<HeaderOption> options;

    // Discovery requests: OCF.discoverPlatform, OCF.discoverDevice, OCF.discoverResources?

    public StimulusOut(ICoServiceProvider client) {
	coServiceProvider = client;
    }

    // public StimulusOut(String theUri, ICoServiceProvider requestor) {
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
// StimulusOut requestOut = new StimulusOut(Resource.URI_PLATFORM,
// 					     platformRequestor);

// StimulusOut requestOut = new StimulusOut(Resource.URI_RESOURCES,
// 					     devAddress, // forces unicast
// 					     platformRequestor);
