package openocf.app;

import openocf.signals.InboundStimulus;
import openocf.utils.Endpoint;

// client
// All methods EXCEPT observeBehavior implemented by CoResourceSP abstract class
public interface ICoResourceSP {

    public int                coReact(InboundStimulus response);   // abstract, to be implemented (but never called) by user

    // the remaining methods are implemented (natively)by CoResourceSP
    // OCDoResource params
    public String             getUri();
    public ICoResourceSP      setUri(String theUri);

    public int                getMethod();
    public ICoResourceSP      setMethod(int m);

    //////////////// NETWORKING params ////////////////
    public String            getIPAddress();
    public String            getBLEAddress();
    public int               getPort();

    // Raw
    public int                getNetworkAdapter();
    public int                getNetworkFlags();

    // networkFlags, broken out
    public boolean            isTransportSecure();
    public ICoResourceSP      setTransportSecure(boolean torf);

    // Transport Protocol flags: mutually exclusive; setting one resets all the others.
    public boolean            isTransportUDP();
    public ICoResourceSP      setTransportUDP(boolean torf);

    public boolean            isTransportTCP();
    public ICoResourceSP      setTransportTCP(boolean torf);

    public boolean            isTransportGATT();
    public ICoResourceSP      setTransportGATT(boolean torf);

    public boolean            isTransportRFCOMM();
    public ICoResourceSP      setTransportRFCOMM(boolean torf);

    public boolean            isTransportNFC();
    public ICoResourceSP      setTransportNFC(boolean torf);

    // IP Protocol flags: only needed to select version of IP protocol
    public boolean            networkIsIP(); // == transportIsUDP
    public ICoResourceSP networkIsIP(boolean torf);

    public boolean            networkIsIPv4();
    public ICoResourceSP networkIsIPv4(boolean torf);

    public boolean            networkIsIPv6();
    public ICoResourceSP networkIsIPv6(boolean torf);

    // IPv6 only:
    public boolean            scopeIsInterface();
    public ICoResourceSP scopeIsInterface(boolean torf);

    public boolean            scopeIsLink();
    public ICoResourceSP scopeIsLink(boolean torf);

    public boolean            scopeIsRealm();
    public ICoResourceSP scopeIsRealm(boolean torf);

    public boolean            scopeIsAdmin();
    public ICoResourceSP scopeIsAdmin(boolean torf);

    public boolean            scopeIsSite();
    public ICoResourceSP scopeIsSite(boolean torf);

    public boolean            scopeIsOrg();
    public ICoResourceSP scopeIsOrg(boolean torf);

    public boolean            scopeIsGlobal();
    public ICoResourceSP scopeIsGlobal(boolean torf);

    // Routing
    public boolean            routingIsMulticast();
    public ICoResourceSP routingIsMulticast(boolean torf);

    public int                qualityOfService();
    public ICoResourceSP qualityOfService(int qos);

    // to add: fields from struct OCClientResponse? they will be
    // updated on receipt of ObservationRecordIn
    public Endpoint      coAddress(); // native

    // what about struct ClientCB?

    // for discovery and presence requests:
    public void               deactivate();
}
