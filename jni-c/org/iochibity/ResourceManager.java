package org.iochibity;

public class ResourceManager
{
    // OCGetNumberOfResources
    public static native int resourceCount();

    // OCSetDefaultDeviceEntityHandler
    public static native int registerDefaultServiceRoutine
	(Object /*OCDeviceEntityHandler*/ serviceRoutine,
	 Object /*void* */ callbackParam);

    // public static native int OCCreateResource(Object /*OCResourceHandle* */ handle,
    public static native int registerResource(Resource /*OCResourceHandle* */ handle,
					      String resourceTypeName,
					      String resourceInterfaceName,
					      String Uri,
					      ResourceServiceProvider serviceRoutine,
					      Object /*void* */ callbackParam,
					      byte   /*uint8_t*/ resourceProperties);
}
