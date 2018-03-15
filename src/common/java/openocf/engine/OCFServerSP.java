package openocf.engine;

import openocf.app.ResourceSP;

import java.util.List;

public class OCFServerSP extends OCFCommonSP
{
    public static native void exhibit(ResourceSP RSP);

    //FIXME: does this belong here?
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

    //FIXME: does this belong here?
    native public static void configureDeviceSP(String deviceName, // setDeviceInfo
						String[] types,
						String specVersion,
						String[] dataModelVersions);

    // below from ServiceManager.java

    // JNI implementations call into //src/ocf/ocf_services_server

    // public static native int OCCreateResource(Object /*OCResourceHandle* */ handle,
    native public synchronized static
	ResourceSP	registerResourceSP(ResourceSP serviceProvider);

    native public static List<ResourceSP> registeredResourceSPs();

    native public static List<ResourceSP> getRelatedResourceSPs();


}
