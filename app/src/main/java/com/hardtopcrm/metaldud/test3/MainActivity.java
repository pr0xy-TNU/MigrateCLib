package com.hardtopcrm.metaldud.test3;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.widget.TextView;
import com.hardtopcrm.metaldud.test3.SizerFwkLite.SizerPose;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        SizerFwkLite instance = SizerFwkLite.getInstance();

        // Example of a call to a native method
        TextView tv1 = findViewById(R.id.sample_text_1);
        TextView tv2 = findViewById(R.id.sample_text_2);
        tv1.setText(instance.getPoseDescription(SizerPose.LEARNING_BACKGROUND));
        tv2.setText(instance.getCurrentPoseInfo());
    }


}
