package org.iochibity;

import java.util.List;
import java.util.LinkedList;

final public class ServiceManager
    // implements IServiceManager
{
    native public static void configurePlatformSP(String platform_id, // setPlatformInfo
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

    native public static void configureDeviceSP(String deviceName, // setDeviceInfo
						String[] types,
						String specVersion,
						String[] dataModelVersions);

    // public static native int OCCreateResource(Object /*OCResourceHandle* */ handle,
    native public synchronized static
	ServiceProvider	registerServiceProvider(ServiceProvider serviceProvider);

    native public static List<ServiceProvider> registeredServiceProviders();

    native public static List<ServiceProvider> getRelatedServiceProviders();


    // public synchronized static native ResourceLocal registerServiceProvider(String Uri,
    // 							String[] resourceTypeName,
    // 							String[] resourceInterfaceName,
    // 							IServiceProvider serviceRoutine,
    // 							// CallbackParam /*void* */ callbackParam,
    // 							byte   /*uint8_t*/ policies);

    // public static native ResourceLocal registerWatchableSP(String Uri,
    // 							   String[] resourceTypeName,
    // 							   String[] resourceInterfaceName,
    // 							   IServiceProvider serviceRoutine,
    // 							   byte   /*uint8_t*/ policies);

    ////////////////////////////////////////////////////////////////
    // stack/internal/ocresourcehandler.h:
    // OCResource *FindResourceByUri(const char* resourceUri);
    // bool IsVirtualResource(const char* resourceUri);
    // OCStackResult GetPlatformInfo(OCPlatformInfo** info);

}
