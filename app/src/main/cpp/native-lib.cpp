#include <jni.h>
#include <string>

extern "C" {
JNIEXPORT jstring JNICALL
Java_com_ikouz_jnidemo_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}


JNIEXPORT jint JNICALL
Java_com_ikouz_jnidemo_MainActivity_intFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    int testInteger = 1234; // this value could be got from native c function.
    return testInteger;
}

JNIEXPORT void JNICALL
Java_com_ikouz_jnidemo_MainActivity_objectOpJNI(
        JNIEnv* env,
        jobject /* this */, jobject configObj) {

    // get jclass
    jclass configCls = env->GetObjectClass(configObj);
    if (configCls == NULL) return; // something wrong.
    // get field id
    jfieldID numFieldID = env->GetFieldID(configCls, "num", "I");
    if (numFieldID == NULL) return;
    // get int field value
    jint num = env->GetIntField(configObj, numFieldID);
    jint newNum = num - 202;
    // set int new field value
    env->SetIntField(configObj, numFieldID, newNum);

    // get string field value
    jfieldID  descFieldID = env->GetFieldID(configCls, "desc", "Ljava/lang/String;");
    jstring desc = (jstring)env->GetObjectField(configObj, descFieldID);
    // convert to const char* (if you need)
    const char* desc_char = env->GetStringUTFChars(desc ,NULL);
    char buff[128] = {0};
    sprintf(buff, "change %s to success", desc_char);
    jstring newDesc = env->NewStringUTF(buff);
    env->SetObjectField(configObj, descFieldID, newDesc);
    //  release (if you have converted);
    env->ReleaseStringUTFChars(desc, desc_char);
}

JNIEXPORT void JNICALL
Java_com_ikouz_jnidemo_MainActivity_jniCallback(JNIEnv* env, jobject obj) {
    // get jclass
    jclass callCls = env->GetObjectClass(obj);
    if (callCls == NULL) return; // something wrong
    // get method ID
    jmethodID  callbackID = env->GetMethodID(callCls, "callbackFromJni", "(Ljava/lang/String;I)V");
    if (callbackID == NULL) return;
    jstring callbackStr = env->NewStringUTF("The method callbacks");
    // call method by id
    env->CallVoidMethod(obj, callbackID, callbackStr, 1);
}

JNIEXPORT jobject JNICALL
Java_com_ikouz_jnidemo_MainActivity_jniGetConfigList(JNIEnv* env, jobject obj) {
    // get arraylist's jclass
    jclass arrayListCls = env->FindClass("java/util/ArrayList");
    if (arrayListCls == NULL) return NULL; // something wrong
    // get ArrayList's constructor, pay attention, constructor's signature must be void
    jmethodID arrayListConstructor = env->GetMethodID(arrayListCls , "<init>","()V");
    // create an arraylist instance
    jobject listInstance = env->NewObject(arrayListCls , arrayListConstructor);
    // get add method
    jmethodID listAddMethodID  = env->GetMethodID(arrayListCls,"add","(Ljava/lang/Object;)Z");
    if (listAddMethodID == NULL) return NULL;
    // inner class should use "$"
    jclass configCls = env->FindClass("com/ikouz/jnidemo/MainActivity$Config");
    // get config class's constructor
    jmethodID configConstructorMethodId = env->GetMethodID(configCls , "<init>", "(ILjava/lang/String;)V");
    if (configConstructorMethodId == NULL) return NULL;
    for(int i = 0 ; i < 3 ; i++) {
        jstring str = env->NewStringUTF("test config");
        // create a config instance
        jobject configObj = env->NewObject(configCls , configConstructorMethodId , i, str);
        // add to arraylist's instance
        env->CallBooleanMethod(listInstance , listAddMethodID , configObj);
    }
    return listInstance;
}

}

