package org.iochibity.test;

import org.iochibity.OCF;
import org.iochibity.DeviceAddress;
import org.iochibity.HeaderOption;
import org.iochibity.Messenger;
import org.iochibity.MsgRequestIn;
import org.iochibity.MsgResponseOut;
import org.iochibity.Payload;
import org.iochibity.PayloadList;
import org.iochibity.PropertyMap;
import org.iochibity.PayloadForResourceState;
import org.iochibity.PropertyString;
import org.iochibity.Resource;
import org.iochibity.ResourceLocal;
import org.iochibity.AServiceProvider;
import org.iochibity.IServiceProvider;
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

public class WhatsitSP
    extends    AServiceProvider
    implements IServiceProvider
{

    int foo = 99;

    WhatsitSP() {
	setUriPath("/a/whatsit");
	addType("core.whatsit");
	addInterface("oc.mi.whatsit");
	setPolicies(Resource.DISCOVERABLE | Resource.SECURE);
    }

    public int serviceRequestIn(MsgRequestIn requestIn)
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

	switch (requestIn.getMethod()) {
	case Method.GET:
	    payloadOut = serviceGetRequest(requestIn);
	    break;
	case Method.PUT:
	    System.out.println("WhatsitSP: method: PUT");
	    break;
	case Method.POST:
	    System.out.println("WhatsitSP: method: POST");
	    payloadOut = servicePostRequest(requestIn);
	    break;
	case Method.DELETE:
	    System.out.println("WhatsitSP: method: DELETE");
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
	// MsgResponseOut responseOut = new MsgResponseOut();

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
	    Messenger.sendResponse(responseOut);
	} catch (Exception e) {
	    System.out.println("[E] WhatisSP" + " | " + "sendResponse exception");
	    e.printStackTrace();
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

    private PayloadList<Payload> serviceGetRequest(MsgRequestIn request)
    {
	System.out.println("WhatsitSP.serviceGetRequest ENTRY");

	// ResourceLocal r = request.getResource();
	System.out.println("WhatsitSP: resource uri: " + this.getUriPath());

	PayloadForResourceState pfrs = new PayloadForResourceState(request);

	System.out.println("WhatsitSP: payload uri: " + pfrs.getUri());
	// pfrs.setUri("/a/foo");
	// System.out.println("WhatsitSP: payload new uri: " + pfrs.getUri());

	pfrs.addResourceType("foo.t.a");
	// pfrs.addResourceType("foo.t.b");
	// List<String> llts = pfrs.getResourceTypes();
	// for (String s : (List<String>)llts) {
	//     System.out.println("WhatsitSP: payload r type: " + s);
	// }
	List<String> llifs = pfrs.getInterfaces();
	llifs.add("foo.if.a");
	// for (String s : (List<String>)llifs) {
	//     System.out.println("WhatsitSP: payload r interface: " + s);
	// }

	PropertyMap<String, Object> pmps = pfrs.getProperties();
	pmps.put("whatsit int", 1);
	pmps.put("whatsit d", 1.1);
	pmps.put("whatsit str", "Hello world");
	pmps.put("whatsit bool", true);
	// for(Map.Entry<String, Object> entry : pmps.entrySet()) {
	//     System.out.println("WhatsitSP: payload r prop: " + entry.getKey() + " = " + entry.getValue());
	// }

	PayloadList<Payload> pll = new PayloadList<Payload>();

	pll.add(pfrs);

	return pll; //payload;
    }

    private PayloadList<Payload> servicePostRequest(MsgRequestIn request)
    {
	System.out.println("WhatsitSP.servicePostRequest ENTRY");

	// ResourceLocal r = request.getResource();
	System.out.println("WhatsitSP: resource uri: " + this.getUriPath());

	PayloadForResourceState pfrs = new PayloadForResourceState(request);

	System.out.println("WhatsitSP: payload uri: " + pfrs.getUri());
	// pfrs.setUri("/a/foo");
	// System.out.println("WhatsitSP: payload new uri: " + pfrs.getUri());

	pfrs.addResourceType("foo.t.a");
	// pfrs.addResourceType("foo.t.b");
	// List<String> llts = pfrs.getResourceTypes();
	// for (String s : (List<String>)llts) {
	//     System.out.println("WhatsitSP: payload r type: " + s);
	// }
	List<String> llifs = pfrs.getInterfaces();
	llifs.add("foo.if.a");
	// for (String s : (List<String>)llifs) {
	//     System.out.println("WhatsitSP: payload r interface: " + s);
	// }

	PropertyMap<String, Object> pmps = pfrs.getProperties();
	pmps.put("whatsit int", 1);
	pmps.put("whatsit d", 1.1);
	pmps.put("whatsit str", "Hello world");
	pmps.put("whatsit bool", true);
	// for(Map.Entry<String, Object> entry : pmps.entrySet()) {
	//     System.out.println("WhatsitSP: payload r prop: " + entry.getKey() + " = " + entry.getValue());
	// }

	PayloadList<Payload> pll = new PayloadList<Payload>();

	pll.add(pfrs);

	return pll; //payload;
    }

}
