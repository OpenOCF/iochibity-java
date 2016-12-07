package org.iochibity;

public class ResourceManager
{
    // OCGetNumberOfResources
    public static native int resourceCount();

    // OCSetDefaultDeviceEntityHandler
    public static native void registerDefaultServiceRoutine
	(Object /*OCDeviceEntityHandler*/ serviceRoutine,
	 Object /*void* */ callbackParam);

    // public static native int OCCreateResource(Object /*OCResourceHandle* */ handle,
    public static native Resource registerResource(String Uri,
						   String[] resourceTypeName,
						   String[] resourceInterfaceName,
						   ResourceServiceProvider serviceRoutine,
						   Object /*void* */ callbackParam,
						   byte   /*uint8_t*/ policies);

    ////////////////////////////////////////////////////////////////
    // stack/internal/ocresourcehandler.h:
    // OCResource *FindResourceByUri(const char* resourceUri);
    // bool IsVirtualResource(const char* resourceUri);
    // OCStackResult GetPlatformInfo(OCPlatformInfo** info);

}
