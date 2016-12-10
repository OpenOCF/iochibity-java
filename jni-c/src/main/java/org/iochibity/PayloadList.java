package org.iochibity;

import org.iochibity.constants.PropertyType;
import java.util.LinkedList;

public class PayloadList<Payload> extends LinkedList<Payload>
{
    private long head; // handle of containing payload

    // in struct OCRepPayload:
    //     OCRepPayloadValue* values;
    // private long handle;	// handle of "property" linked list in containing payload

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

}
