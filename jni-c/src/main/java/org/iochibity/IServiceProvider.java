package org.iochibity;

public interface IServiceProvider {
    // C API: OCEntityHandler
    // typedef OCEntityHandlerResult (*OCEntityHandler)
    // (OCEntityHandlerFlag flag, OCEntityHandlerRequest * entityHandlerRequest, void* callbackParam);

    // private long resourceHandle = 0;

    /*OCEntityHandlerResult*/
    public int serviceRequestIn(MsgRequestIn request);
}
