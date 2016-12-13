package org.iochibity.constants;

public class Method
{
    public static final int NOMETHOD       = 0;

    public static final int GET            = (1 << 0);

    public static final int PUT            = (1 << 1);

    public static final int POST           = (1 << 2);

    public static final int DELETE         = (1 << 3);

    /** Register observe request for most up date notifications ONLY.*/
    public static final int OBSERVE        = (1 << 4);

    /** Register observe request for all notifications, including stale notifications.*/
    public static final int OBSERVE_ALL    = (1 << 5);

    /** De-register observation, intended for internal use.*/
    public static final int CANCEL_OBSERVE = (1 << 6);

    // #ifdef WITH_PRESENCE
    /** Subscribe for all presence notifications of a particular resource.*/
    public static final int PRESENCE       = (1 << 7);
    // #endif

    /** Allows OCDoResource caller to do discovery.*/
    public static final int DISCOVER       = (1 << 8);
}


