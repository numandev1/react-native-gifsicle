//
//  gifsicle_wrapper.m
//  react-native-gifsicle
//
//  Created by Numan on 16/10/2023.
//

#import <Foundation/Foundation.h>
#include "gifsicle_wrapper.h"
#include <string>
#include <gifsicle.h>
#include "testutils.h"
#include <iostream>

using namespace std;

GifsicleWrapper::GifsicleWrapper() {

}

NSString* pathWithoutFileUri(NSString* filePath) {
    if ([filePath hasPrefix:@"file://"]) {
        filePath = [filePath substringFromIndex:7];
    }
    return filePath;
}

NSString* slashifyFilePath(NSString *filePath) {
    if (![filePath isEqualToString:@""]) {
        NSError *error = nil;
        NSRegularExpression *regex = [NSRegularExpression regularExpressionWithPattern:@"^/+" options:0 error:&error];
        
        if (error == nil) {
            NSString *modifiedFilePath = [regex stringByReplacingMatchesInString:filePath options:0 range:NSMakeRange(0, [filePath length]) withTemplate:@"file:///"];
            
            if (![modifiedFilePath hasPrefix:@"file:///"]) {
                // If not, prepend "file:///" to the path
                modifiedFilePath = [NSString stringWithFormat:@"file:///%@", modifiedFilePath];
            }
            
            return modifiedFilePath;
        }
    }
    
    return filePath;
}

static void gifread_error(Gif_Stream* gfs, Gif_Image* gfi, int is_error, const char* error_text)
{
    if (is_error >= 0)
    {
        cerr << "Error reading GIF: " << error_text << endl;
    }

}

string GifsicleWrapper::compressGifCpp(string sourceFile, string destFilePath,GifOptions options) {
    NSString* sourceFileNS = [NSString stringWithUTF8String:sourceFile.c_str()];
    NSString* destFilePathNS = [NSString stringWithUTF8String:destFilePath.c_str()];

    NSString* filePathStr = pathWithoutFileUri(sourceFileNS);
    NSString* destFilePathStr = pathWithoutFileUri(destFilePathNS);

    // Assuming animately::loadFile takes a std::string as argument
        const auto gif_bytes = [NSData dataWithContentsOfFile:filePathStr];

    auto* gif = Gif_FullReadBuffer(reinterpret_cast<const uint8_t*>([gif_bytes bytes]),
                                       [gif_bytes length], GIF_READ_UNCOMPRESSED,
                                       "", gifread_error);


        GifBuffer buffer;
        COptions coptions;
//        parse_options(options, coptions);
    
    GifOptions _options;
    _options.lossy = 200;
    _options.optimize = 3;
    _options.reduce_frames = 0;
    _options.colors = 256;
    _options.scale_x = 1;
    _options.scale_y = 1;
    parse_options(options, coptions);
    

        gifsicle_main(coptions.count(), coptions.options(), gif, &buffer.data, &buffer.size);

        bool isSaved = [[NSData dataWithBytes:buffer.data length:buffer.size] writeToFile:destFilePathStr atomically:YES];
        if(isSaved)
        {
            NSString* destSlashifyFilePath = slashifyFilePath(destFilePathStr);
            string result = [destSlashifyFilePath UTF8String];
            return result;
        }
        else
        {
            NSString* actualSlashifyFilePath = slashifyFilePath(filePathStr);
            string result = [actualSlashifyFilePath UTF8String];
            return result;
        }
}
