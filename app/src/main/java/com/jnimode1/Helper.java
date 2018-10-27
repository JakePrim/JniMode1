package com.jnimode1;

import android.util.Log;

/**
 * @author prim
 * @version 1.0.0
 * @desc
 * @time 2018/10/24 - 6:38 PM
 */
public class Helper {
    private static final String TAG = "Helper";

    int a = 10;

    String s = "test";

    public Helper() {
        Log.e(TAG, "Helper: 构造方法被调用");
    }

    //C/C++ 反射创建的Java对象，调用Java方法
    public void instanceModth(String a, int c, boolean b) {
        Log.e(TAG, "instanceModth: a= " + a + " c= " + c + " b= " + b);
    }

    public static void staticModth(String a, int c, boolean b) {
        Log.e(TAG, "instanceModth: a= " + a + " c= " + c + " b= " + b);
    }
}
