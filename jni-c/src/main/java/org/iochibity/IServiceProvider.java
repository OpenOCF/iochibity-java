package org.iochibity;

import java.util.List;

// All methods EXCEPT react are implemented by ServiceProvider abstract class
public interface IServiceProvider {

    public long                   getHandle(); // OCResource*

    public void                   exhibit();  // Behavior(); // called by user
    public void                   react(); // called by stack

    // OCResource fields
    public LinkId                 getLinkId();

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

    // from incoming request:
    public int              method();
    
}

