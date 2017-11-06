package com.ikouz.jnidemo;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

import java.util.ArrayList;

public class MainActivity extends AppCompatActivity {

    private TextView tvCallback;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of int jni call
        ((TextView) findViewById(R.id.int_text)).setText("int:" + intFromJNI());
        // Example of string jni call
        ((TextView) findViewById(R.id.string_text)).setText("string:" + stringFromJNI());

        // Example of object jni call.
        StringBuilder sb = new StringBuilder();
        Config config = new Config(404, "not found");
        sb.append("object [before]");
        sb.append(" num:");
        sb.append(config.num);
        sb.append(" desc:");
        sb.append(config.desc);
        sb.append(" \n");
        // native method
        objectOpJNI(config);

        sb.append("object [after]");
        sb.append(" num:");
        sb.append(config.num);
        sb.append(" desc:");
        sb.append(config.desc);
        ((TextView) findViewById(R.id.object_text)).setText(sb.toString());

        tvCallback = (TextView)findViewById(R.id.callback_text);
        jniCallback();

        ArrayList<Config> configs = jniGetConfigList();
        StringBuilder configSb = new StringBuilder();
        if (configs != null) {
            for (Config c : configs) {
                configSb.append(c.num);
                configSb.append(" ");
                configSb.append(c.desc);
                configSb.append("\n");
            }
        }
        ((TextView) findViewById(R.id.jni_list_text)).setText(configSb.toString());

    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native int intFromJNI();

    public native void objectOpJNI(Config config);

    public native void jniCallback();

    public native ArrayList<Config> jniGetConfigList();

    // define a function, change text if it was callback.
    public void callbackFromJni(String message, int time) {
        tvCallback.setText(time + " " + message);
    }

    public static class Config {
        public int num;
        public String desc;
        public Config(int num, String desc) {
            this.num = num;
            this.desc = desc;
        }
    }

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }
}
