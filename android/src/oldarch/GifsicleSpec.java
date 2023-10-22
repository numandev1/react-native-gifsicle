package com.gifsicle;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.Promise;

abstract class GifsicleSpec extends ReactContextBaseJavaModule {
  GifsicleSpec(ReactApplicationContext context) {
    super(context);
  }

  public abstract boolean install();

}
