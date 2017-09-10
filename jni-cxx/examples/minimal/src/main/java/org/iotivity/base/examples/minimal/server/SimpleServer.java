/*
 *******************************************************************
 *
 * Copyright 2015 Intel Corporation.
 *
 *-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 */

package org.iotivity.base.examples.minimal.server;

import org.iotivity.base.ModeType;
import org.iotivity.base.OcException;
import org.iotivity.base.OcPlatform;
import org.iotivity.base.PlatformConfig;
import org.iotivity.base.QualityOfService;
import org.iotivity.base.ServiceType;

import java.io.*;
import java.util.LinkedList;
import java.util.List;

/**
 * SimpleServer
 * <p/>
 * SimpleServer is a sample OIC server application.
 * It creates a Light and waits for the incoming client calls to handle
 * various request scenarios.
 */
public class SimpleServer {

    static List<Light> lights = new LinkedList<>();

    /**
     * A local method to configure and initialize platform, and then create a light resource.
     */
    private static void startSimpleServer() {

        PlatformConfig platformConfig = new PlatformConfig(
                ServiceType.IN_PROC,
                ModeType.SERVER,
                "0.0.0.0", // By setting to "0.0.0.0", it binds to all available interfaces
                0,         // Uses randomly available port
                QualityOfService.LOW,
		"oic_svr_db_server.dat"
        );

        msg("Configuring platform.");
        OcPlatform.Configure(platformConfig);

        createNewLightResource("/a/light", "John's light");

        msg("Waiting for the requests...");
        printLine();
    }

    public static void createNewLightResource(String resourceUri, String resourceName){
        msg("Creating a light");
        Light light = new Light(
                resourceUri,     //URI
                resourceName,    //name
                false,           //state
                0                //power
        );
        // msg(light.toString());
	light.printThis();

        msg("Registering light as a resource");
        try {
            light.registerResource();
        } catch (OcException e) {
            msgError(TAG, e.toString());
            msg("Failed to register a light resource");
        }
        lights.add(light);
    }

    private static void stopSimpleServer() {
        for (Light light : lights) {
            try {
                light.unregisterResource();
            } catch (OcException e) {
                msgError(TAG, e.toString());
                msg("Failed to unregister a light resource");
            }
        }
        lights.clear();

        msg("All created resources have been unregistered");
        printLine();
    }

    //******************************************************************************
    // End of the OIC specific code
    //******************************************************************************

    private final static String TAG = SimpleServer.class.getSimpleName();
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

    public static void main(String[] args) {
	// System.loadLibrary("iotivity-jni");
        // System.out.println("FOO");
        // System.loadLibrary("oc_logger");
        // System.loadLibrary("octbstack");
	// System.loadLibrary("connectivity_abstraction");
        // System.loadLibrary("oc");
        // System.out.println("BAR");

        startSimpleServer();

        // // creating a new thread to handle the input
        // Thread t1=new Thread(new Quitter());
        // t1.start();

        System.out.println("press q then ENTER to terminate");

        while(true){
	    try {
		Thread.sleep(2000);
		System.out.println("GUI thread loop");
	    } catch (InterruptedException e) {
		e.printStackTrace();
		msgError(TAG, e.toString());
	    }

            // outputting data until the quit boolean flag is not set
            if(quit==true) break;
            // System.out.println("hello buddy");
        }
    }

    public void sleep(int seconds) {
        try {
            Thread.sleep(seconds * 1000);
        } catch (InterruptedException e) {
            e.printStackTrace();
            msgError(TAG, e.toString());
        }
    }

    public static void msg(final String text) {
        System.out.println("[O]" + TAG + " | " + text);
    }

    public static void msg(final String tag, final String text) {
        System.out.println("[O]" + tag + " | " + text);
    }

    public static void msgError(final String tag ,final String text) {
        System.out.println("[E]" + tag + " | " + text);
    }

    public static void printLine() {
        msg("------------------------------------------------------------------------");
    }

}
