package org.iochibity.test.server;

import org.iochibity.OCF;
import org.iochibity.DeviceAddress;
import org.iochibity.HeaderOption;
import org.iochibity.StimulusIn;
import org.iochibity.ObservationOut;
import org.iochibity.ObservationRecord;
import org.iochibity.ObservationList;
import org.iochibity.PropertyMap;
import org.iochibity.ServiceProvider;
import org.iochibity.IServiceProvider;
import org.iochibity.constants.Method;
import org.iochibity.constants.OCStackResult;
import org.iochibity.constants.ResourcePolicy;
import org.iochibity.constants.ServiceResult;

import org.iochibity.test.Logger;

import org.iochibity.exceptions.OCFNotImplementedException;

import mraa.Dir;
import mraa.Gpio;
import mraa.IntelEdison;
import mraa.mraa;
import mraa.Platform;
import mraa.Result;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.ArrayList;
import java.util.List;
import java.util.LinkedList;
import java.util.Map;

public class LedSP
    extends    ServiceProvider
{

    int foo = 99;

    final static int DEFAULT_IOPIN = 8;
    Gpio gpio;

    public LedSP() {
	setUriPath("/a/led");
	addType("core.led");
	addInterface("oc.mi.led");
	// setPolicies(Resource.DISCOVERABLE | Resource.SECURE);
	// gpio = new Gpio(182);
	gpio = new Gpio(IntelEdison.INTEL_EDISON_GP128.swigValue());
	Result result = gpio.dir(Dir.DIR_OUT);
	if (result != Result.SUCCESS) {
            mraa.printError(result);
            System.exit(1);
        }
    }

    public int observeStimulus(StimulusIn requestIn)
    {
	System.out.println("LedSP.service routine ENTRY");
	Logger.logRequestIn(requestIn);

	System.out.println("LedSP: requestIn callback param foo = " + foo);

	ObservationList<ObservationRecord> observationOut = null;

	// typedef enum
	// {
	//     OC_REQUEST_FLAG = (1 << 1),
	//     OC_OBSERVE_FLAG = (1 << 2)
	// } OCEntityHandlerFlag;
	// if (entityHandlerRequest && (flag & OC_REQUEST_FLAG))

	switch (requestIn.getMethod()) {
	case Method.GET:
	    observationOut = serviceGetRequest(requestIn);
	    break;
	case Method.PUT:
	    System.out.println("LedSP: method: PUT");
	    break;
	case Method.POST:
	    System.out.println("LedSP: method: POST");
	    observationOut = servicePostRequest(requestIn);
	    break;
	case Method.DELETE:
	    System.out.println("LedSP: method: DELETE");
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

	ObservationOut responseOut = new ObservationOut(requestIn, observationOut);
	// ObservationOut responseOut = new ObservationOut();

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
	    this.exhibitBehavior();
	} catch (Exception e) {
	    System.out.println("[E] LedSP" + " | " + "exhibitBehavior exception");
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


	System.out.println("LedSP.service EXIT");
	return ServiceResult.OK;
    }

    private ObservationList<ObservationRecord> serviceGetRequest(StimulusIn request)
    {
	System.out.println("LedSP.serviceGetRequest ENTRY");

	// // ResourceLocal r = request.getResource();
	// System.out.println("LedSP: resource uri: " + this.getUriPath());

	// PayloadForResourceState pfrs = new PayloadForResourceState(request);

	// System.out.println("LedSP: observation uri: " + pfrs.getUri());
	// // pfrs.setUri("/a/foo");
	// // System.out.println("LedSP: observation new uri: " + pfrs.getUri());

	// pfrs.addResourceType("foo.t.a");
	// // pfrs.addResourceType("foo.t.b");
	// // List<String> llts = pfrs.getResourceTypes();
	// // for (String s : (List<String>)llts) {
	// //     System.out.println("LedSP: observation r type: " + s);
	// // }
	// List<String> llifs = pfrs.getInterfaces();
	// llifs.add("foo.if.a");
	// // for (String s : (List<String>)llifs) {
	// //     System.out.println("LedSP: observation r interface: " + s);
	// // }

	// PropertyMap<String, Object> pmps = pfrs.getProperties();
	// pmps.put("LED int", 1);
	// pmps.put("LED d", 1.1);
	// pmps.put("LED str", "Hello world");
	// pmps.put("LED bool", true);
	// // for(Map.Entry<String, Object> entry : pmps.entrySet()) {
	// //     System.out.println("LedSP: observation r prop: " + entry.getKey() + " = " + entry.getValue());
	// // }

	// ObservationList<Observation> pll = new ObservationList<Observation>();

	// // pll.add(pfrs);

	return null;
    }

    private ObservationList<ObservationRecord> servicePostRequest(StimulusIn request)
    {
	System.out.println("LedSP.servicePostRequest ENTRY");

	// ResourceLocal r = request.getResource();
	System.out.println("LedSP: resource uri: " + this.getUriPath());

	 int val = gpio.read();
	 if (val > 0) {
	     gpio.write(0);
	 } else {
	     gpio.write(1);
	 }

	ObservationList<ObservationRecord> pll = new ObservationList<ObservationRecord>();

	return pll; //observationRecord;
    }

}
