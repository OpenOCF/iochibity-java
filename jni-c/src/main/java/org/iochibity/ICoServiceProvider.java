package org.iochibity;

// client
// All methods EXCEPT observeBehavior implemented by CoServiceProvider abstract class
public interface ICoServiceProvider {

    // public long                   getHandle(); // OCDoHandle

    // public StimulusOut            getStimulusOut();

    public String                 getUriPath();
    public void                   setUriPath(String theUri);

    // setters/getters for OCDoResource params
    public int                    getMethod();
    public void                   setMethod(int theMethod);

    // setDest(MULTICAST);
    public DeviceAddress          getDestination();
    public void                   setDestination(DeviceAddress da);

    public void                   setObservation(Observation observation);

    // OCTransportAdapter: OC_ADAPTER_IP, etc.
    public int                    getNetworkProtocol();
    public void                   setNetworkProtocol(int protocol);

    public int                    getNetworkScope();
    public void                   setNetworkScope(int scope);

    public int                    getNetworkPolicies();
    public void                   setNetworkPolicies(int policy);

    public boolean                isTransportSecure();
    public void                   setTransportSecurity(boolean secure);

    public int                    getQualityOfService();
    public void                   setQualityOfService(int qos);

    // to add: fields from struct OCClientResponse? they will be
    // updated on receipt of ObservationIn

    // what about struct ClientCB?

    // black boxing
    public int                    observeBehavior(ObservationIn observationIn); // implemented by user
    public void                   exhibitStimulus(); // called by user
}
