package org.iochibity;

import java.util.List;

// Payload types in C API:
// OCRepPayload - resource representation including properties
// OCResourcePayload    - resource meta, excluding properties
// OCPlatformPayload    - platform discovery
// OCDevicePayload      - device discovery
// OCDiscoveryPayload   - resource discovery
// OCRDDiscoveryPayload - Resource Directory discovery
// OCRDPayload          - Resource Directory
// OCPresencePayload    -
// OCSecurityPayload
// OCResourceCollectionPayload - tags plus links
// OCTagsPayload  - substruct used in OCResourceCollectionPayload
// OCLinksPayload - substruct used in OCResourceCollectionPayload

public interface IObservationRecord
{
    public long getHandle();	// ptr to c struct

    public int getType();	// OCPayloadType

    // ****************************************************************
    // uriPath included in: OCRepPayload, OCResourcePayload,
    // OCLinksPayload (href), OCDiscoveryPayload, OCPlatformPayload
    // NOT included in: OCTagsPayload, OCResourceCollectionPayload,
    // OCRDDiscoveryPayload, OCRDPayload, OCDevicePayload,
    // OCSecurityPayload, OCPresencePayload
    public String getUriPath();
    public void   setUriPath(String theUri);

    // ****************************************************************
    // rtypes included in: OCRepPayload, OCResourcePayload,
    // OCLinksPayload, OCDiscoveryPayload, OCDevicePayload,
    // OCPlatformPayload

    // NOT included in: OCTagsPayload, OCResourceCollectionPayload,
    // OCRDDiscoveryPayload, OCRDPayload, OCSecurityPayload,
    // OCPresencePayload
    public List<String> getResourceTypes();
    // do we need setTypes if we have addType:
    public void setResourceTypes(List<String> rts);
    // public void addResourceType(String rt);

    // ****************************************************************
    // interfaces included in: OCRepPayload, OCResourcePayload,
    // OCLinksPayload, OCDiscoveryPayload, OCDevicePayload,
    // OCPlatformPayload

    // NOT included in: OCTagsPayload, OCResourceCollectionPayload,
    // OCRDDiscoveryPayload, OCRDPayload, OCSecurityPayload,
    // OCPresencePayload
    public List<String> getInterfaces();
    public void setInterfaces(List<String> ifs);
    // public void addInterface(String iface);

    // ****************************************************************
    // properties included in: OCRepPayload
    // indirectly included in: OCDevicePayload, OCPlatformPayload,
    // etc. - we create iotivity-specific properties
    public PropertyMap<String, Object> getProperties();
    // protected void setProperties(PropertyForResource ps);
    // public void putProperty(String name, Object value) { _properties.put(name, value); }

    // ****************************************************************
    // policies (DISCOVERABLE, OBSERVABLE, etc.)
    public long getPolicies();

    // ****************************************************************
    public List<IObservationRecord> getChildren();
}
