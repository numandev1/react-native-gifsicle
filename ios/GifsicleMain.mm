#import "GifsicleMain.h"
#import <React/RCTBridge+Private.h>
#import <React/RCTUtils.h>
#import <jsi/jsi.h>
#import <sys/utsname.h>
#import "YeetJSIUtils.h"
#import <React/RCTBridge+Private.h>
#import "../cpp/bindings.h"
#include <ThreadPool.h>
#include "macros.h"
#include <gifsicleutil.h>


#import <React/RCTUtils.h>
#import <ReactCommon/RCTTurboModule.h>
#include <ReactCommon/CallInvoker.h>


using namespace facebook::jsi;
using namespace std;

@implementation GifsicleMain

@synthesize bridge = _bridge;
@synthesize methodQueue = _methodQueue;
string cacheDirStr;

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
    
    auto &runtime = *jsiRuntime;
    auto callInvoker = bridge.jsCallInvoker;
    
    numandev::install(runtime, callInvoker, [NSTemporaryDirectory() UTF8String]);
//    install(runtime, callInvoker,[NSTemporaryDirectory() UTF8String], self);
  
   
    return @true;
}


    
//static void install(jsi::Runtime &rt, std::shared_ptr<react::CallInvoker> jsCallInvoker, const char *cacheDirPath, GifsicleMain *_Gifsicle) {
//    auto pool = std::make_shared<ThreadPool>();
//    cacheDirStr=cacheDirPath;
//    auto compressGif = HOSTFN("compressGif", 2)
//      {
//        if (sizeof(args) < 2)
//        {
//          throw jsi::JSError(rt, "[react-native-gifsicle][compressGif] Incorrect parameter count");
//          return {};
//        }
//
//         
//          NSString *_filePath = convertJSIStringToNSString(rt, args[0].getString(rt));
//          string filePath = [_filePath UTF8String];
//
//          GifOptions gifOptions = gifsicle_util::jsiObjectToGifOptions(rt, args[1].getObject(rt));
//         
//                     
//
//        auto promiseCtr = rt.global().getPropertyAsFunction(rt, "Promise");
//        auto promise = promiseCtr.callAsConstructor(rt, HOSTFN("executor", 2) {
//          auto resolve = std::make_shared<jsi::Value>(rt, args[0]);
//          auto reject = std::make_shared<jsi::Value>(rt, args[1]);
//
//          auto task =
//            [&rt,&jsCallInvoker, resolve, reject, &gifOptions, &filePath]()
//          {
//            try
//            {
//                
//               string destFilePath= gifsicle_util::getDestFilePath(cacheDirStr);
//               string result = GifsicleWrapper().compressGifCpp(filePath, destFilePath, gifOptions);
//              
//
//                jsCallInvoker->invokeAsync([&rt, resolve, reject, result]
//                {
//                    resolve->asObject(rt).asFunction(rt).call(rt, std::move(result));
//                 });
//            }
//            catch (std::exception &exc)
//            {
//                jsCallInvoker->invokeAsync([&rt, err = exc.what(), reject]
//                                   {
//                throw jsi::JSError(rt, err);
//              });
//            }
//          };
//          pool->queueWork(task);
//          return {};
//        }));
//
//        return promise;
//      });
//    
//    rt.global().setProperty(rt, "compressGif", std::move(compressGif));
//    
//}

// Don't compile this code when we build for the old architecture.
#ifdef RCT_NEW_ARCH_ENABLED
- (shared_ptr<facebook::react::TurboModule>)getTurboModule:
    (const facebook::react::ObjCTurboModule::InitParams &)params {
  return make_shared<facebook::react::NativeGifsicleSpecJSI>(params);
}
#endif
@end
