package org.iochibity;

public class DeviceAddress	// OCDevAddr
{
    // typedef struct
    // {

    public int networkProtocol;	       // OCTransportAdapter adapter;

    public byte     networkPolicies;   // OCTransportFlags flags >> 4

    public byte     networkScope;      // flags && 0x000F

    public boolean  transportSecurity; // flags && 0x0010 : OC_FLAG_SECURE = (1 << 4)

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
