package org.openocf.test;

// import openocf.OCF;
import openocf.app.CoResourceSP;
import openocf.utils.Endpoint;
import openocf.utils.CoAPOption;
// import openocf.Message;
import openocf.behavior.InboundResponse;
import openocf.behavior.OutboundStimulus;
// import openocf.ObservationOut;
// import openocf.IObservationRecord;
import openocf.behavior.ObservationRecord;
// import openocf.ObservationList;
/// import openocf.PropertyMap;
import openocf.app.IResourceSP;
// import openocf.StimulusIn;
import openocf.constants.OCStackResult;
import openocf.constants.ResourcePolicy;
import openocf.constants.ServiceResult;
import openocf.exceptions.OCFNotImplementedException;

import openocf.utils.Endpoint;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.LinkedList;
import java.util.Map;

import android.util.Log;

public class Logger
{
    private final static String TAG = "Logger";

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

    static public void logCoAddress(Endpoint ep)
    {

	// Endpoint da = ep.coAddress();

	if (ep != null) {
	    try {
		Log.i(TAG, "APP_LOGGER Endpoint\t IP address:\t\t" + ep.getIPAddress());
		Log.i(TAG, "APP_LOGGER Endpoint\t port:\t\t\t" + ep.getPort());
		// Log.i(TAG, "APP_LOGGER Endpoint\t network protocol:\t"
		// 		   + String.format("0x%04X", ep.networkProtocol()
		// 				   & 0xFFFFF)
		// 		   + " " + netProtocols.get(ep.networkProtocol()));

		// Log.i(TAG, "APP_LOGGER Endpoint\t network flags:\t\t"
		// 		   + String.format("0x%04X", ep.networkFlags()
		// 				   & 0xFFFFF));

		Log.i(TAG, "APP_LOGGER Endpoint\t IPv4?\t\t\t" + ep.isNetworkIPv4());

		Log.i(TAG, "APP_LOGGER Endpoint\t IPv6?\t\t\t" + ep.isNetworkIPv6());

		Log.i(TAG, "APP_LOGGER Endpoint\t Multicast?\t\t" + ep.isRoutingMulticast());

		// Log.i(TAG, "APP_LOGGER Endpoint\t nework scope:\t\t"
		// 		   + String.format("0x%02X", ep.networkScope())
		// 		   + " " + netScope.get((int)ep.networkScope()));

		Log.i(TAG, "APP_LOGGER Endpoint\t transport security:\t"
				   + ep.isTransportSecure());

		// int scope = (ep.networkPolicy >> 4) & 0x000F;
		// Log.i(TAG, "APP_LOGGER Endpoint\t nework scope:\t\t"
		// 		       // + String.format("0x%02X", scope)
		// 		       + netPolicy.get(scope));

		// String sec = (0 == (ep.networkPolicy & 0x0010))? "OFF" : "ON";
		// Log.i(TAG, "APP_LOGGER Endpoint\t transport security:\t" + sec);

		// Log.i(TAG, "APP_LOGGER Endpoint\t ifindex:\t\t" + ep.ifindex());
		// Log.i(TAG, "REQUEST IN: devaddr route data: " + ep.routeData);


		// Log.i(TAG, "APP_LOGGER Endpoint route data: " + ep.routeData);
	    } catch (NullPointerException e) {
		Log.i(TAG, "Device Address is NULL");
	    }
	} else {
	    Log.i(TAG, "Device Address is NULL");
	}
    }

    // static public void logResourcePolicies(ResourceLocal resource)
    // {
    // 	Log.i(TAG, "RESOURCE: policies: "
    // 			   + String.format("0x%X", resource.policies & 0xFFFFF));
    // 	if ( (resource.policies & ResourcePolicy.DISCOVERABLE) > 0 )
    // 	    {Log.i(TAG, "\tDISCOVERABLE");}
    // 	if ( (resource.policies & ResourcePolicy.ACTIVE) > 0 ) {Log.i(TAG, "\tACTIVE");}
    // 	if ( (resource.policies & ResourcePolicy.OBSERVABLE) > 0) {Log.i(TAG, "\tOBSERVABLE");}
    // 	if ( (resource.policies & ResourcePolicy.SECURE) > 0) {Log.i(TAG, "\tSECURE");}
    // }

    static public void logNetworking(Endpoint ep)
    {
	Log.i(TAG, "APP_LOGGER isTransportUDP?\t\t" + ep.isTransportUDP());
	Log.i(TAG, "APP_LOGGER isTransportTCP?\t\t" + ep.isTransportTCP());
	Log.i(TAG, "APP_LOGGER isTransportGATT?\t\t" + ep.isTransportGATT());
	Log.i(TAG, "APP_LOGGER isTransportRFCOMM?\t" + ep.isTransportRFCOMM());
	Log.i(TAG, "APP_LOGGER isTransportNFC?\t\t" + ep.isTransportNFC());
	Log.i(TAG, "APP_LOGGER networkIsIP?\t\t" + ep.isNetworkIP());
	Log.i(TAG, "APP_LOGGER networkIsIPv4?\t\t" + ep.isNetworkIPv4());
	Log.i(TAG, "APP_LOGGER networkIsIPv6?\t\t" + ep.isNetworkIPv6());
	Log.i(TAG, "APP_LOGGER scopeIsInterface?\t" + ep.isScopeInterface());
	Log.i(TAG, "APP_LOGGER scopeIsLink?\t\t" + ep.isScopeLink());
	Log.i(TAG, "APP_LOGGER isTransportSecure?\t" + ep.isTransportSecure());
	Log.i(TAG, "APP_LOGGER routingIsMulticast?\t" + ep.isRoutingMulticast());
   }

    static public void testNetworking(Endpoint ep)
    {
	boolean torf;
	torf = ep.isTransportUDP();
	Log.i(TAG, "PRE  isTransportUDP? " + ep.isTransportUDP());
	ep.setTransportUDP(true);
	Log.i(TAG, "POST isTransportUDP? (t) " + ep.isTransportUDP());
	ep.setTransportUDP(false);
	Log.i(TAG, "POST isTransportUDP? (f) " + ep.isTransportUDP());
	ep.setTransportUDP(torf);

	torf = ep.isTransportTCP();
	Log.i(TAG, "PRE isTransportTCP? " + ep.isTransportTCP());
	ep.setTransportTCP(true);
	Log.i(TAG, "POST isTransportTCP? (t) " + ep.isTransportTCP());
	ep.setTransportTCP(false);
	Log.i(TAG, "POST isTransportTCP? (f) " + ep.isTransportTCP());
	ep.setTransportTCP(torf);

	torf = ep.isTransportGATT();
	Log.i(TAG, "PRE isTransportGATT? " + ep.isTransportGATT());
	ep.setTransportGATT(true);
	Log.i(TAG, "POST isTransportGATT? (t) " + ep.isTransportGATT());
	ep.setTransportGATT(false);
	Log.i(TAG, "POST isTransportGATT? (f) " + ep.isTransportGATT());
	ep.setTransportGATT(torf);

	torf = ep.isTransportRFCOMM();
	Log.i(TAG, "PRE isTransportRFCOMM? " + ep.isTransportRFCOMM());
	ep.setTransportRFCOMM(true);
	Log.i(TAG, "POST isTransportRFCOMM? (t) " + ep.isTransportRFCOMM());
	ep.setTransportRFCOMM(false);
	Log.i(TAG, "POST isTransportRFCOMM? (f) " + ep.isTransportRFCOMM());
	ep.setTransportRFCOMM(torf);

	torf = ep.isTransportNFC();
	Log.i(TAG, "PRE isTransportNFC? " + ep.isTransportNFC());
	ep.setTransportNFC(true);
	Log.i(TAG, "POST isTransportNFC? (t) " + ep.isTransportNFC());
	ep.setTransportNFC(false);
	Log.i(TAG, "POST isTransportNFC? (f) " + ep.isTransportNFC());
	ep.setTransportNFC(torf);

	Log.i(TAG, "PRE isNetworkIP? " + ep.isNetworkIP());
	// torf = ep.isNetworkIP();
	// ep.setNetworkIP(true);
	// Log.i(TAG, "POST isNetworkIP? (t) " + ep.isNetworkIP());
	// ep.setNetworkIP(false);
	// Log.i(TAG, "POST isNetworkIP? (f) " + ep.isNetworkIP());
	// ep.setNetworkIP(torf);

	// torf = ep.transportIsTCP();
	// ep.transportIsTCP(true); // implies IP
	// Log.i(TAG, "POST isNetworkIP? (t) " + ep.isNetworkIP());
	// ep.transportIsTCP(false);
	// Log.i(TAG, "POST isNetworkIP? (f) " + ep.isNetworkIP());
	// ep.transportIsTCP(torf);

	torf = ep.isNetworkIPv4();
	Log.i(TAG, "PRE isNetworkIPv4? " + ep.isNetworkIPv4());
	ep.setNetworkIPv4(true);
	Log.i(TAG, "POST isNetworkIPv4? (t) " + ep.isNetworkIPv4());
	ep.setNetworkIPv4(false);
	Log.i(TAG, "POST isNetworkIPv4? (f) " + ep.isNetworkIPv4());
	ep.setNetworkIPv4(torf);

	torf = ep.isNetworkIPv6();
	Log.i(TAG, "PRE isNetworkIPv6? " + ep.isNetworkIPv6());
	ep.setNetworkIPv6(true);
	Log.i(TAG, "POST isNetworkIPv6? (t) " + ep.isNetworkIPv6());
	ep.setNetworkIPv6(false);
	Log.i(TAG, "POST isNetworkIPv6? (f) " + ep.isNetworkIPv6());
	ep.setNetworkIPv6(torf);

	torf = ep.isScopeInterface();
	Log.i(TAG, "PRE isScopeInterface? " + ep.isScopeInterface());
	ep.setScopeInterface(true);
	Log.i(TAG, "POST isScopeInterface? (t) " + ep.isScopeInterface());
	ep.setScopeInterface(false);
	Log.i(TAG, "POST isScopeInterface? (f) " + ep.isScopeInterface());
	ep.setScopeInterface(torf);

	torf = ep.isScopeLink();
	Log.i(TAG, "PRE isScopeLink? " + ep.isScopeLink());
	ep.setScopeLink(true);
	Log.i(TAG, "POST isScopeLink? (t) " + ep.isScopeLink());
	ep.setScopeLink(false);
	Log.i(TAG, "POST isScopeLink? (f) " + ep.isScopeLink());
	ep.setScopeLink(torf);

	torf = ep.isTransportSecure();
	Log.i(TAG, "PRE isTransportSecure? " + ep.isTransportSecure());
	ep.setTransportSecure(true);
	Log.i(TAG, "POST isTransportSecure? (t) " + ep.isTransportSecure());
	ep.setTransportSecure(false);
	Log.i(TAG, "POST isTransportSecure? (f) " + ep.isTransportSecure());
	ep.setTransportSecure(torf);

	torf = ep.isRoutingMulticast();
	Log.i(TAG, "PRE isRoutingMulticast? " + ep.isRoutingMulticast());
	ep.setRoutingMulticast(true);
	Log.i(TAG, "POST isRoutingMulticast? (t) " + ep.isRoutingMulticast());
	ep.setRoutingMulticast(false);
	Log.i(TAG, "POST isRoutingMulticast? (f) " + ep.isRoutingMulticast());
	ep.setRoutingMulticast(torf);
    }

    static public void logCoSP(CoResourceSP cosp)
    {
	// Log.i(TAG, "CoSP URI PATH: " + cosp.getUriPath());
	// logEndpoint(cosp.coAddress());

	// Log.i(TAG, "CoSP network protocol: "
	// 		   + String.format("0x%04X",
	// 				   cosp.networkProtocol() & 0xFFFFF));
	// Log.i(TAG, "CoSP network Scope: "    + cosp.networkScope());
	// Log.i(TAG, "CoSP network Policies: "
	// 			       + String.format("0x%04X",
	// 					       cosp.networkPolicies() & 0xFFFFF));
	// Log.i(TAG, "CoSP transport secure?: " + cosp.isTransportSecure());

	// List<String> ts = cosp.getTypes();
	// Log.i(TAG, "SP Types:");
	// ts.forEach(typ -> Log.i(TAG, "\t" + typ));

	// List<String> ifs = cosp.getInterfaces();
	// Log.i(TAG, "SP Interfaces");
	// ifs.forEach(iface -> Log.i(TAG, "\t" + iface));
    }

    static public void logSP(IResourceSP theSP)
    {
	Log.i(TAG, "SP URI PATH: " + theSP.getUriPath());
	List<String> ts = theSP.getTypes();
	Log.i(TAG, "SP Types:");
	// ts.forEach(typ -> Log.i(TAG, "\t" + typ));

	// List<String> ifs = theSP.getInterfaces();
	// Log.i(TAG, "SP Interfaces");
	// ifs.forEach(iface -> Log.i(TAG, "\t" + iface));
    }

    // static public void logResource(ResourceLocal resource)
    // {
    // 	Log.i(TAG, "RESOURCE: logResource ENTRY");
    // 	Log.i(TAG, "RESOURCE: resource uri: " + resource.getUri());

    // 	List<String> tll = resource.getTypes();
    // 	tll.forEach(t -> Log.i(TAG, "RESOURCE: type:     " + t)); // Java 8

    // 	List<String> ifll = resource.getInterfaces();
    // 	ifll.forEach(iface -> Log.i(TAG, "RESOURCE: interface: " + iface));
    // 	// for (int i = 0; i < ifll.size(); i++) {   // Java 7
    // 	//     Log.i(TAG, "REQUEST IN: resource if:    " + ifll.get(i));
    // 	// }

    // 	List<PropertyString> pll = resource.getProperties();
    // 	Log.i(TAG, "RESOURCE: properties count: " + pll.size());
    // 	pll.forEach(p -> Log.i(TAG, "RESOURCE: property: " + p.name + " = " + p.value));

    // 	List<ResourceLocal> cll = resource.getChildren();
    // 	Log.i(TAG, "RESOURCE: child resources count: " + cll.size());
    // 	cll.forEach(ch -> Log.i(TAG, "RESOURCE: child resource: " + ch));

    // 	Log.i(TAG, "RESOURCE: service provider (callback): "
    // 			   + resource.getResourceSP().getClass().getName());

    // 	Log.i(TAG, "RESOURCE: callback param: "
    // 			   + resource.getCallbackParam().getClass().getName());

    // 	Log.i(TAG, "RESOURCE: serial number: " + resource.sn);

    // 	// Instance Id
    // 	// Log.i(TAG, "RESOURCE: resource instance id: " + resource.id.getClass().getName());
    // 	if (resource.id.getClass().getName().equals("openocf.Resource$InstanceId")) {
    // 	    Log.i(TAG, "RESOURCE: resource InstanceId class: InstanceId");
    // 	} else if (resource.id.getClass().getName().equals("openocf.Resource$InstanceOrdinal")) {
    // 	    Log.i(TAG, "RESOURCE: resource InstanceId class: InstanceOrdinal, val="
    // 			       + ((openocf.ResourceLocal.InstanceOrdinal)resource.id).val);
    // 	} else if (resource.id.getClass().getName().equals("openocf.ResourceLocal$InstanceString")) {
    // 	    Log.i(TAG, "RESOURCE: resource InstanceId class: InstanceString, val="
    // 			       + ((openocf.ResourceLocal.InstanceString)resource.id).val);
    // 	} else if (resource.id.getClass().getName().equals("openocf.ResourceLocal$InstanceUuid")) {
    // 	    Log.i(TAG, "RESOURCE: resource InstanceId class: InstanceUuid, val="
    // 			       + ((openocf.ResourceLocal.InstanceUuid)resource.id).val);
    // 	}

    // 	logResourcePolicies(resource);

    // 	try {
    // 	    Log.i(TAG, "RESOURCE: action set: " + resource.getActionSet());
    // 	} catch (OCFNotImplementedException e) {
    // 	    Log.i(TAG, "ERROR**** RESOURCE: getActionSet not implemented.");
    // 	}
    // 	Log.i(TAG, "RESOURCE: logResource EXIT");
    // }

    // static public void logObservation(ObservationRecord observationRecord)
    // {
    // 	// Log.i(TAG, "OBSERVATION: logObservation ENTRY");

    // 	Log.i(TAG, "\tOBSERVED uri: " + observationRecord.getUriPath());

    // 	Log.i(TAG, "\tOBSERVED type: " + observationRecord.getType());

    // 	// log rtypes
    // 	List<String> rtypes = observationRecord.getResourceTypes();
    // 	Log.i(TAG, "\tOBSERVED RESOURCE TYPES count: " + rtypes.size());
    // 	for (String t : (List<String>)rtypes) {
    // 	    Log.i(TAG, "\tOBSERVED rtype: " + t);
    // 	}

    // 	// log interfaces
    // 	List<String> ifaces = observationRecord.getInterfaces();
    // 	Log.i(TAG, "\tOBSERVED INTERFACES count: " + ifaces.size());
    // 	for (String iface : ifaces) {
    // 	    Log.i(TAG, "\tOBSERVED interface: " + iface);
    // 	}

    // 	// log properties (PlatformInfo, DeviceInfo, or "values" for resources)
    // 	PropertyMap<String, Object> pmap = observationRecord.getProperties();
    // 	Log.i(TAG, "\tOBSERVED PROPERTIES count: " + pmap.size());
    // 	for (Map.Entry<String, Object> entry : pmap.entrySet())
    // 	    {
    // 		Log.i(TAG, "\tOBSERVED property: "
    // 				   + entry.getKey()
    // 				   + " = "
    // 				   + entry.getValue());
    // 	    }
    // 	List<IObservationRecord> kids = observationRecord.getChildren();
    // 	if (kids != null) {
    // 	    Log.i(TAG, "\tOBSERVED CHILDREN count: " + kids.size());
    // 	    for (IObservationRecord p : kids) {
    // 		Log.i(TAG, "================ CHILD");
    // 		logObservation((ObservationRecord)p);
    // 	    }
    // 	}
    // }

    // static public void logRequestIn(InboundRequest requestIn)
    // {
    // 	Log.i(TAG, "LOG StimulusIn logRequestIn ENTRY");
    // 	// Log.i(TAG, "LOG StimulusIn this handle: " + requestIn.localHandle);
    // 	// Log.i(TAG, "LOG StimulusIn remote handle: " + requestIn.getRemoteHandle);
    // 	// Log.i(TAG, "LOG StimulusIn resource handle: " + requestIn.getResourceHandle());
    // 	Log.i(TAG, "LOG StimulusIn request method: " + requestIn.getMethod());
    // 	Log.i(TAG, "LOG StimulusIn query : \"" + requestIn.getQueryString() + "\"");
    // 	Log.i(TAG, "LOG StimulusIn msg id : " + requestIn.getMessageId());

    // 	Log.i(TAG, "LOG StimulusIn method : " + requestIn.getMethod());
    // 	Log.i(TAG, "LOG StimulusIn watch action : " + requestIn.watchAction);
    // 	Log.i(TAG, "LOG StimulusIn watch id :     " + requestIn.watchId);


    // 	// ResourceLocal resource = requestIn.getResource();
    // 	// logResource(resource);

    // 	// logEndpoint(requestIn.getRemoteEndpoint());

    // 	// Log.i(TAG, "LOG StimulusIn watch action: " + requestIn.watchAction);
    // 	// Log.i(TAG, "LOG StimulusIn watch id    : " + requestIn.watchId);

    // 	List<CoAPOption> headerOptions = requestIn.getOptions();
    // 	if (headerOptions != null)
    // 	    Log.i(TAG, "LOG StimulusIn header opts ct: " + headerOptions.size());

    // 	// ObservationList<PayloadForResourceState> observation = requestIn.getPDUPayload();
    // 	// if (observation == null) {
    // 	//     Log.i(TAG, "LOG StimulusIn observation is null");
    // 	// }
    // }

    // static public void logRequestIn(CoResourceSP cosp)
    // {
    // 	Log.i(TAG, "APP_LOGGER logRequestIn ENTRY, thread "
    // 			   + Thread.currentThread().getId());

    // 	Log.i(TAG, "APP_LOGGER: stack result: " + cosp.getResult());

    // 	Log.i(TAG, "APP_LOGGER CoSP uri path:\t" + cosp.uriPath());
    // 	Log.i(TAG, "APP_LOGGER CoSP method:\t" + cosp.method());
    // 	// Log.i(TAG, "APP_LOGGER CoSP conn type:\t" + cosp.connType());
    // 	Log.i(TAG, "APP_LOGGER CoSP sec ID:\t"
    // 			   + Arrays.toString(cosp.getCoSecurityId()));
    // 	Log.i(TAG, "APP_LOGGER CoSP serial:\t" + cosp.getNotificationSerial());

    // 	Log.i(TAG, "LOGGING CO-ADDRESS:");
    // 	logCoAddress(cosp);

    // 	// List<CoAPOption> headerOptions = cosp.getOptions();
    // 	// if (headerOptions != null)
    // 	//     Log.i(TAG, "LOG StimulusIn header options ct: " + headerOptions.size());

    // 	// FIXME:
    // 	// if (cosp.result == OCStackResult.OK) {

    // 	//     Log.i(TAG, "APP_LOGGER CoSP OBSERVATIONS:");
    // 	//     // Log.i(TAG, "APP_LOGGER CoSP OBSERVATION type: "
    // 	//     // 		       + cosp.getObservationType()
    // 	//     // 		       + ": "
    // 	//     // 		       + observationTypes.get(cosp.getObservationType()));

    // 	//     ObservationList<ObservationRecord> observationRecords = cosp.getObservationRecords();
    // 	//     if (observationRecords != null) {
    // 	// 	Log.i(TAG, "LOG OBSERVATIONRECORD count: " + observationRecords.size());
    // 	// 	for (ObservationRecord observationRecord : (ObservationList<ObservationRecord>) observationRecords) {
    // 	// 	    List<IObservationRecord> kids = observationRecord.getChildren();
    // 	// 	    if (kids != null) {
    // 	// 		Log.i(TAG, "LOG CHILD OBSERVATIONS count: "
    // 	// 				   + observationRecord.getChildren().size());
    // 	// 	    }
    // 	// 	    // logObservation(observation);
    // 	// 	}
    // 	//     }
    // 	// }
    // }

    static public void logOutboundStimulus(OutboundStimulus request)
    {
	Log.i(TAG, "APP_LOGGERlogOutboundStimulus ENTRY, thread "
			   + Thread.currentThread().getId());

	Log.i(TAG, "APP_LOGGER CoSP uri path:\t" + request.getUri());
	// OBSOLETE Log.i(TAG, "APP_LOGGER CoSP method:\t" + cosp.getMethod());
	// Log.i(TAG, "APP_LOGGER CoSP conn type:\t" + cosp.connType());

	// List<CoAPOption> headerOptions = cosp.getOptions();
	// if (headerOptions != null)
	//     Log.i(TAG, "LOG StimulusIn header options ct: " + headerOptions.size());

	// FIXME:
	// if (cosp.result == OCStackResult.OK) {

	//     Log.i(TAG, "APP_LOGGER CoSP OBSERVATIONS:");
	//     // Log.i(TAG, "APP_LOGGER CoSP OBSERVATION type: "
	//     // 		       + cosp.getObservationType()
	//     // 		       + ": "
	//     // 		       + observationTypes.get(cosp.getObservationType()));

	//     ObservationList<ObservationRecord> observationRecords = cosp.getObservationRecords();
	//     if (observationRecords != null) {
	// 	Log.i(TAG, "LOG OBSERVATIONRECORD count: " + observationRecords.size());
	// 	for (ObservationRecord observationRecord : (ObservationList<ObservationRecord>) observationRecords) {
	// 	    List<IObservationRecord> kids = observationRecord.getChildren();
	// 	    if (kids != null) {
	// 		Log.i(TAG, "LOG CHILD OBSERVATIONS count: "
	// 				   + observationRecord.getChildren().size());
	// 	    }
	// 	    // logObservation(observation);
	// 	}
	//     }
	// }
    }

    static public void logInboundResponse(InboundResponse resp)
    {
	Log.i(TAG, "APP_LOGGER logInboundResponse ENTRY, thread "
			   + Thread.currentThread().getId());

	//	Log.i(TAG, "APP_LOGGER stack result: " + resp.getResult());

	Log.i(TAG, "APP_LOGGER Resp uri path:\t" + resp.getUri());
	// Log.i(TAG, "APP_LOGGER Resp conn type:\t" + resp.connType());
	// Log.i(TAG, "APP_LOGGER Resp sec ID:\t"
	// 		   + Arrays.toString(resp.getCoSecurityId()));
	// Log.i(TAG, "APP_LOGGER Resp serial:\t" + resp.getNotificationSerial());

	Log.i(TAG, "LOGGING CO-ADDRESS:");
	logCoAddress(resp.getEndpoint());

	// List<CoAPOption> headerOptions = cosp.getOptions();
	// if (headerOptions != null)
	//     Log.i(TAG, "LOG StimulusIn header options ct: " + headerOptions.size());

	// FIXME:
	// if (cosp.result == OCStackResult.OK) {

	//     Log.i(TAG, "APP_LOGGER CoSP OBSERVATIONS:");
	//     // Log.i(TAG, "APP_LOGGER CoSP OBSERVATION type: "
	//     // 		       + cosp.getObservationType()
	//     // 		       + ": "
	//     // 		       + observationTypes.get(cosp.getObservationType()));

	//     ObservationList<ObservationRecord> observationRecords = cosp.getObservationRecords();
	//     if (observationRecords != null) {
	// 	Log.i(TAG, "LOG OBSERVATIONRECORD count: " + observationRecords.size());
	// 	for (ObservationRecord observationRecord : (ObservationList<ObservationRecord>) observationRecords) {
	// 	    List<IObservationRecord> kids = observationRecord.getChildren();
	// 	    if (kids != null) {
	// 		Log.i(TAG, "LOG CHILD OBSERVATIONS count: "
	// 				   + observationRecord.getChildren().size());
	// 	    }
	// 	    // logObservation(observation);
	// 	}
	//     }
	// }
    }
}
