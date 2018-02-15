package openocf;

import android.util.Log;
import android.content.Context;

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
public class Service {
    private static final String TAG = "openocf.Service";

    public static native void config_logging(String logfile_fname);
    public static native void config_svrs(String svrs_config_fname);
    public static native String configuration();

    public static void configStandardEnv(String logfile_fname, String svrs_config_fname)
    {
	Log.v(TAG, "config");
	System.out.println("openocf.Service::config");
	try{
	    // Plain Java on macOS:
	    NativeUtils.loadLibraryFromJar("/libopenocf_jni.dylib");
	    // Linux:
	    // NativeUtils.loadLibraryFromJar("/libopenocf_jni.so");
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
	    Log.v(TAG, "getting path for resource " + svrs_config_fname);
	    System.out.println("Getting path for resource " + svrs_config_fname);
	    String f = NativeUtils.extractSVRConfigFile(svrs_config_fname);
	    config_svrs(f);
	} catch(Exception e){
	    System.out.println(e.toString());
	    System.exit(0);
	}
    }

    public static void configAndroidEnv(Context ctx)
    {
	Log.v(TAG, "config");
	System.out.println("openocf.Service::config");
	try{
	    System.loadLibrary("openocfserver");
	} catch (Exception e) {
            // This is probably not the best way to handle exception :-)
	    System.out.println("loadLibrary failed for openocfserver");
            e.printStackTrace();
        }
	System.out.println("LOADED libopenocfserver.so");

	try {
	    config_logging(null);
	} catch(Exception e){
	    System.out.println(e.toString());
	    System.exit(0);
	}

	try {
	    System.out.println("Getting path for resource " + "svrs.cbor");
	    String f = extractAssetToFile(ctx, "svrs.cbor");
	    config_svrs(f);
	} catch(Exception e){
	    System.out.println(e.toString());
	    System.exit(0);
	}
    }

    public static void extractAsset(Context context, String filename)
	// throws BBBEPubFileNotFoundException
    {
	try {
	    InputStream myInput = context.getAssets().open(filename);
	    OutputStream myOutput = context.openFileOutput(filename,
							   Context.MODE_PRIVATE);

	    // transfer bytes from the input file to the output file
	    byte[] buffer = new byte[1024];
	    int length;
	    while ((length = myInput.read(buffer)) > 0) {
		myOutput.write(buffer, 0, length);
	    }

	    // Close the streams
	    myOutput.flush();
	    myOutput.close();
	    myInput.close();
	} catch (Exception e) {
	    System.out.println(e.toString());
	    System.exit(0);
	}
    }
    private static String extractAssetToFile(Context ctx, String fileName)
    {
	Log.v(TAG, "private files path: " + ctx.getFilesDir());
	File file = new File(ctx.getFilesDir(), fileName);
	String fullPath = file.getAbsolutePath();
	if (!file.exists()) {
		extractAsset(ctx, fileName);
	}
	return fullPath;
    }

    private static String getPathForResource(String path) {
	URL url = Service.class.getClassLoader().getResource(path);
	System.out.println("URL for resource: " +  url);
	return url.getPath();
    }

}
