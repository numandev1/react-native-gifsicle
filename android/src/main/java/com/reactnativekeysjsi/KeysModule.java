package com.reactnativekeysjsi;

import android.content.Context;
import android.content.res.Resources;
import android.util.Log;

import androidx.annotation.NonNull;

import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.JavaScriptContextHolder;
import com.facebook.react.bridge.ReadableMap;

import org.json.JSONObject;
import java.lang.reflect.Field;
import java.util.HashMap;
import java.util.Map;

class GifOptions {
  public int lossy;
  public int optimize;
  public int scale_x;
  public int scale_y;
  public int colors;
}


public class KeysModule extends KeysSpec {
  public static final String NAME = "Keys";
  public static ReactApplicationContext reactContext;

  private native void nativeInstall(long jsiPtr);

  public KeysModule(ReactApplicationContext reactContext) {
    super(reactContext);
    this.reactContext = reactContext;
  }

  @Override
  @NonNull
  public String getName() {
    return NAME;
  }

  @ReactMethod(isBlockingSynchronousMethod = true)
  public boolean install() {
    try {
      JavaScriptContextHolder jsContext = this.reactContext.getJavaScriptContextHolder();
      this.nativeInstall(jsContext.get());
      return true;
    } catch (Exception exception) {
      return false;
    }
  }

  static {
    System.loadLibrary("react-native-gifsicle");
  }

  public static native String getJniJsonStringifyData(String key);


  public static native String compressGifCpp(String filePath, String destFilePath, GifOptions gifOptions);

  @ReactMethod
  public void compressGif(String filePath, ReadableMap options, Promise promise) {
    try {

      GifOptions gifOptions = new GifOptions();
      gifOptions.lossy=150;
      String destPath="file://data/user/0/com.keysexample/cache/compressed.gif";
      promise.resolve(compressGifCpp(filePath,destPath,gifOptions));
    } catch (Exception ignore) {
      promise.resolve("no");
    }
  }


  public static String getSecureFor(String key) {
    JSONObject jniData = null;
    try {
      if (jniData == null) {
        String privateKey = PrivateKey.privatekey;
        String jsonString = getJniJsonStringifyData(privateKey);
        jniData = new JSONObject(jsonString);
      }
      if (jniData.has(key)) {
        return jniData.getString(key);
      }
    } catch (Exception ignore) {
      return "";
    }
    return "";
  }

  public Map<String, Object> getPublicKeys() {
    final Map<String, Object> constants = new HashMap<>();
    try {
      int resId = this.reactContext.getResources().getIdentifier("build_config_package", "string", this.reactContext.getPackageName());

      String className;
      try {
        className = this.reactContext.getString(resId);
      } catch (Resources.NotFoundException e) {
        className = this.reactContext.getPackageName();
      }
      Class clazz = Class.forName(className + ".BuildConfig");

      Field[] fields = clazz.getDeclaredFields();
      for (Field f : fields) {
        try {
          constants.put(f.getName(), f.get(null));
        } catch (IllegalAccessException e) {
          Log.d("ReactNative", "ReactConfig: Could not access BuildConfig field " + f.getName());
        }
      }
    } catch (ClassNotFoundException e) {
      Log.d("ReactNative", "ReactConfig: Could not find BuildConfig class");
    }

    return constants;
  }

}
