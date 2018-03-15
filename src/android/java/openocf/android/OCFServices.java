package openocf.android;

import openocf.engine.OCFCommonSP;

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
public class OCFServices extends OCFCommonSP {
    private static final String TAG = "openocf.android.OCFServices";

    public static native void config_logging(String logfile_fname);
    public static native void config_svrs(String svrs_config_fname);
    // public static native String configuration();

    public static void config(Context ctx, String apk)
    {
	Log.v(TAG, "config");
	System.out.println("openocf.android.OCFServices::config");
	try{
	    System.loadLibrary(apk);
	} catch (Exception e) {
            // This is probably not the best way to handle exception :-)
	    System.out.println("loadLibrary failed for openocfserver");
            e.printStackTrace();
        }
	System.out.println("LOADED lib" + apk + ".so");

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
}
