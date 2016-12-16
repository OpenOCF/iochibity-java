package org.iochibity;


public class Messenger
{
    // Client side
    static public native byte[] sendRequest(int method,
					  MsgRequestOut requestOut); // QoS default: low
    static public native byte[] sendRequest(int method,
					  MsgRequestOut requestOut,
					  int qualityOfService);

    // e.g. sendRequest(Method.GET, lightRequestOut, OCF.QOS_HOW);
    // e.g. sendRequest(Method.DISCOVER, platformRequestOut);

    // discovery returns doHandle (token)
    static public native byte[] discoverPlatforms(MsgRequestOut mro); // multicast
    static public native byte[] discoverPlatforms(IServiceRequestor rsr); // multicast
    static public native byte[] discoverPlatforms(IServiceRequestor rsr, DeviceAddress... da);

    static public native byte[] discoverDevices(MsgRequestOut mro);
    static public native byte[] discoverDevices(IServiceRequestor rsr, DeviceAddress... da);

    static public native byte[] discoverResources(MsgRequestOut mro);
    static public native byte[] discoverResources(IServiceRequestor rsr, DeviceAddress... da);

    // Server side
    /* Server: OCDoResponse */
    static public native void sendResponse(MsgResponseOut msgResponseOut); // OCEntityHandlerResponse*

    /* Client: OCDoResource */
    static public native void sendRequest(MsgRequestOut msgRequestOut); // OCEntityHandlerResponse*
}
