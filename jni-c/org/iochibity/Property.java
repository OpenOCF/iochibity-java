package org.iochibity;

public class Property		// OCAttribute
{
    // remove default ctor
    private Property() {}

    public Property(String n, String v) {
	name = n;
	value = v;
    }
    // typedef struct attr_t {

    // Points to next resource in list.*/
    // struct attr_t *next;

    // The name of the attribute; used to look up the attribute in list.
    // for a given attribute SHOULD not be changed once assigned.
    // const char *attrName;
    public String  name;

    // value of the attribute as string.*/
    // char *attrValue;
    public String value;
    // } OCAttribute;
}
