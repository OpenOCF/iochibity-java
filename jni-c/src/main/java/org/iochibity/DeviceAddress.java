package org.iochibity;

public class DeviceAddress	// OCDevAddr
{

    private long handle; // ref to TLS var in underlying C struct?

    native public int       networkProtocol();   // OCTransportAdapter adapter;
    // OC_ADAPTER_IP, OC_ADAPTER_GATT_BLE, etc.

    native public int       networkFlags();  // OCTransportFlags flags

    // FIXME: support the predicates from CoServiceProvider e.g. transportIsUDP etc.
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
