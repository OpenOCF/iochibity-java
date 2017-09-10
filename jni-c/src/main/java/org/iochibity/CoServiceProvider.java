package org.iochibity;

import java.util.List;
import java.util.LinkedList;

/**
 * brief desc of this file
 *
 * It wraps the OCDoResource parameters, plus ...
 */

/* notes

   this class encapsulates both the CoSP (i.e. client request) and the SP (i.e. server response).

   for example, the networking properties are one thing on the way
   out, but could be sth else in the response.  e.g. the response to a
   multicast will be a unicast.

   legacy stuff: OCClientResponse contains redundant networking info,
   in that OCDevAddr contains the same info as connType
   (OCConnectivityType).  The source is commented: connType is for
   backward compatibility.

   but note that OCDevAddr is misnamed; it contains not just address info, but also info about networking params, which is not really address info.

 */

public abstract class CoServiceProvider
    implements ICoServiceProvider
{
    // ctor, initializes TLS vars
    native private void ctorCoSP();
    public CoServiceProvider() {
	ctorCoSP();
    }

    // a CoSP object may be associated with multiple transactions, so
    // keeping a handle member will not work...
    // private long _handle; // OCDoHandle from OCDoResource

    // THe heart of the matter: (co-)actions and (co-)reactions.
    @Override
    abstract public void            coReact(); // must be implemented by user; called by stack

    // the remaining methods implement (natively) the ICoServiceProvider interface
    native   public void            coExhibit(); // called by user

    native public int               method();
    native public CoServiceProvider method(int method);

    // descriptor, for matching against SPs (and setting OCDoResource params)
    native public String            uriPath();
    native public CoServiceProvider uriPath(String uriPath);

    // // FIXME:  do we needd types, interfaces, and props in a CoSP?
    // // private List<String>            _types = new LinkedList<String>();
    // // public  List<String>            getTypes() { return _types; }
    // native public List<String>      types();
    // native public CoServiceProvider addType(String theTypes); // { return _types.add(theTypes); }

    // // private List<String>            _interfaces = new LinkedList<String>();
    // native public List<String>      getInterfaces();  // { return _interfaces; }
    // native public CoServiceProvider addInterface(String iface); // { return _interfaces.add(iface); }

    // private PropertyMap             _properties;
    // public  PropertyMap             getProperties() { return _properties; }
    // public  Object                  putProperty(String key, Object val) { return _properties.put(key, val); }
    // end descriptor

    //////////////// NETWORKING params ////////////////
    // Raw
    native public int               networkAdapter(); // OCTransportAdapter
    native public int               networkFlags();    // OCTransportFlags bitmap "flags"

    // networkFlags, broken out
    native public boolean           transportIsSecure(); // flags & 0x0008
    native public CoServiceProvider transportIsSecure(boolean torf);

    // Transport Protocol flags: mutually exclusive, setting one resets all the others.
    // (comments: xx_ is for CT_ or OC_)
    native public boolean           transportIsUDP();    // xx_ADAPTER_IP (misnamed)
    native public CoServiceProvider transportIsUDP(boolean torf);

    native public boolean           transportIsTCP();    // xx_ADAPTER_TCP
    native public CoServiceProvider transportIsTCP(boolean torf);

    native public boolean           transportIsGATT();   // xx_ADAPTER_GATT_BTLE
    native public CoServiceProvider transportIsGATT(boolean torf);

    native public boolean           transportIsRFCOMM(); // xx_ADAPTER_RFCOMM_BTEDR
    native public CoServiceProvider transportIsRFCOMM(boolean torf);

    native public boolean           transportIsNFC();    // xx_ADAPTER_NFC
    native public CoServiceProvider transportIsNFC(boolean torf);

    // IP flags: only needed to select version of IP protocol
    native public boolean            networkIsIP(); // == transportIsUDP
    native public ICoServiceProvider networkIsIP(boolean torf);

    native public boolean           networkIsIPv4();     // xx_IP_USE_V4 (flags & 0x0010)
    native public CoServiceProvider networkIsIPv4(boolean torf);

    native public boolean           networkIsIPv6();     // xx_IP_USE_V6 (flags & 0x0010)
    native public CoServiceProvider networkIsIPv6(boolean torf);

    // IPv6 only:
    native public boolean           scopeIsInterface();      // flags & 0x000F
    native public CoServiceProvider scopeIsInterface(boolean torf);      // flags & 0x000F

    native public boolean           scopeIsLink();           // flags & 0x000F
    native public CoServiceProvider scopeIsLink(boolean torf);      // flags & 0x000F

    native public boolean           scopeIsRealm();          // flags & 0x000F
    native public CoServiceProvider scopeIsRealm(boolean torf);      // flags & 0x000F

    native public boolean           scopeIsAdmin();
    native public CoServiceProvider scopeIsAdmin(boolean torf);

    native public boolean           scopeIsSite();
    native public CoServiceProvider scopeIsSite(boolean torf);

    native public boolean           scopeIsOrg();
    native public CoServiceProvider scopeIsOrg(boolean torf);

    native public boolean           scopeIsGlobal();
    native public CoServiceProvider scopeIsGlobal(boolean torf);

    // Routing
    native public boolean           routingIsMulticast();
    native public CoServiceProvider routingIsMulticast(boolean torf);

    // Addressing
    native public int               port();		// uint16_t

    native public String            ipAddress();          // char addr[MAX_ADDR_STR_SIZE];

    native public int               qualityOfService();
    native public CoServiceProvider qualityOfService(int qos);

    ////////////////////////////////////////////////////////////////
    // OCClientResponse data:

    // these data describe the corresponding SP:

    // FIXME: resolve clash between CoSP's descr of SP (which are
    // match patterns) and the SP description received in the
    // response.  The former is not really a description, or rather it
    // is an open description; the latter is a closed description.

    // for example, the CoSP might have type="foo.bar", and the
    // response might include that plus several other types.  We do
    // not want to confuse the two "descriptions".

    // one possible solution: leave the open ("query") description
    // (associated with the CoSP) at the top, and keep the closed
    // descriptions (associated with the SP) with the
    // ObservationRecords.  This will work for some payloads, which
    // contains uri, types[], and interfaces[] (e.g. OCRepPayload,
    // OCResourcePayload, etc.) but not others
    // (e.g. OCSecurityPayload).  Note that OCResource also contains
    // uri, types, and interfaces.


    native public DeviceAddress    coAddress();
    native public int              getCoResult();

    native public byte[]           getCoSecurityId();

    // for observables, https://tools.ietf.org/html/rfc7641
    native public int              getNotificationSerial();

    ////////////////////////////////////////////////////////////////
    // OCPayload wrapper
    // NB: ObservationRecords must be read-only!
    // native  List<ObservationRecord> observations();
    // private ObservationRecord      _observationRecord;
    // public  ObservationRecord      getObservationRecord() { return _observationRecord; }
    // public  void                   setObservationRecord(ObservationRecord o) { _observationRecord = o; }

    // // OCResource.rsrcChildResourcesHead (for collections)
    private List<IServiceProvider> _children;
    public  List<IServiceProvider> getChildren() { return _children; }

    // // SPs only?
    // private List<ActionSet>        _actionSet;
    // public  List<ActionSet>        getActionSet() { return _actionSet; }

    // for discovery and presence requests:
    native public void               deactivate();
}
