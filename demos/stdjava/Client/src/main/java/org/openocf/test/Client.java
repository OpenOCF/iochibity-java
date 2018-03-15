package org.openocf.test;

// UI stuff
// import com.googlecode.lanterna.TerminalSize;
// import com.googlecode.lanterna.TextColor;
// import com.googlecode.lanterna.gui2.*;
// import com.googlecode.lanterna.screen.Screen;
// import com.googlecode.lanterna.screen.TerminalScreen;
// import com.googlecode.lanterna.terminal.DefaultTerminalFactory;
// import com.googlecode.lanterna.terminal.Terminal;

import openocf.standard.OCFServices;
// iochibity jni-c stuff
import openocf.engine.OCFClientSP;
// import openocf.CoServiceManager;
import openocf.app.CoResourceSP;
import openocf.utils.EndPoint;
// import openocf.Message;
// import openocf.OutboundStimulus;
// import openocf.ObservationIn;
// import openocf.ObservationOut;
import openocf.signals.HeaderOption;
import openocf.signals.ObservationRecord;
// import openocf.ObservationList;
import openocf.utils.PropertyMap;
import openocf.app.ICoResourceSP;

import org.openocf.test.client.DiscoveryCoRSP;
import org.openocf.test.client.GenericCoRSP;

import openocf.constants.Method;
import openocf.constants.OCStackResult;
import openocf.constants.ResourcePolicy;
import openocf.constants.ServiceResult;
import openocf.exceptions.OCFNotImplementedException;

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


public class Client
{
    static{
	OCFServices.config("/tmp/oocf_client.log", // logfile_fname
			  "/client_config.cbor"   // FIXME: use constant "svrs.cbor"
			  );
    }

    public static DiscoveryCoRSP discoveryCoRSP;

    public static synchronized void promptUser()
    {
	Scanner scanner = new Scanner(System.in);
	String action = null;
	Pattern top  = Pattern.compile("[12micrudnx]");
	Pattern pdrx = Pattern.compile("[dprx]");

    	String uri = null;
	// OutboundStimulus msgRequestOut;

	// CoServiceManager.registerCoServiceProvider(discoveryCoRSP);

	boolean again = true;
	while(again) {
	    System.out.println("Choose an action:");
	    // list messages
	    // inspect message
	    System.out.println("\t1) Discovery");
	    System.out.println("\t2) List discovered resources");
	    System.out.println("\tm) List Messages");
	    System.out.println("\ti) Inspect Message");
	    System.out.println("\tc) CREATE");
	    System.out.println("\tr) RETRIEVE");
	    System.out.println("\tu) UPDATE");
	    // System.out.println("\t6) POST");
	    System.out.println("\td) DELETE");
	    System.out.println("\tn) NOTIFY (WATCH)");
	    System.out.println("\tx) Exit\n");
	    while (!scanner.hasNext(top)) {
		System.out.println("Invalid input, try again");
		scanner.next();
	    }
	    action = scanner.next(top);
	    switch(action) {
	    case "0":		// initialize CoRSPs
		// WhatsitCoRSP whatsitCoRSP = new WhatsitCoRSP();
		// whatsitCoRSP.dest = gWhatsitAddress;
		// whatsitCoRSP.uri  = "/a/whatsit";
		// ServiceManager.registerCoServiceProvider(whatsitCoRSP);
		break;
	    case "1":			// Discover
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

		discoveryCoRSP
		    .setMethod(Method.RETRIEVE)
		    .setUri(uri)
		    // .addType("foo.t.bar")
		    .setTransportUDP(true)
		    // .networkIsIPv6(true)
		    .routingIsMulticast(true);

		// System.out.println("BEFORE EXHIBIT");
		Logger.logNetworking(discoveryCoRSP);
		// Logger.logCoRSP(discoveryCoRSP);

		try {
		    CountDownLatch finished = discoveryCoRSP.latch();
		    // discoveryCoRSP.coExhibit();
		    // Or:  coServiceManager.coExhibit(discoveryCoRSP);
		    OCFClientSP.coExhibit(discoveryCoRSP);
		    finished.await();
		} catch (Exception e) {
		    System.out.println("ERROR: discovery");
		    e.printStackTrace();
		    msgError(TAG, e.toString());
		}
		break;
	    case "2":
		System.out.println("Discovered Resources:");
		// OCFClientSP.registeredCoServiceProviders();
		for (CoResourceSP cosp : OCFClientSP.registeredCoResourceSPs()) {
		    System.out.println("\t" + cosp.getUri());
		}
		break;
	    case "3":
		System.out.println("RETRIEVE: Select a resource.");

		// UI tedium:
		CoResourceSP cosp = null;
		// int sz = ServiceManager.coServiceProviders.size();
		// for(int i = 0; i < sz; i++) {
		//     cosp = ServiceManager.coServiceProviders.get(i);
		//     DeviceAddress da = cosp.coAddress();
		//     String s;
		//     if (da != null) {
		// 	s = i + " : " + da.ipAddress()
		// 	+ " " + da.port()
		// 	    + " " + cosp.uriPath();
		//     } else {
		// 	s = i + " : " + cosp.uriPath();
		//     }
		//     System.out.println("\t" + s);
		// }
		System.out.println();

		int i = 0;
		// while(true) {
		//     while (!scanner.hasNextInt()) {
		// 	System.out.println("Invalid input: " + scanner.next() + "; try again");
		//     }
		//     i = scanner.nextInt();
		//     if ( (i < sz) && i >= 0) {
		// 	break;
		//     } else {
		// 	System.out.println("Invalid input: " + i + "; try again");
		//     }
		// }
		// Yay! UI done.

		// cosp = ServiceManager.coServiceProviders.get(i);
		// cosp.method(OCF.RETRIEVE);
		// Logger.logCoRSP(cosp);
		// try {
		//     cosp.coExhibit();
		//     // cosp.coExhibit();
		//     Thread.sleep(500); // just to let the user prompt work
		// } catch (Exception e) {
		//     e.printStackTrace();
		//     msgError(TAG, e.toString());
		// }
		break;
	    case "4":
		System.out.println("requested WATCH");
		// try {
		//     OutboundStimulus requestOut
		//     	= new OutboundStimulus(new WhatsitSR());
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
		    Thread.sleep(1000);
		} catch (Exception e) {
		    e.printStackTrace();
		    msgError(TAG, e.toString());
		}
	    	break;
	    case "m":
		System.out.println("Listing Messages");
		try {
		    Thread.sleep(1000);
		} catch (Exception e) {
		    e.printStackTrace();
		    msgError(TAG, e.toString());
		}
	    	break;
	    case "x":
		System.out.println("EXITING");
		again = false;
		System.exit(0);
		break;
	    default:
		break;
	    }
	    // give logger time to flush
	    try {
		Thread.sleep(1000);
	    } catch (InterruptedException e) {
		e.printStackTrace();
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
		    OCFClientSP.stop();
		}
	    });

	OCFClientSP.Init(OCFClientSP.CLIENT); // _SERVER);
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

	Thread uithread = new Thread() {
		@Override
		public void run()
		{
		    // cosp must be alloced on same thread that calls its methods
		    discoveryCoRSP = new DiscoveryCoRSP();
		    promptUser();
		}
	    };
	uithread.start();

	OCFClientSP.run();

	// promptUser();

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

    private final static String TAG = Client.class.getSimpleName();
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
