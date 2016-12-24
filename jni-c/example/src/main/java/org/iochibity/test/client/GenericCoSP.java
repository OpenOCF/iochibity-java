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
    extends    CoServiceProvider
    // implements ICoServiceProvider
{
    private int cbdata = 99;

    public GenericCoSP() {
	super();
	setMethod(Method.GET);
    }
    public GenericCoSP(String uri) {
	super();
	setMethod(Method.GET);
	setUriPath(uri);
    }

    // we need info from both response msg and observation payload to create a unicast request
    public GenericCoSP(ObservationIn observationIn, ObservationRecord observationRecord) {
	super();
	System.out.println("GenericCoSP CTOR");
	System.out.println("Uri path: " + observationIn.getUriPath());
	System.out.println("Remote Device Address: " + observationIn.getRemoteDeviceAddress().address);
	System.out.println("Remote Device Port:    " + observationIn.getRemoteDeviceAddress().port);
	setDestination(observationIn.getRemoteDeviceAddress());
	setUriPath(observationRecord.getUriPath());

	// method and qos to be set by user before sending

	// FIXME: deal with header options

	// FIXME: explode OCConnectivityType into network protocol,
	// policies, and scope, and transport security flag
	int connectivityType = observationIn.connType;
	setNetworkPolicies(connectivityType);

	// types, interfaces, properties?
    }

    public int observeBehavior(ObservationIn responseIn)
    {
	System.out.println("JAVA: PlatformCoSP.serviceResponse ENTRY");
	System.out.println("JAVA: cbdata: " + cbdata);
	Logger.logObservationIn(responseIn);

	// save incoming resource info - ServiceManager.registerRemoteResource(...)?
	// update screen ...

	return 0;
    }
}

