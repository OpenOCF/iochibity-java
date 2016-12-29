package org.iochibity.test;

// UI stuff
// import com.googlecode.lanterna.TerminalSize;
// import com.googlecode.lanterna.TextColor;
// import com.googlecode.lanterna.gui2.*;
// import com.googlecode.lanterna.screen.Screen;
// import com.googlecode.lanterna.screen.TerminalScreen;
// import com.googlecode.lanterna.terminal.DefaultTerminalFactory;
// import com.googlecode.lanterna.terminal.Terminal;

// iochibity jni-c stuff
import org.iochibity.OCF;
import org.iochibity.CoServiceProvider;
import org.iochibity.DeviceAddress;
import org.iochibity.Message;
import org.iochibity.StimulusOut;
import org.iochibity.ObservationIn;
import org.iochibity.ObservationOut;
import org.iochibity.HeaderOption;
import org.iochibity.ObservationRecord;
import org.iochibity.ObservationList;
import org.iochibity.PropertyMap;
import org.iochibity.ServiceManager;
import org.iochibity.ICoServiceProvider;

import org.iochibity.test.client.DiscoveryCoSP;
import org.iochibity.test.client.GenericCoSP;

import org.iochibity.constants.OCStackResult;
import org.iochibity.constants.ResourcePolicy;
import org.iochibity.constants.ServiceResult;
import org.iochibity.exceptions.OCFNotImplementedException;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.ArrayList;
import java.util.List;
import java.util.LinkedList;
import java.util.Map;
import java.util.Scanner;
import java.util.concurrent.CountDownLatch;
import java.util.regex.Pattern;

import java.util.stream.Collectors;


public class OCFTestClient
{
    static{
	try{
	    System.loadLibrary("ocfjni_c");
	}catch(Exception e){
	    System.out.println(e.toString());
	}
    }

    static CountDownLatch gLatch;

    // public static class DeviceCoSP implements ICoServiceProvider
    // {
    // 	private int cbdata = 123;

    // 	public int observeBehavior(ObservationIn responseIn)
    // 	{
    // 	    System.out.println("LOG: DeviceCoSP.serviceResponse ENTRY");
    // 	    System.out.println("LOG: cbdata: " + cbdata);
    // 	    Logger.logObservationIn(responseIn);

    // 	    // save incoming resource info - ServiceManager.registerRemoteResource(...)?
    // 	    // update screen ...
    // 	    System.out.flush();
    // 	    return 0;
    // 	}

    // }

    // public static DeviceAddress gRemoteResourceAddress;
    // public static DeviceAddress gLEDAddress;
    // public static DeviceAddress gWhatsitAddress;

    // public static class DiscoverResourcesCoSP implements ICoServiceProvider
    // {
    // 	private int cbdata = 123;

    // 	public int observeBehavior(ObservationIn responseIn)
    // 	{
    // 	    System.out.println("LOG: DiscoverResourcesCoSP.serviceResponse ENTRY");
    // 	    System.out.println("LOG: cbdata: " + cbdata);

    // 	// }
    // 	    Logger.logObservationIn(responseIn);

    // 	    // save incoming resource info - ServiceManager.registerRemoteResource(...)?
    // 	    // update screen ...
    // 	    return 0;
    // 	}
    // }

    // public static class WhatsitCoSP implements ICoServiceProvider
    // {
    // 	private int cbdata = 123;

    // 	public int observeBehavior(ObservationIn responseIn)
    // 	{
    // 	    System.out.println("LOG: DiscoverResourcesCoServiceProvider.serviceResponse ENTRY");
    // 	    System.out.println("LOG: cbdata: " + cbdata);
    // 	    Logger.logObservationIn(responseIn);

    // 	    // save incoming resource info - ServiceManager.registerRemoteResource(...)?
    // 	    // update screen ...
    // 	    return 0;
    // 	}

    // }

    static DiscoveryCoSP discoveryCoSP;

    public static synchronized void promptUser()
    {
	Scanner scanner = new Scanner(System.in);
	String action = null;
	Pattern top  = Pattern.compile("[0-9x]");
	Pattern pdrx = Pattern.compile("[dprx]");

    	String uri = null;
	StimulusOut msgRequestOut;

	boolean again = true;
	while(again) {
	    System.out.println("Choose an action:");
	    System.out.println("\t1) Discover resources");
	    System.out.println("\t2) List discovered resources");
	    System.out.println("\t3) RETRIEVE");
	    System.out.println("\t4) WATCH");
	    System.out.println("\t5) PUT");
	    System.out.println("\t6) POST");
	    System.out.println("\t7) DELETE");
	    System.out.println("\tx) Exit\n");
	    while (!scanner.hasNext(top)) {
		System.out.println("Invalid input, try again");
		scanner.next();
	    }
	    action = scanner.next(top);
	    switch(action) {
	    case "0":		// initialize CoSPs
		// WhatsitCoSP whatsitCoSP = new WhatsitCoSP();
		// whatsitCoSP.dest = gWhatsitAddress;
		// whatsitCoSP.uri  = "/a/whatsit";
		// ServiceManager.registerCoServiceProvider(whatsitCoSP);
		break;
	    case "1":			// Discover resources
		System.out.println("Discover:");
		System.out.println("\tp) Platforms (RETRIEVE oic/p) ");
		System.out.println("\td) Devices   (RETRIEVE oic/d)");
		System.out.println("\tr) Resources (RETRIEVE oic/res");
		System.out.println("\tx) Cancel");
		while (!scanner.hasNext(pdrx)) {
		    System.out.println("Invalid input, try again");
		    scanner.next();
		}
		action = scanner.next(pdrx);

		switch(action) {
		case "p":
		    System.out.println("requested DISCOVER PLATFORMS");
		    uri = "/oic/p";
		    break;
		case "d":
		    System.out.println("requested DISCOVER DEVICES");
		    uri = "/oic/d";
		    break;
		case "r":
		    System.out.println("requested DISCOVER RESOURCES");
		    uri = "/oic/res";
		    break;
		case "x":
		    System.out.println("CANCELLED");
		    break;
		}

		discoveryCoSP
		    .method(OCF.RETRIEVE)
		    .uriPath(uri)
		    .transportIsUDP(true)
		    // .networkIsIPv6(true)
		    .routingIsMulticast(true);

		Logger.logNetworking(discoveryCoSP);

		// System.out.println("BEFORE EXHIBIT");
		// Logger.logRequestOut(discoveryCoSP);
		// Logger.logCoAddress(discoveryCoSP);
		try {
		    discoveryCoSP.exhibit();
		    Thread.sleep(500);
		} catch (Exception e) {
		    System.out.println("ERROR: discovery");
		    e.printStackTrace();
		    msgError(TAG, e.toString());
		}
		break;
	    case "2":
		System.out.println("Discovered resources:");
		for (CoServiceProvider cosp : ServiceManager.coServiceProviders) {
		    System.out.println("\t" + cosp.uriPath());
		}
		System.out.println();
		break;
	    case "3":
		System.out.println("RETRIEVE: Select a resource.");

		// UI tedium:
		CoServiceProvider cosp = null;
		int sz = ServiceManager.coServiceProviders.size();
		for(int i = 0; i < sz; i++) {
		    cosp = ServiceManager.coServiceProviders.get(i);
		    DeviceAddress da = cosp.coAddress();
		    String s;
		    if (da != null) {
			s = i + " : " + da.ipAddress()
			+ " " + da.port()
			    + " " + cosp.uriPath();
		    } else {
			s = i + " : " + cosp.uriPath();
		    }
		    System.out.println("\t" + s);
		}
		System.out.println();

		int i = 0;
		while(true) {
		    while (!scanner.hasNextInt()) {
			System.out.println("Invalid input: " + scanner.next() + "; try again");
		    }
		    i = scanner.nextInt();
		    if ( (i < sz) && i >= 0) {
			break;
		    } else {
			System.out.println("Invalid input: " + i + "; try again");
		    }
		}
		// Yay! UI done.

		cosp = ServiceManager.coServiceProviders.get(i);
		cosp.method(OCF.RETRIEVE);
		Logger.logCoSP(cosp);
		try {
		    cosp.exhibit();
		    // cosp.exhibit();
		    Thread.sleep(500); // just to let the user prompt work
		} catch (Exception e) {
		    e.printStackTrace();
		    msgError(TAG, e.toString());
		}
		break;
	    case "4":
		System.out.println("requested WATCH");
		// try {
		//     StimulusOut requestOut
		//     	= new StimulusOut(new WhatsitSR());
		//     requestOut.dest = gWhatsitAddress;
		//     requestOut.uri  = "/a/whatsit";
		//     byte[] bs = Messenger.exhibitStimulus(OCF.WATCH, requestOut);
		//     Thread.sleep(1000);
		// } catch (Exception e) {
		//     e.printStackTrace();
		//     msgError(TAG, e.toString());
		// }
		break;
	    case "5":
		System.out.println("requested PUT - NOT YET IMPLEMENTED");
		break;
	    case "6":
		System.out.println("requested POST");
		try {
		    // StimulusOut requestOut
		    // 	= new StimulusOut(new WhatsitSR());
		    // requestOut.dest = gWhatsitAddress;
		    // requestOut.uri  = "/a/whatsit";
		    // byte[] bs = Messenger.exhibitStimulus(OCF.POST, requestOut);
		    Thread.sleep(1000);
		} catch (Exception e) {
		    e.printStackTrace();
		    msgError(TAG, e.toString());
		}
	    	break;
	    // case 6:

	    // 	break;
	    // case 7:

	    // 	break;
	    case "x":
		System.out.println("EXITING");
		again = false;
		break;
	    default:
		break;
	    }
	}
    }

    // ****************************************************************
    public static void main(String[] args)
    {
	Runtime.getRuntime().addShutdownHook(new Thread()
	    {
		@Override
		public void run()
		{
		    System.out.println("Shutdown hook running!");
		    OCF.stop();
		}
	    });

	OCF.Init(OCF.CLIENT, "src/main/resources/ocftestclient_config.cbor");
	// ServiceManager.registerPlatform("Fartmaster",
	// 				 "Acme Novelties",
	// 				 "http://acme.example.org",
	// 				 "modelnbr", "mfgdate", "platversion",
	// 				 "osversion", "hwversion", "firmwareversion",
	// 				 "http://acme.example.org/support",
	// 				 new SimpleDateFormat("yyyy.MM.dd.HH.mm.ss").format(new Date()));

	// ServiceManager.registerDevice("Fartmaster2020 Client",
	// 			       new String[] {"type1", "type2"},
	// 			       "version-0.1",
	// 			       new String[] {"dmversion-0.1"});

	discoveryCoSP = new DiscoveryCoSP();
	ServiceManager.registerCoServiceProvider(discoveryCoSP);

	OCF.run();

	promptUser();

	// try {
	//     Thread.sleep(500);
	// } catch (Exception e) {
	//     e.printStackTrace();
	//     msgError(TAG, e.toString());
	// }



	// // Setup terminal and screen layers
	// Screen screen = null;
	// try {
	//     Terminal terminal = new DefaultTerminalFactory().createTerminal();
	//     screen = new TerminalScreen(terminal);
	//     screen.startScreen();
	// } catch (Exception e) {
	//     System.out.println("CAUGHT TERMINAL EXCEPTION");
	// }
        // // Create panel to hold components
        // Panel panel = new Panel();
        // panel.setLayoutManager(new GridLayout(2));

        // panel.addComponent(new Label("Forename"));
        // panel.addComponent(new TextBox());

        // panel.addComponent(new Label("Surname"));
        // panel.addComponent(new TextBox());

        // panel.addComponent(new EmptySpace(new TerminalSize(0,0))); // Empty space underneath labels
        // panel.addComponent(new Button("Submit"));

        // // Create window to hold the panel
        // BasicWindow window = new BasicWindow();
        // window.setComponent(panel);

        // // Create gui and start gui
        // MultiWindowTextGUI gui = new MultiWindowTextGUI(screen,
	// 						new DefaultWindowManager(),
	// 						new EmptySpace(TextColor.ANSI.BLUE));
        // gui.addWindowAndWait(window);

        // while(true){
	//     try {
	// 	Thread.sleep(2000);
	// 	System.out.println("GUI thread loop");
	//     } catch (InterruptedException e) {
	// 	e.printStackTrace();
	// 	msgError(TAG, e.toString());
	//     }
        // }
    }

    public static void msgError(final String tag ,final String text) {
        System.out.println("[E]" + tag + " | " + text);
    }

    private final static String TAG = OCFTestClient.class.getSimpleName();
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
