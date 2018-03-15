package org.openocf.test.client;

import openocf.engine.OCFClientSP;
import openocf.app.CoResourceSP;
import openocf.constants.Method;
// import openocf.ObservationIn;
// import openocf.IObservationRecord;
import openocf.signals.ObservationRecord;
// import openocf.ObservationList;

import openocf.constants.OCStackResult;

import org.openocf.test.Logger;

import java.lang.RuntimeException;
import java.util.List;

import java.util.concurrent.CountDownLatch;


public class DiscoveryCoRSP
    extends  CoResourceSP
    // implements ICoResourceSP
{
    public CountDownLatch finished; // to control UI in OCFTestClient
    public CountDownLatch latch() {
	finished = new CountDownLatch(1);
	return finished;
    }

    public int cbdata = 99;  // callback param data

    public DiscoveryCoRSP() {
	super();
	setMethod(Method.GET);
    }
    public DiscoveryCoRSP(String uri) {
	super();
	setMethod(Method.GET);
	setUri(uri);
    }

    /**
       By the time `coReact` is called, the CoRSPs matching the incoming
       SPs will have been registered with the ServiceManager.  All
       this CoRSP needs to do is coReact.
     */
    // FIXME: call this Observe(Behavior beh)? No, the lower layer observes incoming, passes to coReact method
    @Override
    public int coReact(InboundStimulus resp)
    {
	System.out.println("DiscoveryCoRSP: DiscoveryCoRSP.coReact ENTRY");
	System.out.println("DiscoveryCoRSP: cbdata: " + cbdata);

	Logger.logInboundResponse(this);

	OCFClientSP.retain(resp);

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
	// 	System.out.println("\tOBSERVED: " + observationRecord.getUriPath());
	// 	List<IObservationRecord> kids = observationRecord.getChildren();
	// 	if (kids != null) {
	// 	    for (IObservationRecord childObservationRecord : kids) {
	// 		System.out.println("\t->OBSERVED: " + childObservationRecord.getUriPath());

	// 		GenericCoRSP cosp = new GenericCoRSP(observationIn, (ObservationRecord)childObservationRecord);
	// 		ServiceManager.registerCoResourceSP(cosp);

	// 		if (childObservationRecord.getUriPath().equals("/a/temperature")) {
	// 		    System.out.println("LOG: found temperature resource");
	// 		    // gRemoteResourceAddress = observationIn.getRemoteDeviceAddress();
	// 		    // gRemoteResourceAddress.port
	// 		    // 	= ((Integer)childObservation.getProperties().get("port"))
	// 		    // 	.intValue();
	// 		    // Logger.logDeviceAddress(gRemoteResourceAddress);
	// 		}

	// 		if (childObservationRecord.getUriPath().equals("/a/led")) {
	// 		    System.out.println("LOG: found LED resource");
	// 		    // gLEDAddress = observationIn.getRemoteDeviceAddress();
	// 		    // gLEDAddress.port
	// 		    // 	= ((Integer)childObservationRecord.getProperties().get("port"))
	// 		    // 	.intValue();
	// 		    // Logger.logDeviceAddress(gLEDAddress);
	// 		}

	// 		if (childObservationRecord.getUriPath().equals("/a/whatsit")) {
	// 		    System.out.println("LOG: found whatsit resource");
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

	return 3; // (OC_STACK_KEEP_TRANSACTION | OC_STACK_KEEP_PAYLOAD);
    }
}
