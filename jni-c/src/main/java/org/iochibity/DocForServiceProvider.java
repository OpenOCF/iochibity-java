package org.iochibity;

import java.util.ArrayList;
import java.util.LinkedList;

// base for docs on server - DocRequestIn, DocResponseOut
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
