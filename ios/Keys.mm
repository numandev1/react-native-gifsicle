#import "Keys.h"
#import <React/RCTBridge+Private.h>
#import <React/RCTUtils.h>
#import <jsi/jsi.h>
#import <sys/utsname.h>
#import "YeetJSIUtils.h"
#import <React/RCTBridge+Private.h>

//#import "crypto.cpp"
#import "crypto.h"
#import "gifsicle_wrapper.h"
#import "GeneratedDotEnv.m"
#import "privateKey.m"

using namespace facebook::jsi;
using namespace std;

@implementation Keys

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

static void install(jsi::Runtime &jsiRuntime, Keys *_Keys) {
    auto secureFor = Function::createFromHostFunction(jsiRuntime,
                                                    PropNameID::forAscii(jsiRuntime,
                                                                         "secureFor"),
                                                    0,
                                                      [_Keys](Runtime &runtime,
                                                             const Value &thisValue,
                                                             const Value *arguments,
                                                             size_t count) -> Value {
        NSString *key = convertJSIStringToNSString(runtime, arguments[0].getString(runtime));
        NSString *value = [Keys secureFor:key];
        return Value(runtime, convertNSStringToJSIString(runtime, value));
    });
    
    jsiRuntime.global().setProperty(jsiRuntime, "secureFor", move(secureFor));
    
    
    auto publicKeys = Function::createFromHostFunction(jsiRuntime,
                                                    PropNameID::forAscii(jsiRuntime,
                                                                         "publicKeys"),
                                                    0,
                                                      [_Keys](Runtime &runtime,
                                                             const Value &thisValue,
                                                             const Value *arguments,
                                                             size_t count) -> Value {
        NSDictionary *s = [Keys public_keys];
        return Value(runtime, convertNSDictionaryToJSIObject(runtime, s));
        
    });
    
    jsiRuntime.global().setProperty(jsiRuntime, "publicKeys", move(publicKeys));
    
    
    auto compressGif = Function::createFromHostFunction(jsiRuntime,
                                                    PropNameID::forAscii(jsiRuntime,
                                                                         "compressGif"),
                                                    2,
                                                      [_Keys](Runtime &runtime,
                                                             const Value &thisValue,
                                                             const Value *arguments,
                                                             size_t count) -> Value {
        NSString *filePath = convertJSIStringToNSString(runtime, arguments[0].getString(runtime));
        NSDictionary *options = convertJSIObjectToNSDictionary(runtime, arguments[1].getObject(runtime));
        NSString *result = [Keys compressGif:filePath options:options];
        return Value(runtime, convertNSStringToJSIString(runtime, result));
        
    });
    
    jsiRuntime.global().setProperty(jsiRuntime, "compressGif", move(compressGif));
    
    
    
}

+ (GifOptions)gifOptionsFromDictionary:(NSDictionary *)dictionary {
    GifOptions options;

    // Set default values
    options.lossy = 200;
    options.optimize = 1;
    options.colors = 256;
    options.scale_x = 1;
    options.scale_y = 1;

    // Check if the dictionary contains keys and update the options accordingly
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
          

                 // Convert NSString to std::string
                 std::string sourceFile = [filePath UTF8String];
                 std::string destFile = [destFilePath UTF8String];

                 // Assuming you have a similar conversion method for gifOptions
                  GifOptions gifOptions = [Keys gifOptionsFromDictionary:options];

                 // Perform your operation with C++ strings
                 std::string result = GifsicleWrapper().compressGifCpp(sourceFile, destFile, gifOptions);
                
                 // Convert the result back to NSString
                 NSString *resultObjC = [NSString stringWithUTF8String:result.c_str()];

                 return resultObjC;
      }
      @catch (NSException *exception) {
          return @"";
      }
  }

+ (NSString *)secureFor: (NSString *)key {
      @try {
          NSDictionary *privatesKeyEnv = PRIVATE_KEY;
          NSString *privateKey = [privatesKeyEnv objectForKey:@"privateKey"];
           NSString* stringfyData = [NSString stringWithCString:Crypto().getJniJsonStringifyData([privateKey cStringUsingEncoding:NSUTF8StringEncoding]).c_str() encoding:[NSString defaultCStringEncoding]];
           NSData *data = [stringfyData dataUsingEncoding:NSUTF8StringEncoding];
           NSMutableDictionary *s = [NSJSONSerialization JSONObjectWithData:data options:0 error:NULL];
           NSString *value =[s objectForKey:key];
          return value;
      }
      @catch (NSException *exception) {
          return @"";
      }
  }

  + (NSDictionary *)public_keys {
    return (NSDictionary *)DOT_ENV;
  }

  + (NSString *)publicFor: (NSString *)key {
      NSString *value = (NSString *)[self.public_keys objectForKey:key];
      return value;
  }

// Don't compile this code when we build for the old architecture.
#ifdef RCT_NEW_ARCH_ENABLED
- (std::shared_ptr<facebook::react::TurboModule>)getTurboModule:
    (const facebook::react::ObjCTurboModule::InitParams &)params {
  return std::make_shared<facebook::react::NativeKeysSpecJSI>(params);
}
#endif
@end
