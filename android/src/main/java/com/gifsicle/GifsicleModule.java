package com.gifsicle;

import android.content.res.Resources;
import android.util.Log;

import androidx.annotation.NonNull;

import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.ReactApplicationContext;
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

public class GifsicleModule extends GifsicleSpec {
  public static final String NAME = "GifsicleMain";
  public static ReactApplicationContext reactContext;

  private native void nativeInstall(long jsiPtr);

  public GifsicleModule(ReactApplicationContext reactContext) {
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

  public static native String compressGifCpp(String filePath, String destFilePath, GifOptions gifOptions);

  @ReactMethod
  public void compressGif(String filePath, ReadableMap options, Promise promise) {
    try {

      GifOptions gifOptions = new GifOptions();
      gifOptions.lossy = 150;
      String destPath = "file://data/user/0/com.GifsicleExample/cache/compressed.gif";
      promise.resolve(compressGifCpp(filePath, destPath, gifOptions));
    } catch (Exception ignore) {
      promise.resolve("no");
    }
  }



}
