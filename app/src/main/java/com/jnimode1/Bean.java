package com.jnimode1;

/**
 * @author prim
 * @version 1.0.0
 * @desc
 * @time 2018/10/22 - 11:35 PM
 */
public class Bean {
    int a1;
    static String a2;

    public int getA1() {
        return a1;
    }

    public void setA1(int a1) {
        this.a1 = a1;
    }

    public static void getA2(String a3) {
        a2 = a3;
    }

    public void setA2(String a2) {
        this.a2 = a2;
    }
}
