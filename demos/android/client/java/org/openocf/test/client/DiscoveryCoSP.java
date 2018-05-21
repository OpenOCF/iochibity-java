package org.openocf.test.client;

import android.util.Log;

/// import openocf.CoServiceManager;
import openocf.app.CoResourceSP;
import openocf.constants.Method;
// import openocf.ObservationIn;
// import openocf.IObservationRecord;
import openocf.behavior.InboundResponse;
import openocf.behavior.ObservationRecord;
// import openocf.ObservationList;

import openocf.constants.OCStackResult;

import org.openocf.test.Logger;

import java.lang.RuntimeException;
import java.util.List;

import java.util.concurrent.CountDownLatch;


public class DiscoveryCoSP
    extends  CoResourceSP
    // implements ICoResourceSP
{
    private final String TAG = "DiscoveryCoSP";

    public CountDownLatch finished; // to control UI in OCFTestClient

    public int cbdata = 99;  // callback param data

    public DiscoveryCoSP() {
	super();
	// setMethod(Method.GET);
    }
    public DiscoveryCoSP(String uri) {
	super();
	setUri(uri);
	// setMethod(Method.GET);
    }

    public CountDownLatch latch() {
	finished = new CountDownLatch(1);
	return finished;
    }

    /**
       By the time `coReact` is called, the CoSPs matching the incoming
       SPs will have been registered with the ServiceManager.  All
       this CoSP needs to do is coReact.
     */
    public void coReact(InboundResponse resp)
    {
	Log.i(TAG, "coReact ENTRY");
	Log.i(TAG, "cbdata: " + cbdata);

	Log.i(TAG, "method: " + this._method);

	Logger.logInboundResponse(resp);

	this.isRetain = true;
	resp.isRetain = true;

	// TODO: open flutter channel; resp.getFlutterInboundResponse; send to flutter

	finished.countDown();

	// List<CoResourceSP> cosps = ServiceManager.registeredCoResourceSPs();


	// ServiceManager.registerCoResourceSPs(this.observations());
	// ServiceManager.registerCoResourceSPs(this.getSPObservations());

	// first update _this_ with incoming data
	// this.setDestination(observationIn.getRemoteDeviceAddress());
	// etc. connType, etc.

	// then iterate over Observation payloads
	// if (observationIn.result == OCStackResult.OK) {
	//     ObservationList<ObservationRecord> observationRecords = observationIn.getObservationRecords();
	//     for (ObservationRecord observationRecord
	// 	     : (ObservationList<ObservationRecord>) observationRecords) {
	// 	Log.i(TAG, "\tOBSERVED: " + observationRecord.getUriPath());
	// 	List<IObservationRecord> kids = observationRecord.getChildren();
	// 	if (kids != null) {
	// 	    for (IObservationRecord childObservationRecord : kids) {
	// 		Log.i(TAG, "\t->OBSERVED: " + childObservationRecord.getUriPath());

	// 		GenericCoSP cosp = new GenericCoSP(observationIn, (ObservationRecord)childObservationRecord);
	// 		ServiceManager.registerCoResourceSP(cosp);

	// 		if (childObservationRecord.getUriPath().equals("/a/temperature")) {
	// 		    Log.i(TAG, "LOG: found temperature resource");
	// 		    // gRemoteResourceAddress = observationIn.getRemoteDeviceAddress();
	// 		    // gRemoteResourceAddress.port
	// 		    // 	= ((Integer)childObservation.getProperties().get("port"))
	// 		    // 	.intValue();
	// 		    // Logger.logDeviceAddress(gRemoteResourceAddress);
	// 		}

	// 		if (childObservationRecord.getUriPath().equals("/a/led")) {
	// 		    Log.i(TAG, "LOG: found LED resource");
	// 		    // gLEDAddress = observationIn.getRemoteDeviceAddress();
	// 		    // gLEDAddress.port
	// 		    // 	= ((Integer)childObservationRecord.getProperties().get("port"))
	// 		    // 	.intValue();
	// 		    // Logger.logDeviceAddress(gLEDAddress);
	// 		}

	// 		if (childObservationRecord.getUriPath().equals("/a/whatsit")) {
	// 		    Log.i(TAG, "LOG: found whatsit resource");
	// 		    // gWhatsitAddress = observationIn.getRemoteDeviceAddress();
	// 		    // gWhatsitAddress.port
	// 		    // 	= ((Integer)childObservation.getProperties().get("port"))
	// 		    // 	.intValue();
	// 		    // Logger.logDeviceAddress(gWhatsitAddress);
	// 		}
	// 	    }
	// 	}
	//     }
	// }

	// throw new RuntimeException("test exception");

	// this.deactivate();

	// return 3;
    }
}

