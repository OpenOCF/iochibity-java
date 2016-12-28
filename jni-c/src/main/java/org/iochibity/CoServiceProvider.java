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
    // public  long                   getHandle() { return _handle; }

    // public native StimulusOut      getStimulusOut();

    // NB: this is thread_local in jni:
    // private long                   _c_ptr_OCClientResource;
    // public  long                   getObservationInHandle() { return _observationInHandle; }

    // THe heart of the matter: actions and reactions.
    //
    native   public void           exhibit(); // called by user
    // native   public void           exhibitStimulus(); // called by user
    // native   public void           exhibit(Stimulus stim); // called by user

    // observeBehavior: implemented by user, called by stack
    @Override // ICoServiceProvider
    abstract public void          react();
    // abstract public int            observeBehavior(ObservationIn observationIn);
    // abstract public int            react(Stimulus s);
    // abstract public int            observe(Behavior beh);

    // OCDoResource params
    native public String          uriPath();
    native public void            setUriPath(String uriPath);
    // private String                 _uriPath;
    // public  String                 getUriPath() { return _uriPath; }
    // public  void                   setUriPath(String uriPath) { _uriPath = uriPath; }

    native public int              method();
    private int                    _method;
    public  int                    getMethod() { return _method; }
    public  void                   setMethod(int method) { _method = method; }

    // OCClientResponse data:
    native public String           getCoSecurityId();

    native public int              getCoResult();

    // for observables, https://tools.ietf.org/html/rfc7641
    native public int              getObservationSerial();


    // NOTE: OCDoResource needs a DevAddr and a ConnectivityType,
    // giving redundant networking params.  Why? Maybe so
    // ConnectivityType can be used when DevAddr is NULL (multicast)?

    // // OCDevAddr
    // native public DeviceAddress   coAddress(); // ??

    // public  DeviceAddress          getCoAddress() {
    // 	if (_destination == null) {
    // 	    return coAddress();
    // 	} else {
    // 	    return _destination;
    // 	}
    // }

    // OCDoResources takes an OCConnectivityType arg, in addition to
    // the OCDevAddr arg which contains the same type of data as
    // OCConnectivityType.

    // Why is this? Maybe: if the OCDevAddr* arg is NULL, it's a multicast,
    // and can use the OCConnectivityType params.  Otherwise, it's a
    // unicast targeting the OCDevAddr, whose parameters should be
    // used.

    // If that's right, these should pull data from the device address
    // if we have one, otherwise from connectivityType

    // also OCClientResponse contains an OCConnectivityType member.

    // MULTICAST
    // returns protocol (adapter) for multicasts (from ConnectivityType)
    native public int                    multicastProtocol(); // native

    // UNICAST
    // private DeviceAddress          _destination;
    // public  DeviceAddress          getDestination() { return _destination; }
    // native public  void               setDestination(DeviceAddress dest);


    /**
     * If we have a DevAddr (e.g. from a discovery response), null it
     * out.  If we do not have a DevAddr, we don't have an IP address
     * anyway.
     */

    native public void                   setMulticast();
    native public boolean                isMulticast();
    // {
    // 	System.out.println("Setting dest: " + dest);
    // 	_destination = dest;
    // }
    native public DeviceAddress          coAddress();

    // returns protocol (adapter) for unicasts (from destination DevAddr)
    native public int                    unicastProtocol();

    // OCTransportAdapter: OC_ADAPTER_IP, etc.
    // native public int           networkProtocol();
    // private int                 _networkProtocol;
    // public  int                 getNetworkProtocol() { return _networkProtocol; }
    // native public  void            setNetworkProtocol(int np); // { _networkProtocol = np; }

    native public int              networkFlags(); // OCTransportFlags bitmap "flags"

    // flags, broken out for convenience:
    // OCTransportFlags: OC_SCOPE_LINK etc.
    native public int              networkScope();
    // public int                     _networkScope;
    // public int                     getNetworkScope() { return _networkScope; }
    // public void                    setNetworkScope(int scope) { _networkScope = scope; }

    // OCTransportFlags: OC_IP_USE_V6, OC_MULTICAST, etc.
    native public  int             networkPolicies();
    // private int                    _networkPolicies;
    // public  int                    getNetworkPolicies() { return _networkPolicies; }
    // public  void                   setNetworkPolicies(int np) { _networkPolicies = np; }

    // OCTransportFlags: OC_FLAG_SECURE
    native public boolean          transportIsSecure();
    public boolean                 _transportSecurity;
    // public boolean                 isTransportSecure() { return _transportSecurity; }
    public void                    setTransportSecurity(boolean secure) { _transportSecurity = secure; }

    // private int                    _policies = 0;
    // public  int                    getPolicies() { return _policies; }
    // public  void                   setPolicies(int newPolicies) { _policies = newPolicies; }

    // OCPayload wrapper
    private ObservationRecord            _observationRecord;
    public  ObservationRecord            getObservationRecord() { return _observationRecord; }
    public  void                   setObservationRecord(ObservationRecord o) { _observationRecord = o; }

    private int                    _qualityOfService;
    public  int                    getQualityOfService() { return _qualityOfService; }
    public  void                   setQualityOfService(int qos) { _qualityOfService = qos; }

    // OCClientResponse fields
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
