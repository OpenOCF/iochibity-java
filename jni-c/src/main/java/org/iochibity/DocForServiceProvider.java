package org.iochibity;

import java.util.ArrayList;
import java.util.LinkedList;

// base for docs on server
public class DocForServiceProvider extends Document
{
    // Request local handle, for this OCEntityHandlerRequest
    public long localHandle;

    // Request remote handle - request handle at ORIGIN (i.e. client)? */
    // OCRequestHandle requestHandle;
    public long remoteHandle;

    private long resourceHandle;
    public long getResourceHandle() { return resourceHandle; }
    public native ResourceLocal getResource();

    // the payload from the request PDU.
    private long payloadHandle;
    public native PayloadList<PayloadForResourceState> getPDUPayload();
    // public native Payload getPayload();

    // Number of vendor specific header options sent or recd.
    // DocRequestIn:   uint8_t numRcvdVendorSpecificHeaderOptions;
    //              OCHeaderOption * rcvdVendorSpecificHeaderOptions;
    // DocResponseOut: uint8_t numSendVendorSpecificHeaderOptions;
    //              OCHeaderOption sendVendorSpecificHeaderOptions[MAX_HEADER_OPTIONS];
    private int vendorHeaderOptionCount;
    private long vendorHeaderOptions;
    public native ArrayList<HeaderOption> getVendorHeaderOptions();

    //     /** Pointer of ActionSet which to support group action.*/
    //     OCActionSet *actionsetHead;

    //     /** The instance identifier for this web link in an array of web links - used in links. */
    //     union
    //     {
    //         /** An ordinal number that is not repeated - must be unique in the collection context. */
    //         uint8_t ins;
    //         /** Any unique string including a URI. */
    //         char *uniqueStr;
    //         /** Use UUID for universal uniqueness - used in /oic/res to identify the device. */
    //         OCIdentity uniqueUUID;
    //     };
    // } OCResource;
}
