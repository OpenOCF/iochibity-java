package org.iochibity.test.server;

import org.iochibity.OCF;
// import org.iochibity.CallbackParam;
import org.iochibity.DeviceAddress;
import org.iochibity.HeaderOption;
import org.iochibity.StimulusIn;
import org.iochibity.ObservationOut;
import org.iochibity.Observation;
// import org.iochibity.PayloadForResourceState;
import org.iochibity.ObservationList;
import org.iochibity.PropertyMap;
import org.iochibity.PropertyString;
// import org.iochibity.ResourceLocal;
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

public class LightSP
    extends    ServiceProvider
    // implements IServiceProvider
{
    int foo;			// instead of CallbackParam;

    // @Override
    // public int observeStimulus(int flag, StimulusIn request_in, CallbackParam callbackParam)
    public int observeStimulus(StimulusIn request_in)
    {
	System.out.println("LightSP.service routine ENTRY");
	Logger.logRequestIn(request_in);

	ObservationList<Observation> observationOut = null;

	// typedef enum
	// {
	//     OC_REQUEST_FLAG = (1 << 1),
	//     OC_WATCH_FLAG = (1 << 2)
	// } OCEntityHandlerFlag;
	// if (entityHandlerRequest && (flag & OC_REQUEST_FLAG))

	switch (request_in.getMethod()) {
	case Method.GET:
	    observationOut = serviceGetRequest(request_in);
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

	ObservationOut responseOut = new ObservationOut(request_in, observationOut);
	// ObservationOut responseOut = new ObservationOut();

	// // Indicates that response is NOT in a persistent buffer
	responseOut.persistentBufferFlag = 0;

	/* finally, send ResponeOut */
	try {
	    this.exhibitBehavior();
	} catch (Exception e) {
	    System.out.println("SEND RESPONSE EXCEPTION");
	}

	System.out.println("LightSP.service EXIT");
	return ServiceResult.OK;
    }

    private ObservationList<Observation> serviceGetRequest(StimulusIn request)
    {
	System.out.println("Light: serviceGetRequest ENTRY");
	// ResourceLocal r = request.getResource();
	System.out.println("Light: resource uri: " + this.getUriPath());

	// ObservationForResourceState pfrs = new ObservationForResourceState(r);

	// List<String> llts = pfrs.getResourceTypes();
	// for (String s : (List<String>)llts) {
	//     System.out.println("Light: observation r type: " + s);
	// }

	// List<String> llifs = pfrs.getInterfaces();
	// for (String s : (List<String>)llifs) {
	//     System.out.println("Light: observation r interface: " + s);
	// }

	// PropertyMap<String, Object> pmps = pfrs.getProperties();
	// pmps.put("state", true);
	// for(Map.Entry<String, Object> entry : pmps.entrySet()) {
	//     System.out.println("Light: observation r prop: " + entry.getKey() + " = " + entry.getValue());
	// }

	// ObservationList<Observation> pll = new ObservationList<Observation>();
	// pll.add(pfrs);

	// return pll; //observation;
	return null;
    }

}
