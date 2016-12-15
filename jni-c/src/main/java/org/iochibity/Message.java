package org.iochibity;

import java.util.List;

// i.e. base Message
public class Message implements IMessage
{
    // OCPayloadType enum
    public static final int INVALID        = 0;
    public static final int DISCOVERY      = 1;
    public static final int DEVICE         = 2;
    public static final int PLATFORM       = 3;
    public static final int REPRESENTATION = 4;
    public static final int SECURITY       = 5;
    public static final int PRESENCE       = 6;
    public static final int RD             = 7;
    public static final int NOTIFICATION   = 8;

    // Request local handle, for this (OCEntityHandlerRequest, OCClientResponse)
    public long localHandle;

    private int _method;
    public int getMethod() { return _method; }; // IMessage

    public DeviceAddress _remoteDeviceAddress;
    public DeviceAddress getRemoteDeviceAddress() { return _remoteDeviceAddress; } // IMessage

    // the payload from the request PDU.
    private long _payloadHandle;
    public  long getPayloadHandle() { return _payloadHandle; } // IMessage

    public native PayloadList<Payload> getPayloadList(); // IMessage

    public native int getMsgType(); // IMessage

    // Number of vendor specific header options sent or recd.
    // MsgRequestIn:   uint8_t numRcvdVendorSpecificHeaderOptions;
    //              OCHeaderOption * rcvdVendorSpecificHeaderOptions;
    // MsgResponseOut: uint8_t numSendVendorSpecificHeaderOptions;
    //              OCHeaderOption sendVendorSpecificHeaderOptions[MAX_HEADER_OPTIONS];
    // public  int  optionCount;
    private long ptr_Options;	// OCHeaderOption*
    private List<HeaderOption> _options;
    public native List<HeaderOption> getOptions(); // IMessage
}
