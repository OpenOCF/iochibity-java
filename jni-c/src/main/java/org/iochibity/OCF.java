package org.iochibity;


public class OCF
{
    // OCMode
    public static final int CLIENT = 0;
    public static final int SERVER = 1;
    public static final int CLIENT_SERVER = 2;
    public static final int GATEWAY = 3;

    // OCQualityOfService
    public static final int QOS_LOW    = 0; // best-effort
    public static final int QOS_MEDIUM = 1;
    public static final int QOS_HIGH   = 2; // ACKs confirm delivery
    public static final int QOS_NA     = 3; // stack decides

    // ****************************************************************
    public static native void /*OCStackResult*/
	Init(String /*const char* */ ip_addr,
	     int /*uint16_t*/ port,
	     int /*OCMode*/ mode,
	     String securityConfigFileName);

    public static native void /*OCStackResult*/
	OCInit1(int /*OCMode*/ mode,
		int /*OCTransportFlags*/ server_flags,
		int /*OCTransportFlags*/ client_flags);

    private native int /*OCStackResult*/
    // OCRegisterPersistentStorageHandler(Object /*OCPersistentStorage* */ persistent_storage_handler);
    // OCPersistentStorage ps = { server_fopen, fread, fwrite, fclose, unlink };
    // OCRegisterPersistentStorageHandler(&ps);
	OCRegisterPersistentStorageHandler(Object /*OCPersistentStorage* */ persistent_storage_handler);
    //NB: default ps file is "oic_svr_db.dat"

    public native void /*OCStackResult*/ OCStartMulticastServer();

    public native void /*OCStackResult*/ OCStopMulticastServer();

    public static native void /*OCStackResult*/	run();

    public static native void /*OCStackResult*/ stop();

    // setPlatformInfo =>  ResourceManager.registerPlatform

    // setDeviceInfo =>  ResourceManager.registerDevice

    // OCDoResource =>  ResourceManager.sendResource

    public native void OCCancel(Object /*OCDoHandle*/ handle,
				int /*OCQualityOfService*/ qos,
				Object /*OCHeaderOption* */ options,
				byte /*uint8_t*/ options_count);

    /* OCStackResult OCDoResource */
    static public native void sendRequest(int method,
					  DocRequestOut requestOut); // QoS default: low
    static public native void sendRequest(int method,
					  DocRequestOut requestOut,
					  int qualityOfService);

    // e.g. sendRequest(Method.GET, lightRequestOut, OCF.QOS_HOW);
    // e.g. sendRequest(Method.DISCOVER, platformRequestOut);

    // discovery returns doHandle (token)
    static public native byte[] discoverPlatform(DocRequestOut dro); // multicast
    static public native byte[] discoverPlatform(IResourceServiceRequestor rsr); // multicast
    static public native byte[] discoverPlatform(IResourceServiceRequestor rsr, DeviceAddress... da);

    static public native byte[] discoverDevices(IResourceServiceRequestor rsr, DeviceAddress... da);
    static public native byte[] discoverResources(IResourceServiceRequestor rsr, DeviceAddress... da);



    /* OCStackResult OCDoResponse */
    static public native void sendResponse(DocResponseOut responseOut); // OCEntityHandlerResponse*
}
