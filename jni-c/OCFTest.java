import org.iochibity.OCF;
import org.iochibity.RequestIn;
import org.iochibity.Resource;
import org.iochibity.ResourceManager;
import org.iochibity.ResourceServiceProvider;
import org.iochibity.constants.OCMode;
import org.iochibity.constants.OCStackResult;
import org.iochibity.constants.ResourcePolicy;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.LinkedList;

public class OCFTest
{
    static{
	try{
	    System.loadLibrary("ocfjni_c");
	}catch(Exception e){
	    System.out.println(e.toString());
	}
    }

    public static class MyResourceServiceProvider extends ResourceServiceProvider
    {
	@Override
	public int service(int flag, RequestIn request, Object callbackParam)
	{
	    System.out.println("Resource Service Routine called");
	    return 0;
	}
    }

    static Resource temp = new Resource();
    static MyResourceServiceProvider tempServiceProvider = new MyResourceServiceProvider();

    public static void main(String[] args)
    {
	Runtime.getRuntime().addShutdownHook(new Thread()
	    {
		@Override
		public void run()
		{
		    System.out.println("Shutdown hook running!");
		    OCF.stop();
		}
	    });

	// System.out.println(System.getProperty("java.library.path"));
	int r = OCF.Init(null, 0, OCMode.SERVER, "/Users/gar/iotivity/kit/java/jni-c/data.d/oic_svr_db_server.dat");
	System.out.println("Init returned: " + r);

	r = OCF.setPlatformInfo("Fartmaster",
				"Acme Novelties",
				"http://acme.example.org",
				"modelnbr", "mfgdate", "platversion",
				"osversion", "hwversion", "firmwareversion",
				"http://acme.example.org/support",
				new SimpleDateFormat("yyyy.MM.dd.HH.mm.ss").format(new Date()));

	r = OCF.setDeviceInfo("Fartmaster2020",
			      new String[] {"type1", "type2"},
			      "version-0.1",
			      new String[] {"dmversion-0.1"});

	System.out.println("resource count before: " + ResourceManager.resourceCount());

	r = ResourceManager.registerResource(temp, // OCResourceHandle*
					     "core.temp",
					     "oc.mi.def",
					     "/a/temperature",
					     tempServiceProvider,
					     null,
					     (byte)(ResourcePolicy.DISCOVERABLE
						    | ResourcePolicy.SECURE));
	// | OC_OBSERVABLE | OC_SECURE);

	if (r != OCStackResult.OK) {
	    System.out.println("TEMPERATURE resource service provider registration failed!");
	    // exit (EXIT_FAILURE);
	} else {
	    System.out.println("TEMPERATURE resource service provider registration succeeded!\n");
	}
	System.out.println("resource count after: " + ResourceManager.resourceCount());

	System.out.println("temp handle: " + temp.handle);
	System.out.println("temp uri: " + temp.uri);
	LinkedList<String> ll = temp.getTypes();
	System.out.println("first: " + ll.getFirst());

        System.out.println("press q then ENTER to terminate");

	r = OCF.run();
	System.out.println("run returned: " + r);

        while(true){
	    try {
		Thread.sleep(2000);
		System.out.println("GUI thread loop");
		System.out.println("resource count: " + ResourceManager.resourceCount());
	    } catch (InterruptedException e) {
		e.printStackTrace();
		msgError(TAG, e.toString());
	    }

            // outputting data until the quit boolean flag is not set
            if(quit==true) break;
            // System.out.println("hello buddy");
        }

        // Thread t1=new Thread(new Quitter());
        // t1.start();
    }

    public static void msgError(final String tag ,final String text) {
        System.out.println("[E]" + tag + " | " + text);
    }

    private final static String TAG = OCFTest.class.getSimpleName();
    static BufferedReader in=new BufferedReader(new InputStreamReader(System.in));
    static boolean quit=false;
    static class Quitter implements Runnable {
	public void run(){
	    String msg = null;
	    // threading is waiting for the key Q to be pressed
	    while(true){
		try{
		    msg=in.readLine();
		}catch(IOException e){
		    e.printStackTrace();
		}

		if(msg.equals("Q")) {quit=true;break;}
	    }
	}
    }
}
