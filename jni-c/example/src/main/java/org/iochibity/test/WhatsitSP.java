package org.iochibity.test;

import org.iochibity.OCF;
import org.iochibity.DeviceAddress;
import org.iochibity.HeaderOption;
import org.iochibity.Payload;
import org.iochibity.PayloadList;
import org.iochibity.PropertyMap;
import org.iochibity.PayloadForResourceState;
import org.iochibity.PropertyString;
import org.iochibity.DocRequestIn;
import org.iochibity.ResourceLocal;
// import org.iochibity.Resource$InstanceId;
import org.iochibity.ResourceManager;
import org.iochibity.IResourceServiceProvider;
// import org.iochibity.Response;
import org.iochibity.DocResponseOut;
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

public class WhatsitSP implements IResourceServiceProvider
{

    int foo = 99;

    // @Override
    // public int serviceRequestIn(int flag, DocRequestIn request_in, CallbackParam callbackParam)
    public int serviceRequestIn(DocRequestIn requestIn)
    {
	System.out.println("WhatsitSP.service routine ENTRY");
	Logger.logRequestIn(requestIn);

	System.out.println("WhatsitSP: requestIn callback param foo = " + foo);

	PayloadList<Payload> payloadOut = null;

	// typedef enum
	// {
	//     OC_REQUEST_FLAG = (1 << 1),
	//     OC_OBSERVE_FLAG = (1 << 2)
	// } OCEntityHandlerFlag;
	// if (entityHandlerRequest && (flag & OC_REQUEST_FLAG))

	switch (requestIn.method) {
	case Method.GET:
	    payloadOut = serviceGetRequest(requestIn);
	    break;
	case Method.PUT:
	    System.out.println("WhatsitSP: method: PUT");
	    break;
	case Method.POST:
	    System.out.println("WhatsitSP: method: POST");
	    break;
	case Method.DELETE:
	    System.out.println("WhatsitSP: method: DELETE");
	    break;
	case Method.OBSERVE:
	    break;
	case Method.OBSERVE_ALL:
	    break;
	case Method.CANCEL_OBSERVE:
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

	DocResponseOut responseOut = new DocResponseOut(requestIn, payloadOut);
	// DocResponseOut responseOut = new DocResponseOut();

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


	System.out.println("WhatsitSP.service EXIT");
	return ServiceResult.OK;
    }

    static private PayloadList<Payload> serviceGetRequest(DocRequestIn request)
    {
	System.out.println("WhatsitSP.serviceGetRequest ENTRY");

	ResourceLocal r = request.getResource();
	System.out.println("WhatsitSP: resource uri: " + r.getUri());

	PayloadForResourceState pfrs = new PayloadForResourceState(r);
	Logger.logPayloadType(pfrs);

	System.out.println("WhatsitSP: payload uri: " + pfrs.getUri());
	pfrs.setUri("/a/foo");
	System.out.println("WhatsitSP: payload new uri: " + pfrs.getUri());

	pfrs.addResourceType("foo.t.a");
	pfrs.addResourceType("foo.t.b");
	List<String> llts = pfrs.getResourceTypes();
	for (String s : (List<String>)llts) {
	    System.out.println("WhatsitSP: payload r type: " + s);
	}
	List<String> llifs = pfrs.getInterfaces();
	llifs.add("foo.if.a");
	for (String s : (List<String>)llifs) {
	    System.out.println("WhatsitSP: payload r interface: " + s);
	}

	PropertyMap<String, Object> pmps = pfrs.getProperties();
	pmps.put("whatsit int", 1);
	pmps.put("whatsit d", 1.1);
	pmps.put("whatsit str", "Hello world");
	pmps.put("whatsit bool", true);
	for(Map.Entry<String, Object> entry : pmps.entrySet()) {
	    System.out.println("WhatsitSP: payload r prop: " + entry.getKey() + " = " + entry.getValue());
	}

	PayloadList<Payload> pll = new PayloadList<Payload>();

	pll.add(pfrs);

	return pll; //payload;
    }

}
