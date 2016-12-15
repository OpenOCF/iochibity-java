package org.iochibity;

import java.util.List;
import java.util.LinkedList;

public class PayloadForPlatform extends Payload
{
    // octypes.h:
    // typedef struct OCPlatformPayload
    // {
    //     OCPayload base;
    //     char* uri;
    private String _uri = "";
    public String getUri() { return _uri; }
    public void   setUri(String theUri) { _uri = theUri; }

    //     OCStringLL* rt;
    private List<String> _rtypes = new LinkedList<String>();
    public  List<String> getResourceTypes() { return _rtypes; }
    // do we need setTypes if we have addType:
    public native void setResourceTypes(LinkedList<String> rts);
    public void addResourceType(String rt) { _rtypes.add(rt); }
    // to add/rem a type: get the list, add/remove, then setTypes

    //     OCStringLL* interfaces;
    private List<String> _interfaces;
    public  List<String> getInterfaces() { return _interfaces; }
    public native void setInterfaces(LinkedList<String> ifs);
    public void addInterface(String iface) { _interfaces.add(iface); }

    //     OCPlatformInfo info;
    // We don't support OCPlatform info; instead we transform it to a property map
    private PropertyMap<String, Object> _properties;
    @Override
    public PropertyMap<String, Object> getProperties() { return _properties; }
    public native void setProperties(PropertyForResource ps);
    public void putProperty(String name, Object value) { _properties.put(name, value); }



    // ocpayload.h
    // OC_EXPORT OCPlatformPayload* OCPlatformPayloadCreate(const OCPlatformInfo* platformInfo);
    // OC_EXPORT OCPlatformPayload* OCPlatformPayloadCreateAsOwner(OCPlatformInfo* platformInfo);
    // OC_EXPORT void OCPlatformInfoDestroy(OCPlatformInfo *info);
    // OC_EXPORT void OCPlatformPayloadDestroy(OCPlatformPayload* payload);
}
