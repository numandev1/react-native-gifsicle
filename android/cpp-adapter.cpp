#include <jni.h>
#include <sys/types.h>
#include "pthread.h"
#include <jsi/jsi.h>
#include <map>
#include <android/log.h>
#include <string>

#include <sstream>
#include <iomanip>

#include <iostream>

#include <gifsicle.h>
#include "gifsicle_wrapper.h"

using namespace facebook::jsi;
using namespace std;

JavaVM *java_vm;





jclass java_class;
jobject java_object;

// Define a callback function type for progress reporting
//using ProgressCallback = std::function<void(int)>;
//
//// Declare a global progress callback function
//ProgressCallback progressCallback;

GifOptions convertObjectToCppGifOptions(Runtime &runtime,const Object &gifOptions) {
    GifOptions cppGifOptions;

    if (gifOptions.getProperty(runtime,"lossy").isNumber()) {
        cppGifOptions.lossy = gifOptions.getProperty(runtime,"lossy").asNumber();
    } else {
        cppGifOptions.lossy = 200; // Set default value
    }

    if (gifOptions.getProperty(runtime,"optimize").isNumber()) {
        cppGifOptions.optimize = gifOptions.getProperty(runtime,"optimize").asNumber();
    } else {
        cppGifOptions.optimize = 3; // Set default value
    }

    if (gifOptions.getProperty(runtime,"scale_x").isNumber()) {
        cppGifOptions.scale_x = gifOptions.getProperty(runtime,"scale_x").asNumber();
    } else {
        cppGifOptions.scale_x = 1; // Set default value
    }

    if (gifOptions.getProperty(runtime,"scale_y").isNumber()) {
        cppGifOptions.scale_y = gifOptions.getProperty(runtime,"scale_y").asNumber();
    } else {
        cppGifOptions.scale_y = 1; // Set default value
    }

    if (gifOptions.getProperty(runtime,"colors").isNumber()) {
        cppGifOptions.colors = gifOptions.getProperty(runtime,"colors").asNumber();
    } else {
        cppGifOptions.colors = 256; // Set default value
    }

    if (gifOptions.getProperty(runtime,"reduce_frames").isNumber()) {
        cppGifOptions.reduce_frames = gifOptions.getProperty(runtime,"reduce_frames").asNumber();
    } else {
        cppGifOptions.reduce_frames = 0; // Set default value
    }

    return cppGifOptions;
}

std::string jstringToString(JNIEnv *env, jstring jstr)
{
    const char *cstr = env->GetStringUTFChars(jstr, nullptr);
    std::string str(cstr);
    env->ReleaseStringUTFChars(jstr, cstr);
    return str;
}


/**
 * A simple callback function that allows us to detach current JNI Environment
 * when the thread
 * See https://stackoverflow.com/a/30026231 for detailed explanation
 */

void DeferThreadDetach(JNIEnv *env)
{
    static pthread_key_t thread_key;

    // Set up a Thread Specific Data key, and a callback that
    // will be executed when a thread is destroyed.
    // This is only done once, across all threads, and the value
    // associated with the key for any given thread will initially
    // be NULL.
    static auto run_once = []
    {
        const auto err = pthread_key_create(&thread_key, [](void *ts_env)
                                            {
            if (ts_env) {
                java_vm->DetachCurrentThread();
            } });
        if (err)
        {
            // Failed to create TSD key. Throw an exception if you want to.
        }
        return 0;
    }();

    // For the callback to actually be executed when a thread exits
    // we need to associate a non-NULL value with the key on that thread.
    // We can use the JNIEnv* as that value.
    const auto ts_env = pthread_getspecific(thread_key);
    if (!ts_env)
    {
        if (pthread_setspecific(thread_key, env))
        {
            // Failed to set thread-specific value for key. Throw an exception if you want to.
        }
    }
}

/**
 * Get a JNIEnv* valid for this thread, regardless of whether
 * we're on a native thread or a Java thread.
 * If the calling thread is not currently attached to the JVM
 * it will be attached, and then automatically detached when the
 * thread is destroyed.
 *
 * See https://stackoverflow.com/a/30026231 for detailed explanation
 */
JNIEnv *GetJniEnv()
{
    JNIEnv *env = nullptr;
    // We still call GetEnv first to detect if the thread already
    // is attached. This is done to avoid setting up a DetachCurrentThread
    // call on a Java thread.

    // g_vm is a global.
    auto get_env_result = java_vm->GetEnv((void **)&env, JNI_VERSION_1_6);
    if (get_env_result == JNI_EDETACHED)
    {
        if (java_vm->AttachCurrentThread(&env, NULL) == JNI_OK)
        {
            DeferThreadDetach(env);
        }
        else
        {
            // Failed to attach thread. Throw an exception if you want to.
        }
    }
    else if (get_env_result == JNI_EVERSION)
    {
        // Unsupported JNI version. Throw an exception if you want to.
    }
    return env;
}

static jstring string2jstring(JNIEnv *env, const string &str)
{
    return (*env).NewStringUTF(str.c_str());
}

void install(facebook::jsi::Runtime &jsiRuntime)
{
    auto compressGif = Function::createFromHostFunction(jsiRuntime,
                                                        PropNameID::forAscii(jsiRuntime,
                                                                             "compressGif"),
                                                        2,
                                                        [](Runtime &runtime,
                                                                const Value &thisValue,
                                                                const Value *arguments,
                                                                size_t count) -> Value {
                                                            string filePath = arguments[0].getString(
                                                                            runtime)
                                                                    .utf8(
                                                                            runtime);
                                                            string destFilePathStr="file://data/user/0/com.GifsicleExample/cache/compressed.gif";
                                                            JNIEnv *jniEnv = GetJniEnv();

                                                            Object options = arguments[1].getObject(runtime);
                                                            GifOptions optionss = convertObjectToCppGifOptions(runtime, options);
                                                             string compressedPath = GifsicleWrapper().compressGifCpp(filePath, destFilePathStr, optionss);
                                                            return Value(runtime,
                                                                         String::createFromUtf8(
                                                                                 runtime, compressedPath));

                                                        });

    jsiRuntime.global().setProperty(jsiRuntime, "compressGif", move(compressGif));
}

extern "C" JNIEXPORT void JNICALL
Java_com_gifsicle_GifsicleModule_nativeInstall(JNIEnv *env, jobject thiz, jlong jsi)
{

    auto runtime = reinterpret_cast<facebook::jsi::Runtime *>(jsi);

    if (runtime)
    {
        // example::install(*runtime);
        install(*runtime);
    }

    env->GetJavaVM(&java_vm);
    java_object = env->NewGlobalRef(thiz);
}
