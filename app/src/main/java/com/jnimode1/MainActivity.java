package com.jnimode1;

import android.os.Looper;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;
import android.widget.Toast;

import java.util.Arrays;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "MainActivity";

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //传递基础类型
//        int[] ints = {4, 5, 6};
//
//        testArray(new String[]{"1", "22", "33"}, ints);
//
//        Log.e(TAG, "onCreate: " + Arrays.toString(ints));

        //C++ 反射Java

//        Helper helper = new Helper();
//        fiedHelper(helper);
//        invokeHelper(helper);

        //传递局部引用
//        Bean bean = new Bean();
//        bean.setA1(100);
//        bean.setA2("A2");
//        passObject(bean, "bean");
//        Log.e(TAG, "onCreate: 查看属性是否被C++修改：" + bean.getA1() + "");

        //全局引用
//        String s = test1();
//        Log.e(TAG, "onCreate: 0:"+s);
//        String s1 = test1();
//        Log.e(TAG, "onCreate: 1:"+s1);

//        deleteGolbal();
//        deleteGolbal();
//        updateGolbal(deleteGolbal());
//        deleteGolbal();
//        String s2 = test1();
//        Log.e(TAG, "onCreate: 2:"+s2);

//        dynamicJavaTest();
//        dynamicJavaTest2(1);

        nativeThread();
    }

    public static void staticMothed(int var) {
        Log.e(TAG, "staticMothed: " + var);
    }

    public void instanceMothed(final String str) {
        if (Looper.myLooper() == Looper.getMainLooper()){
            Toast.makeText(this, ""+str, Toast.LENGTH_SHORT).show();
        }else {
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    Toast.makeText(MainActivity.this, ""+str, Toast.LENGTH_SHORT).show();
                }
            });
        }
    }


    public native void nativeThread();
    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    //传递基本类型
    public native void testArray(String[] strs, int[] ints);

    //传递对象
    public native void passObject(Bean bean, String str);

    //反射方法
    public native void invokeHelper(Helper helper);

    public native void fiedHelper(Helper helper);

    public native String test1();

    public native Class deleteGolbal();

    public native void updateGolbal(Class cls);

    public native void dynamicJavaTest();

    public native int dynamicJavaTest2(int i);
}
