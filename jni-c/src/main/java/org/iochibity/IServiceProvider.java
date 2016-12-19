package org.iochibity;

import java.util.List;

public interface IServiceProvider {

    public long                   getHandle(); // OCResource*

    // C API: OCEntityHandler => OCEntityHandlerResult */
    public int                    serviceRequestIn(MsgRequestIn request);

    // OCResource fields
    public InstanceId             getInstanceId();

    public String                 getUriPath();
    public void                   setUriPath(String theUri);

    public List<String>           getTypes();
    public boolean                addType(String t);

    public List<String>           getInterfaces();
    public boolean                addInterface(String iface);

    public PropertyMap            getProperties();
    public Object                 putProperty(String key, Object val);

    public List<IServiceProvider> getChildren();

    // OCResource uint32_t sequenceNum -out of place, this is a Message serial
    // public int          getSerial();      // for observables, https://tools.ietf.org/html/rfc7641

    public List<ActionSet>        getActionSet();
    public int                    getPolicies(); // ???
}
