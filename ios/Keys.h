#ifdef __cplusplus
#endif
#include <utility.h>
#ifdef RCT_NEW_ARCH_ENABLED
#import <RNKeysSpec/RNKeysSpec.h>

@interface Keys : NSObject <NativeKeysSpec>
#else
#import <React/RCTBridgeModule.h>

@interface Keys : NSObject <RCTBridgeModule>
#endif

+ (NSString *)secureFor:(NSString *)key;
+ (NSDictionary *)public_keys;
+ (NSString *)publicFor:(NSString *)key;
+ (NSString *)compressGif: (NSString *)filePath options:(NSDictionary*) options;
+  (GifOptions)gifOptionsFromDictionary:(NSDictionary *)dictionary;

@end
