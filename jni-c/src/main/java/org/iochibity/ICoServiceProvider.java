package org.iochibity;

// client
// All methods EXCEPT observeBehavior implemented by CoServiceProvider abstract class
public interface ICoServiceProvider {

    // black boxing

    // implemented by user: reaction to observed behavior (stimulus or response)
    public void                   react();
    // public int                    observeBehavior(ObservationIn observationIn); // implemented by user

    // called by user:
    public void                   exhibit(); // native routine, called by user
    // public void                   exhibitStimulus(); // called by user

    public String                 uriPath(); // native
    // public String                 getUriPath();
    public void                   setUriPath(String theUri);

    // setters/getters for OCDoResource params
    public int                    method(); // native
    public int                    getMethod();
    public void                   setMethod(int theMethod);

    // MULTICAST
    public int                    multicastProtocol(); // native

    // UNICAST
    public DeviceAddress          coAddress(); // native
    // public DeviceAddress          getDestination();
    // public void                   setDestination(DeviceAddress da); // native

    public void                   setMulticast(); // native

    // public void                   setObservationRecord(ObservationRecord observationRecord);

    // OCTransportAdapter: OC_ADAPTER_IP, etc.
    public int                    unicastProtocol(); // native

    // public int                    getNetworkProtocol();
    // public void                   setNetworkProtocol(int protocol);

    // public int                    networkScope(); // native
    // public int                    getNetworkScope();
    // public void                   setNetworkScope(int scope);

    // public int                    networkPolicies(); // native
    // public int                    getNetworkPolicies();
    // public void                   setNetworkPolicies(int policy);

    // public boolean                transportIsSecure(); // native
    // public boolean                isTransportSecure();
    // public void                   setTransportSecurity(boolean secure);

    public int                    getQualityOfService();
    public void                   setQualityOfService(int qos);

    // to add: fields from struct OCClientResponse? they will be
    // updated on receipt of ObservationRecordIn

    // what about struct ClientCB?
}
