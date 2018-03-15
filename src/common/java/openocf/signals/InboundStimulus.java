package openocf.signals;

import openocf.app.IResourceSP;

/**
 * Wrapper on args to:
 *
 * typedef OCEntityHandlerResult (*OCEntityHandler)
 *                (OCEntityHandlerFlag flag,
 *                 OCEntityHandlerRequest * entityHandlerRequest,
 *                 void* callbackParam);
 *
 * InboundStimulus is passed to application-defined react routine
 */

// NB: client request arrives at server as OCServerRequest

public class InboundStimulus extends Observable
{
    // For internal (JNI) use only:
    private IResourceSP resourceSP; // JNI only


    // typedef struct  //OCEntityHandlerRequest
    // {

    // Handles for for internal (JNI) use only:
    // OCResourceHandle resource; => resourceHandle
    // OCRequestHandle requestHandle - i.e. handle of request at ORIGIN (i.e. client)?;
    // => remoteHandle

    // OCMethod method; // the REST method retrieved from received request PDU.
    private int _method;			// for OCEntityHandlerRequest, not OCClientResponse
    public int getMethod() { return _method; }; // IMessage

    // description of endpoint that sent the request.
    // OCDevAddr devAddr;
    // EXPOSED IN Observable.java

    // resource query sent by client.
    // char * query;
    private String _query;
    // from Message protocol?
    public String getQueryString() { return _query; }

    // Information associated with observation - valid only when OCEntityHandler flag includes
    // * ::OC_OBSERVE_FLAG.
    // (i.e. the flag passed to the resource provider service routine)
    // NOTE: - on the client side this is encoded as a header option
    //       - on server side, the header option is converted to the OCEntityHandler flag
    //         and the OCObservationInfo struct is filled out
    // NOTE: this is only for used register/deregister of watchers (observers)
    // OCObservationInfo obsInfo;
    // typedef struct
    // {
    // Action associated with observation request.*/
    // OCObserveAction action;  /* enum */
    // Identifier for observation being registered/deregistered.*/
    // OCObservationId obsId;   /* uint8_t */
    // } OCObservationInfo;
    // NOTE: C API uses "observe"; we use "watch"

    // NOTE: we encapsulate all this in methods:
    //     Method.WATCH = GET, plus OC_OBSERVE_FLAG, + REGISTER (what about obsId?)
    //     Method.UNWATCH = GET, plus OC_OBSERVE_FLAG, + DEREGISTER
    // and we ignore MQTT for now

    public int watchAction;  // REGISTER, DEREGISTER, NO_OPTION, MQ_SUBSCRIBER, MQ_UNSUBSCRIBER
    public int watchId;
    // // OCEntityHandlerFlag: OC_REQUEST_FLAG || OC_OBSERVE_FLAG
    // public boolean isWatch = false; //  false => isRequest

    // Number of the received vendor specific header options.*/
    // uint8_t numRcvdVendorSpecificHeaderOptions;
    // => Message.getOptions()

    // Pointer to the array of the received vendor specific header options.*/
    // OCHeaderOption * rcvdVendorSpecificHeaderOptions;
    // in MessageForResource
    // Messsage.getOptions.size()

    // Message id.
    // uint16_t messageID;
    private int _messageId;
    public  int getMessageId() { return _messageId; }

    // the payload from the request PDU.
    // OCPayload *payload;
    // => Message.getPayloadList();

    // } OCEntityHandlerRequest;

}
