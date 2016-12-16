package org.iochibity.test;

import org.iochibity.OCF;
import org.iochibity.Message;
import org.iochibity.DeviceAddress;
import org.iochibity.HeaderOption;
import org.iochibity.MsgRequestIn;
import org.iochibity.MsgResponseIn;
import org.iochibity.MsgResponseOut;
import org.iochibity.IPayload;
import org.iochibity.Payload;
import org.iochibity.PayloadList;
import org.iochibity.PayloadForPlatform;
import org.iochibity.PayloadForResourceState;
import org.iochibity.PropertyMap;
import org.iochibity.PropertyString;
import org.iochibity.Resource;
import org.iochibity.ResourceLocal;
import org.iochibity.IServiceProvider;
import org.iochibity.constants.OCMode;
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
    public static final HashMap payloadTypes;
    static {
	errcodeMap = new HashMap<Integer, String>();
        errcodeMap.put(0, "OK");
	errcodeMap.put(1, "RESOURCE_CREATED");
	errcodeMap.put(2, "RESOURCE_DELETED");
	errcodeMap.put(3, "CONTINUE");
	errcodeMap.put(4, "RESOURCE_CHANGED");
	errcodeMap.put(46, "UNAUTHORIZED_REQ");

	payloadTypes = new HashMap<Integer, String>();
	payloadTypes.put(Payload.PLATFORM, "PLATFORM");
	payloadTypes.put(Payload.DEVICE, "DEVICE");
	payloadTypes.put(Payload.REPRESENTATION, "REPRESENTATION");
	payloadTypes.put(Payload.DISCOVERY, "DISCOVERY");
	payloadTypes.put(Payload.SECURITY, "SECURITY");
	payloadTypes.put(Payload.PRESENCE, "PRESENCE");
	payloadTypes.put(Payload.RD, "RD");
	payloadTypes.put(Payload.NOTIFICATION, "NOTIFICATION");
    }

    static public void logDeviceAddress(DeviceAddress da)
    {
	System.out.println("LOG DeviceAddress address:\t" + da.address);
	System.out.println("LOG DeviceAddress port:\t" + da.port);
	System.out.println("LOG DeviceAddress adapter:\t" + da.adapter);
	System.out.println("LOG DeviceAddress flags:\t" + da.flags);
	System.out.println("LOG DeviceAddress ifindex:\t" + da.ifindex);
	// System.out.println("REQUEST IN: devaddr route data: " + da.routeData);


	// System.out.println("LOG DeviceAddress route data: " + da.routeData);
    }

    static public void logResourcePolicies(ResourceLocal resource)
    {
	System.out.println("RESOURCE: policies: "
			   + String.format("0x%X", resource.policies & 0xFFFFF));
	if ( (resource.policies & ResourcePolicy.DISCOVERABLE) > 0 )
	    {System.out.println("\tDISCOVERABLE");}
	if ( (resource.policies & ResourcePolicy.ACTIVE) > 0 ) {System.out.println("\tACTIVE");}
	if ( (resource.policies & ResourcePolicy.OBSERVABLE) > 0) {System.out.println("\tOBSERVABLE");}
	if ( (resource.policies & ResourcePolicy.SECURE) > 0) {System.out.println("\tSECURE");}
    }

    static public void logResource(ResourceLocal resource)
    {
	System.out.println("RESOURCE: logResource ENTRY");
	System.out.println("RESOURCE: resource uri: " + resource.getUri());

	List<String> tll = resource.getTypes();
	tll.forEach(t -> System.out.println("RESOURCE: type:     " + t)); // Java 8

	List<String> ifll = resource.getInterfaces();
	ifll.forEach(iface -> System.out.println("RESOURCE: interface: " + iface));
	// for (int i = 0; i < ifll.size(); i++) {   // Java 7
	//     System.out.println("REQUEST IN: resource if:    " + ifll.get(i));
	// }

	List<PropertyString> pll = resource.getProperties();
	System.out.println("RESOURCE: properties count: " + pll.size());
	pll.forEach(p -> System.out.println("RESOURCE: property: " + p.name + " = " + p.value));

	List<ResourceLocal> cll = resource.getChildren();
	System.out.println("RESOURCE: child resources count: " + cll.size());
	cll.forEach(ch -> System.out.println("RESOURCE: child resource: " + ch));

	System.out.println("RESOURCE: service provider (callback): "
			   + resource.getServiceProvider().getClass().getName());

	System.out.println("RESOURCE: callback param: "
			   + resource.getCallbackParam().getClass().getName());

	System.out.println("RESOURCE: serial number: " + resource.sn);

	// Instance Id
	// System.out.println("RESOURCE: resource instance id: " + resource.id.getClass().getName());
	if (resource.id.getClass().getName().equals("org.iochibity.Resource$InstanceId")) {
	    System.out.println("RESOURCE: resource InstanceId class: InstanceId");
	} else if (resource.id.getClass().getName().equals("org.iochibity.Resource$InstanceOrdinal")) {
	    System.out.println("RESOURCE: resource InstanceId class: InstanceOrdinal, val="
			       + ((org.iochibity.ResourceLocal.InstanceOrdinal)resource.id).val);
	} else if (resource.id.getClass().getName().equals("org.iochibity.ResourceLocal$InstanceString")) {
	    System.out.println("RESOURCE: resource InstanceId class: InstanceString, val="
			       + ((org.iochibity.ResourceLocal.InstanceString)resource.id).val);
	} else if (resource.id.getClass().getName().equals("org.iochibity.ResourceLocal$InstanceUuid")) {
	    System.out.println("RESOURCE: resource InstanceId class: InstanceUuid, val="
			       + ((org.iochibity.ResourceLocal.InstanceUuid)resource.id).val);
	}

	logResourcePolicies(resource);

	try {
	    System.out.println("RESOURCE: action set: " + resource.getActionSet());
	} catch (OCFNotImplementedException e) {
	    System.out.println("ERROR**** RESOURCE: getActionSet not implemented.");
	}
	System.out.println("RESOURCE: logResource EXIT");
    }

    static public void logPayload(Payload payload)
    {
	// System.out.println("PAYLOAD: logPayload ENTRY");
	System.out.println("LOG PAYLOAD uri: " + payload.getUriPath());

	// log rtypes
	List<String> rtypes = payload.getResourceTypes();
	System.out.println("LOG PAYLOAD RESOURCE TYPES count: " + rtypes.size());
	for (String t : (List<String>)rtypes) {
	    System.out.println("LOG PAYLOAD rtype: " + t);
	}

	// log interfaces
	List<String> ifaces = payload.getInterfaces();
	System.out.println("LOG PAYLOAD INTERFACES count: " + ifaces.size());
	for (String iface : ifaces) {
	    System.out.println("LOG PAYLOAD interface: " + iface);
	}

	// log properties (PlatformInfo, DeviceInfo, or "values" for resources)
	PropertyMap<String, Object> pmap = payload.getProperties();
	System.out.println("LOG PAYLOAD PROPERTIES count: " + pmap.size());
	for (Map.Entry<String, Object> entry : pmap.entrySet())
	    {
		System.out.println("LOG PAYLOAD property: "
				   + entry.getKey()
				   + " = "
				   + entry.getValue());
	    }
	List<IPayload> kids = payload.getChildren();
	if (kids != null) {
	    System.out.println("LOG PAYLOAD CHILDREN count: " + kids.size());
	    for (IPayload p : kids) {
		System.out.println("================ CHILD");
		logPayload((Payload)p);
	    }
	}
    }

    static public void logMessageList(PayloadList<PayloadForResourceState> pl)
    {
	System.out.println("Logging PayloadList");
	for (PayloadForResourceState pr : (PayloadList<PayloadForResourceState>)pl) {
	    logPayload(pr);
	    PropertyMap pps = pr.getProperties();
	    System.out.println("PAYLOAD prop ct: " + pps.size());
	}
    }

    static public void logRequestIn(MsgRequestIn requestIn)
    {
	System.out.println("LOG MsgRequestIn logRequestIn ENTRY");
	System.out.println("LOG MsgRequestIn this handle: " + requestIn.localHandle);
	System.out.println("LOG MsgRequestIn remote handle: " + requestIn.remoteHandle);
	System.out.println("LOG MsgRequestIn resource handle: " + requestIn.getResourceHandle());
	System.out.println("LOG MsgRequestIn request method: " + requestIn.getMethod());
	System.out.println("LOG MsgRequestIn query : \"" + requestIn.getQueryString() + "\"");
	System.out.println("LOG MsgRequestIn msg id : " + requestIn.getMessageId());

	ResourceLocal resource = requestIn.getResource();
	logResource(resource);

	logDeviceAddress(requestIn.getRemoteDeviceAddress());

	// System.out.println("LOG MsgRequestIn watch action: " + requestIn.watchAction);
	// System.out.println("LOG MsgRequestIn watch id    : " + requestIn.watchId);

	List<HeaderOption> headerOptions = requestIn.getOptions();
	if (headerOptions != null)
	    System.out.println("LOG MsgRequestIn header opts ct: " + headerOptions.size());

	// PayloadList<PayloadForResourceState> payload = requestIn.getPDUPayload();
	// if (payload != null) {
	System.out.println("LOG MsgRequestIn payload type: " + requestIn.getPayloadType());
	// } else {
	//     System.out.println("LOG MsgRequestIn payload is null");
	// }
    }

    static public void logMsgResponseIn(MsgResponseIn msgResponseIn)
    {
	System.out.println("LOG MsgResponseIn STACK RESULT:\t"
			   + msgResponseIn.result
			   + ": "
			   + errcodeMap.get(msgResponseIn.result));

	System.out.println("LOG MsgResponseIn uri:\t" + msgResponseIn.uri);
	System.out.println("LOG MsgResponseIn conn type:\t" + msgResponseIn.connType);
	System.out.println("LOG MsgResponseIn sec ID:\t" + msgResponseIn.secID);
	System.out.println("LOG MsgResponseIn stack result:\t" + msgResponseIn.result);
	System.out.println("LOG MsgResponseIn serial:\t" + msgResponseIn.serial);

	System.out.println("LOG MsgResponseIn REMOTE DEVICE ADDRESS:");
	logDeviceAddress(msgResponseIn.getRemoteDeviceAddress());

	List<HeaderOption> headerOptions = msgResponseIn.getOptions();
	if (headerOptions != null)
	    System.out.println("LOG MsgRequestIn header options ct: " + headerOptions.size());

	// FIXME:
	if (msgResponseIn.result == OCStackResult.OK) {

	    System.out.println("LOG MsgResponseIn PAYLOAD:");
	    System.out.println("LOG MsgResponseIn PAYLOAD type: "
			       + msgResponseIn.getPayloadType()
			       + ": "
			       + payloadTypes.get(msgResponseIn.getPayloadType()));

	    PayloadList<Payload> pll = msgResponseIn.getPayloadList();
	    System.out.println("LOG PAYLOAD count: " + pll.size());
	    for (Payload payload : (PayloadList<Payload>) pll) {
		logPayload(payload);
	    }
	}
    }
}
