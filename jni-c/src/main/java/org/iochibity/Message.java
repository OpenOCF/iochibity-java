package org.iochibity;

import java.util.List;

// i.e. base Message (OCEntityHandlerRequest, OCClientResponse)
public class Message implements IMessage
{
    // local handle for c struct underlying this object
    private long _localHandle;

    // OCEntityHandlerRequest.resource.uri
    // OCClientResponse.resourceUri
    // OCDiscoveryPayload.uri, OCRepPayload.uri, OCPlatformPayload.uri,
    private String _uriPath;
    public  String getUriPath() { return _uriPath; }
    public  void   setUriPath(String theUri) { _uriPath = theUri; }

    private int _method;			// for OCEntityHandlerRequest, not OCClientResponse
    public int getMethod() { return _method; }; // IMessage

    public DeviceAddress _remoteDeviceAddress;
    public DeviceAddress getRemoteDeviceAddress() { return _remoteDeviceAddress; } // IMessage

    // network stuff? OCClientResponse.connType, OCEntityHandlerRequest.OCDevAddr.adapter/.flags

    // the payload from the request PDU.
    private long _observationHandle;
    public  long getObservationHandle() { return _observationHandle; } // IMessage

    // public native PayloadList<Payload> getPayloadList(); // IMessage

    public native ObservationList<Observation> getObservations(); // IMessage

    // public native int getObservationType(); // IMessage

    // Number of vendor specific header options sent or recd.
    // StimulusIn:   uint8_t numRcvdVendorSpecificHeaderOptions;
    //              OCHeaderOption * rcvdVendorSpecificHeaderOptions;
    // ObservationOut: uint8_t numSendVendorSpecificHeaderOptions;
    //              OCHeaderOption sendVendorSpecificHeaderOptions[MAX_HEADER_OPTIONS];
    // public  int  optionCount;
    // private long ptr_Options;	// OCHeaderOption*

    private List<HeaderOption> _options;
    public native List<HeaderOption> getOptions(); // IMessage
}
