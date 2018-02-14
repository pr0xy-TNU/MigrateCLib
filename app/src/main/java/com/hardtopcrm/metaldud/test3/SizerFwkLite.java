package com.hardtopcrm.metaldud.test3;

/**
 * Created by user on 14.02.18.
 */

class SizerFwkLite {

    private static SizerFwkLite instance;

    enum SizerPose {
        SCAN_NOT_STARTED,
        STEP_INTO_THE_OUTLINE,
        STEP_OUT_OF_VIEW,
        LEARNING_BACKGROUND,
        STEP_BACK_INTO_VIEW,
        LEGS_APART,
        LEGS_TOGETHER,
        SPRAD_ARMS,
        RAISE_HANDS,
        ROTATING,
        ROTATION_STARTED,
        ROTATION90,
        ROTATION180,
        ROTATION270,
        ROTATION_ENDED,
        LOWER_ARMS,
        SCAN_ENDED

    }

    enum SizerPoseError {
        POSEERROR_NOERROR,
        POSEERROR_MOVETOCENTER,
        POSEERROR_STEPBACK,
        POSEERROR_RAISEARMSHIGHER,
        POSEERROR_SPREADARMSMORE,
        POSEERROR_USERNOTINSCENE,
        POSEERROR_LEGSAPART,
        POSEERROR_LEGSTOGETHER,
        POSEERROR_USERINSCENE,
        POSEERROR_VALIDATION_SCENEERROR,
        POSEERROR_USERSTANDSTOOLOW,
        POSEERROR_BACKGROUNDLEARNINGTOOLONG,
        POSEERROR_USERSTANDTOOCLOSE
    }

    public static SizerFwkLite getInstance() {
        if (instance == null) {
            instance = new SizerFwkLite();
            return instance;
        } else {
            return instance;
        }
    }

    public SizerFwkLite() {
        SizerFwkLiteNative();
    }

    public void detectPoseFromNative(int frameID,
        float RGBImageTimestamp,
        float deviceTiltDegrees,
        int head_top_x,
        int head_top_y,
        int head_width,
        int head_height) {
        detectPoseNative(frameID,
            RGBImageTimestamp,
            deviceTiltDegrees,
            head_top_x,
            head_top_y,
            head_width,
            head_height);
    }


    public String getCurDetectedPoseDescriptionFromNative() {
        return getCurDetectedPoseDescriptionNative();
    }

    public boolean shouldUseCurrentFrameForPostMeasurement() {
        return shouldUseCurrentFrameForPostMeasurementNative();
    }

    public String getVerasion() {
        return getVersionNative();
    }

    public String getPoseDescription(SizerPose sizerPose) {
        return getPoseDescriptionNative(convertPoseIntoNative(sizerPose));
    }

    public String getCurrentPoseInfo() {
        return getInfoNative();
    }

    private int convertPoseIntoNative(SizerPose javaPose) {
        switch (javaPose) {

            case SCAN_NOT_STARTED:
                return 0;
            case STEP_INTO_THE_OUTLINE:
                return 1;
            case STEP_OUT_OF_VIEW:
                return 2;
            case LEARNING_BACKGROUND:
                return 3;
            case STEP_BACK_INTO_VIEW:
                return 4;
            case LEGS_APART:
                return 5;
            case LEGS_TOGETHER:
                return 6;
            case SPRAD_ARMS:
                return 7;
            case RAISE_HANDS:
                return 8;
            case ROTATING:
                return 9;
            case ROTATION_STARTED:
                return 10;
            case ROTATION90:
                return 11;
            case ROTATION180:
                return 12;
            case ROTATION270:
                return 13;
            case ROTATION_ENDED:
                return 14;
            case LOWER_ARMS:
                return 15;
            case SCAN_ENDED:
                return 16;
        }
        return 0;
    }


    /**
     * Native methods
     */
    public static native void SizerFwkLiteNative();

    public static native String getCurDetectedPoseDescriptionNative();

    public static native String getPoseDescriptionNative(int sizerPose);

    public static native String getVersionNative();

    public static native String getInfoNative();

    public static native boolean shouldUseCurrentFrameForPostMeasurementNative();

    public static native void detectPoseNative(int frameID,
        float RGBImageTimestamp,
        float deviceTiltDegrees,
        int head_top_x,
        int head_top_y,
        int head_width,
        int head_height);

}
