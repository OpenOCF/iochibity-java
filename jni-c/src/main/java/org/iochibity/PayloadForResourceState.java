package org.iochibity;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;

public class PayloadForResourceState extends Payload // OCRepPayload
{
    // ctor
    // OC_EXPORT OCRepPayload* OCRepPayloadCreate();
    // private native long createPayloadForResourceState();
    // public PayloadForResourceState()
    // {
    // 	handle = createPayloadForResourceState();
    // }
    // wraps BuildResponseRepresentation of ocresourcehandler.h/ocresource.c

    // ctor
    public PayloadForResourceState() {}

    // ctor
    public native PayloadForResourceState createPayload(Resource r);
    public PayloadForResourceState(Resource r)
    {
	createPayload(r);
    }

    // octypes.h:
    // typedef struct OCRepPayload
    // {
    //     OCPayload base;
    //     char* uri;
    private String _uri = "";
    public String getUri() { return _uri; }
    public void   setUri(String theUri) { _uri = theUri; }

    // Resource Types
    // OCStringLL* types;  // next, value
    private List<String> _rtypes = new LinkedList<String>();
    public  List<String> getResourceTypes() { return _rtypes; }
    // do we need setTypes if we have addType:
    public native void setResourceTypes(LinkedList<String> rts);
    public void addResourceType(String rt) { _rtypes.add(rt); }
    // to add/rem a type: get the list, add/remove, then setTypes

    // Resource Interfaces
    //     OCStringLL* interfaces;
    private List<String> _interfaces;
    public  List<String> getInterfaces() { return _interfaces; }
    public native void setInterfaces(LinkedList<String> ifs);
    public void addInterface(String iface) { _interfaces.add(iface); }

    // Resource Properties
    //     OCRepPayloadValue* values;
    private PropertyMap<String, Object> _properties;
    @Override
    public PropertyMap<String, Object> getProperties() { return _properties; }
    public native void setProperties(PropertyForResource ps);
    public void putProperty(String name, Object value) { _properties.put(name, value); }

    //     struct OCRepPayload* next;
    // } OCRepPayload;

// typedef struct OCRepPayloadValue
// {
//     char* name;
//     OCRepPayloadPropType type;
//     union
//     {
//         int64_t i;
//         double d;
//         bool b;
//         char* str;

//         /** ByteString object.*/
//         OCByteString ocByteStr;

//         struct OCRepPayload* obj;
//         OCRepPayloadValueArray arr;
//     };
//     struct OCRepPayloadValue* next;

// } OCRepPayloadValue;

// octypes.h:
// typedef struct
// {
//     OCRepPayloadPropType type;
//     size_t dimensions[MAX_REP_ARRAY_DEPTH];

//     union
//     {
//         int64_t* iArray;
//         double* dArray;
//         bool* bArray;
//         char** strArray;

//         /** pointer to ByteString array.*/
//         OCByteString* ocByteStrArray;

//         struct OCRepPayload** objArray;
//     };
// } OCRepPayloadValueArray;


// octypes.h:
// typedef enum
// {
//     OCREP_PROP_NULL,
//     OCREP_PROP_INT,
//     OCREP_PROP_DOUBLE,
//     OCREP_PROP_BOOL,
//     OCREP_PROP_STRING,
//     OCREP_PROP_BYTE_STRING,
//     OCREP_PROP_OBJECT,
//     OCREP_PROP_ARRAY
// }OCRepPayloadPropType;

    // ocpayload.h:
    // OC_EXPORT OCRepPayload* OCRepPayloadCreate();

    // OC_EXPORT size_t calcDimTotal(const size_t dimensions[MAX_REP_ARRAY_DEPTH]);

    // OC_EXPORT OCRepPayload* OCRepPayloadClone(const OCRepPayload* payload);

    // OC_EXPORT void OCRepPayloadAppend(OCRepPayload* parent, OCRepPayload* child);

    // OC_EXPORT bool OCRepPayloadSetUri(OCRepPayload* payload, const char* uri);
    // public native boolean setUri(PayloadForResourceState payload, String uri);

    // OC_EXPORT bool OCRepPayloadAddResourceType(OCRepPayload* payload, const char* resourceType);
    // OC_EXPORT bool OCRepPayloadAddInterface(OCRepPayload* payload, const char* iface);
    // OC_EXPORT bool OCRepPayloadAddModelVersion(OCRepPayload* payload, const char* dmv);

    // OC_EXPORT bool OCRepPayloadAddResourceTypeAsOwner(OCRepPayload* payload, char* resourceType);
    // OC_EXPORT bool OCRepPayloadAddInterfaceAsOwner(OCRepPayload* payload, char* iface);

    // OC_EXPORT bool OCRepPayloadIsNull(const OCRepPayload* payload, const char* name);
    // OC_EXPORT bool OCRepPayloadSetNull(OCRepPayload* payload, const char* name);

    // OCRepPayloadSetPropInt
    // OC_EXPORT bool OCRepPayloadSetPropInt(OCRepPayload* payload, const char* name, int64_t value);
    // public native boolean put(String name, long value); // int64_t
    // public native boolean put(String name, String value);

    // OCRepPayloadGetPropInt
    // OC_EXPORT bool OCRepPayloadGetPropInt(const OCRepPayload* payload, const char* name, int64_t* value);
    // public native boolean getInt(String name, long value); // int64_t*
    // public native int get(String name);

    // OCRepPayloadSetPropDouble
    // public native boolean setPropDouble(String name, double value);

    // OCRepPayloadGetPropDouble
    // bool OCRepPayloadGetPropDouble(const OCRepPayload* payload, const char* name, double* value)
    // public native double getPropDouble(String name) throws
    // 	OCFTypeException,	// the value found is not a double nor an int
    // 	NoSuchElementException; // there is no such value

    // OC_EXPORT bool OCRepPayloadSetPropByteString(OCRepPayload* payload, const char* name, OCByteString value);

    // OC_EXPORT bool OCRepPayloadSetPropByteStringAsOwner(OCRepPayload* payload, const char* name,
    //         OCByteString* value);
    // OC_EXPORT bool OCRepPayloadGetPropByteString(const OCRepPayload* payload, const char* name,
    //         OCByteString* value);

    // OC_EXPORT bool OCRepPayloadSetPropString(OCRepPayload* payload, const char* name, const char* value);
    // OC_EXPORT bool OCRepPayloadSetPropStringAsOwner(OCRepPayload* payload, const char* name, char* value);
    // OC_EXPORT bool OCRepPayloadGetPropString(const OCRepPayload* payload, const char* name, char** value);

    // OC_EXPORT bool OCRepPayloadSetPropBool(OCRepPayload* payload, const char* name, bool value);
    // OC_EXPORT bool OCRepPayloadGetPropBool(const OCRepPayload* payload, const char* name, bool* value);

    // OC_EXPORT bool OCRepPayloadSetPropObject(OCRepPayload* payload, const char* name, const OCRepPayload* value);
    // OC_EXPORT bool OCRepPayloadSetPropObjectAsOwner(OCRepPayload* payload, const char* name, OCRepPayload* value);
    // OC_EXPORT bool OCRepPayloadGetPropObject(const OCRepPayload* payload, const char* name, OCRepPayload** value);

    // OC_EXPORT bool OCRepPayloadSetByteStringArrayAsOwner(OCRepPayload* payload, const char* name,
    //         OCByteString* array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);
    // OC_EXPORT bool OCRepPayloadSetByteStringArray(OCRepPayload* payload, const char* name,
    //         const OCByteString* array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);

    // OC_EXPORT bool OCRepPayloadGetByteStringArray(const OCRepPayload* payload, const char* name,
    //         OCByteString** array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);

    // OC_EXPORT bool OCRepPayloadSetIntArrayAsOwner(OCRepPayload* payload, const char* name,
    //         int64_t* array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);
    // OC_EXPORT bool OCRepPayloadSetIntArray(OCRepPayload* payload, const char* name,
    //         const int64_t* array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);
    // OC_EXPORT bool OCRepPayloadGetIntArray(const OCRepPayload* payload, const char* name,
    //         int64_t** array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);

    // OC_EXPORT bool OCRepPayloadSetDoubleArrayAsOwner(OCRepPayload* payload, const char* name,
    //         double* array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);
    // OC_EXPORT bool OCRepPayloadSetDoubleArray(OCRepPayload* payload, const char* name,
    //         const double* array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);
    // OC_EXPORT bool OCRepPayloadGetDoubleArray(const OCRepPayload* payload, const char* name,
    //         double** array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);

    // OC_EXPORT bool OCRepPayloadSetStringArrayAsOwner(OCRepPayload* payload, const char* name,
    //         char** array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);
    // OC_EXPORT bool OCRepPayloadSetStringArray(OCRepPayload* payload, const char* name,
    //         const char** array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);
    // OC_EXPORT bool OCRepPayloadGetStringArray(const OCRepPayload* payload, const char* name,
    //         char*** array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);

    // OC_EXPORT bool OCRepPayloadSetBoolArrayAsOwner(OCRepPayload* payload, const char* name,
    //         bool* array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);
    // OC_EXPORT bool OCRepPayloadSetBoolArray(OCRepPayload* payload, const char* name,
    //         const bool* array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);
    // OC_EXPORT bool OCRepPayloadGetBoolArray(const OCRepPayload* payload, const char* name,
    //         bool** array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);

    // OC_EXPORT bool OCRepPayloadSetPropObjectArrayAsOwner(OCRepPayload* payload, const char* name,
    //         OCRepPayload** array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);
    // OC_EXPORT bool OCRepPayloadSetPropObjectArray(OCRepPayload* payload, const char* name,
    //         const OCRepPayload** array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);
    // OC_EXPORT bool OCRepPayloadGetPropObjectArray(const OCRepPayload* payload, const char* name,
    //         OCRepPayload*** array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);

    // OC_EXPORT void OCRepPayloadDestroy(OCRepPayload* payload);

}
