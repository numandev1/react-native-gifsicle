#include <ReactCommon/CallInvokerHolder.h>
#include <fbjni/fbjni.h>
#include <jni.h>
#include <jsi/jsi.h>
#include <typeinfo>
#include "bindings.h"


struct GifsicleCppBridge : jni::JavaClass<GifsicleCppBridge> {
    static constexpr auto kJavaDescriptor =
            "Lcom/gifsicle/GifsicleCppBridge;";

    static void registerNatives() {
        javaClassStatic()->registerNatives(
                {// initialization for JSI
                        makeNativeMethod("installNativeJsi",
                                         GifsicleCppBridge::installNativeJsi),
                });
    }

private:
    static void installNativeJsi(
            jni::alias_ref<jni::JObject> thiz, jlong jsiRuntimePtr,
            jni::alias_ref<react::CallInvokerHolder::javaobject> jsCallInvokerHolder,
            jni::alias_ref<jni::JString> cacheDirPath) {
        auto jsiRuntime = reinterpret_cast<jsi::Runtime *>(jsiRuntimePtr);
        auto jsCallInvoker = jsCallInvokerHolder->cthis()->getCallInvoker();
        std::string cacheDirPathString = cacheDirPath->toStdString();

        numandev::install(*jsiRuntime, jsCallInvoker, cacheDirPathString.c_str());
    }

};




JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *) {
    return jni::initialize(vm, [] { GifsicleCppBridge::registerNatives(); });
}
