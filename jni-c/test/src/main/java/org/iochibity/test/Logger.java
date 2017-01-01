package org.iochibity.test;

import org.iochibity.OCF;
import org.iochibity.CoServiceProvider;
import org.iochibity.DeviceAddress;
import org.iochibity.HeaderOption;
import org.iochibity.Message;
import org.iochibity.ObservationIn;
import org.iochibity.ObservationOut;
import org.iochibity.IObservationRecord;
import org.iochibity.ObservationRecord;
import org.iochibity.ObservationList;
import org.iochibity.PropertyMap;
import org.iochibity.IServiceProvider;
import org.iochibity.StimulusIn;
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
    public static final HashMap netScope;

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
	observationTypes.put(ObservationRecord.PLATFORM, "PLATFORM");
	observationTypes.put(ObservationRecord.DEVICE, "DEVICE");
	observationTypes.put(ObservationRecord.REPRESENTATION, "REPRESENTATION");
	observationTypes.put(ObservationRecord.DISCOVERY, "DISCOVERY");
	observationTypes.put(ObservationRecord.SECURITY, "SECURITY");
	observationTypes.put(ObservationRecord.PRESENCE, "PRESENCE");
	observationTypes.put(ObservationRecord.RD, "RD");
	observationTypes.put(ObservationRecord.NOTIFICATION, "NOTIFICATION");

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

	netPolicy    = new HashMap<Integer, String>();
	netPolicy.put( 0, "OC_DEFAULT_FLAGS"); // = 0,
	netPolicy.put( 0x10, "OC_FLAG_SECURE"); // (1 << 4)
	netPolicy.put( 0x20, "OC_IP_USE_V6");	// (1 << 5)
	netPolicy.put( 0x40, "OC_IP_USE_V4");	// (1 << 6)
	netPolicy.put( 0x80, "OC_MULTICAST");	// (1 << 7)

	netScope    = new HashMap<Integer, String>();
	netScope.put( 0,      "OC_SCOPE_NONE");
	netScope.put( 1,      "OC_SCOPE_INTERFACE");
	netScope.put( 2,      "OC_SCOPE_LINK");
	netScope.put( 3,      "OC_SCOPE_REALM");
	netScope.put( 4,      "OC_SCOPE_ADMIN");
	netScope.put( 5,      "OC_SCOPE_SITE");
	netScope.put( 8,      "OC_SCOPE_ORG");
	netScope.put( 0xE,      "OC_SCOPE_GLOBAL");
    }

    static public void logCoAddress(CoServiceProvider cosp)
    {

	DeviceAddress da = cosp.coAddress();

	if (da != null) {
	    try {
		System.out.println("LOGGER DeviceAddress\t IP address:\t\t" + da.ipAddress());
		System.out.println("LOGGER DeviceAddress\t port:\t\t\t" + da.port());
		System.out.println("LOGGER DeviceAddress\t network protocol:\t"
				   + String.format("0x%04X", da.networkProtocol()
						   & 0xFFFFF)
				   + " " + netProtocols.get(da.networkProtocol()));

		System.out.println("LOGGER DeviceAddress\t network flags:\t\t"
				   + String.format("0x%04X", da.networkFlags()
						   & 0xFFFFF));

		System.out.println("LOGGER DeviceAddress\t IPv4?\t\t\t" + da.isIPv4());

		System.out.println("LOGGER DeviceAddress\t IPv6?\t\t\t" + da.isIPv6());

		System.out.println("LOGGER DeviceAddress\t Multicast?\t\t" + da.isMulticast());

		System.out.println("LOGGER DeviceAddress\t nework scope:\t\t"
				   + String.format("0x%02X", da.networkScope())
				   + " " + netScope.get((int)da.networkScope()));

		System.out.println("LOGGER DeviceAddress\t transport security:\t"
				   + da.transportIsSecure());

		// int scope = (da.networkPolicy >> 4) & 0x000F;
		// System.out.println("LOGGER DeviceAddress\t nework scope:\t\t"
		// 		       // + String.format("0x%02X", scope)
		// 		       + netPolicy.get(scope));

		// String sec = (0 == (da.networkPolicy & 0x0010))? "OFF" : "ON";
		// System.out.println("LOGGER DeviceAddress\t transport security:\t" + sec);

		System.out.println("LOGGER DeviceAddress\t ifindex:\t\t" + da.ifindex());
		// System.out.println("REQUEST IN: devaddr route data: " + da.routeData);


		// System.out.println("LOGGER DeviceAddress route data: " + da.routeData);
	    } catch (NullPointerException e) {
		System.out.println("Device Address is NULL");
	    }
	} else {
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

    static public void logNetworking(CoServiceProvider cosp)
    {
	System.out.println("LOGGER transportIsUDP?\t\t" + cosp.transportIsUDP());
	System.out.println("LOGGER transportIsTCP?\t\t" + cosp.transportIsTCP());
	System.out.println("LOGGER transportIsGATT?\t\t" + cosp.transportIsGATT());
	System.out.println("LOGGER transportIsRFCOMM?\t" + cosp.transportIsRFCOMM());
	System.out.println("LOGGER transportIsNFC?\t\t" + cosp.transportIsNFC());
	System.out.println("LOGGER networkIsIP?\t\t" + cosp.networkIsIP());
	System.out.println("LOGGER networkIsIPv4?\t\t" + cosp.networkIsIPv4());
	System.out.println("LOGGER networkIsIPv6?\t\t" + cosp.networkIsIPv6());
	System.out.println("LOGGER scopeIsInterface?\t" + cosp.scopeIsInterface());
	System.out.println("LOGGER scopeIsLink?\t\t" + cosp.scopeIsLink());
	System.out.println("LOGGER transportIsSecure?\t" + cosp.transportIsSecure());
	System.out.println("LOGGER routingIsMulticast?\t" + cosp.routingIsMulticast());
   }

    static public void testNetworking(CoServiceProvider cosp)
    {
	boolean torf;
	torf = cosp.transportIsUDP();
	System.out.println("PRE  transportIsUDP? " + cosp.transportIsUDP());
	cosp.transportIsUDP(true);
	System.out.println("POST transportIsUDP? (t) " + cosp.transportIsUDP());
	cosp.transportIsUDP(false);
	System.out.println("POST transportIsUDP? (f) " + cosp.transportIsUDP());
	cosp.transportIsUDP(torf);

	torf = cosp.transportIsTCP();
	System.out.println("PRE transportIsTCP? " + cosp.transportIsTCP());
	cosp.transportIsTCP(true);
	System.out.println("POST transportIsTCP? (t) " + cosp.transportIsTCP());
	cosp.transportIsTCP(false);
	System.out.println("POST transportIsTCP? (f) " + cosp.transportIsTCP());
	cosp.transportIsTCP(torf);

	torf = cosp.transportIsGATT();
	System.out.println("PRE transportIsGATT? " + cosp.transportIsGATT());
	cosp.transportIsGATT(true);
	System.out.println("POST transportIsGATT? (t) " + cosp.transportIsGATT());
	cosp.transportIsGATT(false);
	System.out.println("POST transportIsGATT? (f) " + cosp.transportIsGATT());
	cosp.transportIsGATT(torf);

	torf = cosp.transportIsRFCOMM();
	System.out.println("PRE transportIsRFCOMM? " + cosp.transportIsRFCOMM());
	cosp.transportIsRFCOMM(true);
	System.out.println("POST transportIsRFCOMM? (t) " + cosp.transportIsRFCOMM());
	cosp.transportIsRFCOMM(false);
	System.out.println("POST transportIsRFCOMM? (f) " + cosp.transportIsRFCOMM());
	cosp.transportIsRFCOMM(torf);

	torf = cosp.transportIsNFC();
	System.out.println("PRE transportIsNFC? " + cosp.transportIsNFC());
	cosp.transportIsNFC(true);
	System.out.println("POST transportIsNFC? (t) " + cosp.transportIsNFC());
	cosp.transportIsNFC(false);
	System.out.println("POST transportIsNFC? (f) " + cosp.transportIsNFC());
	cosp.transportIsNFC(torf);

	System.out.println("PRE networkIsIP? " + cosp.networkIsIP());
	torf = cosp.transportIsUDP();
	cosp.transportIsUDP(true); // implies IP
	System.out.println("POST networkIsIP? (t) " + cosp.networkIsIP());
	cosp.transportIsUDP(false);
	System.out.println("POST networkIsIP? (f) " + cosp.networkIsIP());
	cosp.transportIsUDP(torf);

	torf = cosp.transportIsTCP();
	cosp.transportIsTCP(true); // implies IP
	System.out.println("POST networkIsIP? (t) " + cosp.networkIsIP());
	cosp.transportIsTCP(false);
	System.out.println("POST networkIsIP? (f) " + cosp.networkIsIP());
	cosp.transportIsTCP(torf);

	torf = cosp.networkIsIPv4();
	System.out.println("PRE networkIsIPv4? " + cosp.networkIsIPv4());
	cosp.networkIsIPv4(true);
	System.out.println("POST networkIsIPv4? (t) " + cosp.networkIsIPv4());
	cosp.networkIsIPv4(false);
	System.out.println("POST networkIsIPv4? (f) " + cosp.networkIsIPv4());
	cosp.networkIsIPv4(torf);

	torf = cosp.networkIsIPv6();
	System.out.println("PRE networkIsIPv6? " + cosp.networkIsIPv6());
	cosp.networkIsIPv6(true);
	System.out.println("POST networkIsIPv6? (t) " + cosp.networkIsIPv6());
	cosp.networkIsIPv6(false);
	System.out.println("POST networkIsIPv6? (f) " + cosp.networkIsIPv6());
	cosp.networkIsIPv6(torf);

	torf = cosp.scopeIsInterface();
	System.out.println("PRE scopeIsInterface? " + cosp.scopeIsInterface());
	cosp.scopeIsInterface(true);
	System.out.println("POST scopeIsInterface? (t) " + cosp.scopeIsInterface());
	cosp.scopeIsInterface(false);
	System.out.println("POST scopeIsInterface? (f) " + cosp.scopeIsInterface());
	cosp.scopeIsInterface(torf);

	torf = cosp.scopeIsLink();
	System.out.println("PRE scopeIsLink? " + cosp.scopeIsLink());
	cosp.scopeIsLink(true);
	System.out.println("POST scopeIsLink? (t) " + cosp.scopeIsLink());
	cosp.scopeIsLink(false);
	System.out.println("POST scopeIsLink? (f) " + cosp.scopeIsLink());
	cosp.scopeIsLink(torf);

	torf = cosp.transportIsSecure();
	System.out.println("PRE transportIsSecure? " + cosp.transportIsSecure());
	cosp.transportIsSecure(true);
	System.out.println("POST transportIsSecure? (t) " + cosp.transportIsSecure());
	cosp.transportIsSecure(false);
	System.out.println("POST transportIsSecure? (f) " + cosp.transportIsSecure());
	cosp.transportIsSecure(torf);

	torf = cosp.routingIsMulticast();
	System.out.println("PRE routingIsMulticast? " + cosp.routingIsMulticast());
	cosp.routingIsMulticast(true);
	System.out.println("POST routingIsMulticast? (t) " + cosp.routingIsMulticast());
	cosp.routingIsMulticast(false);
	System.out.println("POST routingIsMulticast? (f) " + cosp.routingIsMulticast());
	cosp.routingIsMulticast(torf);
    }

    static public void logCoSP(CoServiceProvider cosp)
    {
	// System.out.println("CoSP URI PATH: " + cosp.getUriPath());
	// logDeviceAddress(cosp.coAddress());

	// System.out.println("CoSP network protocol: "
	// 		   + String.format("0x%04X",
	// 				   cosp.networkProtocol() & 0xFFFFF));
	// System.out.println("CoSP network Scope: "    + cosp.networkScope());
	// System.out.println("CoSP network Policies: "
	// 			       + String.format("0x%04X",
	// 					       cosp.networkPolicies() & 0xFFFFF));
	// System.out.println("CoSP transport secure?: " + cosp.transportIsSecure());

	// List<String> ts = cosp.getTypes();
	// System.out.println("SP Types:");
	// ts.forEach(typ -> System.out.println("\t" + typ));

	// List<String> ifs = cosp.getInterfaces();
	// System.out.println("SP Interfaces");
	// ifs.forEach(iface -> System.out.println("\t" + iface));
    }

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

    static public void logObservation(ObservationRecord observationRecord)
    {
	// System.out.println("OBSERVATION: logObservation ENTRY");

	System.out.println("\tOBSERVED uri: " + observationRecord.getUriPath());

	System.out.println("\tOBSERVED type: " + observationRecord.getType());

	// log rtypes
	List<String> rtypes = observationRecord.getResourceTypes();
	System.out.println("\tOBSERVED RESOURCE TYPES count: " + rtypes.size());
	for (String t : (List<String>)rtypes) {
	    System.out.println("\tOBSERVED rtype: " + t);
	}

	// log interfaces
	List<String> ifaces = observationRecord.getInterfaces();
	System.out.println("\tOBSERVED INTERFACES count: " + ifaces.size());
	for (String iface : ifaces) {
	    System.out.println("\tOBSERVED interface: " + iface);
	}

	// log properties (PlatformInfo, DeviceInfo, or "values" for resources)
	PropertyMap<String, Object> pmap = observationRecord.getProperties();
	System.out.println("\tOBSERVED PROPERTIES count: " + pmap.size());
	for (Map.Entry<String, Object> entry : pmap.entrySet())
	    {
		System.out.println("\tOBSERVED property: "
				   + entry.getKey()
				   + " = "
				   + entry.getValue());
	    }
	List<IObservationRecord> kids = observationRecord.getChildren();
	if (kids != null) {
	    System.out.println("\tOBSERVED CHILDREN count: " + kids.size());
	    for (IObservationRecord p : kids) {
		System.out.println("================ CHILD");
		logObservation((ObservationRecord)p);
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

    static public void logRequestIn(CoServiceProvider cosp)
    {
	System.out.println("LOGGER logRequestIn ENTRY, thread "
			   + Thread.currentThread().getId());

	System.out.println("LOGGER: stack result: " + cosp.getCoResult());

	System.out.println("LOGGER CoSP uri path:\t" + cosp.uriPath());
	System.out.println("LOGGER CoSP method:\t" + cosp.method());
	// System.out.println("LOGGER CoSP conn type:\t" + cosp.connType());
	System.out.println("LOGGER CoSP sec ID:\t" + cosp.getCoSecurityId());
	System.out.println("LOGGER CoSP serial:\t" + cosp.getNotificationSerial());

	System.out.println("LOGGING CO-ADDRESS:");
	logCoAddress(cosp);

	// List<HeaderOption> headerOptions = cosp.getOptions();
	// if (headerOptions != null)
	//     System.out.println("LOG StimulusIn header options ct: " + headerOptions.size());

	// FIXME:
	// if (cosp.result == OCStackResult.OK) {

	//     System.out.println("LOGGER CoSP OBSERVATIONS:");
	//     // System.out.println("LOGGER CoSP OBSERVATION type: "
	//     // 		       + cosp.getObservationType()
	//     // 		       + ": "
	//     // 		       + observationTypes.get(cosp.getObservationType()));

	//     ObservationList<ObservationRecord> observationRecords = cosp.getObservationRecords();
	//     if (observationRecords != null) {
	// 	System.out.println("LOG OBSERVATIONRECORD count: " + observationRecords.size());
	// 	for (ObservationRecord observationRecord : (ObservationList<ObservationRecord>) observationRecords) {
	// 	    List<IObservationRecord> kids = observationRecord.getChildren();
	// 	    if (kids != null) {
	// 		System.out.println("LOG CHILD OBSERVATIONS count: "
	// 				   + observationRecord.getChildren().size());
	// 	    }
	// 	    // logObservation(observation);
	// 	}
	//     }
	// }
    }

    static public void logRequestOut(CoServiceProvider cosp)
    {
	System.out.println("LOGGER logRequestOut ENTRY, thread "
			   + Thread.currentThread().getId());

	System.out.println("LOGGER CoSP uri path:\t" + cosp.uriPath());
	System.out.println("LOGGER CoSP method:\t" + cosp.method());
	// System.out.println("LOGGER CoSP conn type:\t" + cosp.connType());

	// List<HeaderOption> headerOptions = cosp.getOptions();
	// if (headerOptions != null)
	//     System.out.println("LOG StimulusIn header options ct: " + headerOptions.size());

	// FIXME:
	// if (cosp.result == OCStackResult.OK) {

	//     System.out.println("LOGGER CoSP OBSERVATIONS:");
	//     // System.out.println("LOGGER CoSP OBSERVATION type: "
	//     // 		       + cosp.getObservationType()
	//     // 		       + ": "
	//     // 		       + observationTypes.get(cosp.getObservationType()));

	//     ObservationList<ObservationRecord> observationRecords = cosp.getObservationRecords();
	//     if (observationRecords != null) {
	// 	System.out.println("LOG OBSERVATIONRECORD count: " + observationRecords.size());
	// 	for (ObservationRecord observationRecord : (ObservationList<ObservationRecord>) observationRecords) {
	// 	    List<IObservationRecord> kids = observationRecord.getChildren();
	// 	    if (kids != null) {
	// 		System.out.println("LOG CHILD OBSERVATIONS count: "
	// 				   + observationRecord.getChildren().size());
	// 	    }
	// 	    // logObservation(observation);
	// 	}
	//     }
	// }
    }

    static public void logResponseIn(CoServiceProvider cosp)
    {
	System.out.println("LOGGER logResponseIn ENTRY, thread "
			   + Thread.currentThread().getId());

	System.out.println("LOGGER stack result: " + cosp.getCoResult());

	System.out.println("LOGGER CoSP uri path:\t" + cosp.uriPath());
	System.out.println("LOGGER CoSP method:\t" + cosp.method());
	// System.out.println("LOGGER CoSP conn type:\t" + cosp.connType());
	System.out.println("LOGGER CoSP sec ID:\t" + cosp.getCoSecurityId());
	System.out.println("LOGGER CoSP serial:\t" + cosp.getNotificationSerial());

	System.out.println("LOGGING CO-ADDRESS:");
	logCoAddress(cosp);

	// List<HeaderOption> headerOptions = cosp.getOptions();
	// if (headerOptions != null)
	//     System.out.println("LOG StimulusIn header options ct: " + headerOptions.size());

	// FIXME:
	// if (cosp.result == OCStackResult.OK) {

	//     System.out.println("LOGGER CoSP OBSERVATIONS:");
	//     // System.out.println("LOGGER CoSP OBSERVATION type: "
	//     // 		       + cosp.getObservationType()
	//     // 		       + ": "
	//     // 		       + observationTypes.get(cosp.getObservationType()));

	//     ObservationList<ObservationRecord> observationRecords = cosp.getObservationRecords();
	//     if (observationRecords != null) {
	// 	System.out.println("LOG OBSERVATIONRECORD count: " + observationRecords.size());
	// 	for (ObservationRecord observationRecord : (ObservationList<ObservationRecord>) observationRecords) {
	// 	    List<IObservationRecord> kids = observationRecord.getChildren();
	// 	    if (kids != null) {
	// 		System.out.println("LOG CHILD OBSERVATIONS count: "
	// 				   + observationRecord.getChildren().size());
	// 	    }
	// 	    // logObservation(observation);
	// 	}
	//     }
	// }
    }
}
