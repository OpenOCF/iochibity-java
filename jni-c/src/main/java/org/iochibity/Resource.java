package org.iochibity;

import org.iochibity.exceptions.OCFNotImplementedException;

import java.util.LinkedList;
import java.util.UUID;

public class Resource
{
    // Policy constants (OCResourceProperty enum)
    public static final int NOMETHOD       = 0;
    public static final int NONE = 0;
    public static final int DISCOVERABLE  = (1 << 0);
    public static final int OBSERVABLE    = (1 << 1);
    public static final int ACTIVE        = (1 << 2);
    public static final int SLOW          = (1 << 3);
    public static final int SECURE        = (1 << 4);
    public static final int EXPLICIT_DISCOVERABLE   = (1 << 5);
}
