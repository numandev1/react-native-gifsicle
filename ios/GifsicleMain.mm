#import "GifsicleMain.h"
#import <React/RCTBridge+Private.h>
#import <React/RCTUtils.h>
#import <jsi/jsi.h>
#import <sys/utsname.h>
#import "YeetJSIUtils.h"
#import <React/RCTBridge+Private.h>
#import "gifsicle_wrapper.h"
#include <thread>

using namespace facebook::jsi;
using namespace std;

@implementation GifsicleMain

@synthesize bridge = _bridge;
@synthesize methodQueue = _methodQueue;

RCT_EXPORT_MODULE()

+ (BOOL)requiresMainQueueSetup {
    return YES;
}

// Installing JSI Bindings
RCT_EXPORT_BLOCKING_SYNCHRONOUS_METHOD(install)
{
    RCTBridge* bridge = [RCTBridge currentBridge];
    RCTCxxBridge* cxxBridge = (RCTCxxBridge*)bridge;
    if (cxxBridge == nil) {
        return @false;
    }

    auto jsiRuntime = (jsi::Runtime*) cxxBridge.runtime;
    if (jsiRuntime == nil) {
        return @false;
    }

//    example::install(*(facebook::jsi::Runtime *)jsiRuntime);
    install(*(facebook::jsi::Runtime *)jsiRuntime, self);
  
   
    return @true;
}
    
static void install(jsi::Runtime &jsiRuntime, GifsicleMain *_Gifsicle) {
    
    auto compressGif = Function::createFromHostFunction(jsiRuntime,
                                                    PropNameID::forAscii(jsiRuntime,
                                                                         "compressGif"),
                                                    3,
                                                      [_Gifsicle](Runtime &runtime,
                                                             const Value &thisValue,
                                                             const Value *arguments,
                                                             size_t count) -> Value {
       
//        NSString *result = [GifsicleMain compressGif:filePath options:options];
//        return Value(runtime, convertNSStringToJSIString(runtime, result));
        
        auto userCallbackRef = std::make_shared<jsi::Object>(arguments[2].getObject(runtime));

//           auto f = [&runtime](shared_ptr<Object> userCallbackRef) {
//               NSString *filePath = convertJSIStringToNSString(runtime, arguments[0].getString(runtime));
//               NSDictionary *options = convertJSIObjectToNSDictionary(runtime, arguments[1].getObject(runtime));
//               NSString *result = [GifsicleMain compressGif:filePath options:options];
//               auto val = jsi::String::createFromUtf8(runtime, std::to_string(std::rand()));
//               auto val=convertNSStringToJSIString(runtime,result);
//               auto val = String::createFromUtf8(runtime, "hello world");
//               auto error = Value::undefined();
//               userCallbackRef->asFunction(runtime).call(runtime, error, val);
//           };
        
        auto f = [&runtime](shared_ptr<Object> userCallbackRef) {
                            auto val = String::createFromUtf8(runtime, "hello world");
                            auto error = Value::undefined();
                            userCallbackRef->asFunction(runtime).call(runtime, val);
                        };

           std::thread thread_object(f,userCallbackRef);
           thread_object.detach();

           return Value::undefined();
        
    });
    
    jsiRuntime.global().setProperty(jsiRuntime, "compressGif", move(compressGif));
    
}

+ (GifOptions)gifOptionsFromDictionary:(NSDictionary *)dictionary {
    GifOptions options;

    // Set default values
    options.lossy = 200;
    options.optimize = 3;
    options.colors = 256;
    options.scale_x = 0;
    options.scale_y = 0;
    options.reduce_frames=0;
    options.width = 0;
    options.height = 0;

    // Check if the dictionary contains Gifsicle and update the options accordingly
    NSNumber *lossyValue = dictionary[@"lossy"];
    if (lossyValue != nil) {
        options.lossy = [lossyValue integerValue];
    }

    NSNumber *optimizeValue = dictionary[@"optimize"];
    if (optimizeValue != nil) {
        options.optimize = [optimizeValue integerValue];
    }

    NSNumber *colorsValue = dictionary[@"colors"];
    if (colorsValue != nil) {
        options.colors = [colorsValue integerValue];
    }

    NSNumber *scaleXValue = dictionary[@"scale_x"];
    if (scaleXValue != nil) {
        options.scale_x = [scaleXValue integerValue];
    }

    NSNumber *scaleYValue = dictionary[@"scale_y"];
    if (scaleYValue != nil) {
        options.scale_y = [scaleYValue integerValue];
    }
    
    NSNumber *reduceFramesValue = dictionary[@"reduce_frames"];
    if (reduceFramesValue != nil) {
        options.reduce_frames = [reduceFramesValue integerValue];
    }
    
    NSNumber *widthValue = dictionary[@"width"];
    if (widthValue != nil) {
        options.width = [widthValue integerValue];
    }
    
    NSNumber *heightValue = dictionary[@"height"];
    if (heightValue != nil) {
        options.height = [heightValue integerValue];
    }

    return options;
}

+ (NSString *)compressGif: (NSString *)filePath options:(NSDictionary*) options {
      @try {
          GifsicleWrapper wrapper;
       
          
          NSUUID *uuid = [NSUUID UUID];
          NSString *imageNameWihtoutExtension = [uuid UUIDString];
          NSString *imageName=[imageNameWihtoutExtension stringByAppendingPathExtension:@"gif"];
          NSString *destFilePath =
              [NSTemporaryDirectory() stringByAppendingPathComponent:imageName];
          

                 // Convert NSString to string
                 string sourceFile = [filePath UTF8String];
                 string destFile = [destFilePath UTF8String];

                 // Assuming you have a similar conversion method for gifOptions
                  GifOptions gifOptions = [GifsicleMain gifOptionsFromDictionary:options];

                 // Perform your operation with C++ strings
                 string result = GifsicleWrapper().compressGifCpp(sourceFile, destFile, gifOptions);
                
                 // Convert the result back to NSString
                 NSString *resultObjC = [NSString stringWithUTF8String:result.c_str()];

                 return resultObjC;
      }
      @catch (NSException *exception) {
          return @"";
      }
  }

// Don't compile this code when we build for the old architecture.
#ifdef RCT_NEW_ARCH_ENABLED
- (shared_ptr<facebook::react::TurboModule>)getTurboModule:
    (const facebook::react::ObjCTurboModule::InitParams &)params {
  return make_shared<facebook::react::NativeGifsicleSpecJSI>(params);
}
#endif
@end
