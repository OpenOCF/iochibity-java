package org.iochibity;

import java.util.List;
import java.util.LinkedList;

/**
 * brief desc of this file
 * 
 * It wraps the OCDoResource parameters, plus ...
 */

public abstract class CoServiceProvider
    implements ICoServiceProvider
{
    // ctor, initializes TLS vars
    native private void ctorCoSP();
    public CoServiceProvider() {
	ctorCoSP();
    }

    // FIXME: rename to _transactionHandle? _stimulusHandle?
    private long                   _handle; // OCDoHandle from OCDoResource

    // THe heart of the matter: actions and reactions.
    @Override
    abstract public void            react(); // must be implemented by user; called by stack

    // the remaining methods implement (natively) the ICoServiceProvider interface
    native   public void            exhibit(); // called by user

    // OCDoResource params
    native public String            uriPath();
    native public CoServiceProvider uriPath(String uriPath);

    native public int               method();
    native public CoServiceProvider method(int method);

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
    // OCPayload wrapper
    private ObservationRecord      _observationRecord;
    public  ObservationRecord      getObservationRecord() { return _observationRecord; }
    public  void                   setObservationRecord(ObservationRecord o) { _observationRecord = o; }

    native public DeviceAddress    coAddress();

    native public String           getCoSecurityId();

    native public int              getCoResult();

    // for observables, https://tools.ietf.org/html/rfc7641
    native public int              getObservationSerial();

    private List<String>           _types = new LinkedList<String>();
    public  List<String>           getTypes() { return _types; }
    public  boolean                addType(String theTypes) { return _types.add(theTypes); }

    private List<String>           _interfaces = new LinkedList<String>();
    public List<String>            getInterfaces() { return _interfaces; }
    public boolean                 addInterface(String iface) { return _interfaces.add(iface); }

    private PropertyMap            _properties;
    public  PropertyMap            getProperties() { return _properties; }
    public  Object                 putProperty(String key, Object val) { return _properties.put(key, val); }

    private List<IServiceProvider> _children;
    public  List<IServiceProvider> getChildren() { return _children; }

    private List<ActionSet>        _actionSet;
    public  List<ActionSet>        getActionSet() { return _actionSet; }

}

