import org.iochibity.OCF;
import org.iochibity.DeviceAddress;
import org.iochibity.Payload;
import org.iochibity.PayloadRepresentation;
import org.iochibity.RequestIn;
import org.iochibity.Resource;
import org.iochibity.ResourceManager;
import org.iochibity.ResourceServiceProvider;
import org.iochibity.Response;
import org.iochibity.ResponseOut;
import org.iochibity.constants.OCMethod;
import org.iochibity.constants.OCMode;
import org.iochibity.constants.OCStackResult;
import org.iochibity.constants.ResourcePolicy;
import org.iochibity.constants.ServiceResult;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.LinkedList;

public class OCFTest
{
    static{
	try{
	    System.loadLibrary("ocfjni_c");
	}catch(Exception e){
	    System.out.println(e.toString());
	}
    }

    static private void logRequestIn(RequestIn request)
    {
	System.out.println("TEST ResourceServiceProvider.service ENTRY");
	// System.out.println("resourceHandle: " + request.resourceHandle);
	System.out.println("TEST handle at origin: " + request.handleAtOrigin);
	System.out.println("TEST request method: " + request.method);
	System.out.println("TEST query : \"" + request.query + "\"");
	System.out.println("TEST msg id : " + request.messageId);
	System.out.println("TEST request vendorHeaderOptionsCount: " + request.vendorHeaderOptionsCount);

	Resource resource = request.getResource();
	System.out.println("TEST resource uri: " + resource.uri);
	LinkedList<String> tll = resource.getTypes();
	tll.forEach(t -> System.out.println("TEST resource type: " + t)); // Java 8
	LinkedList<String> ifll = resource.getInterfaces();
	ifll.forEach(iface -> System.out.println("TEST resource if: " + iface)); // Java 8
	// for (int i = 0; i < ifll.size(); i++) {
	//     System.out.println("TEST resource if:    " + ifll.get(i));
	// }

	System.out.println("TEST devaddr adapter: " + request.deviceAddress.adapter);
	System.out.println("TEST devaddr flags: " + String.format("0x%04X", request.deviceAddress.flags & 0xFFFFF));
	System.out.println("TEST devaddr port: " + request.deviceAddress.port);
	System.out.println("TEST devaddr address: " + request.deviceAddress.address);
	System.out.println("TEST devaddr ifindex: " + request.deviceAddress.ifindex);
	System.out.println("TEST devaddr route data: " + request.deviceAddress.routeData);


	Payload payload = request.getPayload();
	if (payload != null) {
	    System.out.println("TEST payload type: " + payload.type);
	} else {
	    System.out.println("TEST payload is null");
	}
    }

    static public class TemperatureServiceProvider extends ResourceServiceProvider
    {
	static private Payload serviceGetRequest(int flag, RequestIn reqeust, Object callbackParam)
	{
	    System.out.println("TEST serviceGetRequest ENTRY");
	    PayloadRepresentation payload = new PayloadRepresentation();

	    //     OCEntityHandlerResult ehResult = OC_EH_OK;
	    //     OCEntityHandlerResponse response = { .requestHandle = NULL }; /* ResponseOut */
	    //     OCRepPayload* new_payload = OCRepPayloadCreate();
	    //     if(!new_payload)
	    //     {
	    //         printf("Failed to allocate Payload\n");
	    // 	ehResult = OC_EH_ERROR;
	    //     }

	    // #define RSC_URI_TEMPERATURE  "/a/temperature"
	    //     OCRepPayloadSetUri(new_payload, RSC_URI_TEMPERATURE);
	    //     OCRepPayloadSetPropInt(new_payload, "temp", 72);

	    return payload;
	}

	@Override
	public int service(int flag, RequestIn request, Object callbackParam)
	{
	    logRequestIn(request);

	    ResponseOut response = new ResponseOut();
	    Payload payload;

	    switch (request.method) {
	    case OCMethod.GET:
		payload = serviceGetRequest(flag, request, callbackParam);
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

	    /* now populate ResponseOut object with payload, etc. */

	    /* finally, send ResponeOut */
	    OCF.sendResponse(response);

	    /* 2. create a ResponseOut object */
	    /* 3. populate the ResponseOut */
	    /* 4. send the ResponseOut */


	    //     /* *payload = new_payload; */
	    //     // If the result isn't an error or forbidden, send response
	    //     if (!((ehResult == OC_EH_ERROR) || (ehResult == OC_EH_FORBIDDEN)))
	    // 	{
	    // 	    // Format the response.  Note this requires some info about the request
	    // 	    response.requestHandle = crequest_in->requestHandle;
	    // 	    response.resourceHandle = crequest_in->resource;
	    // 	    response.ehResult = ehResult;
	    // 	    response.payload = (OCPayload*)new_payload; /* testing */
	    // 	    // Indicate that response is NOT in a persistent buffer
	    // 	    response.persistentBufferFlag = 0;

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

    static Resource temperatureResource = new Resource();
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
	int r = OCF.Init(null, 0, OCMode.SERVER, "/Users/gar/iotivity/kit/java/jni-c/data.d/oic_svr_db_server.dat");
	System.out.println("Init returned: " + r);

	r = OCF.setPlatformInfo("Fartmaster",
				"Acme Novelties",
				"http://acme.example.org",
				"modelnbr", "mfgdate", "platversion",
				"osversion", "hwversion", "firmwareversion",
				"http://acme.example.org/support",
				new SimpleDateFormat("yyyy.MM.dd.HH.mm.ss").format(new Date()));

	r = OCF.setDeviceInfo("Fartmaster2020",
			      new String[] {"type1", "type2"},
			      "version-0.1",
			      new String[] {"dmversion-0.1"});

	System.out.println("resource count before: " + ResourceManager.resourceCount());

	r = ResourceManager.registerResource(temperatureResource, // OCResourceHandle*
					     "core.temp",
					     "oc.mi.def",
					     "/a/temperature",
					     tempServiceProvider,
					     null,
					     (byte)(ResourcePolicy.DISCOVERABLE
						    | ResourcePolicy.SECURE));
	// | OC_OBSERVABLE | OC_SECURE);

	if (r != OCStackResult.OK) {
	    System.out.println("TEMPERATURE resource service provider registration failed!");
	    // exit (EXIT_FAILURE);
	} else {
	    System.out.println("TEMPERATURE resource service provider registration succeeded!\n");
	}
	System.out.println("resource count after: " + ResourceManager.resourceCount());

	System.out.println("temperatureResource handle: " + temperatureResource.handle);
	System.out.println("temperatureResource uri: " + temperatureResource.uri);
	LinkedList<String> tll = temperatureResource.getTypes();
	System.out.println("temperatureResource type (first): " + tll.getFirst());
	LinkedList<String> ifll = temperatureResource.getInterfaces();
	System.out.println("temperatureResource if   (first): " + ifll.getFirst());

        System.out.println("press q then ENTER to terminate");

	r = OCF.run();
	System.out.println("run returned: " + r);

        while(true){
	    try {
		Thread.sleep(2000);
		System.out.println("GUI thread loop");
		System.out.println("resource count: " + ResourceManager.resourceCount());
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

    private final static String TAG = OCFTest.class.getSimpleName();
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
