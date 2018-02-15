package com.hardtopcrm.metaldud.test3;

import android.annotation.SuppressLint;
import android.databinding.DataBindingUtil;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Paint.Style;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.util.SparseArray;
import android.view.View;
import com.google.android.gms.vision.Frame;
import com.google.android.gms.vision.face.Face;
import com.google.android.gms.vision.face.FaceDetector;
import com.google.android.gms.vision.face.Landmark;
import com.hardtopcrm.metaldud.test3.databinding.ActivityMainBinding;
import com.hardtopcrm.metaldud.test3.entity.CustomFace;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    private final String LOG_TAG = "LOG_TAG";
    public List<CustomFace> customFaceList = new ArrayList<>();
    private FaceDetector faceDetector;
    public ActivityMainBinding binding;
    private SizerFwkLite instance;
    private Bitmap bitFace, editBitmap;


    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        init();
        editBitmap = Bitmap
            .createBitmap(bitFace.getWidth(), bitFace.getHeight(), bitFace.getConfig());
        binding.btnDataProcessing.setOnClickListener(this::detec);


    }

    public void init() {
        SizerFwkLite instance = SizerFwkLite.getInstance();

        binding = DataBindingUtil.setContentView(this, R.layout.activity_main);

        InputStream inputStream = getResources()
            .openRawResource(R.raw.test_5);
        bitFace = BitmapFactory.decodeStream(inputStream);

        faceDetector = new FaceDetector.Builder(getApplicationContext())
            .setTrackingEnabled(false)
            .setLandmarkType(FaceDetector.ALL_LANDMARKS)
            .setMode(FaceDetector.ALL_CLASSIFICATIONS)
            .build();
        if (faceDetector.isOperational()) {
            Log.d(LOG_TAG, "onCreate: isOperable true");
        } else {
            Log.d(LOG_TAG, "onCreate: isOperable false");

        }
    }

    public void detec(View view) {
        float scale = getResources().getDisplayMetrics().density;
        Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);
        paint.setColor(Color.rgb(12, 255, 10));
        paint.setShadowLayer(1f, 0f, 1f, Color.WHITE);
        paint.setStyle(Style.STROKE);
        paint.setStrokeWidth(1f);
        Canvas canvas = new Canvas(editBitmap);
        canvas.drawBitmap(bitFace, 0, 0, paint);
        Frame frame = new Frame.Builder().setBitmap(editBitmap).build();
        SparseArray<Face> faces = faceDetector.detect(frame);
        customFaceList = new ArrayList<>();
        CustomFace temp = new CustomFace();
        for (int i = 0; i < faces.size(); i++) {
            Face tempFace = faces.valueAt(i);

            canvas.drawRect(
                tempFace.getPosition().x,
                tempFace.getPosition().y,
                tempFace.getPosition().x + tempFace.getWidth(),
                tempFace.getPosition().y + tempFace.getHeight(),
                paint);
            @SuppressLint("DefaultLocale") String result = String
                .format("pos.x: %f pos.y: %f width: %f height: %f", tempFace.getPosition().x,
                    tempFace.getPosition().y, tempFace.getWidth(), tempFace.getHeight());
            Log.d(LOG_TAG, "detec:" + result);

            for (Landmark item : tempFace.getLandmarks()
                ) {
                int cx = (int) item.getPosition().x;
                int cy = (int) item.getPosition().y;
                canvas.drawCircle(cx, cy, 4, paint);
            }
            temp.setHeadTopX(tempFace.getPosition().x);
            temp.setHeadTopY(tempFace.getPosition().y);
            temp.setHeadWidth(tempFace.getWidth());
            temp.setHeadHeigth(tempFace.getHeight());
            customFaceList.add(temp);
        }
        if (faces.size() == 0) {

        } else {
            binding.ivCustomFace.setImageBitmap(editBitmap);
        }
        /**
         * In this part with request current pose from c++lib, main part of code
         *
         */
        SizerFwkLite sizerFwkLite = SizerFwkLite.getInstance();
        for (int i = 0; i < customFaceList.size(); i++) {
            sizerFwkLite.detectPose(customFaceList.get(i));
            Log.d(LOG_TAG, "onCreate: " + sizerFwkLite.getCurDetectedPoseDescription());
        }

    }


    public void createFacesBorder() {
        CustomFace face = new CustomFace();

    }


}
