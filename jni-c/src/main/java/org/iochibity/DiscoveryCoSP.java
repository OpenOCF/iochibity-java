package org.iochibity.test.client;

import org.iochibity.CoServiceProvider;
import org.iochibity.ServiceManager;

import org.iochibity.constants.Method;

public class DiscoveryCoSP
    extends  CoServiceProvider
{

    public DiscoveryCoSP() {
	super();
	method(Method.GET);
    }

    public void coReact()
    {
	System.out.println("DiscoveryCoSP: DiscoveryCoSP.coReact ENTRY");

	return;
    }
}

