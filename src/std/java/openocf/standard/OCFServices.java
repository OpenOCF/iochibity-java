package openocf.standard;

import openocf.engine.OCFCommonSP;

import java.io.File;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.URL;

import cz.adamh.utils.NativeUtils;

/**
 *
 */
public class OCFServices extends OCFCommonSP {
    private static final String TAG = "openocf.standard.OCFServices";

    public static native void config_logging(String logfile_fname);
    public static native void config_svrs(String svrs_config_fname);
    // public static native String configuration();

    public static void config(String logfile_fname, String svrs_config_fname)
    {
	System.out.println("openocf.standard.OCFServices::config");
	try{
	    // Plain Java on macOS:
	    // NativeUtils.loadLibraryFromJar("/libopenocf_jni.dylib");
	    // Linux:
	    NativeUtils.loadLibraryFromJar("/libopenocf_jni.so");
	} catch (Exception e) {
            // This is probably not the best way to handle exception :-)
	    System.out.println("NativeUtils.loadLibraryFromJar failed");
            e.printStackTrace();
        }
	System.out.println("LOADED libopenocfserver.so");

	try {
	    config_logging(logfile_fname);
	} catch(Exception e){
	    System.out.println(e.toString());
	    System.exit(0);
	}

	try {
	    System.out.println("Getting path for resource " + "/svrs.cbor"); // svrs_config_fname);
	    String f = NativeUtils.extractSVRConfigFile("/svrs.cbor");
	    config_svrs(f);
	} catch(Exception e){
	    System.out.println(e.toString());
	    System.exit(0);
	}
    }
}
