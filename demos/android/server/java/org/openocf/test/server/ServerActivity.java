package org.openocf.test.server;

import org.openocf.test.server.R;
import openocf.android.OCFService;

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
public class ServerActivity extends ListActivity {

    private static final Uri DOCS_URI = Uri.parse(
            "http://developer.android.com/design/building-blocks/buttons.html#borderless");

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

	try{
	    Log.v("Bazel ServerActivity:", "onCreate");
	    OCFService.config(getApplicationContext(), "server"); // 2nd arg is name of android_binary from BUILD
	    Log.v("Bazel", "JNI says: " + OCFService.configuration());
	    // System.loadLibrary("mraajava");
	}catch(Exception e){
	    System.out.println(e.toString());
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
                finish();
            }
        });
	new Thread(new Runnable() {
		public void run() {
		    AndroidServer.go();
		}
	    }).start();
    }

    private BaseAdapter mListAdapter = new BaseAdapter() {
        @Override
        public int getCount() {
            return 10;
        }

        @Override
        public Object getItem(int position) {
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

            // Because the list item contains multiple touch targets, you should not override
            // onListItemClick. Instead, set a click listener for each target individually.

            convertView.findViewById(R.id.primary_target).setOnClickListener(
                    new View.OnClickListener() {
                        @Override
                        public void onClick(View view) {
                            Toast.makeText(ServerActivity.this,
                                    // R.string.touched_primary_message,
					   OCFService.configuration(),
                                    Toast.LENGTH_SHORT).show();
                        }
                    });

            convertView.findViewById(R.id.secondary_action).setOnClickListener(
                    new View.OnClickListener() {
                        @Override
                        public void onClick(View view) {
                            Toast.makeText(ServerActivity.this,
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
