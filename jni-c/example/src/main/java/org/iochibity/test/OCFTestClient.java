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
import org.iochibity.MsgRequestOut;
import org.iochibity.MsgResponseIn;
import org.iochibity.MsgResponseOut;
import org.iochibity.HeaderOption;
import org.iochibity.Payload;
import org.iochibity.PayloadList;
import org.iochibity.PropertyMap;
import org.iochibity.PayloadForResourceState;
import org.iochibity.PropertyString;
import org.iochibity.Resource;
import org.iochibity.ResourceLocal;
import org.iochibity.ResourceManager;
import org.iochibity.IResourceServiceRequestor;
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

public class OCFTestClient
{
    static{
	try{
	    System.loadLibrary("ocfjni_c");
	}catch(Exception e){
	    System.out.println(e.toString());
	}
    }

    public static class PlatformRequestor implements IResourceServiceRequestor
    {
	private int cbdata = 99;

	public int serviceResponseIn(MsgResponseIn responseIn)
	{
	    System.out.println("JAVA: PlatformRequestor.serviceResponse ENTRY");
	    System.out.println("JAVA: cbdata: " + cbdata);
	    Logger.logMsgResponseIn(responseIn);

	    // save incoming resource info - ResourceManager.registerRemoteResource(...)?
	    // update screen ...
	    return 0;
	}

    }

    public static class DeviceRequestor implements IResourceServiceRequestor
    {
	private int cbdata = 123;

	public int serviceResponseIn(MsgResponseIn responseIn)
	{
	    System.out.println("LOG: DeviceRequestor.serviceResponse ENTRY");
	    System.out.println("LOG: cbdata: " + cbdata);
	    Logger.logMsgResponseIn(responseIn);

	    // save incoming resource info - ResourceManager.registerRemoteResource(...)?
	    // update screen ...
	    return 0;
	}

    }

    public static DeviceAddress gRemoteResourceAddr;

    public static class DiscoverResourcesRequestor implements IResourceServiceRequestor
    {
	private int cbdata = 123;

	public int serviceResponseIn(MsgResponseIn responseIn)
	{
	    System.out.println("LOG: DiscoverResourcesRequestor.serviceResponse ENTRY");
	    System.out.println("LOG: cbdata: " + cbdata);
	    Logger.logMsgResponseIn(responseIn);

	    gRemoteResourceAddr = responseIn.getRemoteDeviceAddress();

	    // save incoming resource info - ResourceManager.registerRemoteResource(...)?
	    // update screen ...
	    return 0;
	}

    }

    public static class WhatsitRequestor implements IResourceServiceRequestor
    {
	private int cbdata = 123;

	public int serviceResponseIn(MsgResponseIn responseIn)
	{
	    System.out.println("LOG: DiscoverResourcesRequestor.serviceResponse ENTRY");
	    System.out.println("LOG: cbdata: " + cbdata);
	    Logger.logMsgResponseIn(responseIn);

	    // save incoming resource info - ResourceManager.registerRemoteResource(...)?
	    // update screen ...
	    return 0;
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

	OCF.Init(null, 0, OCF.SERVER, "src/main/resources/ocftestclient_config.cbor");

	// ResourceManager.registerPlatform("Fartmaster",
	// 				 "Acme Novelties",
	// 				 "http://acme.example.org",
	// 				 "modelnbr", "mfgdate", "platversion",
	// 				 "osversion", "hwversion", "firmwareversion",
	// 				 "http://acme.example.org/support",
	// 				 new SimpleDateFormat("yyyy.MM.dd.HH.mm.ss").format(new Date()));

	// ResourceManager.registerDevice("Fartmaster2020 Client",
	// 			       new String[] {"type1", "type2"},
	// 			       "version-0.1",
	// 			       new String[] {"dmversion-0.1"});

	OCF.run();

	try {
	    MsgRequestOut requestOut
		= new MsgRequestOut(new PlatformRequestor());
		// = new MsgRequestOut((IResourceServiceRequestor)(new PlatformRequestor()));
	    byte[] bs = OCF.discoverPlatforms(requestOut);
	} catch (Exception e) {
	    System.out.println("ERROR: discoverPlatforms");
	}

	try {
	    MsgRequestOut requestOut
		= new MsgRequestOut(new DeviceRequestor());
	    byte[] bs = OCF.discoverDevices(requestOut);
	} catch (Exception e) {
	    System.out.println("ERROR: discoverDevices");
	}

	try {
	    MsgRequestOut requestOut
		= new MsgRequestOut(new DiscoverResourcesRequestor());
	    byte[] bs = OCF.discoverResources(requestOut);
	} catch (Exception e) {
	    System.out.println("ERROR: discoverResources");
	}

	try {
	    Thread.sleep(1000);
	} catch (Exception e) {
	    e.printStackTrace();
	    msgError(TAG, e.toString());
	}

	try {
	    MsgRequestOut requestOut
		= new MsgRequestOut(new WhatsitRequestor());
	    requestOut.dest = gRemoteResourceAddr;
	    requestOut.uri  = "/a/temperature";
	    byte[] bs = OCF.sendRequest(Method.GET, requestOut);
	} catch (Exception e) {
	    System.out.println("ERROR: discoverResources");
	}


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

        while(true){
	    try {
		Thread.sleep(2000);
		System.out.println("GUI thread loop");
	    } catch (InterruptedException e) {
		e.printStackTrace();
		msgError(TAG, e.toString());
	    }
        }
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
