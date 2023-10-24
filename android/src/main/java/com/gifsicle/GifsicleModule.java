package com.gifsicle;


import androidx.annotation.NonNull;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactMethod;

public class GifsicleModule extends GifsicleSpec {
  public static final String NAME = "GifsicleMain";
  public static ReactApplicationContext reactContext;

  public GifsicleModule(ReactApplicationContext reactContext) {
    super(reactContext);
    this.reactContext = reactContext;
  }

  @NonNull
  @Override
  public String getName() {
    return NAME;
  }

  @ReactMethod(isBlockingSynchronousMethod = true)
  public boolean install() {
    try {
      System.loadLibrary("react-native-gifsicle");
      GifsicleCppBridge.instance.install(this.reactContext);
      return true;
    } catch (Exception exception) {
      return false;
    }
  }

}
