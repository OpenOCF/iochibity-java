package org.iochibity;

public class Payload
{
    private long _handle;      // OCPayload*
    public  long getHandle() { return _handle; }

    // FIXME: make type private with getter
    public int type = 0;	// OCPayloadType

    // OCPayloadType enum
    public static final int INVALID        = 0;
    public static final int DISCOVERY      = 1;
    public static final int DEVICE         = 2;
    public static final int PLATFORM       = 3;
    public static final int REPRESENTATION = 4;
    public static final int SECURITY       = 5;
    public static final int PRESENCE       = 6;
    public static final int RD             = 7;
}
