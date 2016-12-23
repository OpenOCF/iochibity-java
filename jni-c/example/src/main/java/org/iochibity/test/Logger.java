package org.iochibity.test;

import org.iochibity.OCF;
import org.iochibity.Message;
import org.iochibity.DeviceAddress;
import org.iochibity.HeaderOption;
import org.iochibity.StimulusIn;
import org.iochibity.ObservationIn;
import org.iochibity.ObservationOut;
import org.iochibity.IObservation;
import org.iochibity.Observation;
import org.iochibity.ObservationList;
import org.iochibity.PayloadForPlatform;
// import org.iochibity.PayloadForResourceState;
import org.iochibity.PropertyMap;
import org.iochibity.PropertyString;
// import org.iochibity.Resource;
// import org.iochibity.ResourceLocal;
import org.iochibity.IServiceProvider;
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
import java.util.HashMap;
import java.util.List;
import java.util.LinkedList;
import java.util.Map;

public class Logger
{
    public static final HashMap errcodeMap;
    public static final HashMap observationTypes;
    public static final HashMap netProtocols;
    public static final HashMap netPolicy;

    static {
	errcodeMap = new HashMap<Integer, String>();
        errcodeMap.put(0, "OK");
	errcodeMap.put(1, "RESOURCE_CREATED");
	errcodeMap.put(2, "RESOURCE_DELETED");
	errcodeMap.put(3, "CONTINUE");
	errcodeMap.put(4, "RESOURCE_CHANGED");
	errcodeMap.put(26, "INVALID_PARAM");
	errcodeMap.put(33, "RESOURCE NOT FOUND");
	errcodeMap.put(40, "INVALID_OPTION");
	errcodeMap.put(46, "UNAUTHORIZED_REQ");
	errcodeMap.put(52, "OBSERVER REGISTRATION FAILURE");
	errcodeMap.put(53, "RESOURCE UNOBSERVABLE");
	errcodeMap.put(54, "METHOD NOT ALLOWED");
	errcodeMap.put(255, "ERROR");
	errcodeMap.put(402, "CA_BAD_OPT");

	observationTypes = new HashMap<Integer, String>();
	observationTypes.put(Observation.PLATFORM, "PLATFORM");
	observationTypes.put(Observation.DEVICE, "DEVICE");
	observationTypes.put(Observation.REPRESENTATION, "REPRESENTATION");
	observationTypes.put(Observation.DISCOVERY, "DISCOVERY");
	observationTypes.put(Observation.SECURITY, "SECURITY");
	observationTypes.put(Observation.PRESENCE, "PRESENCE");
	observationTypes.put(Observation.RD, "RD");
	observationTypes.put(Observation.NOTIFICATION, "NOTIFICATION");

	netProtocols = new HashMap<Integer, String>();
	netProtocols.put(0, "OC_DEFAULT_ADAPTER");
	netProtocols.put(1, "OC_ADAPTER_IP");	     // = (1 << 0),
	netProtocols.put(2, "OC_ADAPTER_GATT_BTLE"); // = (1 << 1),
	netProtocols.put(3, "OC_ADAPTER_RFCOMM_BTEDR"); // = (1 << 2),
	/**Remote Access over XMPP.*/
	netProtocols.put(4, "OC_ADAPTER_REMOTE_ACCESS"); // = (1 << 3),
	/** CoAP over TCP.*/
	netProtocols.put(5, "OC_ADAPTER_TCP"); // = (1 << 4),
	netProtocols.put(6, "OC_ADAPTER_NFC"); // = (1 << 5)

	netPolicy = new HashMap<Integer, String>();
	netPolicy.put( 0, "OC_DEFAULT_FLAGS"); // = 0,
	netPolicy.put( 0x10, "OC_FLAG_SECURE"); // (1 << 4)
	netPolicy.put( 0x20, "OC_IP_USE_V6");	// (1 << 5)
	netPolicy.put( 0x40, "OC_IP_USE_V4");	// (1 << 6)
	netPolicy.put( 0x80, "OC_MULTICAST");	// (1 << 7)
	netPolicy.put( 0x1,      "OC_SCOPE_INTERFACE");
	netPolicy.put( 0x2,      "OC_SCOPE_LINK");
	netPolicy.put( 0x3,      "OC_SCOPE_REALM");
	netPolicy.put( 0x4,      "OC_SCOPE_ADMIN");
	netPolicy.put( 0x5,      "OC_SCOPE_SITE");
	netPolicy.put( 0x8,      "OC_SCOPE_ORG");
	netPolicy.put( 0xE,      "OC_SCOPE_GLOBAL");
    }

    static public void logDeviceAddress(DeviceAddress da)
    {

	try {
	    System.out.println("LOG DeviceAddress\t address:\t" + da.address);
	    System.out.println("LOG DeviceAddress\t port:\t\t" + da.port);
	    System.out.println("LOG DeviceAddress\t network protocol:\t"
			       + String.format("0x%04X", da.networkProtocol & 0xFFFFF)
			       + " " + netProtocols.get(da.networkProtocol));

	    System.out.println("LOG DeviceAddress\t nework policies:\t\t"
			       + String.format("0x%04X", da.networkPolicies));

	    System.out.println("LOG DeviceAddress\t nework scope:\t\t"
			       + String.format("0x%04X", da.networkScope));

	    System.out.println("LOG DeviceAddress\t transport security:\t\t" + da.transportSecurity);

	    // int scope = (da.networkPolicy >> 4) & 0x000F;
	    // System.out.println("LOG DeviceAddress\t nework scope:\t\t"
	    // 		       // + String.format("0x%02X", scope)
	    // 		       + netPolicy.get(scope));

	    // String sec = (0 == (da.networkPolicy & 0x0010))? "OFF" : "ON";
	    // System.out.println("LOG DeviceAddress\t transport security:\t" + sec);

	    System.out.println("LOG DeviceAddress\t ifindex:\t" + da.ifindex);
	    // System.out.println("REQUEST IN: devaddr route data: " + da.routeData);


	    // System.out.println("LOG DeviceAddress route data: " + da.routeData);
	} catch (NullPointerException e) {
	    System.out.println("Device Address is NULL");
	}
    }

    // static public void logResourcePolicies(ResourceLocal resource)
    // {
    // 	System.out.println("RESOURCE: policies: "
    // 			   + String.format("0x%X", resource.policies & 0xFFFFF));
    // 	if ( (resource.policies & ResourcePolicy.DISCOVERABLE) > 0 )
    // 	    {System.out.println("\tDISCOVERABLE");}
    // 	if ( (resource.policies & ResourcePolicy.ACTIVE) > 0 ) {System.out.println("\tACTIVE");}
    // 	if ( (resource.policies & ResourcePolicy.OBSERVABLE) > 0) {System.out.println("\tOBSERVABLE");}
    // 	if ( (resource.policies & ResourcePolicy.SECURE) > 0) {System.out.println("\tSECURE");}
    // }

    static public void logSP(IServiceProvider theSP)
    {
	System.out.println("SP URI PATH: " + theSP.getUriPath());
	List<String> ts = theSP.getTypes();
	System.out.println("SP Types:");
	ts.forEach(typ -> System.out.println("\t" + typ));

	List<String> ifs = theSP.getInterfaces();
	System.out.println("SP Interfaces");
	ifs.forEach(iface -> System.out.println("\t" + iface));
    }

    // static public void logResource(ResourceLocal resource)
    // {
    // 	System.out.println("RESOURCE: logResource ENTRY");
    // 	System.out.println("RESOURCE: resource uri: " + resource.getUri());

    // 	List<String> tll = resource.getTypes();
    // 	tll.forEach(t -> System.out.println("RESOURCE: type:     " + t)); // Java 8

    // 	List<String> ifll = resource.getInterfaces();
    // 	ifll.forEach(iface -> System.out.println("RESOURCE: interface: " + iface));
    // 	// for (int i = 0; i < ifll.size(); i++) {   // Java 7
    // 	//     System.out.println("REQUEST IN: resource if:    " + ifll.get(i));
    // 	// }

    // 	List<PropertyString> pll = resource.getProperties();
    // 	System.out.println("RESOURCE: properties count: " + pll.size());
    // 	pll.forEach(p -> System.out.println("RESOURCE: property: " + p.name + " = " + p.value));

    // 	List<ResourceLocal> cll = resource.getChildren();
    // 	System.out.println("RESOURCE: child resources count: " + cll.size());
    // 	cll.forEach(ch -> System.out.println("RESOURCE: child resource: " + ch));

    // 	System.out.println("RESOURCE: service provider (callback): "
    // 			   + resource.getServiceProvider().getClass().getName());

    // 	System.out.println("RESOURCE: callback param: "
    // 			   + resource.getCallbackParam().getClass().getName());

    // 	System.out.println("RESOURCE: serial number: " + resource.sn);

    // 	// Instance Id
    // 	// System.out.println("RESOURCE: resource instance id: " + resource.id.getClass().getName());
    // 	if (resource.id.getClass().getName().equals("org.iochibity.Resource$InstanceId")) {
    // 	    System.out.println("RESOURCE: resource InstanceId class: InstanceId");
    // 	} else if (resource.id.getClass().getName().equals("org.iochibity.Resource$InstanceOrdinal")) {
    // 	    System.out.println("RESOURCE: resource InstanceId class: InstanceOrdinal, val="
    // 			       + ((org.iochibity.ResourceLocal.InstanceOrdinal)resource.id).val);
    // 	} else if (resource.id.getClass().getName().equals("org.iochibity.ResourceLocal$InstanceString")) {
    // 	    System.out.println("RESOURCE: resource InstanceId class: InstanceString, val="
    // 			       + ((org.iochibity.ResourceLocal.InstanceString)resource.id).val);
    // 	} else if (resource.id.getClass().getName().equals("org.iochibity.ResourceLocal$InstanceUuid")) {
    // 	    System.out.println("RESOURCE: resource InstanceId class: InstanceUuid, val="
    // 			       + ((org.iochibity.ResourceLocal.InstanceUuid)resource.id).val);
    // 	}

    // 	logResourcePolicies(resource);

    // 	try {
    // 	    System.out.println("RESOURCE: action set: " + resource.getActionSet());
    // 	} catch (OCFNotImplementedException e) {
    // 	    System.out.println("ERROR**** RESOURCE: getActionSet not implemented.");
    // 	}
    // 	System.out.println("RESOURCE: logResource EXIT");
    // }

    static public void logObservation(Observation observation)
    {
	// System.out.println("OBSERVATION: logObservation ENTRY");

	System.out.println("\tOBSERVED uri: " + observation.getUriPath());

	System.out.println("\tOBSERVED type: " + observation.getType());

	// log rtypes
	List<String> rtypes = observation.getResourceTypes();
	System.out.println("\tOBSERVED RESOURCE TYPES count: " + rtypes.size());
	for (String t : (List<String>)rtypes) {
	    System.out.println("\tOBSERVED rtype: " + t);
	}

	// log interfaces
	List<String> ifaces = observation.getInterfaces();
	System.out.println("\tOBSERVED INTERFACES count: " + ifaces.size());
	for (String iface : ifaces) {
	    System.out.println("\tOBSERVED interface: " + iface);
	}

	// log properties (PlatformInfo, DeviceInfo, or "values" for resources)
	PropertyMap<String, Object> pmap = observation.getProperties();
	System.out.println("\tOBSERVED PROPERTIES count: " + pmap.size());
	for (Map.Entry<String, Object> entry : pmap.entrySet())
	    {
		System.out.println("\tOBSERVED property: "
				   + entry.getKey()
				   + " = "
				   + entry.getValue());
	    }
	List<IObservation> kids = observation.getChildren();
	if (kids != null) {
	    System.out.println("\tOBSERVED CHILDREN count: " + kids.size());
	    for (IObservation p : kids) {
		System.out.println("================ CHILD");
		logObservation((Observation)p);
	    }
	}
    }

    static public void logRequestIn(StimulusIn requestIn)
    {
	System.out.println("LOG StimulusIn logRequestIn ENTRY");
	// System.out.println("LOG StimulusIn this handle: " + requestIn.localHandle);
	// System.out.println("LOG StimulusIn remote handle: " + requestIn.getRemoteHandle);
	// System.out.println("LOG StimulusIn resource handle: " + requestIn.getResourceHandle());
	System.out.println("LOG StimulusIn request method: " + requestIn.getMethod());
	System.out.println("LOG StimulusIn query : \"" + requestIn.getQueryString() + "\"");
	System.out.println("LOG StimulusIn msg id : " + requestIn.getMessageId());

	System.out.println("LOG StimulusIn method : " + requestIn.getMethod());
	System.out.println("LOG StimulusIn watch action : " + requestIn.watchAction);
	System.out.println("LOG StimulusIn watch id :     " + requestIn.watchId);


	// ResourceLocal resource = requestIn.getResource();
	// logResource(resource);

	// logDeviceAddress(requestIn.getRemoteDeviceAddress());

	// System.out.println("LOG StimulusIn watch action: " + requestIn.watchAction);
	// System.out.println("LOG StimulusIn watch id    : " + requestIn.watchId);

	List<HeaderOption> headerOptions = requestIn.getOptions();
	if (headerOptions != null)
	    System.out.println("LOG StimulusIn header opts ct: " + headerOptions.size());

	// ObservationList<PayloadForResourceState> observation = requestIn.getPDUPayload();
	// if (observation == null) {
	//     System.out.println("LOG StimulusIn observation is null");
	// }
    }

    static public void logObservationIn(ObservationIn observationIn)
    {
	System.out.println("LOG ObservationIn STACK RESULT:\t"
			   + observationIn.result
			   + ": "
			   + errcodeMap.get(observationIn.result));

	System.out.println("LOG ObservationIn uri path:\t" + observationIn.getUriPath());
	System.out.println("LOG ObservationIn conn type:\t" + observationIn.connType);
	System.out.println("LOG ObservationIn sec ID:\t" + observationIn.secID);
	System.out.println("LOG ObservationIn stack result:\t" + observationIn.result);
	System.out.println("LOG ObservationIn serial:\t" + observationIn.serial);

	System.out.println("LOG ObservationIn REMOTE DEVICE ADDRESS:");
	logDeviceAddress(observationIn.getRemoteDeviceAddress());

	List<HeaderOption> headerOptions = observationIn.getOptions();
	if (headerOptions != null)
	    System.out.println("LOG StimulusIn header options ct: " + headerOptions.size());

	// FIXME:
	if (observationIn.result == OCStackResult.OK) {

	    System.out.println("LOG ObservationIn OBSERVATIONS:");
	    // System.out.println("LOG ObservationIn OBSERVATION type: "
	    // 		       + observationIn.getObservationType()
	    // 		       + ": "
	    // 		       + observationTypes.get(observationIn.getObservationType()));

	    ObservationList<Observation> observations = observationIn.getObservations();
	    if (observations != null) {
		System.out.println("LOG OBSERVATION count: " + observations.size());
		for (Observation observation : (ObservationList<Observation>) observations) {
		    List<IObservation> kids = observation.getChildren();
		    if (kids != null) {
			System.out.println("LOG CHILD OBSERVATIONS count: "
					   + observation.getChildren().size());
		    }
		    // logObservation(observation);
		}
	    }
	}
    }
}
