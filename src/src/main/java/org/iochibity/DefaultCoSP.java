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

    public int coReact()
    {
	System.out.println("DefaultCoSP: DefaultCoSP.coReact ENTRY");
	return 0;
    }
}

