package org.iochibity.constants;

public class OCStackResult {
    /** Success status code - START HERE.*/
    public static final char OK = 0;
    public static final char RESOURCE_CREATED = 1;
    public static final char RESOURCE_DELETED = 2;
    public static final char CONTINUE = 3;
    public static final char RESOURCE_CHANGED = 4;
    /** Success status code - END HERE.*/

    // /** Error status code - START HERE.*/
    // INVALID_URI(20),
    // INVALID_QUERY,
    // INVALID_IP,
    // INVALID_PORT,
    // INVALID_CALLBACK,
    // INVALID_METHOD,

    // /** Invalid parameter.*/
    // INVALID_PARAM,
    // INVALID_OBSERVE_PARAM,
    // NO_MEMORY,
    // COMM_ERROR,
    // TIMEOUT,
    // ADAPTER_NOT_ENABLED,
    // NOTIMPL,

    // /** Resource not found.*/
    // NO_RESOURCE,

    // /** e.g: not supported method or interface.*/
    // RESOURCE_ERROR,
    // SLOW_RESOURCE,
    // DUPLICATE_REQUEST,

    // /** Resource has no registered observers.*/
    // NO_OBSERVERS,
    // OBSERVER_NOT_FOUND,
    // VIRTUAL_DO_NOT_HANDLE,
    // INVALID_OPTION,

    // /** The remote reply contained malformed data.*/
    // MALFORMED_RESPONSE,
    // PERSISTENT_BUFFER_REQUIRED,
    // INVALID_REQUEST_HANDLE,
    // INVALID_DEVICE_INFO,
    // INVALID_JSON,

    // /** Request is not authorized by Resource Server. */
    // UNAUTHORIZED_REQ,
    // TOO_LARGE_REQ,

    // /** Error code from PDM */
    // PDM_IS_NOT_INITIALIZED,
    // DUPLICATE_UUID,
    // INCONSISTENT_DB,

    // /**
    //  * Error code from OTM
    //  * This error is pushed from DTLS interface when handshake failure happens
    //  */
    // AUTHENTICATION_FAILURE,

    // /** Insert all new error codes here!.*/
    // #ifdef WITH_PRESENCE
    // PRESENCE_STOPPED(128),
    // PRESENCE_TIMEOUT,
    // PRESENCE_DO_NOT_HANDLE,
    // #endif
    /** ERROR in stack.*/
    public static final char ERROR = 255;
    /** Error status code - END HERE.*/

    // private int val;
    // private OCStackResult(int val) { this.val = val; }
}

