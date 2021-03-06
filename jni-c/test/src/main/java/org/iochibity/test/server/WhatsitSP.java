package org.iochibity.test.server;

import org.iochibity.OCF;
import org.iochibity.DeviceAddress;
import org.iochibity.HeaderOption;
// import org.iochibity.StimulusIn;
// import org.iochibity.ObservationOut;
import org.iochibity.ObservationRecord;
// import org.iochibity.ObservationList;
import org.iochibity.PropertyMap;
import org.iochibity.Resource;
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

public class WhatsitSP
    extends  ServiceProvider
    // implements IServiceProvider
{

    int foo = 99;

    public WhatsitSP() {
	setUriPath("/foo/whatsit");
	addType("foo.t.whatsit");
	addInterface("foo.if.whatsit");
	setPolicies(Resource.DISCOVERABLE
		    // | Resource.WATCHABLE
		    | Resource.SECURE);
    }

    public WhatsitSP(String uriPath) {
	setUriPath(uriPath);
	addType("foo.t.whatsit");
	addInterface("foo.if.whatsit");
	setPolicies(Resource.DISCOVERABLE
		    // | Resource.WATCHABLE
		    | Resource.SECURE);
    }

    @Override
    public void react()
    {
	System.out.println("WhatsitSP.react routine ENTRY");
	// Logger.logRequestIn(stimulusIn);

	System.out.println("WhatsitSP: stimulusIn callback param foo = " + foo);

	switch (this.method()) {
	case Method.GET:
	    System.out.println("WhatsitSP: method: GET");
	    // FIXME: try catch?
	    // this.reactToGetObservation(stimulusIn);
	    break;
	case Method.PUT:
	    System.out.println("WhatsitSP: method: PUT");
	    break;
	case Method.POST:
	    System.out.println("WhatsitSP: method: POST");
	    // observationsOut = servicePostRequest(stimulusIn);
	    break;
	case Method.DELETE:
	    System.out.println("Whatsit method: DELETE");
	    break;
	case Method.WATCH:
	    System.out.println("Whatsit method: WATCH");
	    // observationsOut = serviceGetRequest(stimulusIn);
	    break;
	case Method.WATCH_ALL:
	    System.out.println("Whatsit method: WATCH_ALL");
	    break;
	case Method.CANCEL_WATCH:
	    System.out.println("Whatsit method: CANCEL_WATCH");
	    break;
	case Method.PRESENCE:
	    System.out.println("Whatsit method: PRESENCE");
	    break;
	case Method.DISCOVER:
	    System.out.println("Whatsit method: DISCOVER");
	    // should not happen - DISCOVER method works only on client side
	    // and discover logic is handled by the stack on the server
	    break;
	case Method.NOMETHOD:
	    System.out.println("Whatsit method: NOMETHOD");
	    break;
	default:
	    System.out.println("Whatsit method: UNKNOWN");
	    break;
	}

	// send response
	try {
	    this.exhibit();
	    // this.exhibit();
	} catch (Exception e) {
	    System.out.println("[E] WhatisSP" + " | " + "exhibit exception");
	    e.printStackTrace();
	}

	System.out.println("WhatsitSP.react EXIT");
	return; // ServiceResult.OK;
    }

    // private ObservationList<Observation> serviceGetRequest(StimulusIn request)
    // private ObservationList<Observation> ObserveGetStimulus(StimulusIn stimulusIn)
    private void reactToGetObservation()
    {
	System.out.println("WhatsitSP.serviceGetRequest ENTRY");

	// to react to a stimulus, the ServiceProvider sets its
	// internal state (types, interfaces, properties, etc.).  Then
	// the exhibit will use the state to generate and send
	// an Observation.

	// NOTE that messages and messaging are hidden.

	// Mandatory: first react to the StimulusIn. This sets some of
	// the internal state of the ServiceProvider
	// this.react(stimulusIn);

	// Now modify internal state.  The `exhibit` routine
	// will use it to generate and send an observation message qua
	// response.

	// FIXME: these should modify the underlying OCResource, using e.g.
	// OCBindResourceTypeToResource and OCBindResourceInterfaceToResource
	this.addType("foo.t.a");
	this.addInterface("foo.if.a");

	// if we were providing services for a real sensor instrument,
	// we would read the instrument here and set properties
	// appropriately.
	this.putProperty("whatsit int", 1);
	this.putProperty("whatsit d", 1.1);
	this.putProperty("whatsit str", "Hello world");
	this.putProperty("whatsit bool", true);

	// what if we have child resources?

	return;
    }

    // private ObservationList<Observation> servicePostRequest(StimulusIn request)
    // {
    // 	System.out.println("WhatsitSP.servicePostRequest ENTRY");

    // 	// ResourceLocal r = request.getResource();
    // 	System.out.println("WhatsitSP: resource uri: " + this.getUriPath());

    // 	PayloadForResourceState pfrs = new PayloadForResourceState(request);

    // 	System.out.println("WhatsitSP: observation uri: " + pfrs.getUri());
    // 	// pfrs.setUri("/a/foo");
    // 	// System.out.println("WhatsitSP: observation new uri: " + pfrs.getUri());

    // 	pfrs.addResourceType("foo.t.a");
    // 	// pfrs.addResourceType("foo.t.b");
    // 	// List<String> llts = pfrs.getResourceTypes();
    // 	// for (String s : (List<String>)llts) {
    // 	//     System.out.println("WhatsitSP: observation r type: " + s);
    // 	// }
    // 	List<String> llifs = pfrs.getInterfaces();
    // 	llifs.add("foo.if.a");
    // 	// for (String s : (List<String>)llifs) {
    // 	//     System.out.println("WhatsitSP: observation r interface: " + s);
    // 	// }

    // 	PropertyMap<String, Object> pmps = pfrs.getProperties();
    // 	pmps.put("whatsit int", 1);
    // 	pmps.put("whatsit d", 1.1);
    // 	pmps.put("whatsit str", "Hello world");
    // 	pmps.put("whatsit bool", true);
    // 	// for(Map.Entry<String, Object> entry : pmps.entrySet()) {
    // 	//     System.out.println("WhatsitSP: observation r prop: " + entry.getKey() + " = " + entry.getValue());
    // 	// }

    // 	ObservationList<Observation> pll = new ObservationList<Observation>();

    // 	pll.add(pfrs);

    // 	return pll; //observation;
    // }

}
