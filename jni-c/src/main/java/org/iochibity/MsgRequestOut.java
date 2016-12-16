package org.iochibity;

import org.iochibity.constants.ConnectivityType;

import java.util.List;

// created by client
public class MsgRequestOut extends MsgForServiceRequestor
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

    public String        uri;
    // method supplied as arg to sendRequest, not as data
    // int           method;
    public DeviceAddress dest = null;
    Payload       payload;

    int protocol; // ConnectivityType.DEFAULT, etc.

    // QoS supplied as arg to sendRequest, not as data
    int qualityOfService; // OCF.QOS_LOW, MED, HI, NA

    // OCCallbackData fields:
    IServiceRequestor serviceRequestor; // OCClientResponseHandler
    long context;	       // void*
    long contextDeleter;       // typedef void (* OCClientContextDeleter)(void *context);

    List<HeaderOption> options;

    // Discovery requests: OCF.discoverPlatform, OCF.discoverDevice, OCF.discoverResources?

    public MsgRequestOut(IServiceRequestor requestor) {
	serviceRequestor = requestor;
    }

    public MsgRequestOut(String theUri, IServiceRequestor requestor) {
	uri              = theUri;
	// method           = theMethod;
	dest             = null;
	payload          = null;
	protocol         = ConnectivityType.DEFAULT;
	// qos              = OCF.QOS_LOW;
	serviceRequestor = requestor;
	context          = 0;
	contextDeleter   = 0;
    }
}

// examples:
// MsgRequestOut requestOut = new MsgRequestOut(Resource.URI_PLATFORM,
// 					     platformRequestor);

// MsgRequestOut requestOut = new MsgRequestOut(Resource.URI_RESOURCES,
// 					     devAddress, // forces unicast
// 					     platformRequestor);
