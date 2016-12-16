package org.iochibity.test;

import org.iochibity.OCF;
// import org.iochibity.CallbackParam;
import org.iochibity.DeviceAddress;
import org.iochibity.HeaderOption;
import org.iochibity.Messenger;
// import org.iochibity.MsgForServiceProvider;
import org.iochibity.MsgRequestIn;
import org.iochibity.MsgResponseOut;
import org.iochibity.Payload;
import org.iochibity.PayloadList;
import org.iochibity.PropertyMap;
import org.iochibity.PayloadForResourceState;
import org.iochibity.PropertyString;
import org.iochibity.Resource;
import org.iochibity.ResourceLocal;
import org.iochibity.IServiceProvider;
import org.iochibity.ServicesManager;
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

public class OCFTestServer
{
    static{
	try{
	    System.loadLibrary("ocfjni_c");
	}catch(Exception e){
	    System.out.println(e.toString());
	}
    }

    static public class TemperatureSP implements IServiceProvider
    {
	int foo = 72;
	public void hello() { System.out.println("Hello from callback: " + foo); }

	private PayloadList<Payload> serviceGetRequest(MsgRequestIn requestIn)
	{
	    System.out.println("TEST serviceGetRequest ENTRY");

	    System.out.println("TEST callback param: " + foo);
	    hello();

	    ResourceLocal r = requestIn.getResource();
	    System.out.println("TEST resource uri: " + r.getUri());

	    PayloadForResourceState pfrs = new PayloadForResourceState(r);

	    System.out.println("TEST payload uri: " + pfrs.getUri());
	    pfrs.setUri("/a/foo");
	    System.out.println("TEST payload new uri: " + pfrs.getUri());

	    pfrs.addResourceType("foo.t.a");
	    pfrs.addResourceType("foo.t.b");
	    List<String> llts = pfrs.getResourceTypes();
	    for (String s : (List<String>)llts) {
		System.out.println("TEST payload r type: " + s);
	    }
	    List<String> llifs = pfrs.getInterfaces();
	    llifs.add("foo.if.a");
	    for (String s : (List<String>)llifs) {
		System.out.println("TEST payload r interface: " + s);
	    }

	    PropertyMap<String, Object> pmps = pfrs.getProperties();
	    pmps.put("i1", -1);
	    pmps.put("i2", 0);
	    pmps.put("i3", 1);
	    pmps.put("d1", -1.1);
	    pmps.put("d2", 0.0);
	    pmps.put("d3", 1.1);
	    pmps.put("s1", "Hello world");
	    pmps.put("s2", "Goodbye world");
	    pmps.put("b1", true);
	    pmps.put("b2", false);
	    for(Map.Entry<String, Object> entry : pmps.entrySet()) {
		System.out.println("TEST payload r prop: " + entry.getKey() + " = " + entry.getValue());
	    }

	    PayloadList<Payload> pll = new PayloadList<Payload>();

	    pll.add(pfrs);

	    return pll; //payload;
	}

	@Override
	public int serviceRequestIn(MsgRequestIn requestIn)
	{
	    System.out.println("TEST TemperatureSP.service routine ENTRY");
	    Logger.logRequestIn(requestIn);

	    PayloadList<Payload> payloadOut = null;

	    // typedef enum
	    // {
	    //     OC_REQUEST_FLAG = (1 << 1),
	    //     OC_OBSERVE_FLAG = (1 << 2)
	    // } OCEntityHandlerFlag;
	    // if (entityHandlerRequest && (flag & OC_REQUEST_FLAG))

	    switch (requestIn.getMethod()) {
	    case Method.GET:
		payloadOut = serviceGetRequest(requestIn);
		break;
	    case Method.PUT:
		System.out.println("TEST method: PUT");
		break;
	    case Method.POST:
		System.out.println("TEST method: POST");
		break;
	    case Method.DELETE:
		System.out.println("TEST method: DELETE");
		break;
	    case Method.WATCH:
		break;
	    case Method.WATCH_ALL:
		break;
	    case Method.CANCEL_WATCH:
		break;
	    case Method.PRESENCE:
		break;
	    case Method.DISCOVER:
		break;
	    case Method.NOMETHOD:
		break;
	    default:
		break;
	    }

	    MsgResponseOut responseOut = new MsgResponseOut(requestIn, payloadOut);

	    try {
		Messenger.sendResponse(responseOut);
	    } catch (Exception e) {
		System.out.println("SEND RESPONSE EXCEPTION");
	    }

	    System.out.println("TemperatureSP.service EXIT");
	    return ServiceResult.OK;
	}
    }

    static TemperatureSP tempSP = new TemperatureSP();

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

	// System.out.println(System.getProperty("java.library.path"));
	OCF.Init(null, 0, OCMode.SERVER, "src/main/resources/ocftestserver_config.cbor");

	ServicesManager.registerPlatformProvider("Fartmaster",
					 "Acme Novelties",
					 "http://acme.example.org",
					 "modelnbr", "mfgdate", "platversion",
					 "osversion", "hwversion", "firmwareversion",
					 "http://acme.example.org/support",
					 new SimpleDateFormat("yyyy.MM.dd.HH.mm.ss").format(new Date()));

	ServicesManager.registerDeviceProvider("Fartmaster2020 Server",
				       new String[] {"type1", "type2"},
				       "version-0.1",
				       new String[] {"dmversion-0.1"});

	ResourceLocal l = null;
	l = ServicesManager.registerServiceProvider("/a/light",	          // uri String
					     new String[] {"core.light"}, // array of typenames
					     new String[] {"oc.mi.l"}, // array of ifnames
					     new LightSP(),            // IServiceProvider
					     // lspData,
					     (byte)(Resource.DISCOVERABLE
						     | Resource.SECURE));
	Logger.logResource(l);

	ResourceLocal tr = null;
	tr = ServicesManager.registerServiceProvider("/a/temperature",
					      new String[] {"core.temperature"},
					      new String[] {"oc.mi.def"},
					      tempSP,
					      // new MyParam(99),
					      (byte)(Resource.DISCOVERABLE
						     | Resource.SECURE));
	Logger.logResource(tr);

	ResourceLocal whatsit = null;
	whatsit = ServicesManager.registerServiceProvider("/a/whatsit",
						   new String[] {"core.whatist"},
						   new String[] {"oc.mi.whatsit"},
						   new WhatsitSP(),
						   // new WhatsitSP.CBData(123),
						   (byte)(Resource.DISCOVERABLE
							  | Resource.SECURE));

	Logger.logResource(whatsit);

	OCF.run();

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

    private final static String TAG = OCFTestServer.class.getSimpleName();
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
