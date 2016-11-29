package org.iochibity;

public class DeviceAddress	// OCDevAddr
{
    // typedef struct
    // {
    // adapter type.
    // OCTransportAdapter      adapter;
    public int adapter;

    // transport modifiers.
    // OCTransportFlags        flags;
    public int flags;

    // for IP.
    // uint16_t                port;
    public int port;

    // address for all adapters.
    // char                    addr[MAX_ADDR_STR_SIZE];
    public String address;

    // usually zero for default interface.
    // uint32_t                ifindex;
    public int ifindex;

    // /* GAR: FIXME: this allows stack and app code to differ */
    // #if defined (ROUTING_GATEWAY) || defined (ROUTING_EP)
    //     char                    routeData[MAX_ADDR_STR_SIZE]; //destination GatewayID:ClientId
    public String routeData;
    // #endif
    // } OCDevAddr;
}
