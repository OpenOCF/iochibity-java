package org.openocf.test.client;

import org.openocf.test.Logger;

import org.openocf.test.client.R;
import openocf.OpenOCF;
import openocf.OpenOCFClient;
import openocf.ConfigAndroid;
import openocf.behavior.OutboundStimulus;
import openocf.constants.Method;


import android.app.ListActivity;
import android.content.ActivityNotFoundException;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.Toast;

// import android.support.v7.app.AppCompatActivity;

/**
 * This activity demonstrates the <b>borderless button</b> styling from the Holo visual language.
 * The most interesting bits in this sample are in the layout files (res/layout/).
 * <p>
 * See <a href="http://developer.android.com/design/building-blocks/buttons.html#borderless">
 * borderless buttons</a> at the Android Design guide for a discussion of this visual style.
 */
public class ClientActivity extends ListActivity {

    private static final String TAG = "ClientActivity";

    private static final Uri DOCS_URI = Uri.parse(
            "http://developer.android.com/design/building-blocks/buttons.html#borderless");

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

	try{
	    Log.v("OCF", "Bazel ClientActivity: onCreate");
	    ConfigAndroid.config(getApplicationContext(),
				 "client");  // 2nd arg is name of android_binary from BUILD
	    Log.v("OCF", "JNI says: " + OpenOCF.configuration());
	    // System.loadLibrary("mraajava");
	}catch(Exception e){
	    Log.i("OCF", e.toString());
	}

        setContentView(R.layout.sample_main);

        setListAdapter(mListAdapter);

        findViewById(R.id.cancel_button).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                finish();
            }
        });

        findViewById(R.id.ok_button).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                // finish();
		DiscoveryCoSP discoveryCoRSP = new DiscoveryCoSP();
		discoveryCoRSP.setUri("/oic/res");
		// .setMethod(Method.DISCOVER);

		OutboundStimulus requestOut =
		    new OutboundStimulus(discoveryCoRSP)
		    .setMethod(Method.DISCOVER)
		    .setQualityOfService(OpenOCFClient.QOS_HIGH);

		//		Logger.logOutboundStimulus(requestOut);

		Log.i(TAG, "DiscoveryCoRSP method before coexhibit: " + discoveryCoRSP._method);

		OpenOCFClient.coExhibit(requestOut);

		Log.i(TAG, "DiscoveryCoRSP method after coexhibit: " + discoveryCoRSP._method);
            }
        });

	// AndroidClient client = new AndroidClient();
	new Thread(new Runnable() {
		public void run() {
		    ClientConfig.configure();
		}
	    }).start();
    }

    private BaseAdapter mListAdapter = new BaseAdapter() {
        @Override
        public int getCount() {
	    // OpenOCF.getMessageCount();
            return 10;
        }

        @Override
        public Object getItem(int position) {
	    // return OpenOCF.getMessage(position); // returns InboundResponse object
            return null;
        }

        @Override
        public long getItemId(int position) {
            return position + 1;
        }

        @Override
        public View getView(int position, View convertView, ViewGroup container) {
            if (convertView == null) {
                convertView = getLayoutInflater().inflate(R.layout.list_item, container, false);
            }

	    // get current item to be displayed
	    //InboundResponse currentResponse = (Item) getItem(position);

            // Because the list item contains multiple touch targets, you should not override
            // onListItemClick. Instead, set a click listener for each target individually.

            convertView.findViewById(R.id.primary_target).setOnClickListener(
                    new View.OnClickListener() {
                        @Override
                        public void onClick(View view) {
                            Toast.makeText(ClientActivity.this,
                                    // R.string.touched_primary_message,
					   OpenOCFClient.configuration(),
                                    Toast.LENGTH_SHORT).show();
                        }
                    });

            convertView.findViewById(R.id.secondary_action).setOnClickListener(
                    new View.OnClickListener() {
                        @Override
                        public void onClick(View view) {
                            Toast.makeText(ClientActivity.this,
                                    R.string.touched_secondary_message,
                                    Toast.LENGTH_SHORT).show();
                        }
                    });
            return convertView;
        }
    };

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        super.onCreateOptionsMenu(menu);
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.docs_link:
                try {
                    startActivity(new Intent(Intent.ACTION_VIEW, DOCS_URI));
                } catch (ActivityNotFoundException ignored) {
                }
                return true;
        }
        return super.onOptionsItemSelected(item);
    }
}
