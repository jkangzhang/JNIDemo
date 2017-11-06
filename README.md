
## 基本类型

对于基本类型的传递，只需要按照表格找到对应即可。

|    Java Type    | Native Type | Description  |
| ---------- | --- | --- |
| boolean |  jboolean | 8 bits, unsigned  |
| byte | jbyte | 8 bits, signed |
| char | jchar | 16 bits, unsigned |
| short | jshort | 16 bits, signed |
| int | jint | 32 bits, signed |
| long | jlong | 64 bits, signed | 
| float | jfloat | 32 bits |
| double | jdouble | 64 bits |
| void | void | not applicable |



## 引用类型

JNI中还支持多种引用类型，其对应的返回类型如下表。

| Java Type | Native Type | Description |
| ---- | ---- | --- |
| Class | jclass | Class类型 |
| Object | jobject | Java中的对象 |
| String | jstring | 字符串 |
| Object[] | jobjectArray | 对象类型数组 |
| boolean[] | jbyteArray | 布尔型数组 |
| char[] | jcharArray | 字符型数组 |
| short[] | jshortArray | 短整型数组 |
| int[] | jintArray | 整形数组 |
| long[] | jlongArray | 长整型数组 |
| float[] | jfloatArray | 浮点型数组 |
| double[] | jdoubleArray | 双浮点型数组 |

## 实战举例

### JNI返回基本类型

定义好返回值即可，此处以返回一个int类型为例子  

**Java层**  

```
public native String stringFromJNI();

```

**JNI层**

```
JNIEXPORT jint JNICALL
Java_com_ikouz_jnidemo_MainActivity_intFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    int testInteger = 1234; // this value could be got from native c function.
    return testInteger;
}
```

### JNI返回String类型

string只需要注意返回值。

**Java层**  

```
public native String stringFromJNI();

```

**JNI层**

```
JNIEXPORT jstring JNICALL
Java_com_ikouz_jnidemo_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
```

### JNI读取并更改Object中字段

从Java中传入一个Object, 先读取出Object中一个字段的值，再更改其中的一个字段。  


**Java层**  

```
public native void objectOpJNI(Config config);

// 以一个名为Conifg的Java类举例
public static class Config {
   public int num;
   public String desc;
   public Config(int num, String desc) {
      this.num = num;
      this.desc = desc;
   }
}
```

对于Object稍微复杂些，需要按照以下几个步骤来操作Object中的值：

- 获取Object相应的类

```
jclass configCls = env->GetObjectClass(configObj);

```
- 获取需要访问的字段对应的id

```
jfieldID numFieldID = env->GetFieldID(configCls, "num", "I");
 
```
这里以Java类中的"num"字段为例，通过GetFieldID获取对应的fieldID，这里的需要注意的是最后一个参数，数据类型签名，和Java类中的字段必须对应上，这里"num"是一个int型数据，对应的类型签名为"I"，完整的签名如下表：

| Type | Signature |
| --- | --- |
| boolean | Z |
| byte | B |
| char | C |
| short | S |
| int |  I |
| long | L |
| float | F |
| double | D |
| void | V |
| Oject | LClassName |
| Arrays | [array-type |
| methods | (argument-types)return-type |

需要注意且比较常用的是String类型，在Java中是Object，因此要获取其fieldID需要用"Ljava/lang/String;"。  

- 获取字段的值

```
jint num = env->GetIntField(configObj, numFieldID);

```

- 再通过fieldID给字段重新赋值

```
env->SetIntField(configCls, numFieldID, 200);
```

- 释放内存(如果需要的话)

**JNI层**

```
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
```

### JNI调用Java方法

JNI同样也可以调用Java类中的方法。

**Java层**

```
public native void jniCallback();

// define a function, change text if it was callback.
public void callbackFromJni(String message) {
    tvCallback.setText(message);
}
```

JNI层按照以下的步骤来调用Java的方法

- 获取Object相应的类

```
jclass callCls = env->GetObjectClass(obj);

```

- 获取需要访问的函数的methodID

```
jmethodID  callbackID = env->GetMethodID(callCls, "callbackFromJni", "(Ljava/lang/String;I)V");

```

第二个参数传递的是要调用的函数名，最后一个参数传递的是被调用函数的参数类型的签名(参考前面的列表)，多个参数间用[**;**]来分隔。

- 通过methodID进行调用

```
env->CallVoidMethod(obj, callbackID, callbackStr, 1);

```
从第三位后，参数个数和被调用函数的参数个数保持一致。


**JNI层**

```
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

```

### JNI返回对象的集合

这是一个比较复杂的例子，要在JNI中创建一个ArrayList并添加数据。详细看代码注释。

**Java层**

```
public native ArrayList<Config> jniGetConfigList();

```
**Native层**

```
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
```

## 总结

JNI中在处理较复杂的类型时，需要特别注意变量类型、返回值及包名的问题；大量的反射会给代码造成程序的健壮性变差，而且JNI层的代码不易于调试，因此建议加入必要的判断及异常情况日志。

--- 
参考文章：

1. [Oracle JNI Types and Data Structures](https://docs.oracle.com/javase/8/docs/technotes/guides/jni/spec/types.html)
2. [JNI 字符串处理](http://wiki.jikexueyuan.com/project/jni-ndk-developer-guide/string.html)
3. [JNI学习积累之三 ---- 操作JNI函数以及复杂对象传递](http://blog.csdn.net/qinjuning/article/details/7607214)