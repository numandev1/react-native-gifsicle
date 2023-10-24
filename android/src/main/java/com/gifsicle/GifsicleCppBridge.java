package com.gifsicle;

import com.facebook.react.bridge.ReactContext;
import com.facebook.react.turbomodule.core.CallInvokerHolderImpl;

public class GifsicleCppBridge {
  private native void installNativeJsi(long jsContextNativePointer, CallInvokerHolderImpl jsCallInvokerHolder, String docPath);
  public static final GifsicleCppBridge instance = new GifsicleCppBridge();

  public void install(ReactContext context) {

    long jsContextPointer = context.getJavaScriptContextHolder().get();
    CallInvokerHolderImpl jsCallInvokerHolder = (CallInvokerHolderImpl)context.getCatalystInstance().getJSCallInvokerHolder();
    final String path = context.getCacheDir().getAbsolutePath();

    installNativeJsi(
      jsContextPointer,
      jsCallInvokerHolder,
      path
    );
  }
}
