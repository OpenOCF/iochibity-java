package org.iochibity.test;

import org.iochibity.OCF;
import org.iochibity.CallbackParam;
import org.iochibity.DeviceAddress;
import org.iochibity.HeaderOption;
import org.iochibity.MessageForResource;
import org.iochibity.Payload;
import org.iochibity.PayloadList;
import org.iochibity.PropertyMap;
import org.iochibity.PayloadForResourceState;
import org.iochibity.PropertyString;
import org.iochibity.DocRequestIn;
import org.iochibity.Resource;
import org.iochibity.ResourceLocal;
// import org.iochibity.Resource$InstanceId;
import org.iochibity.ResourceManager;
import org.iochibity.ResourceServiceProvider;
// import org.iochibity.Response;
import org.iochibity.DocResponseOut;
import org.iochibity.constants.OCMethod;
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

    static public class MyParam extends CallbackParam
    {
	int foo;
	public MyParam(int i) { foo = i; }
	public void hello() { System.out.println("Hello from callback: " + foo); }
    }

    static public class TemperatureSP extends ResourceServiceProvider
    {
	static private PayloadList<Payload> serviceGetRequest(int flag,
							      DocRequestIn request,
							      MyParam callbackParam)
	{
	    System.out.println("TEST serviceGetRequest ENTRY");

	    System.out.println("TEST callback param: " + callbackParam.foo);
	    callbackParam.hello();

	    ResourceLocal r = request.getResource();
	    System.out.println("TEST resource uri: " + r.getUri());

	    PayloadForResourceState pfrs = new PayloadForResourceState(r);
	    Logger.logPayloadType(pfrs);

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
	public int service(int flag, DocRequestIn request_in, CallbackParam callbackParam)
	{
	    System.out.println("TEST TemperatureSP.service routine ENTRY");
	    Logger.logRequestIn(request_in);

	    PayloadList<Payload> payloadOut = null;

	    // typedef enum
	    // {
	    //     OC_REQUEST_FLAG = (1 << 1),
	    //     OC_OBSERVE_FLAG = (1 << 2)
	    // } OCEntityHandlerFlag;
	    // if (entityHandlerRequest && (flag & OC_REQUEST_FLAG))

	    switch (request_in.method) {
	    case OCMethod.GET:
		payloadOut = serviceGetRequest(flag, request_in, (MyParam)callbackParam);
		break;
	    case OCMethod.PUT:
		System.out.println("TEST method: PUT");
		break;
	    case OCMethod.POST:
		System.out.println("TEST method: POST");
		break;
	    case OCMethod.DELETE:
		System.out.println("TEST method: DELETE");
		break;
	    case OCMethod.OBSERVE:
		break;
	    case OCMethod.OBSERVE_ALL:
		break;
	    case OCMethod.CANCEL_OBSERVE:
		break;
	    case OCMethod.PRESENCE:
		break;
	    case OCMethod.DISCOVER:
		break;
	    case OCMethod.NOMETHOD:
		break;
	    default:
		break;
	    }

	    DocResponseOut responseOut = new DocResponseOut(request_in, payloadOut);

	    try {
		OCF.sendResponse(responseOut);
	    } catch (Exception e) {
		System.out.println("SEND RESPONSE EXCEPTION");
	    }

	    System.out.println("TemperatureSP.service EXIT");
	    return ServiceResult.OK;
	}
    }

    static LightSP lightSP = new LightSP();
    static LightSP.CBData lspData = new LightSP.CBData(99);

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
	OCF.Init(null, 0, OCMode.SERVER, "resources/ocftestserver_config.cbor");

	ResourceManager.registerPlatform("Fartmaster",
					 "Acme Novelties",
					 "http://acme.example.org",
					 "modelnbr", "mfgdate", "platversion",
					 "osversion", "hwversion", "firmwareversion",
					 "http://acme.example.org/support",
					 new SimpleDateFormat("yyyy.MM.dd.HH.mm.ss").format(new Date()));

	ResourceManager.registerDevice("Fartmaster2020",
				       new String[] {"type1", "type2"},
				       "version-0.1",
				       new String[] {"dmversion-0.1"});

	ResourceLocal l = null;
	l = ResourceManager.registerResource("/a/light",	          // uri String
					     new String[] {"core.light"}, // array of typenames
					     new String[] {"oc.mi.l"}, // array of ifnames
					     lightSP, // ResourceServiceProvider
					     lspData,
					     (byte)(Resource.DISCOVERABLE
						     | Resource.SECURE));
	Logger.logResource(l);

	ResourceLocal tr = null;
	tr = ResourceManager.registerResource("/a/temperature",
					      new String[] {"core.temperature"},
					      new String[] {"oc.mi.def"},
					      new TemperatureSP(),
					      new MyParam(99),
					      (byte)(Resource.DISCOVERABLE
						     | Resource.SECURE));
	Logger.logResource(tr);

	ResourceLocal whatsit = null;
	whatsit = ResourceManager.registerResource("/a/whatsit",
						   new String[] {"core.whatist"},
						   new String[] {"oc.mi.whatsit"},
						   new WhatsitSP(),
						   new WhatsitSP.CBData(123),
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
