package org.iochibity.test.client;

import org.iochibity.CoServiceProvider;
import org.iochibity.constants.Method;
import org.iochibity.ObservationIn;
import org.iochibity.ObservationRecord;
import org.iochibity.ObservationList;
import org.iochibity.constants.OCStackResult;

import org.iochibity.test.Logger;

import java.util.List;

public class GenericCoSP
    extends  CoServiceProvider
    // implements ICoServiceProvider
{
    private int cbdata = 99;

    public GenericCoSP() {
	super();
	method(Method.GET);
    }
    public GenericCoSP(String uri) {
	super();
	method(Method.GET);
	uriPath(uri);
    }

    // we need info from both response msg and observation payload to create a unicast request
    public GenericCoSP(ObservationIn observationIn, ObservationRecord observationRecord) {
	super();
	System.out.println("GenericCoSP CTOR");
	System.out.println("Uri path: " + observationIn.getUriPath());
	System.out.println("Remote Device Address: "
			   + observationIn.getRemoteDeviceAddress().ipAddress());
	System.out.println("Remote Device Port:    "
			   + observationIn.getRemoteDeviceAddress().port());

	// we don't need the following setDestination anymore since
	// the incoming response rec is already stored in a TLS var,
	// making the remote DevAddr available.
	// setDestination(observationIn.getRemoteDeviceAddress());
	uriPath(observationRecord.getUriPath());

	// method and qos to be set by user before sending

	// FIXME: deal with header options

	// FIXME: explode OCConnectivityType into network protocol,
	// policies, and scope, and transport security flag
	int connectivityType = observationIn.connType;
	// setNetworkPolicies(connectivityType);

	// types, interfaces, properties?
    }

    // public int observeBehavior(ObservationIn responseIn)
    public void coreact()
    {
	System.out.println("JAVA: GenericCoSP.coreact ENTRY");
	System.out.println("JAVA: cbdata: " + cbdata);
	// Logger.logObservationIn(responseIn);
	// Logger.logObservationIn(this);

	// save incoming resource info - ServiceManager.registerRemoteResource(...)?
	// update screen ...

	return;
    }
}

