package org.iochibity;

import org.iochibity.constants.ConnectivityType;

import java.util.List;

// created by client
public class DocRequestOut extends DocForServiceRequestor
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

    String        uri;
    // method supplied as arg to sendRequest, not as data
    // int           method;
    DeviceAddress dest;
    Payload       payload;

    int protocol; // ConnectivityType.DEFAULT, etc.

    // QoS supplied as arg to sendRequest, not as data
    int qualityOfService; // OCF.QOS_LOW, MED, HI, NA

    // OCCallbackData fields:
    IResourceServiceRequestor serviceRequestor; // OCClientResponseHandler
    long context;	       // void*
    long contextDeleter;       // typedef void (* OCClientContextDeleter)(void *context);

    List<HeaderOption> options;

    // Discovery requests: OCF.discoverPlatform, OCF.discoverDevice, OCF.discoverResources?

    public DocRequestOut(IResourceServiceRequestor requestor) {
	serviceRequestor = requestor;
    }

    public DocRequestOut(String theUri, IResourceServiceRequestor requestor) {
	switch(uri) {
	case Resource.URI_PLATFORM:
	case Resource.URI_DEVICE:
	case Resource.URI_RESOURCES:
	    uri              = theUri;
	    // method           = theMethod;
	    dest             = null;
	    payload          = null;
	    protocol         = ConnectivityType.DEFAULT;
	    // qos              = OCF.QOS_LOW;
	    serviceRequestor = requestor;
	    context          = 0;
	    contextDeleter   = 0;
	    break;
	default:
	    break;
	}
    }
}

// examples:
// DocRequestOut requestOut = new DocRequestOut(Resource.URI_PLATFORM,
// 					     platformRequestor);

// DocRequestOut requestOut = new DocRequestOut(Resource.URI_RESOURCES,
// 					     devAddress, // forces unicast
// 					     platformRequestor);
