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

public class LightSP extends ResourceServiceProvider
{

    static public class CBData extends CallbackParam
    {
	int foo;
	public CBData(int i) { foo = i; }
    }

    @Override
    public int service(int flag, DocRequestIn request_in, CallbackParam callbackParam)
    {
	System.out.println("LightSP.service routine ENTRY");
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
	    payloadOut = serviceGetRequest(flag, request_in, callbackParam);
	    break;
	case OCMethod.PUT:
	    System.out.println("Light: method: PUT");
	    break;
	case OCMethod.POST:
	    System.out.println("Light: method: POST");
	    break;
	case OCMethod.DELETE:
	    System.out.println("Light: method: DELETE");
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
	// DocResponseOut responseOut = new DocResponseOut();

	// // Indicates that response is NOT in a persistent buffer
	responseOut.persistentBufferFlag = 0;

	/* finally, send ResponeOut */
	try {
	    OCF.sendResponse(responseOut);
	} catch (Exception e) {
	    System.out.println("SEND RESPONSE EXCEPTION");
	}

	System.out.println("LightSP.service EXIT");
	return ServiceResult.OK;
    }

    static private PayloadList<Payload> serviceGetRequest(int flag,
							  DocRequestIn request,
							  CallbackParam callbackParam)
    {
	System.out.println("Light: serviceGetRequest ENTRY");
	ResourceLocal r = request.getResource();
	System.out.println("Light: resource uri: " + r.getUri());

	PayloadForResourceState pfrs = new PayloadForResourceState(r);
	Logger.logPayloadType(pfrs);

	List<String> llts = pfrs.getResourceTypes();
	for (String s : (List<String>)llts) {
	    System.out.println("Light: payload r type: " + s);
	}

	List<String> llifs = pfrs.getInterfaces();
	for (String s : (List<String>)llifs) {
	    System.out.println("Light: payload r interface: " + s);
	}

	PropertyMap<String, Object> pmps = pfrs.getProperties();
	pmps.put("state", true);
	for(Map.Entry<String, Object> entry : pmps.entrySet()) {
	    System.out.println("Light: payload r prop: " + entry.getKey() + " = " + entry.getValue());
	}

	PayloadList<Payload> pll = new PayloadList<Payload>();
	pll.add(pfrs);

	return pll; //payload;
    }

}
