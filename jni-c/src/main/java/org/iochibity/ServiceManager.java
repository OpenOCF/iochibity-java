package org.iochibity;

import java.util.List;
import java.util.LinkedList;

public class ServiceManager
{
    public static List<CoServiceProvider> coServiceProviders;

    static {
	coServiceProviders = new LinkedList<CoServiceProvider>();
    }

    // OCGetNumberOfResources
    public static native int resourceCount();

    public static native void registerPlatformProvider(String platform_id, // setPlatformInfo
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

    public static native void registerDeviceProvider(String deviceName, // setDeviceInfo
					    String[] types,
					    String specVersion,
					    String[] dataModelVersions);

    // OCSetDefaultDeviceEntityHandler
    public static native void registerDefaultServiceRoutine
	(Object /*OCDeviceEntityHandler*/ serviceRoutine,
	 Object /*void* */ callbackParam);

    // public static native int OCCreateResource(Object /*OCResourceHandle* */ handle,
    public synchronized static native ServiceProvider
	registerServiceProvider(ServiceProvider serviceProvider);

    public synchronized static void registerCoServiceProvider(CoServiceProvider coServiceProvider)
    {
	System.out.println("registerCoServiceProvider ENTRY");
	coServiceProviders.add(coServiceProvider);
    }



    // public synchronized static native ResourceLocal registerServiceProvider(String Uri,
    // 							String[] resourceTypeName,
    // 							String[] resourceInterfaceName,
    // 							IServiceProvider serviceRoutine,
    // 							// CallbackParam /*void* */ callbackParam,
    // 							byte   /*uint8_t*/ policies);

    // public static native ResourceLocal registerWatchableProvider(String Uri,
    // 								 String[] resourceTypeName,
    // 								 String[] resourceInterfaceName,
    // 								 IServiceProvider serviceRoutine,
    // 								 byte   /*uint8_t*/ policies);

    ////////////////////////////////////////////////////////////////
    // stack/internal/ocresourcehandler.h:
    // OCResource *FindResourceByUri(const char* resourceUri);
    // bool IsVirtualResource(const char* resourceUri);
    // OCStackResult GetPlatformInfo(OCPlatformInfo** info);

}
