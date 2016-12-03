package org.iochibity;

import java.util.HashMap;
import java.util.LinkedList;

public class PayloadResource extends Payload // OCRepPayload
{
    // octypes.h:
    // typedef struct OCRepPayload
    // {
    //     OCPayload base;
    //     char* uri;
    public String uri;
    //     OCStringLL* types;
    // typedef struct OCStringLL
    // {
    //     struct OCStringLL *next;
    //     char* value;
    // } OCStringLL;

    public native LinkedList<String> getTypes();
    //     OCStringLL* interfaces;
    public native LinkedList<String> getInterfaces();

    //     OCRepPayloadValue* values;
    public native HashMap getProperties();

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
    public native boolean setUri(PayloadResource payload, String uri);

    // OC_EXPORT bool OCRepPayloadAddResourceType(OCRepPayload* payload, const char* resourceType);
    // OC_EXPORT bool OCRepPayloadAddInterface(OCRepPayload* payload, const char* iface);
    // OC_EXPORT bool OCRepPayloadAddModelVersion(OCRepPayload* payload, const char* dmv);

    // OC_EXPORT bool OCRepPayloadAddResourceTypeAsOwner(OCRepPayload* payload, char* resourceType);
    // OC_EXPORT bool OCRepPayloadAddInterfaceAsOwner(OCRepPayload* payload, char* iface);

    // OC_EXPORT bool OCRepPayloadIsNull(const OCRepPayload* payload, const char* name);
    // OC_EXPORT bool OCRepPayloadSetNull(OCRepPayload* payload, const char* name);

    // OCRepPayloadSetPropInt
    // OC_EXPORT bool OCRepPayloadSetPropInt(OCRepPayload* payload, const char* name, int64_t value);
    public native boolean putInt(String name, long value); // int64_t

    // OCRepPayloadGetPropInt
    // OC_EXPORT bool OCRepPayloadGetPropInt(const OCRepPayload* payload, const char* name, int64_t* value);
    // public native boolean getInt(String name, long value); // int64_t*
    public native int getInt(String name);

    // OCRepPayloadSetPropDouble
    public native boolean setPropDouble(String name, double value);

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
