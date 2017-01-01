package org.iochibity;

// client
// All methods EXCEPT observeBehavior implemented by CoServiceProvider abstract class
public interface ICoServiceProvider {

    // black boxing

    public void               coreact();   // implemented by user

    // the remaining methods are implemented (natively)by CoServiceProvider
    public void               exhibit(); // called by user

    // OCDoResource params
    public String             uriPath();
    public ICoServiceProvider uriPath(String theUri);

    public int                method();
    public ICoServiceProvider method(int m);

    //////////////// NETWORKING params ////////////////
    // Raw
    public int                networkAdapter();
    public int                networkFlags();

    // networkFlags, broken out
    public boolean            transportIsSecure();
    public ICoServiceProvider transportIsSecure(boolean torf);

    // Transport Protocol flags: mutually exclusive; setting one resets all the others.
    public boolean            transportIsUDP();
    public ICoServiceProvider transportIsUDP(boolean torf);

    public boolean            transportIsTCP();
    public ICoServiceProvider transportIsTCP(boolean torf);

    public boolean            transportIsGATT();
    public ICoServiceProvider transportIsGATT(boolean torf);

    public boolean            transportIsRFCOMM();
    public ICoServiceProvider transportIsRFCOMM(boolean torf);

    public boolean            transportIsNFC();
    public ICoServiceProvider transportIsNFC(boolean torf);

    // IP Protocol flags: only needed to select version of IP protocol
    public boolean            networkIsIP(); // == transportIsUDP
    public ICoServiceProvider networkIsIP(boolean torf);

    public boolean            networkIsIPv4();
    public ICoServiceProvider networkIsIPv4(boolean torf);

    public boolean            networkIsIPv6();
    public ICoServiceProvider networkIsIPv6(boolean torf);

    // IPv6 only:
    public boolean            scopeIsInterface();
    public ICoServiceProvider scopeIsInterface(boolean torf);

    public boolean            scopeIsLink();
    public ICoServiceProvider scopeIsLink(boolean torf);

    public boolean            scopeIsRealm();
    public ICoServiceProvider scopeIsRealm(boolean torf);

    public boolean            scopeIsAdmin();
    public ICoServiceProvider scopeIsAdmin(boolean torf);

    public boolean            scopeIsSite();
    public ICoServiceProvider scopeIsSite(boolean torf);

    public boolean            scopeIsOrg();
    public ICoServiceProvider scopeIsOrg(boolean torf);

    public boolean            scopeIsGlobal();
    public ICoServiceProvider scopeIsGlobal(boolean torf);

    // Routing
    public boolean            routingIsMulticast();
    public ICoServiceProvider routingIsMulticast(boolean torf);

    public int                qualityOfService();
    public ICoServiceProvider qualityOfService(int qos);

    // to add: fields from struct OCClientResponse? they will be
    // updated on receipt of ObservationRecordIn
    public DeviceAddress              coAddress(); // native

    // what about struct ClientCB?
}
