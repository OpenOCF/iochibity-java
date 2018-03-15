package openocf.app;

import openocf.constants.Method;
import openocf.signals.InboundStimulus;

public class DefaultCoRSP
    extends  CoResourceSP
{
    public DefaultCoRSP() {
	super();
	setMethod(Method.RETRIEVE);
    }

    @Override
    public int coReact(InboundStimulus msg)
    {
	System.out.println("DefaultCoRSP.coReact ENTRY");
	return 0;
    }
}
