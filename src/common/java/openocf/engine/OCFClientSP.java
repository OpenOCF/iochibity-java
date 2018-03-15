package openocf.engine;

import java.net.URL;
import java.util.List;

import openocf.app.CoResourceSP;
import openocf.payload.CoResourceLink;
import openocf.signals.InboundResponse;

public class OCFClientSP
    extends OCFCommonSP
    // implements ICoResourceDBM
{
    public static native void coExhibit(CoResourceSP CoRSP);
    // native public void            coExhibit(); // called by user

    // below: from CoServiceManager.
    // FIXME: this does not belong here? move to openocf/ocfresources?
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

    // FIXME: this does not belong here?
    public static native void configureDeviceSP(String deviceName, // setDeviceInfo
						String[] types,
						String specVersion,
						String[] dataModelVersions);

    // wrap OCSetDefaultDeviceEntityHandler:
    native public static CoResourceSP registerDefaultCoResourceSP(CoResourceSP cosp);


    // FIXME: define an interface for the CoResourceDBM API

    // CoResourceDBM Services: virtual service managing database of CoRSPs
    // jni maps to ocf_services_client/coresource_dbm.c

    native public synchronized static void retain(InboundResponse response);

    // public static native int OCCreateResource(Object /*OCResourceHandle* */ handle,
    // native public synchronized static CoResourceSP registerCoResourceSP(CoResourceSP coRSP);

    // native public static List<CoResourceSP> registeredCoResourceSPs();
    native public static List<InboundResponse> getInboundResponses();

    native public static InboundResponse getInboundResponse(long handle);

    // resource link = OCResourcePayload within OCDiscoveryPayload
    native public static List<CoResourceLink> getCoResourceLinks();

    native public static CoResourceLink getCoResourceLink(URL url);

    // native public static List<CoResourceSP> getRelatedCoResourceSPs();
}
