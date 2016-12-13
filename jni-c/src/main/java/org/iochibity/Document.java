package org.iochibity;

import java.util.List;

// i.e. base Document
public class Document
{
    // the payload from the request PDU.
    private long payloadHandle;
    public native PayloadList<PayloadForResourceState> getPDUPayload();
    public native int getPayloadType();

    // Number of vendor specific header options sent or recd.
    // DocRequestIn:   uint8_t numRcvdVendorSpecificHeaderOptions;
    //              OCHeaderOption * rcvdVendorSpecificHeaderOptions;
    // DocResponseOut: uint8_t numSendVendorSpecificHeaderOptions;
    //              OCHeaderOption sendVendorSpecificHeaderOptions[MAX_HEADER_OPTIONS];
    public  int  optionCount;
    private long ptr_Options;	// OCHeaderOption*
    public native List<HeaderOption> getOptions();

}
