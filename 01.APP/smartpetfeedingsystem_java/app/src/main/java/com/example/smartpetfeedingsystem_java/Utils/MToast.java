package com.example.smartpetfeedingsystem_java.Utils;

import android.content.Context;
import android.widget.Toast;

public class MToast {
    /**
     * 优化Toast队列  能够直接显示当前的信息而非排队等待上一消息结束
     */
    private static Toast toast;
    public static void mToast(Context context, String text){
        if (toast != null) {
            toast.cancel();
        }
        toast = Toast.makeText(context,text,Toast.LENGTH_SHORT);
        toast.show();
    }
}
