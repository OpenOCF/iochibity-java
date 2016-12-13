package org.iochibity.test;

import org.iochibity.OCF;
import org.iochibity.DeviceAddress;
import org.iochibity.HeaderOption;
import org.iochibity.Payload;
import org.iochibity.PayloadList;
import org.iochibity.PropertyMap;
import org.iochibity.PayloadForResourceState;
import org.iochibity.PropertyString;
import org.iochibity.DocRequestIn;
import org.iochibity.DocResponseIn;
import org.iochibity.Resource;
import org.iochibity.ResourceLocal;
// import org.iochibity.Resource$InstanceId;
import org.iochibity.ResourceManager;
import org.iochibity.IResourceServiceProvider;
import org.iochibity.DocResponseOut;
// import org.iochibity.constants.Method;
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
import java.util.List;
import java.util.LinkedList;
import java.util.Map;

public class Logger
{
    static public void logDocResponseIn(DocResponseIn drspi)
    {
	System.out.println("JAVA: response uri:\t" + drspi.uri);
	System.out.println("JAVA: response conn type:\t" + drspi.connType);
	System.out.println("JAVA: response sec ID:\t" + drspi.secID);
	System.out.println("JAVA: response stack result:\t" + drspi.result);
	System.out.println("JAVA: response serial:\t" + drspi.serial);
	System.out.println("JAVA: response option count:\t" + drspi.optionCount);
	logDeviceAddress(drspi.deviceAddress);
    }

    static public void logDeviceAddress(DeviceAddress da)
    {
	System.out.println("JAVA: response dev address:\t" + da.address);
	System.out.println("JAVA: response dev port:\t" + da.port);
	System.out.println("JAVA: response dev adapter:\t" + da.adapter);
	System.out.println("JAVA: response dev flags:\t" + da.flags);
	System.out.println("JAVA: response dev ifindex:\t" + da.ifindex);
	// System.out.println("JAVA: response dev route data: " + da.routeData);
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

    static public void logPayload(PayloadForResourceState pr)
    {
	// System.out.println("PAYLOAD: logPayload ENTRY");
	System.out.println("PAYLOAD: uri: " + pr.getUri());
	// System.out.println("PAYLOAD: logPayload EXIT");
    }

    static public void logPayloadType(Payload p)
    {
	switch (p.type) {
	case Payload.INVALID:
	    System.out.println("TEST payload type: INVALID (" + p.type + ")");
	    break;
	case Payload.DISCOVERY:
	    System.out.println("TEST payload type: DISCOVERY (" + p.type + ")");
	    break;
	case Payload.DEVICE:
	    System.out.println("TEST payload type: DEVICE (" + p.type + ")");
	    break;
	case Payload.PLATFORM:
	    System.out.println("TEST payload type: PLATFORM (" + p.type + ")");
	    break;
	case Payload.REPRESENTATION:
	    System.out.println("TEST payload type: REPRESENTATION (" + p.type + ")");
	    break;
	case Payload.SECURITY:
	    System.out.println("TEST payload type: SECURITY (" + p.type + ")");
	    break;
	case Payload.PRESENCE:
	    System.out.println("TEST payload type: PRESENCE (" + p.type + ")");
	    break;
	case Payload.RD:
	    System.out.println("TEST payload type: RD (" + p.type + ")");
	    break;
	default:
	    System.out.println("TEST payload type: UNKNOWN (" + p.type + ")");
	    break;
	}
    }

    static public void logPayloadList(PayloadList<PayloadForResourceState> pl)
    {
	System.out.println("Logging PayloadList");
	for (PayloadForResourceState pr : (PayloadList<PayloadForResourceState>)pl) {
	    logPayload(pr);
	    PropertyMap pps = pr.getProperties();
	    System.out.println("PAYLOAD: prop ct: " + pps.size());
	}
    }

    static public void logRequestIn(DocRequestIn request)
    {
	System.out.println("REQUEST IN: logRequestIn ENTRY");
	System.out.println("REQUEST IN: this handle: " + request.localHandle);
	System.out.println("REQUEST IN: handle at origin: " + request.remoteHandle);
	System.out.println("REQUEST IN: resource handle: " + request.getResourceHandle());
	System.out.println("REQUEST IN: request method: " + request.method);
	System.out.println("REQUEST IN: query : \"" + request.query + "\"");
	System.out.println("REQUEST IN: msg id : " + request.messageId);

	ResourceLocal resource = request.getResource();
	logResource(resource);

	System.out.println("REQUEST IN: devaddr adapter: " + request.deviceAddress.adapter);
	System.out.println("REQUEST IN: devaddr flags: " + String.format("0x%04X", request.deviceAddress.flags & 0xFFFFF));
	System.out.println("REQUEST IN: devaddr port: " + request.deviceAddress.port);
	System.out.println("REQUEST IN: devaddr address: " + request.deviceAddress.address);
	System.out.println("REQUEST IN: devaddr ifindex: " + request.deviceAddress.ifindex);
	System.out.println("REQUEST IN: devaddr route data: " + request.deviceAddress.routeData);

	System.out.println("REQUEST IN: observe action: " + request.observeAction);
	System.out.println("REQUEST IN: observe id    : " + request.observeId);

	List<HeaderOption> vendorOptions = request.getOptions();
	System.out.println("REQUEST IN: vendor opts ct: " + vendorOptions.size());

	PayloadList<PayloadForResourceState> payload = request.getPDUPayload();
	if (payload != null) {
	    System.out.println("REQUEST IN: payload type: " + payload.element().type);
	} else {
	    System.out.println("REQUEST IN: payload is null");
	}
    }
}
