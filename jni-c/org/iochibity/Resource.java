package org.iochibity;

import org.iochibity.exceptions.OCFNotImplementedException;

import java.util.LinkedList;
import java.util.UUID;

public class Resource
{
    // public void Resource(Resource r)
    // {
    // 	handle = r.handle;
    // 	uri    = r.uri;
    // }

    public long handle;		// this OCResource*

    // Points to next resource in list.
    // struct OCResource *next;
    public native long getNext();

    // Relative path on the device; will be combined with base url to create fully qualified path.
    public String uri;

    // OCResourceType *rsrcType;  /* linked list */
    public native LinkedList<String> getTypes();

    // OCResourceInterface *rsrcInterface;  /* linked list */
    public native LinkedList<String> getInterfaces();

    // OCAttribute *rsrcAttributes;  /* linked list */
    // NB: this does not seem to be set anywhere by the stack or example pgms
    // OCAttribute only holds string values
    public native LinkedList<Property> getProperties();

    // Array of pointers to resources; can be used to represent a container of resources.
    // (i.e. hierarchies of resources) or for reference resources (i.e. for a resource collection).
    // OCChildResource *rsrcChildResourcesHead;   /* linked list */
    public native LinkedList<Resource> getChildren();

    // Pointer to function that handles the entity bound to the resource.
    // This handler has to be explicitly defined by the programmer.
    // OCEntityHandler entityHandler;   /* fn pointer */
    // we instead return the ResourceServiceProvider object
    public native ResourceServiceProvider getServiceProvider();

    // Callback parameter.*/
    // void * entityHandlerCallbackParam;
    public native CallbackParam getCallbackParam();

    // Properties on the resource – defines meta information on the resource.
    // (ACTIVE, DISCOVERABLE etc ).  NB: we call these Policies, per OCF, not "properties"
    // OCResourceProperty resourceProperties ;
    public int policies; //  see constants.ResourcePolicy

    /* @note: Methods supported by this resource should be based on the interface targeted
     * i.e. look into the interface structure based on the query request Can be removed here;
     * place holder for the note above.*/
    /* method_t methods; */

    // Sequence number for observable resources. Per the CoAP standard it is a 24 bit value.*/
    // uint32_t sequenceNum;
    public int sn;		// abbrev: Serial Number

    // Pointer of ActionSet which to support group action.*/
    // OCActionSet *actionsetHead;  /* linked list */
    public native LinkedList<ActionSet> getActionSet() throws OCFNotImplementedException;

    //     /** The instance identifier for this web link in an array of web links - used in links. */
    //     union
    //     {
    //         /** An ordinal number that is not repeated - must be unique in the collection context. */
    //         uint8_t ins;
    //         /** Any unique string including a URI. */
    //         char *uniqueStr;
    //         /** Use UUID for universal uniqueness - used in /oic/res to identify the device. */
    //         OCIdentity uniqueUUID;
    //     };
    // } OCResource;
    public class InstanceId {};
    public class InstanceOrdinal extends InstanceId { public int val = 0; }
    public class InstanceString  extends InstanceId { public String val; }
    public class InstanceUuid    extends InstanceId { public UUID val; }
    public InstanceId id = new InstanceId();

    ////////////////////////////////////////////////////////////////
    /*  ocresource.h: */
    // OCStackResult BuildResponseRepresentation(const OCResource *resourcePtr,
    // 					      OCRepPayload** payload)
    public native LinkedList<PayloadResource> getPayloads();

}
