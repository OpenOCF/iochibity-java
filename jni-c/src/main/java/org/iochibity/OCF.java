package org.iochibity;


public class OCF
{
    // Methods
    public static final int NOMETHOD       = 0;
    public static final int RETRIEVE       = (1 << 0); // GET
    public static final int PUT            = (1 << 1);
    public static final int POST           = (1 << 2);
    public static final int DELETE         = (1 << 3);
    public static final int WATCH        = (1 << 4); // OBSERVE
    public static final int WATCH_ALL    = (1 << 5); // OBSERVE_ALL
    public static final int CANCEL_WATCH = (1 << 6); // CANCEL_OBSERVE

    // #ifdef WITH_PRESENCE
    /** Subscribe for all presence notifications of a particular resource.*/
    public static final int PRESENCE       = (1 << 7);
    // #endif

    /** Allows OCDoResource caller to do discovery.*/
    public static final int DISCOVER       = (1 << 8);

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
	Init(int /*OCMode*/ mode,
	     String securityConfigFileName);

    // public static native void /*OCStackResult*/
    // 	Init(String /*const char* */ ip_addr,
    // 	     int /*uint16_t*/ port,
    // 	     int /*OCMode*/ mode,
    // 	     String securityConfigFileName);

    public static native void /*OCStackResult*/
	Init(int /*OCMode*/ mode,
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

    // setPlatformInfo =>  ServiceManager.registerPlatform

    // setDeviceInfo =>  ServiceManager.registerDevice

    // OCDoResource =>  ServiceManager.sendResource

    public native void OCCancel(Object /*OCDoHandle*/ handle,
				int /*OCQualityOfService*/ qos,
				Object /*OCHeaderOption* */ options,
				byte /*uint8_t*/ options_count);
}
