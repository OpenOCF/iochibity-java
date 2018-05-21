package org.openocf.test.client;

import openocf.OpenOCFClient;
import openocf.utils.Endpoint;
import openocf.utils.CoAPOption;
// import openocf.StimulusIn;
// import openocf.PropertyMap;
import openocf.app.ResourceSP;
import openocf.app.IResourceSP;
// import openocf.ServiceManager;

// import org.openocf.test.client.DiscoveryCoSP;
// import org.openocf.test.client.GenericCoSP;

import openocf.exceptions.OCFNotImplementedException;

import openocf.constants.Method;
import openocf.constants.OCStackResult;
import openocf.constants.ResourcePolicy;
import openocf.constants.ServiceResult;


// import mraa.mraa;
// import mraa.IntelEdison;
// import mraa.Platform;
// import mraa.Result;

import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.ArrayList;
import java.util.List;
import java.util.LinkedList;
import java.util.Map;

// FIXME: make this a singleton?
public class ClientConfig
{
    // public static DiscoveryCoSP discoveryCoSP;

    static Handler uiHandler = new Handler(Looper.getMainLooper()) {
	    /*
	     * handleMessage() defines the operations to perform when
	     * the Handler receives a new Message to process.
	     */
	    @Override
	    public void handleMessage(Message inputMessage) {
		System.out.println("DISCOVERY CoRSP Handling Message");
		// update the UI
	    }
	};

    public static void configure() // String[] args)
    {
	Log.v("Bazel AndroidClient:", "go");

	// Platform platform = mraa.getPlatformType();
	// if (platform != Platform.INTEL_EDISON_FAB_C) {
	//     System.err.println("Error: This program can only be run on edison");
	//     System.exit(Result.ERROR_INVALID_PLATFORM.swigValue());
	// }

	Runtime.getRuntime().addShutdownHook(new Thread()
	    {
		@Override
		public void run()
		{
		    System.out.println("Shutdown hook running!");
		    OpenOCFClient.stop();
		}
	    });

	// System.out.println(System.getProperty("java.library.path"));
	OpenOCFClient.Init(OpenOCFClient.CLIENT); // _SERVER);

	// ServiceManager.configurePlatformSP("76a10fbf-0cbb-4e27-a748-cec0eb9bdc93",
	// 				   "Acme Novelties",
	// 				   "http://acme.example.org",
	// 				   "modelnbr", "mfgdate", "platversion",
	// 				   "osversion", "hwversion", "firmwareversion",
	// 				   "http://acme.example.org/support",
	// 				   new SimpleDateFormat("yyyy.MM.dd.HH.mm.ss").format(new Date()));

	// ServiceManager.configureDeviceSP("Fartmaster2020 Client",
	// 				 new String[] {"type1", "type2"},
	// 				 "version-0.1",
	// 				 new String[] {"dmversion-0.1"});

	// TemperatureSP tempSP = new TemperatureSP();
	// tempSP.addType("foo.t.bar");
	// tempSP.addInterface("foo.if.bar");
	// Logger.logSP(tempSP);

	// ResourceSP tSP = null;
	// tSP = ServiceManager.registerResourceSP(tempSP);

	// ServiceManager.registerResourceSP(new LedSP());

	OpenOCFClient.run();

        while(true){
	    try {
		Thread.sleep(2000);
		// System.out.println("GUI thread loop");
	    } catch (InterruptedException e) {
		e.printStackTrace();
		msgError(TAG, e.toString());
	    }
        }
    }

    public static void msgError(final String tag ,final String text) {
        System.out.println("[E]" + tag + " | " + text);
    }

    private final static String TAG = ClientConfig.class.getSimpleName();
    // static BufferedReader in=new BufferedReader(new InputStreamReader(System.in));
    // static boolean quit=false;
    // static class Quitter implements Runnable {
    // 	public void run(){
    // 	    String msg = null;
    // 	    // threading is waiting for the key Q to be pressed
    // 	    while(true){
    // 		try{
    // 		    msg=in.readLine();
    // 		}catch(IOException e){
    // 		    e.printStackTrace();
    // 		}

    // 		if(msg.equals("Q")) {quit=true;break;}
    // 	    }
    // 	}
    // }
}
