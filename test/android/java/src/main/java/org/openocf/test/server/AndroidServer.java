package org.openocf.test.server;

import openocf.Service;
import org.iochibity.OCF;
import org.iochibity.DeviceAddress;
import org.iochibity.HeaderOption;
// import org.iochibity.StimulusIn;
import org.iochibity.PropertyMap;
import org.iochibity.ServiceProvider;
import org.iochibity.IServiceProvider;
import org.iochibity.ServiceManager;

// import org.openocf.test.server.LedSP;
// import org.openocf.test.server.LightSP;
// import org.openocf.test.server.TemperatureSP;
import org.openocf.test.server.WhatsitSP;

import org.iochibity.exceptions.OCFNotImplementedException;

import org.iochibity.constants.Method;
import org.iochibity.constants.OCStackResult;
import org.iochibity.constants.ResourcePolicy;
import org.iochibity.constants.ServiceResult;


// import mraa.mraa;
// import mraa.IntelEdison;
// import mraa.Platform;
// import mraa.Result;

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

public class AndroidServer
{
    // static{			// FIXME: put this in openocf/Service.java?
    // 	Log.v("Bazel AndroidServer:", "Static Init");
    // 	Service.config(//null,
    // 		       "/tmp/oocf_server.log", // logfile_fname
    // 		       "/server_config.cbor"
    // 		       // "OCFTestServer.runfiles/__main__/test/server_config.cbor" // svrs_config_fname
    // 		       );
    // }

    public static void go() // String[] args)
    {
	Log.v("Bazel AndroidServer:", "go");

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
		    OCF.stop();
		}
	    });

	// System.out.println(System.getProperty("java.library.path"));
	OCF.Init(OCF.SERVER); // , "src/main/resources/ocftestserver_config.cbor");

	ServiceManager.configurePlatformSP("76a10fbf-0cbb-4e27-a748-cec0eb9bdc92",
					   "Acme Novelties",
					   "http://acme.example.org",
					   "modelnbr", "mfgdate", "platversion",
					   "osversion", "hwversion", "firmwareversion",
					   "http://acme.example.org/support",
					   new SimpleDateFormat("yyyy.MM.dd.HH.mm.ss").format(new Date()));

	ServiceManager.configureDeviceSP("Fartmaster2020 Server",
					 new String[] {"type1", "type2"},
					 "version-0.1",
					 new String[] {"dmversion-0.1"});

	// TemperatureSP tempSP = new TemperatureSP();
	// tempSP.addType("foo.t.bar");
	// tempSP.addInterface("foo.if.bar");
	// Logger.logSP(tempSP);

	// ServiceProvider tSP = null;
	// tSP = ServiceManager.registerServiceProvider(tempSP);

	ServiceManager.registerServiceProvider(new WhatsitSP());
	ServiceManager.registerServiceProvider(new WhatsitSP("/bar/whatsit"));

	// ServiceManager.registerServiceProvider(new LedSP());

	OCF.run();

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

    private final static String TAG = AndroidServer.class.getSimpleName();
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
