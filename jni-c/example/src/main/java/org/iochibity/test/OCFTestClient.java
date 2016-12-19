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
import org.iochibity.DeviceAddress;
import org.iochibity.Message;
import org.iochibity.Messenger;
import org.iochibity.MsgRequestOut;
import org.iochibity.MsgResponseIn;
import org.iochibity.MsgResponseOut;
import org.iochibity.HeaderOption;
import org.iochibity.IPayload;
import org.iochibity.Payload;
import org.iochibity.PayloadList;
import org.iochibity.PropertyMap;
import org.iochibity.PayloadForResourceState;
import org.iochibity.PropertyString;
import org.iochibity.Resource;
import org.iochibity.ResourceLocal;
import org.iochibity.ServicesManager;
import org.iochibity.IServiceRequestor;
import org.iochibity.constants.Method;
import org.iochibity.constants.OCMode;
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

    public static class PlatformRequestor implements IServiceRequestor
    {
	private int cbdata = 99;

	public int serviceResponseIn(MsgResponseIn responseIn)
	{
	    System.out.println("JAVA: PlatformRequestor.serviceResponse ENTRY");
	    System.out.println("JAVA: cbdata: " + cbdata);
	    Logger.logMsgResponseIn(responseIn);

	    // save incoming resource info - ServicesManager.registerRemoteResource(...)?
	    // update screen ...
	    System.out.flush();
	    return 0;
	}

    }

    public static class DeviceRequestor implements IServiceRequestor
    {
	private int cbdata = 123;

	public int serviceResponseIn(MsgResponseIn responseIn)
	{
	    System.out.println("LOG: DeviceRequestor.serviceResponse ENTRY");
	    System.out.println("LOG: cbdata: " + cbdata);
	    Logger.logMsgResponseIn(responseIn);

	    // save incoming resource info - ServicesManager.registerRemoteResource(...)?
	    // update screen ...
	    System.out.flush();
	    return 0;
	}

    }

    public static DeviceAddress gRemoteResourceAddress;
    public static DeviceAddress gLEDAddress;
    public static DeviceAddress gWhatsitAddress;

    public static class DiscoverResourcesRequestor implements IServiceRequestor
    {
	private int cbdata = 123;

	public int serviceResponseIn(MsgResponseIn responseIn)
	{
	    System.out.println("LOG: DiscoverResourcesRequestor.serviceResponse ENTRY");
	    System.out.println("LOG: cbdata: " + cbdata);

	    if (responseIn.result == OCStackResult.OK) {
		if (responseIn.getPayloadType() == Payload.DISCOVERY) {
		    PayloadList<Payload> pll = responseIn.getPayloadList();
		    for (Payload payload : (PayloadList<Payload>) pll) {
			List<IPayload> kids = payload.getChildren();
			if (kids != null) {
			    for (IPayload kid : kids) {
				System.out.println("LOG KID URIPATH: " + kid.getUriPath());
				if (kid.getUriPath().equals("/a/temperature")) {
				    System.out.println("LOG: found temperature resource");
				    gRemoteResourceAddress = responseIn.getRemoteDeviceAddress();
				    gRemoteResourceAddress.port
					= ((Integer)kid.getProperties().get("port"))
					.intValue();
				    Logger.logDeviceAddress(gRemoteResourceAddress);
				}
				if (kid.getUriPath().equals("/a/led")) {
				    System.out.println("LOG: found LED resource");
				    gLEDAddress = responseIn.getRemoteDeviceAddress();
				    gLEDAddress.port
					= ((Integer)kid.getProperties().get("port"))
					.intValue();
				    Logger.logDeviceAddress(gLEDAddress);
				}
				if (kid.getUriPath().equals("/a/whatsit")) {
				    System.out.println("LOG: found whatsit resource");
				    gWhatsitAddress = responseIn.getRemoteDeviceAddress();
				    gWhatsitAddress.port
					= ((Integer)kid.getProperties().get("port"))
					.intValue();
				    Logger.logDeviceAddress(gWhatsitAddress);
				}
			    }
			}
		    }
		}
	    }
	    Logger.logMsgResponseIn(responseIn);

	    // save incoming resource info - ServicesManager.registerRemoteResource(...)?
	    // update screen ...
	    return 0;
	}
    }

    public static class WhatsitRequestor implements IServiceRequestor
    {
	private int cbdata = 123;

	public int serviceResponseIn(MsgResponseIn responseIn)
	{
	    System.out.println("LOG: DiscoverResourcesRequestor.serviceResponse ENTRY");
	    System.out.println("LOG: cbdata: " + cbdata);
	    Logger.logMsgResponseIn(responseIn);

	    // save incoming resource info - ServicesManager.registerRemoteResource(...)?
	    // update screen ...
	    return 0;
	}

    }

    public static synchronized void promptUser()
    {
	Scanner sc = new Scanner(System.in);
	String action = null;

	Pattern top  = Pattern.compile("[0-9x]");
	Pattern pdrx = Pattern.compile("[dprx]");

	boolean again = true;
	while(again) {
	    System.out.println("Choose an action:");
	    System.out.println("\t1) Discover resources");
	    System.out.println("\t2) List discovered resources");
	    System.out.println("\t3) GET");
	    System.out.println("\t3) WATCH");
	    System.out.println("\t4) PUT");
	    System.out.println("\t5) POST");
	    System.out.println("\t8) DELETE");
	    System.out.println("\tx) Exit\n");
	    while (!sc.hasNext(top)) { System.out.println("Invalid input, try again"); sc.next(); }
	    action = sc.next(top);
	    switch(action) {
	    case "1":			// Discover resources
		System.out.println("Discover:");
		System.out.println("\tp) Platforms (GET oic/p) ");
		System.out.println("\td) Devices   (GET oic/d)");
		System.out.println("\tr) Resources (GET oic/res");
		System.out.println("\tx) Cancel");
		while (!sc.hasNext(pdrx)) {  System.out.println("Invalid input, try again"); sc.next(); }
		action = sc.next(pdrx);
		switch(action) {
		case "p":
		    System.out.println("requested DISCOVER PLATFORMS");
		    try {
			MsgRequestOut requestOut
			    = new MsgRequestOut(new PlatformRequestor());
			// = new MsgRequestOut((IServiceRequestor)(new PlatformRequestor()));
			byte[] bs = Messenger.discoverPlatforms(requestOut);
			Thread.sleep(1000);
		    } catch (Exception e) {
			System.out.println("ERROR: discovering platforms");
		    }
		    break;
		case "d":
		    System.out.println("requested DISCOVER DEVICES");
		    try {
			MsgRequestOut requestOut
			    = new MsgRequestOut(new DeviceRequestor());
			byte[] bs = Messenger.discoverDevices(requestOut);
			Thread.sleep(1000);
		    } catch (Exception e) {
			System.out.println("ERROR: discoverDevices");
		    }
		    break;
		case "r":
		    System.out.println("requested DISCOVER RESOURCES");
		    try {
			MsgRequestOut requestOut
			    = new MsgRequestOut(new DiscoverResourcesRequestor());
			byte[] bs = Messenger.discoverResources(requestOut);
			Thread.sleep(1000);
		    } catch (Exception e) {
			System.out.println("ERROR: discoverResources");
		    }
		    break;
		case "x":
		    System.out.println("CANCELLED");
		    break;
		}
		break;
	    case "2":

		break;
	    case "3":
		System.out.println("requested GET");
		try {
		    MsgRequestOut requestOut
			= new MsgRequestOut(new WhatsitRequestor());
		    requestOut.dest = gWhatsitAddress;
		    requestOut.uri  = "/a/whatsit";
		    byte[] bs = Messenger.sendRequest(Method.GET, requestOut);
		    Thread.sleep(1000);
		} catch (Exception e) {
		    e.printStackTrace();
		    msgError(TAG, e.toString());
		}
		break;
	    // case 4:

	    // 	break;
	    case "5":
		System.out.println("requested POST");
		try {
		    MsgRequestOut requestOut
			= new MsgRequestOut(new WhatsitRequestor());
		    requestOut.dest = gWhatsitAddress;
		    requestOut.uri  = "/a/led";
		    byte[] bs = Messenger.sendRequest(Method.POST, requestOut);
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

	OCF.Init(null, 0, OCF.CLIENT_SERVER, "src/main/resources/ocftestclient_config.cbor");
	// ServicesManager.registerPlatform("Fartmaster",
	// 				 "Acme Novelties",
	// 				 "http://acme.example.org",
	// 				 "modelnbr", "mfgdate", "platversion",
	// 				 "osversion", "hwversion", "firmwareversion",
	// 				 "http://acme.example.org/support",
	// 				 new SimpleDateFormat("yyyy.MM.dd.HH.mm.ss").format(new Date()));

	// ServicesManager.registerDevice("Fartmaster2020 Client",
	// 			       new String[] {"type1", "type2"},
	// 			       "version-0.1",
	// 			       new String[] {"dmversion-0.1"});

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
