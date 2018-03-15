// Note that we have several "endpoint" structs:

// ocf/ocendpoint.c:

// typedef struct CAEndpoint_s
// {
//     CATransportAdapter_t    adapter;    // adapter type
//     CATransportFlags_t      flags;      // transport modifiers
//     uint16_t                port;       // for IP
//     char                    addr[MAX_ADDR_STR_SIZE_CA]; // address for all
//     uint32_t                ifindex;    // usually zero for default interface
//     char                    remoteId[CA_MAX_IDENTITY_SIZE]; // device ID of remote device
// #if defined (ROUTING_GATEWAY) || defined (ROUTING_EP)
//     char                    routeData[MAX_ADDR_STR_SIZE_CA]; /**< GatewayId:ClientId of
//                                                                     destination. **/
// #endif
// } CAEndpoint_t;

// ocf/_protocol.h:
// typedef struct
// {
//     OCTransportAdapter      adapter;
//     OCTransportFlags        flags;
//     uint16_t                port;
//     char                    addr[MAX_ADDR_STR_SIZE];
//     uint32_t                ifindex;
//     char                    routeData[MAX_ADDR_STR_SIZE];
//     char                    remoteId[MAX_IDENTITY_SIZE];
// } OCDevAddr;

// NOTE: "OCDevAddr must be the same as CAEndpoint (in CACommon.h)." But
// routeData is not ifdef'ed in OCDevAddr, and the order is reversed.
/*
 * An OCDevAddr is a "Data structure to encapsulate
 * IPv4/IPv6/Contiki/lwIP device addresses. OCDevAddr must be the same
 * as CAEndpoint (in CACommon.h)."
 */

// ocf/ocpayload.c:

// typedef struct OCEndpointPayload
// {
//     char* tps;
//     char* addr;
//     OCTransportFlags family;
//     uint16_t port;
//     uint16_t pri;
//     struct OCEndpointPayload* next;
// } OCEndpointPayload;

// The difference: OCEndpointPayload does not contain an interface index, remote id, route

// And we often have e.g. OCDevAddr *endpoint, and things like:

// ocstack.c :: CopyDevAddrToEndpoint(const OCDevAddr *in, CAEndpoint_t *out)


// OCEndpointPayload is used inside OCResourcePayload.


// So we dispense with the "device" part and just call this an EndPoint

package openocf.utils;

public class Endpoint	// OCDevAddr
{

    private long handle; // ref to TLS var in underlying C struct?

    native public int       networkProtocol();   // OCTransportAdapter adapter;
    // OC_ADAPTER_IP, OC_ADAPTER_GATT_BLE, etc.

    native public int       networkFlags();  // OCTransportFlags flags

    // FIXME: support the predicates from CoResourceSP e.g. transportIsUDP etc.
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
