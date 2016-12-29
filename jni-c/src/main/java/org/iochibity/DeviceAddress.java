package org.iochibity;


// FIXME: make this thread safe: getters only. No setters allowed.
// Updates must go through (CoServiceProvider.
public class DeviceAddress	// OCDevAddr
{

    private long handle; // ref to TLS var in underlying C struct?

    native public int       networkProtocol();   // OCTransportAdapter adapter;

    native public int       networkFlags(); // OCTransportFlags bitmap "flags"

    //native public byte      networkPolicies(); // OCTransportFlags flags & 0x00FF
    native public boolean   transportIsSecure(); // flags & 0x0008
    native public boolean   isIPv4();	         // flags & 0x0010
    native public boolean   isIPv6();            // flags & 0x0020
    native public boolean   isMulticast();       // flags & 0x0040

    native public byte      networkScope();      // flags & 0x000F



    native public int       port();		// uint16_t

    native public String    ipAddress();          // char addr[MAX_ADDR_STR_SIZE];

    native public int       ifindex();		// uint32_t, usually zero for default interface.

    // /* GAR: FIXME: this allows stack and app code to differ */
    // #if defined (ROUTING_GATEWAY) || defined (ROUTING_EP)
    //     char                    routeData[MAX_ADDR_STR_SIZE]; //destination GatewayID:ClientId
    native public String    routeData();
    // #endif
    // } OCDevAddr;
}
