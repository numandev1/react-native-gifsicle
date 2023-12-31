project(react-native-gifsicle)
cmake_minimum_required(VERSION 3.9.0)

set(TARGET_NAME gifsiclelib)
set(PACKAGE_NAME "react-native-gifsicle")
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)
set(BUILD_DIR ${CMAKE_SOURCE_DIR}/build)
set(GIFSICLE_INCLUDE "${CMAKE_SOURCE_DIR}/../cpp/include")

set(SOURCES
    ../cpp/gifsicle/clp.c
    ../cpp/gifsicle/fmalloc.c
    ../cpp/gifsicle/giffunc.c
    ../cpp/gifsicle/gifread.c
    ../cpp/gifsicle/gifunopt.c
    ../cpp/gifsicle/merge.c
    ../cpp/gifsicle/optimize.c
    ../cpp/gifsicle/quantize.c
    ../cpp/gifsicle/support.c
    ../cpp/gifsicle/xform.c
    ../cpp/gifsicle/gifsicle.c
    ../cpp/gifsicle/gifwrite.c
    ../cpp/gifsicle/progress.cpp
    ../cpp/gifsicle/utility.cpp
)

    # Consume shared libraries and headers from prefabs
    set(DEPS_INSTALL_DIR ${CMAKE_BINARY_DIR}/../cpp/includes)

    include_directories(
        ${PACKAGE_NAME}
        "../cpp"
            ${DEPS_INSTALL_DIR}
            ${GIFSICLE_INCLUDE}
    )

    file(GLOB LIBRN_DIR " ${BUILD_DIR}/react-native-0*/jni/${ANDROID_ABI} ")
    message(STATUS " LIBRN_DIR: ${LIBRN_DIR} ")


    add_library(
        ${PACKAGE_NAME}
        SHARED
            ../cpp/bindings.cpp
            ../cpp/bindings.h
        ./cpp-adapter.cpp
            ../cpp/ThreadPool.h
            ../cpp/ThreadPool.cpp
            ../cpp/gifsicleutil.h
            ../cpp/gifsicleutil.cpp
            ../cpp/gifsicle_wrapper.h
            ../cpp/gifsicle_wrapper.cpp
            ../cpp/filesystem_wrapper.h
            ../node_modules/react-native/ReactAndroid/src/main/jni/react/turbomodule/ReactCommon/CallInvokerHolder.cpp
            ${SOURCES}
    )

#    target_include_directories(${PACKAGE_NAME} PUBLIC ${GIFSICLELIB_FOLDER})
#    target_include_directories(${PACKAGE_NAME} PUBLIC ${GIFSICLE_INCLUDE} ${DEPS_INSTALL_DIR}/include)

target_include_directories(
        ${PACKAGE_NAME}
        PRIVATE
        PUBLIC ${GIFSICLELIB_FOLDER}
        PUBLIC ${GIFSICLE_INCLUDE} ${DEPS_INSTALL_DIR}/include
        "../cpp"
        "${NODE_MODULES_DIR}/react-native/ReactCommon"
        "${NODE_MODULES_DIR}/react-native/ReactCommon/callinvoker"
        "${NODE_MODULES_DIR}/react-native/ReactAndroid/src/main/jni/react/turbomodule" # <-- CallInvokerHolder JNI wrapper
)

    target_compile_definitions(${PACKAGE_NAME} PUBLIC HAVE_CONFIG_H)

    find_library(
        LOG_LIB
        log
    )

    find_library(
        REACT_NATIVE_JNI_LIB
        reactnativejni
        PATHS ${LIBRN_DIR}
        NO_CMAKE_FIND_ROOT_PATH
    )

    set_target_properties(
        ${PACKAGE_NAME} PROPERTIES
        CXX_STANDARD 17
        CXX_EXTENSIONS OFF
        POSITION_INDEPENDENT_CODE ON
    )

    find_package(ReactAndroid REQUIRED CONFIG)
    find_package(fbjni REQUIRED CONFIG)

    target_link_libraries(
        ${PACKAGE_NAME}
        ${LOG_LIB}
            fbjni::fbjni
        ReactAndroid::jsi
        ReactAndroid::reactnativejni
        ReactAndroid::react_nativemodule_core
        android
    )
