package org.iochibity;

import java.util.List;

public interface IMessage
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

    public DeviceAddress getRemoteDeviceAddress();

    // the payload from the request PDU.
    // long payloadHandle = 0;
    public long getPayloadHandle();

    public PayloadList<Payload> getPayloadList();

    public int getMsgType();

    // Number of vendor specific header options sent or recd.
    // MsgRequestIn:   uint8_t numRcvdVendorSpecificHeaderOptions;
    //              OCHeaderOption * rcvdVendorSpecificHeaderOptions;
    // MsgResponseOut: uint8_t numSendVendorSpecificHeaderOptions;
    //              OCHeaderOption sendVendorSpecificHeaderOptions[MAX_HEADER_OPTIONS];
    // public  int  optionCount;
    // private long ptr_Options;	// OCHeaderOption*
    public List<HeaderOption> getOptions();

}
