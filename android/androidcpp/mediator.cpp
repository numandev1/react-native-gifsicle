
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

std::string jstring2string(JNIEnv *env, jstring jStr)
{
    if (!jStr)
        return "";

    const jclass stringClass = env->GetObjectClass(jStr);
    const jmethodID getBytes = env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
    const jbyteArray stringJbytes = (jbyteArray) env->CallObjectMethod(jStr, getBytes,
                                                                       env->NewStringUTF("UTF-8"));

    size_t length = (size_t) env->GetArrayLength(stringJbytes);
    jbyte *pBytes = env->GetByteArrayElements(stringJbytes, NULL);

    std::string ret = std::string((char *) pBytes, length);
    env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

    env->DeleteLocalRef(stringJbytes);
    env->DeleteLocalRef(stringClass);
    return ret;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_reactnativekeysjsi_KeysModule_getJniJsonStringifyData(JNIEnv * env, jobject thiz,jstring key) {
    auto *crypto = new Crypto();
    std::string _key{jstring2string(env,key)};
    return env->NewStringUTF(crypto->getJniJsonStringifyData(_key).c_str());
}


static void gifread_error(Gif_Stream* gfs, Gif_Image* gfi, int is_error, const char* error_text)
{
    if (is_error >= 0)
    {
        __android_log_print(ANDROID_LOG_ERROR, "nomi nomi error", "Error reading GIF: %s", error_text);
        std::cerr << "Error reading GIF: " << error_text << std::endl;
    }

}

GifOptions convertJavaToCppGifOptions(JNIEnv *env, jobject gifOptions) {
    jclass gifOptionsClass = env->GetObjectClass(gifOptions);
    GifOptions cppGifOptions;

    jfieldID lossyField = env->GetFieldID(gifOptionsClass, "lossy", "I");
    cppGifOptions.lossy = lossyField ? env->GetIntField(gifOptions, lossyField) : 0;
    if (cppGifOptions.lossy == 0) {
        cppGifOptions.lossy = 200; // Set default value
    }

    jfieldID optimizeField = env->GetFieldID(gifOptionsClass, "optimize", "I");
    cppGifOptions.optimize = optimizeField ? env->GetIntField(gifOptions, optimizeField) : 0;
    if (cppGifOptions.optimize == 0) {
        cppGifOptions.optimize = 3; // Set default value
    }

    jfieldID scale_xField = env->GetFieldID(gifOptionsClass, "scale_x", "I");
    cppGifOptions.scale_x = scale_xField ? env->GetIntField(gifOptions, scale_xField) : 0;
    if (cppGifOptions.scale_x == 0) {
        cppGifOptions.scale_x = 1; // Set default value
    }

    jfieldID scale_yField = env->GetFieldID(gifOptionsClass, "scale_y", "I");
    cppGifOptions.scale_y = scale_yField ? env->GetIntField(gifOptions, scale_yField) : 0;
    if (cppGifOptions.scale_y == 0) {
        cppGifOptions.scale_y = 1; // Set default value
    }

    jfieldID colorsField = env->GetFieldID(gifOptionsClass, "colors", "I");
    cppGifOptions.colors = colorsField ? env->GetIntField(gifOptions, colorsField) : 0;
    if (cppGifOptions.colors == 0) {
        cppGifOptions.colors = 256; // Set default value
    }

    return cppGifOptions;
}

std::string pathWithoutFileUri(JNIEnv * env,jstring filePath){
    std::string _filePath{jstring2string(env,filePath)};
    std::regex pattern("^file://*");
    _filePath = std::regex_replace(_filePath, pattern, "");
    return _filePath;
}

std::string slashifyFilePath(JNIEnv * env,std::string filePath){
    if (filePath.find("/") == 0) {
        std::regex pattern("^/+");
        filePath = std::regex_replace(filePath, pattern, "file:///");
    }

    return filePath;
}


extern "C" JNIEXPORT jstring JNICALL
Java_com_reactnativekeysjsi_KeysModule_compressGifCpp(JNIEnv * env, jobject thiz,jstring filePath, jstring destFilePath, jobject gifOptions) {

    std::string filePathStr=pathWithoutFileUri(env,filePath);
    std::string destFilePathStr=pathWithoutFileUri(env,destFilePath);

    const std::filesystem::path gif_file{filePathStr};

    const auto gif_bytes{
            animately::loadFile(gif_file.u8string())};



    auto* gif{Gif_FullReadBuffer(reinterpret_cast<const uint8_t*>(gif_bytes.data()),
                                 gif_bytes.size(), GIF_READ_UNCOMPRESSED,
                                 "", gifread_error)};

    GifOptions options = convertJavaToCppGifOptions(env, gifOptions);

    GifBuffer buffer;
    COptions coptions;
    parse_options(options, coptions);

    gifsicle_main(coptions.count(), coptions.options(), gif, &buffer.data, &buffer.size);

    bool isSaved=animately::saveFile(reinterpret_cast<const std::byte*>(buffer.data),buffer.size,destFilePathStr);
    if(isSaved)
    {
        std::string destSlashifyFilePath=slashifyFilePath(env,destFilePathStr);
        jstring result = env->NewStringUTF(destSlashifyFilePath.c_str());
        return result;
    } else{
        std::string actualSlashifyFilePath=slashifyFilePath(env,filePathStr);
        jstring result = env->NewStringUTF(actualSlashifyFilePath.c_str());
        return result;
    }

}
