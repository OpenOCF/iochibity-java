package org.iochibity.test.client;

import org.iochibity.CoServiceProvider;
import org.iochibity.OCF;

public class DefaultCoSP
    extends  CoServiceProvider
{
    public DefaultCoSP() {
	super();
	method(OCF.RETRIEVE);
    }

    public void coreact()
    {
	System.out.println("DefaultCoSP: DefaultCoSP.coreact ENTRY");
    }
}

