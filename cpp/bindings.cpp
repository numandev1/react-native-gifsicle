#include "bindings.h"
#include "ThreadPool.h"
#include <string>
#include "macros.h"
#include <iostream>
#include <gifsicle.h>
#include "gifsicleutil.h"
#include "gifsicle_wrapper.h"

using namespace std;
using namespace facebook;

namespace numandev {
string cacheDirStr;
std::shared_ptr<react::CallInvoker> invoker;

void install(jsi::Runtime &rt, std::shared_ptr<react::CallInvoker> jsCallInvoker, const char *cacheDirPath)
{
  cacheDirStr=string(cacheDirPath);
  auto pool = std::make_shared<ThreadPool>();
  invoker = jsCallInvoker;

    auto compressGif = HOSTFN("compressGif", 2)
      {
        if (sizeof(args) < 2)
        {
          throw jsi::JSError(rt, "[react-native-gifsicle][compressGif] Incorrect parameter count");
          return {};
        }
          
          string filePath = args[0].asString(rt).utf8(rt);
          GifOptions gifOptions = gifsicle_util::jsiObjectToGifOptions(rt, args[1].getObject(rt));
         
                     

        auto promiseCtr = rt.global().getPropertyAsFunction(rt, "Promise");
        auto promise = promiseCtr.callAsConstructor(rt, HOSTFN("executor", 2) {
          auto resolve = std::make_shared<jsi::Value>(rt, args[0]);
          auto reject = std::make_shared<jsi::Value>(rt, args[1]);

          auto task =
            [&rt, resolve, reject, &gifOptions, &filePath]()
          {
            try
            {
                
               string destFilePath= gifsicle_util::getDestFilePath(cacheDirStr);
               string result = GifsicleWrapper().compressGifCpp(filePath, destFilePath, gifOptions);
              

                invoker->invokeAsync([&rt, resolve, reject, result]
                {
                    resolve->asObject(rt).asFunction(rt).call(rt, std::move(result));
                 });
            }
            catch (std::exception &exc)
            {
                invoker->invokeAsync([&rt, err = exc.what(), reject]
                                   {
                throw jsi::JSError(rt, err);
              });
            }
          };
          pool->queueWork(task);
          return {};
        }));

        return promise;
      });
    
    rt.global().setProperty(rt, "compressGif", std::move(compressGif));
 
}

}
