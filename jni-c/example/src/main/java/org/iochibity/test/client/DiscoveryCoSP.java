package org.iochibity.test.client;

import org.iochibity.CoServiceProvider;
// import org.iochibity.ICoServiceProvider;
import org.iochibity.constants.Method;
import org.iochibity.ObservationIn;
import org.iochibity.IObservation;
import org.iochibity.Observation;
import org.iochibity.ObservationList;
import org.iochibity.ServiceManager;

import org.iochibity.constants.OCStackResult;

import org.iochibity.test.Logger;

import java.util.List;

public class DiscoveryCoSP
    extends  CoServiceProvider
    // implements ICoServiceProvider
{
    private int cbdata = 99;

    public DiscoveryCoSP() {
	super();
	setMethod(Method.GET);
    }
    public DiscoveryCoSP(String uri) {
	super();
	setMethod(Method.GET);
	setUriPath(uri);
    }

    public int observeBehavior(ObservationIn observationIn)
    {
	System.out.println("JAVA: DiscoveryCoSP.serviceResponse ENTRY");
	System.out.println("JAVA: cbdata: " + cbdata);
	Logger.logObservationIn(observationIn);

	// first update _this_ with incoming data
	this.setDestination(observationIn.getRemoteDeviceAddress());
	// etc. connType, etc.

	// then iterate over Observation payloads
	if (observationIn.result == OCStackResult.OK) {
	    ObservationList<Observation> observations = observationIn.getObservations();
	    for (Observation observation : (ObservationList<Observation>) observations) {
		System.out.println("\tOBSERVED: " + observation.getUriPath());
		List<IObservation> kids = observation.getChildren();
		if (kids != null) {
		    for (IObservation childObservation : kids) {
			System.out.println("\t->OBSERVED: " + childObservation.getUriPath());

			GenericCoSP cosp = new GenericCoSP(observationIn, (Observation)childObservation);
			ServiceManager.registerCoServiceProvider(cosp);

			if (childObservation.getUriPath().equals("/a/temperature")) {
			    System.out.println("LOG: found temperature resource");
			    // gRemoteResourceAddress = observationIn.getRemoteDeviceAddress();
			    // gRemoteResourceAddress.port
			    // 	= ((Integer)childObservation.getProperties().get("port"))
			    // 	.intValue();
			    // Logger.logDeviceAddress(gRemoteResourceAddress);
			}

			if (childObservation.getUriPath().equals("/a/led")) {
			    System.out.println("LOG: found LED resource");
			    // gLEDAddress = observationIn.getRemoteDeviceAddress();
			    // gLEDAddress.port
			    // 	= ((Integer)childObservation.getProperties().get("port"))
			    // 	.intValue();
			    // Logger.logDeviceAddress(gLEDAddress);
			}

			if (childObservation.getUriPath().equals("/a/whatsit")) {
			    System.out.println("LOG: found whatsit resource");
			    // gWhatsitAddress = observationIn.getRemoteDeviceAddress();
			    // gWhatsitAddress.port
			    // 	= ((Integer)childObservation.getProperties().get("port"))
			    // 	.intValue();
			    // Logger.logDeviceAddress(gWhatsitAddress);
			}
		    }
		}
	    }
	}

	return 0;
    }

}

