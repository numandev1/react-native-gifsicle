
#include <jni.h>
#include <string>
#include "crypto.h"
#include <utility.h>
#include <gifsicle.h>
#include "testutils.h"
#include <filesystem>
#include <android/log.h>
#include <iostream>
#include <inttypes.h>
#include <regex>
#include <progress.h>
#include "gifsicle_wrapper.h"
using namespace std;
string jstring2string(JNIEnv *env, jstring jStr)
{
    if (!jStr)
        return "";

    const jclass stringClass = env->GetObjectClass(jStr);
    const jmethodID getBytes = env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
    const jbyteArray stringJbytes = (jbyteArray)env->CallObjectMethod(jStr, getBytes,
                                                                      env->NewStringUTF("UTF-8"));

    size_t length = (size_t)env->GetArrayLength(stringJbytes);
    jbyte *pBytes = env->GetByteArrayElements(stringJbytes, NULL);

    string ret = string((char *)pBytes, length);
    env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

    env->DeleteLocalRef(stringJbytes);
    env->DeleteLocalRef(stringClass);
    return ret;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_reactnativekeysjsi_KeysModule_getJniJsonStringifyData(JNIEnv *env, jobject thiz, jstring key)
{
    auto *crypto = new Crypto();
    string _key{jstring2string(env, key)};
    return env->NewStringUTF(crypto->getJniJsonStringifyData(_key).c_str());
}


GifOptions convertJavaToCppGifOptions(JNIEnv *env, jobject gifOptions)
{
    jclass gifOptionsClass = env->GetObjectClass(gifOptions);
    GifOptions cppGifOptions;

    jfieldID lossyField = env->GetFieldID(gifOptionsClass, "lossy", "I");
    cppGifOptions.lossy = lossyField ? env->GetIntField(gifOptions, lossyField) : 0;
    if (cppGifOptions.lossy == 0)
    {
        cppGifOptions.lossy = 200; // Set default value
    }

    jfieldID optimizeField = env->GetFieldID(gifOptionsClass, "optimize", "I");
    cppGifOptions.optimize = optimizeField ? env->GetIntField(gifOptions, optimizeField) : 0;
    if (cppGifOptions.optimize == 0)
    {
        cppGifOptions.optimize = 3; // Set default value
    }

    jfieldID scale_xField = env->GetFieldID(gifOptionsClass, "scale_x", "I");
    cppGifOptions.scale_x = scale_xField ? env->GetIntField(gifOptions, scale_xField) : 0;
    if (cppGifOptions.scale_x == 0)
    {
        cppGifOptions.scale_x = 1; // Set default value
    }

    jfieldID scale_yField = env->GetFieldID(gifOptionsClass, "scale_y", "I");
    cppGifOptions.scale_y = scale_yField ? env->GetIntField(gifOptions, scale_yField) : 0;
    if (cppGifOptions.scale_y == 0)
    {
        cppGifOptions.scale_y = 1; // Set default value
    }

    jfieldID colorsField = env->GetFieldID(gifOptionsClass, "colors", "I");
    cppGifOptions.colors = colorsField ? env->GetIntField(gifOptions, colorsField) : 0;
    if (cppGifOptions.colors == 0)
    {
        cppGifOptions.colors = 256; // Set default value
    }

    return cppGifOptions;
}


extern "C" JNIEXPORT jstring JNICALL
Java_com_reactnativekeysjsi_KeysModule_compressGifCpp(JNIEnv *env, jobject thiz, jstring filePath, jstring destFilePath, jobject gifOptions)
{
    std::string sourceFilePathStr{jstring2string(env,filePath)};
    std::string destFilePathStr{jstring2string(env,destFilePath)};

    GifOptions options = convertJavaToCppGifOptions(env, gifOptions);
    string compressedPath = GifsicleWrapper().compressGifCpp(sourceFilePathStr, destFilePathStr, options);
    jstring result = env->NewStringUTF(compressedPath.c_str());
    return result;
}
