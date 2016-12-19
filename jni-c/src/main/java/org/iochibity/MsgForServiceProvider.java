package org.iochibity;

import java.util.ArrayList;
import java.util.LinkedList;

// base for docs on server - MsgRequestIn, MsgResponseOut
public class MsgForServiceProvider extends Message
{
    // Request remote handle - request handle at ORIGIN (i.e. client)? */

    private long                     _remoteRequestHandle; // OCRequestHandle requestHandle = OCRequest*
    // public  long                    getRemoteRequestHandle() { return _remoteHandle; }

    private long                    _resourceHandle; // OCResource*
    // public  long                    getResourceHandle() { return _resourceHandle; }

    // since this MsgRequestIn gets passed to ServiceProvider, we do
    // not be able to get it
    // public  native IServiceProvider getServiceProvider();   // getResource();

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
