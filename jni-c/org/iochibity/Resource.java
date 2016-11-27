package org.iochibity;

import java.util.LinkedList;

public class Resource
{
    public long handle;

    // typedef struct OCResource {

    //     /** Points to next resource in list.*/
    //     struct OCResource *next;

    //     /** Relative path on the device; will be combined with base url to create fully qualified path.*/
    //     char *uri;
    public String uri;

    //     /** Resource type(s); linked list.*/
    //     OCResourceType *rsrcType;
    public native LinkedList<String> getTypes();

    //     /** Resource interface(s); linked list.*/
    //     OCResourceInterface *rsrcInterface;
    public native LinkedList<String> getInterfaces();

    //     OCAttribute *rsrcAttributes;
    public native LinkedList<ResourceProperty> getProperties();

    // Array of pointers to resources; can be used to represent a container of resources.
    // (i.e. hierarchies of resources) or for reference resources (i.e. for a resource collection).

    //     /** Child resource(s); linked list.*/
    //     OCChildResource *rsrcChildResourcesHead;
    public native LinkedList<Resource> getChildren();

    //     /** Pointer to function that handles the entity bound to the resource.
    //      *  This handler has to be explicitly defined by the programmer.*/
    //     OCEntityHandler entityHandler;
    public native ResourceServiceProvider getServiceProvider();

    //     /** Callback parameter.*/
    //     void * entityHandlerCallbackParam;
    public native Object getCallbackParam();

    //     /** Properties on the resource – defines meta information on the resource.
    //      * (ACTIVE, DISCOVERABLE etc ). */

    //     OCResourceProperty resourceProperties ;
    public native int getPolicies();

    //     /* @note: Methods supported by this resource should be based on the interface targeted
    //      * i.e. look into the interface structure based on the query request Can be removed here;
    //      * place holder for the note above.*/
    //     /* method_t methods; */

    //     /** Sequence number for observable resources. Per the CoAP standard it is a 24 bit value.*/
    //     uint32_t sequenceNum;
    public int sequenceNbr;

    //     /** Pointer of ActionSet which to support group action.*/
    //     OCActionSet *actionsetHead;

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
}
