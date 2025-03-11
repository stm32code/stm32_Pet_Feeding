package com.example.smartpetfeedingsystem_java.Utils;

import android.view.View;

public class ViewSetIsClick {
    public static void setViewIsClick(View view, boolean click){
        view.setClickable(click);
        view.setEnabled(click);
        view.setFocusable(click);
    }
}
