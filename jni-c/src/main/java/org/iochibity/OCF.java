package org.iochibity;

public class OCF
{
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

    /* OCStackResult OCDoResponse */
    static public native void sendResponse(DocResponseOut responseOut); // OCEntityHandlerResponse*
}
