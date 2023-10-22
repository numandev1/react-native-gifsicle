#ifdef __cplusplus
#endif

#include <utility.h>
#ifdef RCT_NEW_ARCH_ENABLED
#import <RNGifsicleSpec/RNGifsicleSpec.h>

@interface GifsicleMain : NSObject <NativeGifsicleSpec>
#else
#import <React/RCTBridgeModule.h>

@interface GifsicleMain : NSObject <RCTBridgeModule>
#endif


+ (NSString *)compressGif: (NSString *)filePath options:(NSDictionary*) options;
+  (GifOptions)gifOptionsFromDictionary:(NSDictionary *)dictionary;

@end
