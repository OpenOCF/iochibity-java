package org.iochibity;

public class ResourceManager
{
    // OCGetNumberOfResources
    public static native int resourceCount();

    public static native void registerPlatform(String platform_id, // setPlatformInfo
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

    public static native void registerDevice(String deviceName, // setDeviceInfo
					    String[] types,
					    String specVersion,
					    String[] dataModelVersions);

    // OCSetDefaultDeviceEntityHandler
    public static native void registerDefaultServiceRoutine
	(Object /*OCDeviceEntityHandler*/ serviceRoutine,
	 Object /*void* */ callbackParam);

    // public static native int OCCreateResource(Object /*OCResourceHandle* */ handle,
    public static native ResourceLocal registerResource(String Uri,
							String[] resourceTypeName,
							String[] resourceInterfaceName,
							IResourceServiceProvider serviceRoutine,
							// CallbackParam /*void* */ callbackParam,
							byte   /*uint8_t*/ policies);

    public static native ResourceLocal registerWatchableResource(String Uri,
								 String[] resourceTypeName,
								 String[] resourceInterfaceName,
								 IResourceServiceProvider serviceRoutine,
								 byte   /*uint8_t*/ policies);

    ////////////////////////////////////////////////////////////////
    // stack/internal/ocresourcehandler.h:
    // OCResource *FindResourceByUri(const char* resourceUri);
    // bool IsVirtualResource(const char* resourceUri);
    // OCStackResult GetPlatformInfo(OCPlatformInfo** info);

}
