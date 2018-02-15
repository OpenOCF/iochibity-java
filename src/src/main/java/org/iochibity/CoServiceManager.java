package org.iochibity;

import java.util.List;
import java.util.LinkedList;

final public class CoServiceManager
    // implements ICoServiceManager
{
    public static native void configurePlatformSP(String platform_id, // setPlatformInfo
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

    public static native void configureDeviceSP(String deviceName, // setDeviceInfo
						String[] types,
						String specVersion,
						String[] dataModelVersions);

    // OCSetDefaultDeviceEntityHandler
    native public static CoServiceProvider registerDefaultCoServiceProvider(CoServiceProvider cosp);

    // public static native int OCCreateResource(Object /*OCResourceHandle* */ handle,
    native public synchronized static
	CoServiceProvider registerCoServiceProvider(CoServiceProvider serviceProvider);

    native public static List<CoServiceProvider> registeredCoServiceProviders();

    native public static List<CoServiceProvider> getRelatedCoServiceProviders();


    // public synchronized static native ResourceLocal registerCoServiceProvider(String Uri,
    // 							String[] resourceTypeName,
    // 							String[] resourceInterfaceName,
    // 							ICoServiceProvider serviceRoutine,
    // 							// CallbackParam /*void* */ callbackParam,
    // 							byte   /*uint8_t*/ policies);

    // public static native ResourceLocal registerWatchableSP(String Uri,
    // 							   String[] resourceTypeName,
    // 							   String[] resourceInterfaceName,
    // 							   ICoServiceProvider serviceRoutine,
    // 							   byte   /*uint8_t*/ policies);

    ////////////////////////////////////////////////////////////////
    // stack/internal/ocresourcehandler.h:
    // OCResource *FindResourceByUri(const char* resourceUri);
    // bool IsVirtualResource(const char* resourceUri);
    // OCStackResult GetPlatformInfo(OCPlatformInfo** info);

}
