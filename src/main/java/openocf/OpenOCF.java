package openocf;

//import openocf.engine.OCFCommonSP;

import java.io.File;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.URL;

// import cz.adamh.utils.NativeUtils;

/**
 *
 */
public class OpenOCF {
    private static final String TAG = "openocf.std.OpenOCF";

    // private static final String JNILIB = "/libopenocf_jni.so";
    private static final String JNILIB = "/libopenocf_jni.dylib";

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

    native public static void config_logging(String logfile_fname);
    native public static void config_svrs(String svrs_config_fname);

    // public static void config(String logfile_fname, String svrs_config_fname)
    // {
    // 	System.out.println("openocf.standard.OCFServices::config");
    // 	try{
    // 	    // Plain Java on macOS:
    // 	    NativeUtils.loadLibraryFromJar(JNILIB);
    // 	    // Linux:
    // 	    // NativeUtils.loadLibraryFromJar(JNILIB);
    // 	} catch (Exception e) {
    //         // This is probably not the best way to handle exception :-)
    // 	    System.out.println("NativeUtils.loadLibraryFromJar failed");
    //         e.printStackTrace();
    //     }
    // 	System.out.println("LOADED " + JNILIB);

    // 	try {
    // 	    config_logging(logfile_fname);
    // 	} catch(Exception e){
    // 	    System.out.println(e.toString());
    // 	    System.exit(0);
    // 	}

    // 	try {
    // 	    System.out.println("Getting path for resource " + "/svrs.cbor"); // svrs_config_fname);
    // 	    String f = NativeUtils.extractSVRConfigFile("/svrs.cbor");
    // 	    config_svrs(f);
    // 	} catch(Exception e){
    // 	    System.out.println(e.toString());
    // 	    System.exit(0);
    // 	}
    // }

    // below: from CoServiceManager.
    // FIXME: this does not belong here? move to openocf/ocfresources?
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

    // FIXME: this does not belong here?
     native public static void configureDeviceSP(String deviceName, // setDeviceInfo
						String[] types,
						String specVersion,
						String[] dataModelVersions);

    // returns configuration info from engine, e.g. version string
    native public static String configuration();

     native public static void /*OCStackResult*/
	 Init(int /*OCMode*/ mode);
	     // String securityConfigFileName);

    // public static native void /*OCStackResult*/
    // 	Init(String /*const char* */ ip_addr,
    // 	     int /*uint16_t*/ port,
    // 	     int /*OCMode*/ mode,
    // 	     String securityConfigFileName);

     native public static void /*OCStackResult*/
	Init(int /*OCMode*/ mode,
	     int /*OCTransportFlags*/ server_flags,
	     int /*OCTransportFlags*/ client_flags);

    native private int /*OCStackResult*/
    // OCRegisterPersistentStorageHandler(Object /*OCPersistentStorage* */ persistent_storage_handler);
    // OCPersistentStorage ps = { server_fopen, fread, fwrite, fclose, unlink };
    // OCRegisterPersistentStorageHandler(&ps);
	XOCRegisterPersistentStorageHandler(Object /*OCPersistentStorage* */ persistent_storage_handler);
    //NB: default ps file is "oic_svr_db.dat"

    native public void /*OCStackResult*/ OCStartMulticastServer();

    native public void /*OCStackResult*/ OCStopMulticastServer();

    native public static void /*OCStackResult*/	run();

    native public static void /*OCStackResult*/ stop();

    // setPlatformInfo =>  ServiceManager.registerPlatform

    // setDeviceInfo =>  ServiceManager.registerDevice

    // OCDoResource =>  ServiceManager.sendResource

    native public void OCCancel(Object /*OCDoHandle*/ handle,
				int /*OCQualityOfService*/ qos,
				Object /*OCHeaderOption* */ options,
				byte /*uint8_t*/ options_count);
}
