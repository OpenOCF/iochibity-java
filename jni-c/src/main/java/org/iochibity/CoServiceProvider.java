package org.iochibity;

import java.util.List;
import java.util.LinkedList;

public abstract class CoServiceProvider
    implements ICoServiceProvider
{
    private long                   _handle; // OCDoHandle
    // public  long                   getHandle() { return _handle; }

    // public native StimulusOut      getStimulusOut();

    private long                   _observationInHandle; // OCClientResource*
    // public  long                   getObservationInHandle() { return _observationInHandle; }

    // OCDoResource params
    private String                 _uriPath;
    public  String                 getUriPath() { return _uriPath; }
    public  void                   setUriPath(String uriPath) { _uriPath = uriPath; }

    private int                    _method;
    public  int                    getMethod() { return _method; }
    public  void                   setMethod(int method) { _method = method; }

    // NOTE: OCDoResource needs a DevAddr and a ConnectivityType,
    // giving redundant networking params.  Why? Maybe so
    // ConnectivityType can be used when DevAddr is NULL (multicast)?

    // OCDevAddr
    private DeviceAddress          _destination;
    public  DeviceAddress          getDestination() { return _destination; }
    public  void                   setDestination(DeviceAddress dest)
    {
	System.out.println("Setting dest: " + dest);
	_destination = dest;
    }

    // OCTransportAdapter: OC_ADAPTER_IP, etc.
    private int                    _networkProtocol;
    public  int                    getNetworkProtocol() { return _networkProtocol; }
    public  void                   setNetworkProtocol(int np) { _networkProtocol = np; }

    // OCTransportFlags: OC_SCOPE_LINK etc.
    public int                     _networkScope;
    public int                     getNetworkScope() { return _networkScope; }
    public void                    setNetworkScope(int scope) { _networkScope = scope; }

    // OCTransportFlags: OC_IP_USE_V6, OC_MULTICAST, etc.
    private int                    _networkPolicies;
    public  int                    getNetworkPolicies() { return _networkPolicies; }
    public  void                   setNetworkPolicies(int np) { _networkPolicies = np; }

    // OCTransportFlags: OC_FLAG_SECURE
    public boolean                 _transportSecurity;
    public boolean                 isTransportSecure() { return _transportSecurity; }
    public void                    setTransportSecurity(boolean secure) { _transportSecurity = secure; }

    // private int                    _policies = 0;
    // public  int                    getPolicies() { return _policies; }
    // public  void                   setPolicies(int newPolicies) { _policies = newPolicies; }

    // OCPayload wrapper
    private Observation            _observation;
    public  Observation            getObservation() { return _observation; }
    public  void                   setObservation(Observation o) { _observation = o; }

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

    // OCResource uint32_t sequenceNum -out of place, this is a Message serial
    // public int          getSerial();      // for observables, https://tools.ietf.org/html/rfc7641

    private List<ActionSet>        _actionSet;
    public  List<ActionSet>        getActionSet() { return _actionSet; }

    // black boxing
    native   public void           exhibitStimulus(); // called by user
    // observeBehavior: implemented by user, called by stack
    abstract public int            observeBehavior(ObservationIn observationIn);
}
