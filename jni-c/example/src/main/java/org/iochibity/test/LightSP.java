package org.iochibity.test;

import org.iochibity.OCF;
// import org.iochibity.CallbackParam;
import org.iochibity.DeviceAddress;
import org.iochibity.HeaderOption;
import org.iochibity.MsgRequestIn;
import org.iochibity.MsgResponseOut;
import org.iochibity.Payload;
import org.iochibity.PayloadForResourceState;
import org.iochibity.PayloadList;
import org.iochibity.PropertyMap;
import org.iochibity.PropertyString;
import org.iochibity.ResourceLocal;
import org.iochibity.ResourceManager;
import org.iochibity.IResourceServiceProvider;
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

public class LightSP implements IResourceServiceProvider
{
    int foo;			// instead of CallbackParam;

    // @Override
    // public int serviceRequestIn(int flag, MsgRequestIn request_in, CallbackParam callbackParam)
    public int serviceRequestIn(MsgRequestIn request_in)
    {
	System.out.println("LightSP.service routine ENTRY");
	Logger.logRequestIn(request_in);

	PayloadList<Payload> payloadOut = null;

	// typedef enum
	// {
	//     OC_REQUEST_FLAG = (1 << 1),
	//     OC_WATCH_FLAG = (1 << 2)
	// } OCEntityHandlerFlag;
	// if (entityHandlerRequest && (flag & OC_REQUEST_FLAG))

	switch (request_in.getMethod()) {
	case Method.GET:
	    payloadOut = serviceGetRequest(request_in);
	    break;
	case Method.PUT:
	    System.out.println("Light: method: PUT");
	    break;
	case Method.POST:
	    System.out.println("Light: method: POST");
	    break;
	case Method.DELETE:
	    System.out.println("Light: method: DELETE");
	    break;
	    // NOTE: in C API, WATCH (OBSERVE) methods on RequestOut become GET
	    // method plus OBSERVE flag (OC_REQUEST_FLAG) on RequestIn.
	    // It is sent as a header option.
	    // Our API reconsitutes the original method:
	case Method.WATCH:	// = GET with OBSERVE_REGISTER option
	    break;
	case Method.WATCH_ALL:	// = GET with OBSERVE_REGISTER option plus ???
	    break;
	case Method.CANCEL_WATCH: // = GET with OBSERVE_DEREGISTER option
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

	MsgResponseOut responseOut = new MsgResponseOut(request_in, payloadOut);
	// MsgResponseOut responseOut = new MsgResponseOut();

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

    static private PayloadList<Payload> serviceGetRequest(MsgRequestIn request)
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
