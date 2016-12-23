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
    public long getObservationHandle();

    public ObservationList<Observation> getObservations();

    // this does not belong in Message:
    // public int getObservationType();

    // Number of vendor specific header options sent or recd.
    // StimulusIn:   uint8_t numRcvdVendorSpecificHeaderOptions;
    //              OCHeaderOption * rcvdVendorSpecificHeaderOptions;
    // ObservationOut: uint8_t numSendVendorSpecificHeaderOptions;
    //              OCHeaderOption sendVendorSpecificHeaderOptions[MAX_HEADER_OPTIONS];
    // public  int  optionCount;
    // private long ptr_Options;	// OCHeaderOption*
    public List<HeaderOption> getOptions();

}
