package org.iochibity;

public class PayloadForLinks extends Payload
{
    // typedef struct OCLinksPayload
    // {
    //     /** This is the target relative URI. */
    //     char *href;
    //     /** The relation of the target URI referenced by the link to the context URI;
    //      * The default value is null. */
    //     char *rel;
    //     /** Resource Type - A standard OIC specified or vendor defined resource
    //      * type of the resource referenced by the target URI. */
    //     OCStringLL *rt;
    //     /** Interface - The interfaces supported by the resource referenced by the target URI. */
    //     OCStringLL *itf;
    //     /** Bitmap - The bitmap holds observable, discoverable, secure option flag. */
    //     uint8_t p;
    //     /** A title for the link relation. Can be used by the UI to provide a context. */
    //     char *title;
    //     /** This is used to override the context URI e.g. override the URI of the containing collection. */
    //     char *anchor;
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
    //     /** Time to keep holding resource.*/
    //     uint64_t ttl;
    //     /** A hint of the media type of the representation of the resource referenced by the target URI. */
    //     OCStringLL *type;
    //     /** Holding address of the next resource. */
    //     struct OCLinksPayload *next;
    // } OCLinksPayload;
}
