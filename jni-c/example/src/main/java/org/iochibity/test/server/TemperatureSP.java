package org.iochibity.test.server;

import org.iochibity.OCF;
import org.iochibity.DeviceAddress;
import org.iochibity.HeaderOption;
import org.iochibity.StimulusIn;
import org.iochibity.ObservationList;
import org.iochibity.ObservationOut;
import org.iochibity.ObservationRecord;
import org.iochibity.PropertyMap;
import org.iochibity.ServiceManager;
import org.iochibity.ServiceProvider;
import org.iochibity.IServiceProvider;
import org.iochibity.constants.Method;
import org.iochibity.constants.OCStackResult;
import org.iochibity.constants.ResourcePolicy;
import org.iochibity.constants.ServiceResult;

import org.iochibity.test.Logger;

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

public class TemperatureSP
    extends    ServiceProvider
    // implements IServiceProvider
{
    int foo = 72;
    public void hello() { System.out.println("Hello from callback: " + foo); }

    public TemperatureSP() {
	setUriPath("/a/temperature");
	addType("core.temp");
	addInterface("oc.mi.temp");
	// setPolicies(Resource.DISCOVERABLE | Resource.SECURE);
    }

    public int observeStimulus(StimulusIn requestIn)
    {
	System.out.println("TEST TemperatureSP.service routine ENTRY");
	Logger.logRequestIn(requestIn);

	ObservationList<ObservationRecord> observationRecordOut = null;

	// typedef enum
	// {
	//     OC_REQUEST_FLAG = (1 << 1),
	//     OC_OBSERVE_FLAG = (1 << 2)
	// } OCEntityHandlerFlag;
	// if (entityHandlerRequest && (flag & OC_REQUEST_FLAG))

	switch (requestIn.getMethod()) {
	case Method.GET:
	    observationRecordOut = serviceGetRequest(requestIn);
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
	    System.out.println("TEST method: WATCH");
	    observationRecordOut = serviceGetRequest(requestIn);
	    break;
	case Method.WATCH_ALL:
	    System.out.println("TEST method: WATCH_ALL");
	    break;
	case Method.CANCEL_WATCH:
	    System.out.println("TEST method: CANCEL_WATCH");
	    break;
	case Method.PRESENCE:
	    System.out.println("TEST method: PRESENCE");
	    break;
	case Method.DISCOVER:
	    System.out.println("TEST method: DISCOVER");
	    // should not happen - DISCOVER method works only on client side
	    // and discover logic is handled by the stack on the server
	    break;
	case Method.NOMETHOD:
	    System.out.println("TEST method: NOMETHOD");
	    break;
	default:
	    System.out.println("TEST method: UNKNOWN");
	    break;
	}

	ObservationOut responseOut = new ObservationOut(requestIn, observationRecordOut);

	try {
	    this.exhibit();
	} catch (Exception e) {
	    System.out.println("SEND RESPONSE EXCEPTION");
	}

	System.out.println("TemperatureSP.service EXIT");
	return ServiceResult.OK;
    }

    private ObservationList<ObservationRecord> serviceGetRequest(StimulusIn requestIn)
    {
	System.out.println("TEST serviceGetRequest ENTRY");

	System.out.println("TEST callback param: " + foo);
	hello();

	// ResourceLocal r = requestIn.getResource();
	System.out.println("TEST resource uri: " + this.getUriPath());

	// PayloadForResourceState pfrs = new PayloadForResourceState(r);

	// System.out.println("TEST observation uri: " + pfrs.getUri());
	// pfrs.setUri("/a/foo");
	// System.out.println("TEST observation new uri: " + pfrs.getUri());

	// pfrs.addResourceType("foo.t.a");
	// pfrs.addResourceType("foo.t.b");
	// List<String> llts = pfrs.getResourceTypes();
	// for (String s : (List<String>)llts) {
	// 	System.out.println("TEST observation r type: " + s);
	// }
	// List<String> llifs = pfrs.getInterfaces();
	// llifs.add("foo.if.a");
	// for (String s : (List<String>)llifs) {
	// 	System.out.println("TEST observation r interface: " + s);
	// }

	// PropertyMap<String, Object> pmps = pfrs.getProperties();
	// pmps.put("i1", -1);
	// pmps.put("i2", 0);
	// pmps.put("i3", 1);
	// pmps.put("d1", -1.1);
	// pmps.put("d2", 0.0);
	// pmps.put("d3", 1.1);
	// pmps.put("s1", "Hello world");
	// pmps.put("s2", "Goodbye world");
	// pmps.put("b1", true);
	// pmps.put("b2", false);
	// for(Map.Entry<String, Object> entry : pmps.entrySet()) {
	// 	System.out.println("TEST observation r prop: " + entry.getKey() + " = " + entry.getValue());
	// }

	// ObservationList<Observation> pll = new ObservationList<Observation>();

	// pll.add(pfrs);

	// return pll; //observation;
	return null;
    }

}

