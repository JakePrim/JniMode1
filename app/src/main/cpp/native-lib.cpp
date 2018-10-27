#include <jni.h>
#include <string>
#include <android/log.h>
#include <pthread.h>
using namespace std;

//log 宏定义
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "JNI_TEST", __VA_ARGS__);

extern "C" JNIEXPORT jstring
JNICALL
Java_com_jnimode1_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}


extern "C"
JNIEXPORT void JNICALL
Java_com_jnimode1_MainActivity_testArray(JNIEnv *env, jobject instance, jobjectArray strs,
                                         jintArray ints_) {
    //获取当前数组的地址
    jint *ints = env->GetIntArrayElements(ints_, NULL);
    int32_t length = env->GetArrayLength(ints_);
    // TODO
    for (int i = 0; i < length; ++i) {
        LOGE("获取参数:%d", ints[i]);
        ints[i] = 100;
    }

    //0 刷新Java数组并释放 c/c++数组 如果在C++中改变数组的值 Java中数组的值也会改变
    //JNI_COMMIT 1 只刷新Java数组 如果在C++中改变数组的值 Java中数组的值也会改变
    //JNI_ABORT 2 只释放并没有刷新Java数组 如果在C++中改变数组的值 Java中数组的值不会改变
    env->ReleaseIntArrayElements(ints_, ints, 2);

    int32_t str_length = env->GetArrayLength(strs);
    for (int i = 0; i < str_length; ++i) {
        jstring jobject1 = static_cast<jstring>(env->GetObjectArrayElement(strs, i));
        //转换成可操作的c/c++的字符串
        const char *getStr = env->GetStringUTFChars(jobject1, JNI_FALSE);
        LOGE("获取String：%s", getStr)
        env->ReleaseStringUTFChars(jobject1, getStr);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_jnimode1_MainActivity_passObject(JNIEnv *env, jobject instance, jobject bean,
                                          jstring str_) {
    const char *str = env->GetStringUTFChars(str_, 0);

    // TODO 反射调用Java方法
    //1 获取Java对象的class对象
    jclass beanClass = env->GetObjectClass(bean);
    //2 找到要调用的方法
    //参数3 传递的是签名 javap -s 全类名:生成签名 int --> I
    jmethodID geta1 = env->GetMethodID(beanClass, "getA1", "()I");
    //调用方法 int getA1()
    jint jint1 = env->CallIntMethod(bean, geta1);
    LOGE("C++ 调用Java Bean对象的getA1():%d", jint1);

    //void --> V
    jmethodID seta1 = env->GetMethodID(beanClass, "setA1", "(I)V");
    //调用setA1(int i)方法
    env->CallVoidMethod(bean, seta1, 200);
    LOGE("C++ 调用Java Bean对象的setA1(200)");

    //String -->Ljava.lang.String;
//    jmethodID geta2 = env->GetStaticMethodID(beanClass, "getA2", "(Ljava/lang/String;)V");
//    //创建Java字符串
//    jstring jstring1 = env->NewStringUTF("getA2是Java静态变量,被C++调用");
//    //调用静态方法
//    env->CallStaticVoidMethod(beanClass, geta2, jstring1);
////    LOGE("C++ 调用Java Bean对象的静态方法getA2(%s)", jstring1);
//    //释放局部引用
//    env->DeleteLocalRef(jstring1);


    //修改属性
    jfieldID fieldId = env->GetFieldID(beanClass, "a1", "I");
    env->SetIntField(bean, fieldId, 50);

    //如果参数为list map

    env->ReleaseStringUTFChars(str_, str);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_jnimode1_MainActivity_invokeHelper(JNIEnv *env, jobject instance, jobject helper) {
    jclass helperClass;
    //判断传递的对象是否为空
    if (helper != NULL) {
        //获得Helper类
        LOGE("helper对象不为空")
        helperClass = env->GetObjectClass(helper);
    } else {
        LOGE("helper对象为空")
        helperClass = env->FindClass("com/jnimode1/Helper");
    }

    //构造方法
    jmethodID constructMethod = env->GetMethodID(helperClass, "<init>", "()V");
    if (helper == NULL) {
        //创建helper对象并调用构造方法
        helper = env->NewObject(helperClass, constructMethod);
    }
    //调用普通方法
    jmethodID instanceMethod = env->GetMethodID(helperClass, "instanceModth",
                                                "(Ljava/lang/String;IZ)V");
    LOGE("准备调用方法instanceModth")
    //将字符串转换为Java的调用方式
    jstring string = env->NewStringUTF("C++调用");
    LOGE("开始调用instanceModth")
    env->CallVoidMethod(helper, instanceMethod, string, 20, 0);



    //调用静态方法
    jmethodID staticMethod = env->GetStaticMethodID(helperClass, "staticModth",
                                                    "(Ljava/lang/String;IZ)V");
    jstring staticString = env->NewStringUTF("C++调用静态方法");
    env->CallStaticVoidMethod(helperClass, staticMethod, staticString, 30, 1);
    env->DeleteLocalRef(helperClass);
    env->DeleteLocalRef(staticString);

    //主要需要释放对象及引用
    env->DeleteLocalRef(helper);
    env->DeleteLocalRef(string);


}

//反射Java中类的属性
extern "C"
JNIEXPORT void JNICALL
Java_com_jnimode1_MainActivity_fiedHelper(JNIEnv *env, jobject instance, jobject helper) {
    jclass helperClass;
    //判断传递的对象是否为空
    if (helper != NULL) {
        //获得Helper类
        LOGE("helper对象不为空")
        helperClass = env->GetObjectClass(helper);
    } else {
        LOGE("helper对象为空")
        helperClass = env->FindClass("com/jnimode1/Helper");
    }

    jfieldID aId = env->GetFieldID(helperClass, "a", "I");

    //获得属性值
    jint a = env->GetIntField(helper, aId);
    LOGE("a:%d", a);

    //修改属性值
    env->SetIntField(helper, aId, 100);

    //获得属性值
    jint a1 = env->GetIntField(helper, aId);
    LOGE("a1:%d", a1);

    jfieldID sId = env->GetFieldID(helperClass, "s", "Ljava/lang/String;");

    //获取字符串的值
    jstring s = static_cast<jstring>(env->GetObjectField(helper, sId));
    const char *as = env->GetStringUTFChars(s, 0);
    LOGE("获取s的值%s", as);
    //不用之后释放
    env->ReleaseStringUTFChars(s, as);

    //修改字符串的值
    jstring new_str = env->NewStringUTF("ssss");
    env->SetObjectField(helper, sId, new_str);
    //注意释放引用
    env->DeleteLocalRef(new_str);

    //获取字符串的值
    jstring s1 = static_cast<jstring>(env->GetObjectField(helper, sId));
    const char *as1 = env->GetStringUTFChars(s1, 0);
    LOGE("修改后s的值%s", as1);
    //不用之后释放
    env->ReleaseStringUTFChars(s1, as1);


    env->DeleteLocalRef(helperClass);
}

static jstring globalStr;

//全局引用
extern "C"
JNIEXPORT jstring JNICALL
Java_com_jnimode1_MainActivity_test1(JNIEnv *env, jobject instance) {


    if (globalStr == NULL) {
        jstring str = env->NewStringUTF("C++字符串");
        globalStr = static_cast<jstring>(env->NewGlobalRef(str));
        env->DeleteLocalRef(str);
    } else {
        jstring str = env->NewStringUTF("global 不为null,C++字符串");
        globalStr = static_cast<jstring>(env->NewGlobalRef(str));
        env->DeleteLocalRef(str);
    }

    return globalStr;

}


extern "C"
JNIEXPORT jclass JNICALL
Java_com_jnimode1_MainActivity_deleteGolbal(JNIEnv *env, jobject instance) {

    static jclass class1;

    //对于弱引用，如果引用的对象被回收返回true 否则返回 false
    //在使用弱引用时，必须先检查缓存过的弱引用是指向活动的对象，还是指向一个已经被GC的对象
    //对于局部和全局的引用判断是否引用了Java的null对象
    jboolean isEnqule = env->IsSameObject(class1, NULL);
    if (class1 == NULL || isEnqule) {
        jclass j1 = env->GetObjectClass(instance);
        class1 = static_cast<jclass>(env->NewWeakGlobalRef(j1));
        env->DeleteLocalRef(j1);
        LOGE("123");
    } else {
        LOGE("456");
    }

    return class1;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_jnimode1_MainActivity_updateGolbal(JNIEnv *env, jobject instance, jclass jclass1) {

    env->DeleteWeakGlobalRef(jclass1);

}

//-------------- 动态注册 ----------------//

JavaVM *_vm;



//类名
static const char *mClassName = "com/jnimode1/MainActivity";



jobject _instance;

void *task(void *args){
     JNIEnv *env;

    //将本地当前线程附加到 jvm 并获得jnienv
    //成功则返回0
    jint res = _vm->AttachCurrentThread(&env,0);

    if (res != JNI_OK){
        return 0;
    }

    jclass jclass1 = env->GetObjectClass(_instance);

    jmethodID staticMethod = env->GetStaticMethodID(jclass1,"staticMothed","(I)V");

    env->CallStaticVoidMethod(jclass1,staticMethod,100);


    jmethodID  jmethodID1 = env->GetMethodID(jclass1,"instanceMothed","(Ljava/lang/String;)V");

    jstring jstring1 = env->NewStringUTF("我是C++");



    env->CallVoidMethod(_instance,jmethodID1,jstring1);

    env->DeleteLocalRef(jclass1);
    env->DeleteLocalRef(jstring1);
    env->DeleteGlobalRef(_instance);

    //分离
    _vm->DetachCurrentThread();

    return 0;

}

void dynamicTest() {
    LOGE("我是被动态注册的方法dynamicTest");
}

//JNIEnv *env, jobject instance 如果传参数这两个参数必须存在
jint dynamicTest2(JNIEnv *env, jobject instance, jint i) {
    LOGE("我是被动态注册的方法dynamicTest2");
    return i;
}

void nativeThread2(JNIEnv *env,jobject instace){
    _instance = env->NewGlobalRef(instace);//_instacne 设置为全局引用
    pthread_t pthread;
    pthread_create(&pthread,0,task,0);
}

//静态的Jni native 方法数组
static const JNINativeMethod method[] = {
        //方法名    签名    本地方法
        {"dynamicJavaTest",  "()V",  (void *) dynamicTest},
        {"dynamicJavaTest2", "(I)I", (int *) dynamicTest2},
        {"nativeThread", "()V", (void *) nativeThread2}
};

//返回Jni 版本
int JNI_OnLoad(JavaVM *vm, void *r) {
    LOGE("JNI_OnLoad");
    _vm = vm;
    JNIEnv *env = 0;
    //获得JNIEnv 这里会返回一个值 小于0 代表失败
    jint res = vm->GetEnv((void **) (&env), JNI_VERSION_1_6);
    //判断返回结果
    if (res != JNI_OK) {
        return -1;
    }
    //根据类名找到类，注意有native的类不能被混淆
    jclass jcls = env->FindClass(mClassName);
    //动态注册  第一个参数 类  第二个参数 方法数组  第三个参数 注册多少个方法
    env->RegisterNatives(jcls, method, sizeof(method) / sizeof(JNINativeMethod));
    return JNI_VERSION_1_6;
}


























