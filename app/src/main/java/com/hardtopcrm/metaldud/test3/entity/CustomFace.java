package com.hardtopcrm.metaldud.test3.entity;

/**
 * Created by user on 15.02.18.
 */

public class CustomFace {

    private float headTopX;
    private float headTopY;
    private float headWidth;

    public float getHeadTopX() {
        return headTopX;
    }

    public void setHeadTopX(float headTopX) {
        this.headTopX = headTopX;
    }

    public float getHeadTopY() {
        return headTopY;
    }

    public void setHeadTopY(float headTopY) {
        this.headTopY = headTopY;
    }

    public float getHeadWidth() {
        return headWidth;
    }

    public void setHeadWidth(float headWidth) {
        this.headWidth = headWidth;
    }

    public float getHeadHeigth() {
        return headHeigth;
    }

    public void setHeadHeigth(float headHeigth) {
        this.headHeigth = headHeigth;
    }

    private float headHeigth;


    public CustomFace() {

    }

    @Override
    public String toString() {
        return "CustomFace[" +
            "headTopX=" + headTopX +
            ", headTopY=" + headTopY +
            ", headWidth=" + headWidth +
            ", headHeigth=" + headHeigth +
            ']';
    }
}
