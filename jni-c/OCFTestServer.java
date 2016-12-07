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
import org.iochibity.RequestIn;
import org.iochibity.Resource;
// import org.iochibity.Resource$InstanceId;
import org.iochibity.ResourceManager;
import org.iochibity.ResourceServiceProvider;
import org.iochibity.Response;
import org.iochibity.ResponseOut;
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

    static private void logResourcePolicies(Resource resource)
    {
	System.out.println("RESOURCE: policies: "
			   + String.format("0x%X", resource.policies & 0xFFFFF));
	if ( (resource.policies & ResourcePolicy.DISCOVERABLE) > 0 )
	    {System.out.println("\tDISCOVERABLE");}
	if ( (resource.policies & ResourcePolicy.ACTIVE) > 0 ) {System.out.println("\tACTIVE");}
	if ( (resource.policies & ResourcePolicy.OBSERVABLE) > 0) {System.out.println("\tOBSERVABLE");}
	if ( (resource.policies & ResourcePolicy.SECURE) > 0) {System.out.println("\tSECURE");}
    }

    static private void logResource(Resource resource)
    {
	System.out.println("RESOURCE: logResource ENTRY");
	System.out.println("RESOURCE: resource uri: " + resource.getUri());

	List<String> tll = resource.getTypes();
	tll.forEach(t -> System.out.println("RESOURCE: type:     " + t)); // Java 8

	List<String> ifll = resource.getInterfaces();
	ifll.forEach(iface -> System.out.println("RESOURCE: interface: " + iface));
	// for (int i = 0; i < ifll.size(); i++) {   // Java 7
	//     System.out.println("REQUEST IN: resource if:    " + ifll.get(i));
	// }

	List<PropertyString> pll = resource.getProperties();
	System.out.println("RESOURCE: properties count: " + pll.size());
	pll.forEach(p -> System.out.println("RESOURCE: property: " + p.name + " = " + p.value));

	List<Resource> cll = resource.getChildren();
	System.out.println("RESOURCE: child resources count: " + cll.size());
	cll.forEach(ch -> System.out.println("RESOURCE: child resource: " + ch));

	System.out.println("RESOURCE: service provider (callback): "
			   + resource.getServiceProvider().getClass().getName());

	System.out.println("RESOURCE: callback param: "
			   + resource.getCallbackParam().getClass().getName());

	System.out.println("RESOURCE: serial number: " + resource.sn);

	// Instance Id
	// System.out.println("RESOURCE: resource instance id: " + resource.id.getClass().getName());
	if (resource.id.getClass().getName().equals("org.iochibity.Resource$InstanceId")) {
	    System.out.println("RESOURCE: resource InstanceId class: InstanceId");
	} else if (resource.id.getClass().getName().equals("org.iochibity.Resource$InstanceOrdinal")) {
		System.out.println("RESOURCE: resource InstanceId class: InstanceOrdinal, val="
				   + ((org.iochibity.Resource.InstanceOrdinal)resource.id).val);
	} else if (resource.id.getClass().getName().equals("org.iochibity.Resource$InstanceString")) {
		System.out.println("RESOURCE: resource InstanceId class: InstanceString, val="
				   + ((org.iochibity.Resource.InstanceString)resource.id).val);
	} else if (resource.id.getClass().getName().equals("org.iochibity.Resource$InstanceUuid")) {
		System.out.println("RESOURCE: resource InstanceId class: InstanceUuid, val="
				   + ((org.iochibity.Resource.InstanceUuid)resource.id).val);
	}

	logResourcePolicies(resource);

	try {
	    System.out.println("RESOURCE: action set: " + resource.getActionSet());
	} catch (OCFNotImplementedException e) {
	    System.out.println("ERROR**** RESOURCE: getActionSet not implemented.");
	}
	System.out.println("RESOURCE: logResource EXIT");
    }

    static private void logPayload(PayloadForResourceState pr)
    {
	// System.out.println("PAYLOAD: logPayload ENTRY");
	System.out.println("PAYLOAD: uri: " + pr.getUri());
	// System.out.println("PAYLOAD: logPayload EXIT");
    }

    static private void logPayloadType(Payload p)
    {
	switch (p.type) {
	case Payload.INVALID:
	    System.out.println("TEST payload type: INVALID (" + p.type + ")");
	    break;
	case Payload.DISCOVERY:
	    System.out.println("TEST payload type: DISCOVERY (" + p.type + ")");
	    break;
	case Payload.DEVICE:
	    System.out.println("TEST payload type: DEVICE (" + p.type + ")");
	    break;
	case Payload.PLATFORM:
	    System.out.println("TEST payload type: PLATFORM (" + p.type + ")");
	    break;
	case Payload.REPRESENTATION:
	    System.out.println("TEST payload type: REPRESENTATION (" + p.type + ")");
	    break;
	case Payload.SECURITY:
	    System.out.println("TEST payload type: SECURITY (" + p.type + ")");
	    break;
	case Payload.PRESENCE:
	    System.out.println("TEST payload type: PRESENCE (" + p.type + ")");
	    break;
	case Payload.RD:
	    System.out.println("TEST payload type: RD (" + p.type + ")");
	    break;
	default:
	    System.out.println("TEST payload type: UNKNOWN (" + p.type + ")");
	    break;
	}
    }

static private void logPayloadList(PayloadList<PayloadForResourceState> pl)
    {
	System.out.println("Logging PayloadList");
	for (PayloadForResourceState pr : (PayloadList<PayloadForResourceState>)pl) {
	    logPayload(pr);
	    PropertyMap pps = pr.getProperties();
	    System.out.println("PAYLOAD: prop ct: " + pps.size());
	}
    }

    static private void logRequestIn(RequestIn request)
    {
	System.out.println("REQUEST IN: logRequestIn ENTRY");
	System.out.println("REQUEST IN: this handle: " + request.localHandle);
	System.out.println("REQUEST IN: handle at origin: " + request.remoteHandle);
	System.out.println("REQUEST IN: resource handle: " + request.getResourceHandle());
	System.out.println("REQUEST IN: request method: " + request.method);
	System.out.println("REQUEST IN: query : \"" + request.query + "\"");
	System.out.println("REQUEST IN: msg id : " + request.messageId);

	Resource resource = request.getResource();
	logResource(resource);

	System.out.println("REQUEST IN: devaddr adapter: " + request.deviceAddress.adapter);
	System.out.println("REQUEST IN: devaddr flags: " + String.format("0x%04X", request.deviceAddress.flags & 0xFFFFF));
	System.out.println("REQUEST IN: devaddr port: " + request.deviceAddress.port);
	System.out.println("REQUEST IN: devaddr address: " + request.deviceAddress.address);
	System.out.println("REQUEST IN: devaddr ifindex: " + request.deviceAddress.ifindex);
	System.out.println("REQUEST IN: devaddr route data: " + request.deviceAddress.routeData);

	System.out.println("REQUEST IN: observe action: " + request.observeAction);
	System.out.println("REQUEST IN: observe id    : " + request.observeId);

	ArrayList<HeaderOption> vendorOptions = request.getVendorHeaderOptions();
	System.out.println("REQUEST IN: vendor opts ct: " + vendorOptions.size());

	PayloadList<PayloadForResourceState> payload = request.getPDUPayload();
	if (payload != null) {
	    System.out.println("REQUEST IN: payload type: " + payload.element().type);
	} else {
	    System.out.println("REQUEST IN: payload is null");
	}
    }

    static public class MyParam extends CallbackParam
    {
	int foo;
	public MyParam(int i) { foo = i; }
    }

    static public class TemperatureServiceProvider extends ResourceServiceProvider
    {
	static private PayloadList<Payload> serviceGetRequest(int flag, RequestIn request, Object callbackParam)
	{
	    System.out.println("TEST serviceGetRequest ENTRY");
	    // System.out.println("TEST rqst's resource handle: " + r.getHandle());
	    // PayloadList<PayloadForResourceState> payload_list = r.getPayload();
	    // logPayloadList(payload_list);

	    PayloadList<Payload> pll = new PayloadList<Payload>();

	    Resource r = request.getResource();
	    System.out.println("TEST resource uri: " + r.getUri());

	    PayloadForResourceState pfrs = new PayloadForResourceState(r);
	    logPayloadType(pfrs);

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

            // // OCRepPayloadSetPropBool(payload, "state", Light.state);
	    // pfrs.setBool("state", true);

	    pll.add(pfrs);

	    // System.out.println("TEST: pr uri: " + pll.uri);
	    // PayloadProperties pps = payload.getProperties();
	    // System.out.println("TEST: pr prop ct: " + pps.size());
	    // pps.put("FOO", 99);
	    // PayloadProperties pps2 = payload.getProperties();
	    // System.out.println("TEST: pr prop ct 2: " + pps2.size());

	    // OCRepPayloadSetPropInt(payload, "temp", 72);
	    // pl.putInt("temp", 72); // since props is a hashmap
	    // or:  pl.getProperties() returns a hashmap ...

	    return pll; //payload;
	}

	@Override
	public int service(int flag, RequestIn request_in, Object callbackParam)
	{
	    System.out.println("TEST TemperatureServiceProvider.service routine ENTRY");
	    logRequestIn(request_in);

	    PayloadList<Payload> payloadOut = null;

	    // typedef enum
	    // {
	    //     OC_REQUEST_FLAG = (1 << 1),
	    //     OC_OBSERVE_FLAG = (1 << 2)
	    // } OCEntityHandlerFlag;
	    // if (entityHandlerRequest && (flag & OC_REQUEST_FLAG))

	    switch (request_in.method) {
	    case OCMethod.GET:
		payloadOut = serviceGetRequest(flag, request_in, callbackParam);
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

	    ResponseOut responseOut = new ResponseOut(request_in, payloadOut);
	    // ResponseOut responseOut = new ResponseOut();

	    // if we create a new URI (e.g. on a POST request) then:
	    responseOut.setResourceUri("a/foo/bar");

	    // 	{
	    // 	    // Format the response.  Note this requires some info about the request
	    // 	    response.requestHandle = crequest_in->requestHandle;
	    // 	    response.resourceHandle = crequest_in->resource;
	    // 	    response.ehResult = ehResult;
	    // 	    response.payload = (OCPayload*)new_payload; /* testing */
            // response.numSendVendorSpecificHeaderOptions = 0;
            // memset(response.sendVendorSpecificHeaderOptions, 0,
            //        sizeof response.sendVendorSpecificHeaderOptions);
            // memset(response.resourceUri, 0, sizeof response.resourceUri);

            // // Indicates that response is NOT in a persistent buffer
            responseOut.persistentBufferFlag = 0;

	    /* finally, send ResponeOut */
	    try {
		OCF.sendResponse(responseOut);
	    } catch (Exception e) {
		System.out.println("SEND RESPONSE EXCEPTION");
	    }
	    // 	    // Send the response
	    // 	    if (OCDoResponse(&response) != OC_STACK_OK)
	    // 		{
	    // 		    printf("Error sending response\n");
	    // 		    ehResult = OC_EH_ERROR;
	    // 		}
	    // 	}
	    //     return ehResult;


	    System.out.println("ResourceServiceProvider.service EXIT");
	    return ServiceResult.OK;
	}
    }

    static TemperatureServiceProvider tempServiceProvider = new TemperatureServiceProvider();

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
	OCF.Init(null, 0, OCMode.SERVER, "/Users/gar/iotivity/kit/java/jni-c/data.d/oic_svr_db_server.dat");

	OCF.setPlatformInfo("Fartmaster",
				"Acme Novelties",
				"http://acme.example.org",
				"modelnbr", "mfgdate", "platversion",
				"osversion", "hwversion", "firmwareversion",
				"http://acme.example.org/support",
				new SimpleDateFormat("yyyy.MM.dd.HH.mm.ss").format(new Date()));

	OCF.setDeviceInfo("Fartmaster2020",
			  new String[] {"type1", "type2"},
			  "version-0.1",
			  new String[] {"dmversion-0.1"});

	Resource tr = null; // temperature resource
	tr = ResourceManager.registerResource("/a/temperature",	          // uri String
					      new String[] {"core.temp"}, // array of typenames
					      new String[] {"oc.mi.def"}, // array of ifnames
					      tempServiceProvider, // ResourceServiceProvider
					      new MyParam(99), // CallbackParam object
					      (byte)(Resource.DISCOVERABLE
						     | Resource.SECURE));
	// | OC_OBSERVABLE | OC_SECURE);

	// if (r != OCStackResult.OK) {
	//     System.out.println("TEMPERATURE resource service provider registration failed!");
	//     // exit (EXIT_FAILURE);
	// } else {
	//     System.out.println("TEMPERATURE resource service provider registration succeeded!\n");
	// }

	logResource(tr);

	// System.out.println("temperatureResource handle: " + tr.handle);
	// System.out.println("temperatureResource uri: " + tr.getUri());
	// LinkedList<String> tll = tr.getTypes();
	// System.out.println("temperatureResource type (first): " + tll.getFirst());
	// LinkedList<String> ifll = tr.getInterfaces();
	// System.out.println("temperatureResource if   (first): " + ifll.getFirst());

	// System.out.println("temperatureResource policies: " + tr.policies);

	OCF.run();

        while(true){
	    try {
		Thread.sleep(2000);
		System.out.println("GUI thread loop");
	    } catch (InterruptedException e) {
		e.printStackTrace();
		msgError(TAG, e.toString());
	    }

            // outputting data until the quit boolean flag is not set
            if(quit==true) break;
            // System.out.println("hello buddy");
        }

        // Thread t1=new Thread(new Quitter());
        // t1.start();
    }

    public static void msgError(final String tag ,final String text) {
        System.out.println("[E]" + tag + " | " + text);
    }

    private final static String TAG = OCFTestServer.class.getSimpleName();
    static BufferedReader in=new BufferedReader(new InputStreamReader(System.in));
    static boolean quit=false;
    static class Quitter implements Runnable {
	public void run(){
	    String msg = null;
	    // threading is waiting for the key Q to be pressed
	    while(true){
		try{
		    msg=in.readLine();
		}catch(IOException e){
		    e.printStackTrace();
		}

		if(msg.equals("Q")) {quit=true;break;}
	    }
	}
    }
}
