package org.iochibity;

public class OCF
{
    public static native int /*OCStackResult*/
	Init(String /*const char* */ ip_addr,
	     int /*uint16_t*/ port,
	     int /*OCMode*/ mode,
	     String securityConfigFileName);

    public static native int /*OCStackResult*/
	OCInit1(int /*OCMode*/ mode,
		int /*OCTransportFlags*/ server_flags,
		int /*OCTransportFlags*/ client_flags);

    private native int /*OCStackResult*/
	// OCRegisterPersistentStorageHandler(Object /*OCPersistentStorage* */ persistent_storage_handler);
    // OCPersistentStorage ps = { server_fopen, fread, fwrite, fclose, unlink };
    // OCRegisterPersistentStorageHandler(&ps);
	OCRegisterPersistentStorageHandler(Object /*OCPersistentStorage* */ persistent_storage_handler);
    //NB: default ps file is "oic_svr_db.dat"

    public native int /*OCStackResult*/ OCStartMulticastServer();

    public native int /*OCStackResult*/ OCStopMulticastServer();

    public static native int /*OCStackResult*/	run();

    public static native void /*OCStackResult*/ stop();

    public static native int setPlatformInfo(String platform_id,
					       String manufacturer_name,
					       String manufacturer_url,
					       String model_number,
					       String date_of_manufacture,
					       String platform_version,
					       String operating_system_version,
					       String hardware_version,
					       String firmware_version,
					       String support_url,
					       String system_time);

    public static native int setDeviceInfo(String deviceName,
					     String[] types,
					     String specVersion,
					     String[] dataModelVersions);

    public native int /*OCStackResult*/
	OCDoResource(Object   /*OCDoHandle* */       handle,
		     int      /*OCMethod*/           method,
		     String   /*const char* */       requestUri,
		     Object   /*const OCDevAddr* */  destination,
		     Object   /*OCPayload* */        payload,
		     int      /*OCConnectivityType*/ connectivityType,
		     int      /*OCQualityOfService*/ qos,
		     Object   /*OCCallbackData* */   cbData,
		     Object   /*OCHeaderOption* */   options,
		     byte     /*uint8_t*/            optionsCount);

    public native int /*OCStackResult*/
	OCCancel(Object /*OCDoHandle*/ handle,
		 int /*OCQualityOfService*/ qos,
		 Object /*OCHeaderOption* */ options,
		 byte /*uint8_t*/ options_count);

    /* OCStackResult OCDoResponse */
    static public native int sendResponse(ResponseOut response); // OCEntityHandlerResponse*
}
